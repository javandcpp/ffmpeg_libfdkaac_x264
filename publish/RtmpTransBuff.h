#ifndef _DEF_RTMP_TRANS_BUFF_HEADER_H
#define _DEF_RTMP_TRANS_BUFF_HEADER_H
#include "RtpDataFrame.h"
#include <list>
using namespace std;

/*
   该类没有临界区保护，要求读数据只能有一个线程，写数据也只能有一个线程
*/
//抖动缓存
class CRtmpTransBuff
{

public:
    CRtmpTransBuff();    

    ~CRtmpTransBuff(void);

public:
    //创建接收通道
    BOOL CreateChannel(long alRoomId, BYTE abyDevType, short asMicIndex, int anQueueSize,int anJitterTime);

    //是否已经创建
    BOOL IsCreated() { return m_pszRtpFrameQueue != NULL; }

    //关闭通道
    void CloseChannel(void);

    //初始化
    void Init();

	//重置channel
	void ResetChannel(void);

	//检查无效的TransBuffer
	void CheckInvalidTransBuf();

	// 是否是视频通道
	BOOL IsVideoChannel(void); 

public:
    //插入一帧数据(0:表示失败,1:正常插入:2插入成功,但需要补发)
    int InsertRtpFrame(CRtpDataFrame *apNewFrame);

	void SetAudioChannel(CRtmpTransBuff* apAudioChannelBuf) { m_pAudioChannelBuf = apAudioChannelBuf; }

	UINT  GetRemoteTimeStamp() {return m_ulFirstRemoteStamp; }
	UINT  GetLocalTimeStamp() { return m_ulFirstReadTime; }
	bool  IsInputDataTimeOut(DWORD dwMaxTimeOut);

	UINT  GetRawTimeStamp()/* {return m_ulRawTimestamp; }*/;
	/* Add by wanghonggang */
	int CanReadRtpFrame();
	int  ReadRtpFrame(char *apFrameBuff, UINT auBuffLen, UINT &auReadLen,BOOL &abMark, BOOL& bBigVideo,UINT& aTimeStmp, INT64& readPackNum, INT64& lostPackNum);

public:
    //计算丢包率
    int CalcLostFraction();

public:
    //设置通道ID
    void SetChannelID(long alChannelID);

    //取得通道ID
    long GetChannelID() { return m_nChannelID;}

	BOOL IsPreClose() { return m_n64PreCloseTime != 0; }

public:
	void DumpDetail();

public:
    //预备关闭
    void PrepareClose(); 

    //取的预备关闭时间
    INT64 GetPrepareCloseTime();

	/* Dump transbuffer info */
	//void DumpTransInfo(FILE* apFile);

	WORD				m_wVideoWidth;
	WORD				m_wVideoHeight;
private:
    //根据指定长度创建RTP数据帧队列
    BOOL CreateRtpFrameQueue(int anQueueLen);

public:
    __inline void PositionAdd(int &nPos)
    {
        if (++nPos >= m_nRtpQueueLen){
            nPos = 0;
        }
    }

    __inline int PositionSub(int nPos)
    {
        if (nPos == 0){
            return m_nRtpQueueLen - 1;
        }
        else{
            return --nPos; 
        }
    }

	/* 计算队列长度 */
	int  GetQueueLen();
    //插入位置
    int                    m_nQueueTailPos;                        //RtpFrame队列的队尾
	/* Add by wanghonggang */
	int                    m_nQueueHeadPos;

    CRtpDataFrame  *m_pszRtpFrameQueue;        //保存RtpFrame的队列
    WORD                   m_wLastFrameSeq;                                     //最后读取的包序号

    int                    m_nRtpQueueLen;       //RtpFrame的队列长度
    //和读写相关的变量
private:
    UINT                   m_nChannelID;

    BYTE                   *m_pszRtpRecvState;         //RtpFrame的接收情况(0:未收到 1:已收 2: 已通知重发);

    UINT                   *m_pLastResendTimes;  //重发时间表
private:

    //删除位置
    BOOL                   m_bFirstRemove;
    INT64                   m_dwFirstRemoveTime;                 //本地时间戳
    UINT                   m_ulFirstRemoveStamp;              //对方时间戳
    int                    m_nJitterDelay;                         //当前抖动量    
 

private:
    //丢包情况统计
    INT64                   m_nLostBeginTime;
    int                    m_nLostFraction;          //丢包率
    int                    m_nReadFrameCount;               //已读取数据包数量
    int                    m_nLostFrameCount;         //已丢失数据包数量

	UINT m_ulFirstReadTime;
	UINT m_ulRawTimestamp;
	UINT m_ulFirstRemoteStamp;
	BYTE m_byDevType;
	short m_sMicIndex;
	long  m_lRoomID;
	int m_nCurrJitterDelay;
	DWORD m_dwInputDataTimeLast;

	CRtmpTransBuff* m_pAudioChannelBuf;

	BOOL m_bFirstRead;

private:
    //开始关闭时间
    INT64		m_n64PreCloseTime;

    //最后开始删除时间
    INT64		m_n64LastRemoveTime;

	/* 增加统计信息 */
	INT64 m_i64InsertOK;
	INT64 m_i64ReadError;

	INT64 m_i64LastTraceTime;

    unsigned short m_last_seq[512];
};
#endif /*_DEF_RTMP_TRANS_BUFF_HEADER_H */
