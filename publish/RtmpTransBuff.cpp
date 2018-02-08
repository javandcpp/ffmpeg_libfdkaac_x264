#include "DebugTrace.h"
#include "RtmpTransBuff.h"
#include "BaseThread.h"
#include "ConstDeff.h"
#include <string.h>

#ifdef WIN32
#include <Mmsystem.h>
#pragma comment(lib, "Winmm.lib")
#endif

extern unsigned int timeGetTime();

CRtmpTransBuff::CRtmpTransBuff()
{
    m_pszRtpRecvState = NULL;
    m_pszRtpFrameQueue = NULL;             //保存RtpFrame的队列
    m_pLastResendTimes = NULL;
    m_nRtpQueueLen = 0;

    m_nQueueTailPos = 0; //RtpFrame队列的队尾

	/* Add by wanghonggang */
	m_ulFirstReadTime = 0;
	m_ulFirstRemoteStamp = 0;
	m_nQueueHeadPos = 0;
	m_sMicIndex = 0;
	m_lRoomID   = 0;
	m_ulRawTimestamp = 0;
	m_nCurrJitterDelay = 120;
	m_bFirstRead = TRUE;
	m_pAudioChannelBuf = NULL;
	m_dwInputDataTimeLast = 0;

    m_n64PreCloseTime = 0;
    m_nChannelID = 0;

	m_i64InsertOK  = 0;
	m_i64ReadError = 0;

	m_i64LastTraceTime = 0;

    m_wLastFrameSeq = 0;

    memset(&m_last_seq,0,sizeof(m_last_seq));
}

CRtmpTransBuff::~CRtmpTransBuff(void)
{
	
}


//初始化
void CRtmpTransBuff::Init()
{
    m_nLostFraction = 0;          //丢包率
    m_nReadFrameCount = 0;         //已读取数据包数量
    m_nLostFrameCount = 0;          //已丢失数据包数量
    m_nLostBeginTime = CBaseThread::GetSystemTime();

    m_nQueueTailPos = 0; //RtpFrame队列的队尾
    m_bFirstRemove = TRUE;
    m_ulFirstRemoveStamp = 0;
    m_n64LastRemoveTime = CBaseThread::GetSystemTime();
 
    m_n64PreCloseTime = 0;

    m_wLastFrameSeq = 0;

    //ASSERT(m_pszRtpRecvState != NULL && m_nRtpQueueLen > 0);
    memset(m_pszRtpRecvState,0,sizeof(BYTE) *m_nRtpQueueLen); //初始化接收状态
    memset(m_pLastResendTimes,0,sizeof(UINT) *m_nRtpQueueLen); //初始化时间信息
}

void CRtmpTransBuff::CloseChannel(void)
{
    if (m_pszRtpFrameQueue)                //保存RtpFrame的队列
    {
        delete[] m_pszRtpFrameQueue;
        m_pszRtpFrameQueue = NULL;

        m_nRtpQueueLen = 0;
    }
    if (m_pszRtpRecvState)                     //RtpFrame的接收情况(0:未收到 1:已收 2: 已通知重发);
    {
        delete[] m_pszRtpRecvState;
        m_pszRtpRecvState = NULL;
    }
    if (m_pLastResendTimes)
    {
        delete []m_pLastResendTimes;
        m_pLastResendTimes = NULL;
    }
}

//预备关闭
void CRtmpTransBuff::PrepareClose()
{ 
    m_n64PreCloseTime = CBaseThread::GetSystemTime();  
}

//取的预备关闭时间
INT64 CRtmpTransBuff::GetPrepareCloseTime()
{
    if (m_n64PreCloseTime != 0)
    {
        return CBaseThread::GetSystemTime() - m_n64PreCloseTime;
    }
    return 0;
}

BOOL CRtmpTransBuff::CreateChannel(long alRoomId, BYTE abyDevType, short asMicIndex, int anQueueSize,int anJitterTime)
{
	m_lRoomID    = alRoomId;
	m_byDevType  = abyDevType;
	m_sMicIndex  = asMicIndex;

	m_nQueueHeadPos = 0;
	m_nQueueTailPos = 0;
	m_ulFirstRemoteStamp = 0;
	m_ulFirstReadTime = timeGetTime();
	m_nCurrJitterDelay = anJitterTime;
	m_bFirstRead = TRUE;

    if (!CreateRtpFrameQueue(anQueueSize))
    {
        TRACE1("CRtmpTransBuff::CreateChannel: CreateDeCodecMgr failed!\n");
        CloseChannel();
        return FALSE;
    }

    Init();
    return TRUE;
}

//根据指定长度创建RTP数据帧队列
BOOL CRtmpTransBuff::CreateRtpFrameQueue(int anQueueLen)
{
    ASSERT(anQueueLen > 0);
    ASSERT(NULL == m_pszRtpFrameQueue);
    ASSERT(NULL == m_pszRtpRecvState);

    m_pszRtpFrameQueue = new CRtpDataFrame[anQueueLen]; 
    if (NULL == m_pszRtpFrameQueue)
    {
        TRACE1("CRtmpTransBuff::CreateRtpFrameQueue: new m_pszRtpFrameQueue fail!\n");
        return FALSE;
    }
    m_pszRtpRecvState = new BYTE[anQueueLen]; 
    if (NULL == m_pszRtpRecvState)
    {
        TRACE1("CRtmpTransBuff::CreateRtpFrameQueue: new m_pszRtpRecvState fail!\n");
        return FALSE;
    }
    m_pLastResendTimes = new UINT[anQueueLen];
    if (NULL == m_pLastResendTimes)
    {
        TRACE1("CRtmpTransBuff::CreateRtpFrameQueue: new m_pLastResendTimes fail!\n");
        return FALSE;
    }
    memset(m_pszRtpRecvState,0,sizeof(BYTE) *anQueueLen); //初始化接收状态
    m_nRtpQueueLen = anQueueLen; 
    return TRUE;
}


// 插入一个新帧
int CRtmpTransBuff::InsertRtpFrame(CRtpDataFrame *apNewFrame)
{
    if (m_nQueueHeadPos == m_nQueueTailPos)  //队列为空？也有可能满了吧？
    {
        // 清空接收标记
        memset(&m_last_seq,0,sizeof(m_last_seq));
    }

    unsigned short seq = apNewFrame->GetSequenceNumber();
    int index = seq & (511);
    if (m_last_seq[index] == seq)
    {
        return -1;
    }

    m_last_seq[index] = seq;
    
	//TraceLog0("时间戳测试,包序号:%d.队头:%d,队尾:%d,队尾位置%d,队头位置%d,DevType:%d,GetMark:%d:GetTimeStamp:%d\n",\
	//	apNewFrame->GetSequenceNumber(),m_pszRtpFrameQueue[m_nQueueHeadPos].GetSequenceNumber(),m_pszRtpFrameQueue[m_nQueueTailPos].GetSequenceNumber(),
	//	m_nQueueTailPos,m_nQueueHeadPos,m_byDevType,apNewFrame->GetMarker(),apNewFrame->GetTimestamp());

    ASSERT(apNewFrame != NULL);

	m_dwInputDataTimeLast = timeGetTime();

    if (m_nQueueHeadPos == m_nQueueTailPos)  //队列为空
    {
        m_bFirstRemove = TRUE;

        m_ulFirstRemoveStamp = apNewFrame->GetTimestamp();
        m_dwFirstRemoveTime = CBaseThread::GetSystemTime();
		m_ulFirstReadTime = timeGetTime();

        //以最后读出的包序号为根据计算插入位置
        int nPreTailPos = PositionSub(m_nQueueTailPos);    //(m_nQueueTailPos + m_nRtpQueueLen - 1) % m_nRtpQueueLen;

        if (m_wLastFrameSeq == m_pszRtpFrameQueue[nPreTailPos].GetSequenceNumber()) //最后读出的包序号有效
        {
            //比较新包和最后读出的包序号
            int nPackSeqGap = (apNewFrame->GetSequenceNumber() + 65536 - m_wLastFrameSeq) % 65536;
            if (nPackSeqGap < 10 && nPackSeqGap > 0)  
            {
                //TraceLog6("插入空队列!前面有数据没收到。队头序号:%d,队头:%d,包序号:%d,插入位置:%d.\n",\
                    m_wLastFrameSeq, m_nQueueHeadPos, m_pszRtpFrameQueue[nPreTailPos].GetSequenceNumber(), m_nQueueTailPos);
                TraceLog6("Empty queue, not first pack\n");

                //中间的数据包丢失
                for (int nIndex  = 1; nIndex < nPackSeqGap; nIndex++)
                {
                    m_pszRtpRecvState[m_nQueueTailPos] = 0;                 //未收到
                    PositionAdd(m_nQueueTailPos);//m_nQueueTailPos = (m_nQueueTailPos + 1) % m_nRtpQueueLen;                             
                }

                //复制数据
                m_pszRtpFrameQueue[m_nQueueTailPos] = *apNewFrame;       
                m_pszRtpRecvState[m_nQueueTailPos] = 1; //标记已收到


                PositionAdd(m_nQueueTailPos);//m_nQueueTailPos = (m_nQueueTailPos + 1) % m_nRtpQueueLen;                             
                return 0;            
            }
        }

        TraceLog6("Empty queue\n");
        //空队列
        m_pszRtpFrameQueue[m_nQueueTailPos] = *apNewFrame;               
        m_pszRtpRecvState[m_nQueueTailPos] = 1; //标记已收到
        PositionAdd(m_nQueueTailPos);

		m_i64InsertOK++;

        return 0;
    }

    //队列非空,计算队列长度
	/* Wanghonggang */
    int lnQueueLen = (m_nQueueTailPos + m_nRtpQueueLen - m_nQueueHeadPos) % m_nRtpQueueLen;
    //ASSERT(lnQueueLen > 0 && lnQueueLen < m_nRtpQueueLen);

    //计算队尾数据包的位置
    int liPreTailPos = PositionSub(m_nQueueTailPos);    //(m_nQueueTailPos + m_nRtpQueueLen - 1) % m_nRtpQueueLen;

	//计算新包和队尾数据包的序号差
	//ASSERT(m_pszRtpRecvState[liPreTailPos] == 1); //该位置的数据包必定有效
	UINT preSeq = m_pszRtpFrameQueue[liPreTailPos].GetSequenceNumber();
	UINT curSeq = apNewFrame->GetSequenceNumber();
	UINT preTimt = m_pszRtpFrameQueue[liPreTailPos].GetTimestamp();
	UINT curTimt = apNewFrame->GetTimestamp();

    int nPackSeqGap = (apNewFrame->GetSequenceNumber() + 65536 - m_pszRtpFrameQueue[liPreTailPos].GetSequenceNumber()) % 65536;
    if (nPackSeqGap == 0)
    {
        //插入失败
        //TraceLog0("扔掉队尾相同数据包,包序号:%d.队头:%d,队尾:%d,队尾位置%d.\n",\
            apNewFrame->GetSequenceNumber(),m_pszRtpFrameQueue[m_nQueueHeadPos].GetSequenceNumber(),\
            m_pszRtpFrameQueue[liPreTailPos].GetSequenceNumber(),liPreTailPos);
        return -2;
    }

    //若新包序号小于队尾数据包,则nPackSeqGap接近于65535;反之若小于30则表示新包序号大,应插入队尾
    //if (nPackSeqGap < 20 && ((lnQueueLen + nPackSeqGap + 1) < m_nRtpQueueLen))  //避免队列越界
    if (nPackSeqGap < 60000 )//避免队列越界
    //if (nPackSeqGap < m_nRtpQueueLen - lnQueueLen) // 队列剩余的长度
    {
        //中间的数据包丢失
        for (int nIndex  = 1; nIndex < nPackSeqGap; nIndex++)
        {
            m_pszRtpRecvState[m_nQueueTailPos] = 0;                 //未收到
            PositionAdd(m_nQueueTailPos);//m_nQueueTailPos = (m_nQueueTailPos + 1) % m_nRtpQueueLen;                             
        }

        //复制数据
        m_pszRtpFrameQueue[m_nQueueTailPos] = *apNewFrame;       
        m_pszRtpRecvState[m_nQueueTailPos] = 1; //标记已收到
        PositionAdd(m_nQueueTailPos);//m_nQueueTailPos = (m_nQueueTailPos + 1) % m_nRtpQueueLen;                             
		m_i64InsertOK++;

        return 0;
    }
    else
    {
        //判断新包序号是否小于队尾数据包
        nPackSeqGap = (m_pszRtpFrameQueue[liPreTailPos].GetSequenceNumber() + 65536 - apNewFrame->GetSequenceNumber()) % 65536;
        if (nPackSeqGap  < lnQueueLen)  //乱序后到的或丢失重发的数据包
        {                      
            //apNewFrame的时间戳应该小于队尾的时间戳
            //ASSERT(m_pszRtpFrameQueue[liPreTailPos].GetTimestamp() >= apNewFrame->GetTimestamp()); 

            int nInsertPos = (liPreTailPos + m_nRtpQueueLen - nPackSeqGap) % m_nRtpQueueLen;
            
			if  (m_pszRtpRecvState[nInsertPos] != 1 \
				|| m_pszRtpFrameQueue[nInsertPos].GetSequenceNumber() != apNewFrame->GetSequenceNumber())
            {
				if (m_pszRtpRecvState[nInsertPos] == 1)
				{
					TraceLog4("CRtmpTransBuff::接收状态为1，但包序号:%d与新包:%d不一致,插入:%d,队头:%d.队尾:%d.\n",\
						m_pszRtpFrameQueue[nInsertPos].GetSequenceNumber(),apNewFrame->GetSequenceNumber(),nInsertPos, m_nQueueHeadPos,m_nQueueTailPos);
				}

				TRACE4("丢失数据插入成功!序号:%d,位置:%d,次数:%d,队头:%d.队尾:%d.\n",\
					apNewFrame->GetSequenceNumber(),nInsertPos,m_pszRtpRecvState[nInsertPos], m_nQueueHeadPos,m_nQueueTailPos);

                m_pszRtpFrameQueue[nInsertPos] = *apNewFrame;
                m_pszRtpRecvState[nInsertPos] = 1; //标记已收到
               
				m_i64InsertOK++;
                return 0;
            }
			else
			{
			/*	TraceLog0("扔掉位置%d相同数据包,包序号:%d.队头:%d,队尾:%d,队尾位置%d.\n",nInsertPos,\
					apNewFrame->GetSequenceNumber(),m_pszRtpFrameQueue[m_nQueueHeadPos].GetSequenceNumber(),\
					m_pszRtpFrameQueue[liPreTailPos].GetSequenceNumber(),liPreTailPos);
				return FALSE;*/
                return -5;
			}
        }
        else
        {
            return -4;
        }
    }
    //插入失败
  //  TraceLog0("扔掉数据包,包序号:%d.队头:%d,队尾:%d,队尾位置%d.队头位置:%d\n",\
  //      apNewFrame->GetSequenceNumber(),m_pszRtpFrameQueue[m_nQueueHeadPos].GetSequenceNumber(),\
		//m_pszRtpFrameQueue[liPreTailPos].GetSequenceNumber(),liPreTailPos,m_nQueueHeadPos);

    return -3;
}

void CRtmpTransBuff::SetChannelID(long alChannelID) 
{ 
    m_nChannelID = alChannelID;               
}

int CRtmpTransBuff::CanReadRtpFrame()
{
	int lnQueueHeadPos = m_nQueueHeadPos;
	ASSERT(m_pszRtpRecvState != NULL && m_pszRtpFrameQueue != NULL);
	if(m_pszRtpRecvState == NULL || m_pszRtpFrameQueue == NULL)
	{
		RecordLog(0,"CRecvChannel::CanReadRtpFrame: ERR!!! 队列为空: "<< m_nQueueHeadPos<<" QueueLen : "<< m_nRtpQueueLen<<" a/v: "<<m_byDevType<<"micidex: "<<m_sMicIndex <<" RoomID "<<m_lRoomID);
		return -1;
	}

	ASSERT(lnQueueHeadPos >=0 && lnQueueHeadPos < m_nRtpQueueLen);
	if(lnQueueHeadPos < 0 || lnQueueHeadPos >= m_nRtpQueueLen)
	{
		RecordLog(0,"CRecvChannel::CanReadRtpFrame: ERR!!! HeadPos : "<< m_nQueueHeadPos<<" QueueLen : "<< m_nRtpQueueLen<<" a/v: "<<m_byDevType<<"micidex: "<<m_sMicIndex <<" RoomID "<<m_lRoomID);
		return -2;
	}

	ASSERT(m_nQueueTailPos >=0 && m_nQueueTailPos < m_nRtpQueueLen);
	if(m_nQueueTailPos < 0 || m_nQueueTailPos >= m_nRtpQueueLen)
	{
		RecordLog(0,"CRecvChannel::CanReadRtpFrame: ERR!!! TailPos: "<< m_nQueueTailPos<<" QueueLen : "<< m_nRtpQueueLen<<" a/v: "<<m_byDevType<<"micidex: "<<m_sMicIndex <<" RoomID "<<m_lRoomID);
		return -3;
	}

	//找到第一个有效的数据包
	while (m_pszRtpRecvState[lnQueueHeadPos] != 1 && lnQueueHeadPos != m_nQueueTailPos)
	{
		PositionAdd(lnQueueHeadPos);        //lnQueueHeadPos = (lnQueueHeadPos + 1) % m_nRtpQueueLen;		
	}

	//若队列为空,返回FALSE
	if (lnQueueHeadPos == m_nQueueTailPos){
		return -4;
	}

	//根据队列长度检查是否到了读包时间
	int lnPreTailPos = PositionSub(m_nQueueTailPos);       //(m_nQueueTailPos + m_nRtpQueueLen - 1) % m_nRtpQueueLen;	

	ASSERT(m_pszRtpRecvState[lnPreTailPos] == 1); //该位置的数据包必定有效
	while (m_pszRtpRecvState[lnPreTailPos] != 1)
	{
		if (lnQueueHeadPos == lnPreTailPos){ //若队列为空,返回FALSE
			return -5;
		}
		lnPreTailPos = PositionSub(lnPreTailPos);		
	}


    int ret = 0;

	//计算出队列中数据包的时间差
	int lnQueuePacketTimeGap = m_pszRtpFrameQueue[lnPreTailPos].GetTimestamp() - m_pszRtpFrameQueue[lnQueueHeadPos].GetTimestamp();
	int lnQueueLen = (m_nQueueTailPos + m_nRtpQueueLen - m_nQueueHeadPos) % m_nRtpQueueLen; 	

	//if (CBaseThread::GetSystemTime() - m_i64LastTraceTime > 20 * 1000)
	//{
	//	TraceLog0("出队列中数据包的时间差%d\n",lnQueuePacketTimeGap);
	//	TraceLog0("队列长度%d\n",lnQueueLen);
	//	TraceLog0("%d\n",m_nCurrJitterDelay);
	//	m_i64LastTraceTime = CBaseThread::GetSystemTime();
	//}

	if (m_byDevType == DEF_AUDIO_DEVICE  || m_pAudioChannelBuf == NULL)
	{   
		//根据m_ulFirstReadTime检查是否到了读包时间
		int lnLacalStartTimeGap = timeGetTime() - m_ulFirstReadTime;

		//第一次或静音结束后检查队列长度
		if (m_bFirstRead)
		{
			if (lnLacalStartTimeGap - m_nCurrJitterDelay >= 0 || lnQueuePacketTimeGap >= m_nCurrJitterDelay)
			{
				TRACE4("CanReadRtpFrame: 用户:%I64d,设备:%d,启动时长:%d,包内时差：%d。\n",\
					m_nChannelID,m_byDevType,lnLacalStartTimeGap,lnQueuePacketTimeGap);
				return 0;
			}
            else
            {
                ret = -1;
            }
		}
		else
		{   
			//根据双方时间检查是否到了读包时间
			int lnPeerPacketArriveTimeGap = m_pszRtpFrameQueue[lnQueueHeadPos].GetTimestamp() - m_ulFirstRemoteStamp;
			int lnTimeStampGap = lnLacalStartTimeGap - lnPeerPacketArriveTimeGap;

			if (lnTimeStampGap > 0 || lnQueuePacketTimeGap >= m_nCurrJitterDelay)
			{
				//TRACE4("CanReadRtpFrame: 用户:%I64d,设备:%d,时间差:%d,包内时差:%d。\n",\
				//	m_nChannelID,m_byDevType,lnTimeStampGap,lnQueueTimeGap);		
				return 0;
			}			
            else
            {
                ret = -2;
            }
		}		
	}
	else
	{	
		//视频通道
		ASSERT(m_byDevType != DEF_AUDIO_DEVICE  && m_pAudioChannelBuf != NULL);

		//实现视频和语音的同步m_pAudioChannelTimeStamp
		if (m_pAudioChannelBuf->GetRemoteTimeStamp() != 0) //语音包已经开始读取
		{
			CRtmpTransBuff* pAudio = (CRtmpTransBuff*)m_pAudioChannelBuf;
			if (pAudio->IsInputDataTimeOut(100))
			{//超过1秒没有音频数据，读视频包
				goto READ_VIDEO;
			}
			//分别计算本地时间差和对方时间差
			int lnLacalStartTimeGap = timeGetTime() - m_pAudioChannelBuf->GetLocalTimeStamp();
			int lnPeerPacketArriveTimeGap = m_pszRtpFrameQueue[lnQueueHeadPos].GetTimestamp() - m_pAudioChannelBuf->GetRemoteTimeStamp();

			if (lnLacalStartTimeGap > lnPeerPacketArriveTimeGap )
			{
				//TRACE4("视频同步读取: 用户:%I64d,设备:%d,时间戳差:%d。\n",\
				//	m_nChannelID,m_byDevType,lnLacalStartTimeGap - lnPeerPacketArriveTimeGap);	
				return 0;
			}			
			if (lnQueuePacketTimeGap >= m_nCurrJitterDelay)
			{
				TRACE4("视频提前读取: 用户:%I64d,时间戳差：%d,队列内时间:%d。\n",\
					m_nChannelID,m_byDevType,lnLacalStartTimeGap - lnPeerPacketArriveTimeGap,lnQueuePacketTimeGap);	
				return 0;
			}
            else
            {
                ret = -3;
            }
		}
		else
		{
READ_VIDEO:
			//根据m_ulFirstReadTime检查是否到了读包时间
			int lnLacalStartTimeGap = timeGetTime() - m_ulFirstReadTime;

			//第一次或静音结束后检查队列长度
			if (m_bFirstRead)
			{
				if (lnLacalStartTimeGap - m_nCurrJitterDelay >= 0 || lnQueuePacketTimeGap >= m_nCurrJitterDelay)
				{
					TRACE4("CanReadRtpFrame: 用户:%I64d,设备:%d,启动时长:%d,包内时差：%d。\n",\
						m_nChannelID,m_byDevType,lnLacalStartTimeGap,lnQueuePacketTimeGap);
					return 0;
				}
                else
                {
                    ret = -4;
                }
			}
			else
			{   
				//根据双方时间检查是否到了读包时间
				int lnPeerPacketArriveTimeGap = m_pszRtpFrameQueue[lnQueueHeadPos].GetTimestamp() - m_ulFirstRemoteStamp;
				int lnTimeStampGap = lnLacalStartTimeGap - lnPeerPacketArriveTimeGap;

				if (lnTimeStampGap > 0 || lnQueuePacketTimeGap >= m_nCurrJitterDelay)
				{
					return 0;
				}			
                else
                {
                    TRACE4("CanReadRtpFrame: 用户:%I64d,设备:%d,时间差:%d,包内时差:%d。\n",\
                        m_nChannelID,m_byDevType,lnTimeStampGap,lnQueuePacketTimeGap);		
                    ret = -5;
                }
			}		
		}
	}
	return -10 + ret;
}

//读取一帧数据(读取成功:0 暂无数据:<0 数据包丢失:-1
int CRtmpTransBuff::ReadRtpFrame(char *apFrameBuff, UINT auBuffLen, UINT &auReadLen,BOOL &abMark, BOOL& bBigVideo,UINT& aTimestamp, INT64& readPackNum, INT64& lostPackNum)
{
	static UINT unLastAudioTimestamp = 0;
	static UINT unLastVedioTimestamp = 0;

	BOOL bIsNeedSync = FALSE;
	auReadLen = 0;
	//检测是否可读帧
    int ret = CanReadRtpFrame();
	if (ret != 0)
	{
		m_i64ReadError++;
		return -100 + ret;
	}

    readPackNum = 0;
    lostPackNum = 0;
	//先检查丢包情况
	int lnLostFrameCount = 0;	

	//计算队列长度
	int lnQueueLen = (m_nQueueTailPos + m_nRtpQueueLen - m_nQueueHeadPos) % m_nRtpQueueLen; 	

    // 还没收到的包，跳过
	while (m_pszRtpRecvState[m_nQueueHeadPos] != 1 && (lnLostFrameCount < lnQueueLen))
	{
		m_wLastFrameSeq++;
		lnLostFrameCount++;

		TRACE4("ReadFrame:数据包没收到! 用户:%I64d,设备:%d,包序号:%d,接收状态:%d,HeadPos:%d.\n\n",\
			m_nChannelID,m_byDevType,m_wLastFrameSeq,m_pszRtpRecvState[m_nQueueHeadPos],m_nQueueHeadPos);

		m_pszRtpRecvState[m_nQueueHeadPos] = 0;
		PositionAdd(m_nQueueHeadPos);          //m_nQueueHeadPos = (m_nQueueHeadPos + 1) % m_nRtpQueueLen;
	}

	if (lnLostFrameCount > 0)
	{	
		m_i64ReadError++;
		return -1;
	}

    lostPackNum += lnLostFrameCount;
	//清空计数器
	lnLostFrameCount = 0;

	//GetExtendMark()返回真有分包
	//GetExtProfile() == 0

    // 读取小包
	while ((m_pszRtpFrameQueue[m_nQueueHeadPos].GetExtendMark() \
		&& (m_pszRtpFrameQueue[m_nQueueHeadPos].GetExtProfile() == 0)) \
		&& (m_pszRtpRecvState[m_nQueueHeadPos] == 1 \
		&& (m_nQueueHeadPos != m_nQueueTailPos))) //分包的个数为0	
	{
		lnLostFrameCount++;

		m_wLastFrameSeq = m_pszRtpFrameQueue[m_nQueueHeadPos].GetSequenceNumber();
		TRACE4("ReadFrame 分包个数为0!用户:%I64d,设备:%d,位置:%d,序号:%d,接收状态:%d。\n",\
			m_nChannelID,m_byDevType,m_nQueueHeadPos,m_wLastFrameSeq,m_pszRtpRecvState[m_nQueueHeadPos]);

		m_pszRtpRecvState[m_nQueueHeadPos] = 0;
		PositionAdd(m_nQueueHeadPos);          //m_nQueueHeadPos = (m_nQueueHeadPos + 1) % m_nRtpQueueLen;			
	}

    lostPackNum += lnLostFrameCount;
	if (lnLostFrameCount > 0)
	{	
		m_i64ReadError++;
		return -1;
	}

	if (m_nQueueHeadPos == m_nQueueTailPos) //队列为空
	{
		m_i64ReadError++;
		return -3;
	}

	ASSERT(m_pszRtpRecvState[m_nQueueHeadPos] == 1);
	ASSERT(m_nQueueHeadPos != m_nQueueTailPos);

	//计算需要读取的数据包总数
	int lnPacketTotal = 1;
	if (m_pszRtpFrameQueue[m_nQueueHeadPos].GetExtendMark())	
	{  
		lnPacketTotal = m_pszRtpFrameQueue[m_nQueueHeadPos].GetExtProfile();   //ExtProfile指出了小包的个数
		ASSERT(lnPacketTotal > 1);
	}

	//复制RTP包头标志
	abMark = m_pszRtpFrameQueue[m_nQueueHeadPos].GetMarker();
	m_ulRawTimestamp = m_pszRtpFrameQueue[m_nQueueHeadPos].GetTimestamp();
	UINT luFirstRemoteStamp = m_ulRawTimestamp;
	aTimestamp = luFirstRemoteStamp;
	UINT unSequenceNumber = 0;

	//清空计数器
	lnLostFrameCount = 0;
	int lnReadPackCount = 0;
	int lnReadDataLen = 0;
	lnQueueLen = (m_nQueueTailPos + m_nRtpQueueLen - m_nQueueHeadPos) % m_nRtpQueueLen; 

	//读取整个数据包
	while ((lnReadPackCount < lnPacketTotal) && (m_nQueueHeadPos != m_nQueueTailPos))	
	{	
		lnReadPackCount++;

		if (m_pszRtpRecvState[m_nQueueHeadPos] == 1)
		{			
			unSequenceNumber = m_pszRtpFrameQueue[m_nQueueHeadPos].GetSequenceNumber();

			//ASSERT(abMark == m_pszRtpFrameQueue[m_nQueueHeadPos].GetMarker());	              // GetMarker()应该相等
			ASSERT(auBuffLen >= (UINT)(lnReadDataLen + m_pszRtpFrameQueue[m_nQueueHeadPos].GetPayloadSize())); //确保存储空间足够

#ifdef _DEBUG
			if (!m_bFirstRead && lnReadPackCount > 1)		{
				unsigned short lsFrameSeqNum = m_wLastFrameSeq;
				ASSERT(++lsFrameSeqNum == m_pszRtpFrameQueue[m_nQueueHeadPos].GetSequenceNumber());
			}
			ASSERT(m_pszRtpRecvState[m_nQueueHeadPos] == 1);
#endif
			//更新m_wLastFrameSeq(最后读取的包序号)
			m_wLastFrameSeq = m_pszRtpFrameQueue[m_nQueueHeadPos].GetSequenceNumber();	

			int nPayloadSize = m_pszRtpFrameQueue[m_nQueueHeadPos].GetPayloadSize();
			if (auBuffLen >= (UINT)(lnReadDataLen + nPayloadSize))
			{
				memcpy(apFrameBuff+lnReadDataLen,m_pszRtpFrameQueue[m_nQueueHeadPos].GetPayloadPtr(),nPayloadSize);
				lnReadDataLen += nPayloadSize;

				EnumPayloadTypes ePayloadType = m_pszRtpFrameQueue[m_nQueueHeadPos].GetPayloadType();
				if(e_RtpWmscreenVideoData == ePayloadType)
				{
					bBigVideo = TRUE;
				}
			}
			else
			{
				lnLostFrameCount++;

				TRACE3("ReadFrame:存储空间不够!用户:%I64d,设备:%d,Buff长度: %d,已读字节:%d,新数据长度:%d.\n",\
					m_nChannelID,m_byDevType,auBuffLen,lnReadDataLen,nPayloadSize);
			}
		}	
		else //数据包已丢失
		{
			m_wLastFrameSeq++;
			lnLostFrameCount++;
			TRACE4("数据包没收到(2)! 用户:%I64d,设备:%d,位置:%d,包序号:%d,接收状态:%d.\n\n\n",\
				m_nChannelID,m_byDevType,m_nQueueHeadPos,m_wLastFrameSeq,m_pszRtpRecvState[m_nQueueHeadPos]);
		}		
		m_pszRtpRecvState[m_nQueueHeadPos] = 0;
		PositionAdd(m_nQueueHeadPos);          //m_nQueueHeadPos = (m_nQueueHeadPos + 1) % m_nRtpQueueLen;
	}


    lostPackNum += lnLostFrameCount;
    readPackNum = lnReadPackCount;

    // 小包没丢失，返回读取的包
    // 小包丢失，  返回读取错误
	if (lnReadPackCount == lnPacketTotal && lnLostFrameCount == 0)
	{
		//非语音数据要和语音数据同步
		if (m_bFirstRead || (abMark && m_byDevType == DEF_AUDIO_DEVICE)) 
		{		
			m_ulFirstReadTime = timeGetTime();			
			m_ulFirstRemoteStamp = luFirstRemoteStamp;			
		}		

		m_bFirstRead = FALSE;
		auReadLen = lnReadDataLen;

		if(m_nQueueHeadPos == 0)
		{
			m_ulRawTimestamp = m_pszRtpFrameQueue[0].GetTimestamp();
		}

		if(m_byDevType == DEF_AUDIO_DEVICE)
		{
			TRACE4("RTMP Send: 音频 roomid=[%d] micindex=[%d] seq=[%d] timestamp=[%d] bmark=[%d]\r\n", m_lRoomID, \
				m_sMicIndex+1, m_pszRtpFrameQueue[m_nQueueHeadPos].GetSequenceNumber(), m_pszRtpFrameQueue[m_nQueueHeadPos].GetTimestamp(),
				m_pszRtpFrameQueue[m_nQueueHeadPos].GetMarker());

			if (aTimestamp- unLastAudioTimestamp > 1000)
			{
				//TraceLog0("时间戳测试,队尾位置%d,队头位置%d,DevType:Audio,GetMark:%d:GetTimeStamp:%d,SequenceNumber:%d,差值:%d\n",\
				//	m_nQueueTailPos,m_nQueueHeadPos,abMark,aTimestamp,unSequenceNumber,aTimestamp- unLastAudioTimestamp );
			}

			unLastAudioTimestamp = aTimestamp;
		}
		else
		{
			TRACE4("RTMP Send: 视频 roomid=[%d] micindex=[%d] seq=[%d] timestamp=[%d] bmark=[%d]\r\n", m_lRoomID, \
				m_sMicIndex+1, m_pszRtpFrameQueue[m_nQueueHeadPos].GetSequenceNumber(), m_pszRtpFrameQueue[m_nQueueHeadPos].GetTimestamp(),\
				m_pszRtpFrameQueue[m_nQueueHeadPos].GetMarker());

			if (aTimestamp- unLastVedioTimestamp > 1000)
			{
				//TraceLog0("时间戳测试,队尾位置%d,队头位置%d,DevType:video,GetMark:%d:GetTimeStamp:%d,SequenceNumber%d,差值:%d\n",\
				//	m_nQueueTailPos,m_nQueueHeadPos,abMark,aTimestamp,unSequenceNumber,aTimestamp- unLastAudioTimestamp );
			}
			unLastVedioTimestamp = aTimestamp;
		}



		return 0;
	}
	else
	{
        lostPackNum += readPackNum;
		if (lnReadPackCount > 0) 
		{
			m_i64ReadError++;
			return -1;
		}

		m_i64ReadError++;
		return -2;
	}
}

UINT CRtmpTransBuff::GetRawTimeStamp()
{
	return m_pszRtpFrameQueue[m_nQueueHeadPos].GetTimestamp() - m_ulRawTimestamp;
}

bool CRtmpTransBuff::IsInputDataTimeOut(DWORD dwMaxTimeOut)
{
	DWORD dwNow = timeGetTime();
	DWORD dwCap = dwNow - m_dwInputDataTimeLast;
	return (dwCap >= dwMaxTimeOut);
}

void CRtmpTransBuff::ResetChannel(void)
{
    memset(&m_last_seq,0,sizeof(m_last_seq));

	m_nQueueTailPos = 0;
	m_nQueueHeadPos = 0;
	m_ulFirstReadTime = 0;
	m_ulFirstRemoteStamp = 0;
	m_ulRawTimestamp = 0;
	m_bFirstRead = TRUE;
	m_dwInputDataTimeLast = 0;

	m_i64InsertOK  = 0;
	m_i64ReadError = 0;

    m_wLastFrameSeq = 0;
}

BOOL CRtmpTransBuff::IsVideoChannel(void)
{
	return DEF_VIDEO_DEVICE == m_byDevType;
}

void CRtmpTransBuff::CheckInvalidTransBuf()
{
	if(0 == m_i64InsertOK)
	{
		TRACE1("CRtmpTransBuff::CheckInvalidTransBuf(): ResetChannel()!\r\n");
		ResetChannel();
		if(m_pAudioChannelBuf != NULL)
		{
			m_pAudioChannelBuf->ResetChannel();
		}
	}

	m_i64InsertOK  = 0;
	m_i64ReadError = 0;
}

int CRtmpTransBuff::GetQueueLen()
{
	int lnQueueLen = (m_nQueueTailPos + m_nRtpQueueLen - m_nQueueHeadPos) % m_nRtpQueueLen;
	return lnQueueLen;
}

//void CRtmpTransBuff::DumpTransInfo(FILE* apFile)
//{
//	fprintf(apFile, "\r\n      TransBuf DUMP INFO: MicIndex = %d   DevType = %d   \r\n", m_sMicIndex + 1, m_byDevType);
	//fprintf(apFile, "      InsertOK = %lld   ReadError = %lld   QueueLen = %d\r\n", m_i64InsertOK, m_i64ReadError, GetQueueLen());
//}

void CRtmpTransBuff::DumpDetail()
{
	TraceLog0("m_nQueueTailPos:%d,nRtpQueueLen:%d,m_nQueueHeadPos:%d\n",m_nQueueTailPos,m_nRtpQueueLen,m_nQueueHeadPos);

}
