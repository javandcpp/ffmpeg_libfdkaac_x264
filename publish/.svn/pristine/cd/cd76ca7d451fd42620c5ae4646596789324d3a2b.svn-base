
#include <string.h>

#include <unordered_map>
#include "DebugTrace.h"
#include "RtpDataFrame.h"
#include "ConfigManager.h"
#include "flv_service.h"
#include "flv.h"

#define CONNSTA_CONN_BREAK  0   // 连接断开
#define CONNSTA_CONNED      1   // 已经连接
#define CONNSTA_CONNING  2   // 正在连接


FLVService::FLVService()
{
	for (int i = 0; i < 1/*DEF_THREAD_COUNT*/; i++)
	{
		m_quPtrFLVFrame[i].Init(500000);
	}

	m_i64UdpInFlux = 0;
	m_i64UdpPacketCount = 0;

	m_bStop = TRUE;
	m_iThreadCount = 0;
	m_pRtmpMgr = NULL;

    	m_id = 0;
    	m_id = CSingletonCfgManager::Instance().GetValue(
            "test","uid");
    m_startTime = time(NULL);
}

FLVService::~FLVService()
{
	Close();
}

BOOL FLVService::Open()
{
    m_push_url = (std::string)CSingletonCfgManager::Instance().GetValue("server","publish_base_url");

	if (!m_bStop)
	{
		return TRUE;
	}
	
	m_pRtmpMgr = CreateRtmpManager();
	m_pRtmpMgr->Init(this);

	m_bStop = FALSE;

	//for(int i = 0; i < DEF_THREAD_COUNT; i++)
	{
		CBaseThread loThread;
		loThread.BeginThread(FLVFramingThread,this);
	}


	//for(int i = 0; i < DEF_THREAD_COUNT; i++)
	{
		CBaseThread loThread;
		loThread.BeginThread(FLVEncodingSendThread,this);
	}

	return TRUE;
}

void FLVService::Close()
{
	m_bStop = TRUE;

	while(m_iThreadCount)
	{
		CBaseThread::Sleep(10);
	}

	for (int i = 0; i < DEF_THREAD_COUNT; i++)
	{
		delete []m_arrayFLVData[i];

		m_quFLV[i].Clear();
		m_quFLVPool[i].Clear();
	}
	
	if (m_pRtmpMgr)
	{
	  m_pRtmpMgr->Close();
	  ReleaseRtmpManager(m_pRtmpMgr);
	  m_pRtmpMgr = NULL;
	}

    for (int i = 0; i < DEF_THREAD_COUNT; i++)
    {
        CCriticalAutoLock loLock(m_CriForFlvMap[i]);
        for (auto iter = m_mapFLVInfo[i].begin(); iter != m_mapFLVInfo[i].end(); )
        {
                iter->second.m_oAudioBuff.CloseChannel();
                iter->second.m_oVideoBuff.CloseChannel();
                m_mapFLVInfo[i].erase(iter++);
        }
    }

    FLVFrame* lpFLVFrame = NULL;
    while(m_quPtrFLVFrame[0].Pop(lpFLVFrame) && lpFLVFrame)
    {
        delete lpFLVFrame;
    }
    
}

void FLVService::OnConnect(int64_t streamID, bool abResult)
{
    m_lockRtmpMap.Lock();
#ifndef DEF_RECONN_CDN
    m_oRtmpMap[streamID] = abResult;
#else
    //TraceLog6("[%ld] FLVService::OnConnect : %d\n", streamID, abResult);
    TraceLog0("== %lld == [OnConnect] : %d\n", streamID, abResult);
    time_t t = 0;

    auto it = m_oRtmpMap.find(streamID);
    if (it != m_oRtmpMap.end())
    {
        it->second.m_conSta = abResult;
        it->second.m_t      = t;
    }
    else
    {
        TraceLog0("FLVService::OnConnect : map insert - ID:%ld, Result:%d\n",streamID, abResult);
        m_oRtmpMap.insert(std::make_pair(streamID, RtmpSta(abResult, t)));
    }
#endif
    m_lockRtmpMap.UnLock();
}

void FLVService::OnDisconnet(int64_t streamID)
{
    m_lockRtmpMap.Lock();
#ifndef DEF_RECONN_CDN
    m_oRtmpMap[streamID] = 0;
#else
    //TraceLog6("[%ld] FLVService::OnDisconnet \n", streamID);
    TraceLog0("== %lld == [OnDisconnet]\n", streamID);
    // 需要立刻重连
    auto it = m_oRtmpMap.find(streamID);
    if (it != m_oRtmpMap.end())
    {
        it->second.m_conSta = false;
        it->second.m_t      = 0;
    }
    else
    {
        TraceLog0("FLVService::OnDisConnect : map insert - ID:%ld\n",streamID);
        m_oRtmpMap.insert(std::make_pair(streamID, RtmpSta(false, 0)));
    }
#endif
    m_lockRtmpMap.UnLock();
}

unsigned int FLVService::FLVFramingThread(void* apVoid)
{
	static int index = 0;
	FLVService* lpThis = (FLVService*)(apVoid);
	if (lpThis)
	{
		lpThis->m_oCriForThreadCount.Lock();
		lpThis->m_iThreadCount++;
		lpThis->m_oCriForThreadCount.UnLock();

		lpThis->FLVFraming(index++);

		lpThis->m_oCriForThreadCount.Lock();
		lpThis->m_iThreadCount--;
		lpThis->m_oCriForThreadCount.UnLock();
	}

	return 0;

}
unsigned int FLVService::FLVEncodingSendThread(void* apVoid)
{
	static int index = 0;
	FLVService* lpThis = (FLVService*)(apVoid);
	if (lpThis)
	{
		lpThis->m_oCriForThreadCount.Lock();
		lpThis->m_iThreadCount++;
		lpThis->m_oCriForThreadCount.UnLock();

		lpThis->FLVEncodingSendThread(index++);

		lpThis->m_oCriForThreadCount.Lock();
		lpThis->m_iThreadCount--;
		lpThis->m_oCriForThreadCount.UnLock();
	}

	return 0;
}

unsigned int FLVService::FLVEncodeThread(void* apVoid)
{
	static int index = 0;
	FLVService* lpThis = (FLVService*)(apVoid);
	if (lpThis)
	{
		lpThis->m_oCriForThreadCount.Lock();
		lpThis->m_iThreadCount++;
		lpThis->m_oCriForThreadCount.UnLock();

		//lpThis->FLVEncode(index++);

		lpThis->m_oCriForThreadCount.Lock();
		lpThis->m_iThreadCount--;
		lpThis->m_oCriForThreadCount.UnLock();
	}

	return 0;
}


void FLVService::DealRecvUdp(char *apRecvData,int aiDataLen)
{
	WORD wPackType = *(WORD*)(apRecvData);
	if (PACK_CMS_FLV_DATA == wPackType)
		DealCMSRtp(apRecvData,aiDataLen);

	if (PACK_SVR_LRDS_CMSP_DATA_ID == wPackType)
		DealLRDSData(apRecvData,aiDataLen);
}

void FLVService::DealLRDSData(char *apRecvData,int aiDataLen)
{
	STRU_SVR_LRDS_CMSP_DATA_ID loPack;
	if (!loPack.UnPack(apRecvData,aiDataLen))
	{
		TraceLog0("%s STRU_SVR_LRDS_CMSP_DATA_ID UnPack failed \n",__FUNCTION__);
		return;
	}

    

	m_i64UdpPacketCount++;	
	m_i64UdpInFlux += aiDataLen;

	STRU_MS_RDS_RTP_DATA loMSRDS;
	if (!loMSRDS.UnPack((char*)loPack.m_byData,loPack.m_wDataLen))
	{
		TraceLog0("%s STRU_MS_RDS_RTP_DATA UnPack failed \n",__FUNCTION__);
		return;
	}

    if(m_id != 0 && loMSRDS.m_oPackHead.m_i64UserID != m_id)
        return;

	CRtpDataFrame loRtpFrame(loMSRDS.m_szRtpData,1560,loMSRDS.m_nRtpDataLen);
    if (loRtpFrame.ReturnHeaderSize() > loMSRDS.m_nRtpDataLen)
    {
        TraceLog0("[%lld]RtpDataFrame error - headersize : %d > RtpDataLen : %d\n", loMSRDS.m_oPackHead.m_i64UserID, loRtpFrame.ReturnHeaderSize(), loMSRDS.m_nRtpDataLen);
        loRtpFrame.PrintData();
        return;
    }
	if (loRtpFrame.GetPayloadType() == e_RtcpSystemTime)
	{
		return;
	}

	STRU_CMS_FLV_DATA loCMSFLV;
	loCMSFLV.m_i64UserID = loMSRDS.m_oPackHead.m_i64UserID;
	loCMSFLV.m_byFPS = 12;
	loCMSFLV.m_wHeight = loMSRDS.m_sFrameHeight;
	loCMSFLV.m_wWidth = loMSRDS.m_sFrameWidth;
	loCMSFLV.m_byDevType = loMSRDS.m_byDevType;
    
	loCMSFLV.m_wRtpDataLen = (loMSRDS.m_nRtpDataLen > 1152) ? 1152 : loMSRDS.m_nRtpDataLen;
	memcpy(loCMSFLV.m_szRtpData,loMSRDS.m_szRtpData,loCMSFLV.m_wRtpDataLen);


	DealCMSFLV(loCMSFLV);
}
void FLVService::DealCMSRtp(char *apRecvData,int aiDataLen)
{
	STRU_CMS_FLV_DATA loCMSFLV;
	if (!loCMSFLV.UnPack(apRecvData,aiDataLen))
	{
		return;
	}

	m_i64UdpPacketCount++;	
	m_i64UdpInFlux += aiDataLen;

	DealCMSFLV(loCMSFLV);

}


void FLVService::DealCMSFLV(STRU_CMS_FLV_DATA& aoCMSFLV)
{
	//static INT64 li64LastCheckTime = CBaseThread::GetSystemTime();

	STRU_CMS_FLV_DATA& loCMSFLV = aoCMSFLV;

	CRtpDataFrame loRtpFrame(loCMSFLV.m_szRtpData,1560,loCMSFLV.m_wRtpDataLen);
    if (loRtpFrame.ReturnHeaderSize() > loCMSFLV.m_wRtpDataLen)
    {
        TraceLog0("== %lld == DealCMSFLV - RtpDataFrame error - headersize : %d > RtpDataLen : %d\n", loCMSFLV.m_i64UserID, loRtpFrame.ReturnHeaderSize(), loCMSFLV.m_wRtpDataLen);
        // loRtpFrame.PrintData();
        return;
    }
	UINT nChannelID = loRtpFrame.GetSyncSource();

    //if (loCMSFLV.m_i64UserID == 10002016129)
    {
        if (loCMSFLV.m_byDevType == DEF_VIDEO_DEVICE)
        {
            TraceLog6("== %lld == [Recv Video RtpFrame seq] : %lld - %llu, %lld, len : %lld\n", loCMSFLV.m_i64UserID, loRtpFrame.GetSequenceNumber(), loRtpFrame.GetTimestamp(), nChannelID, loCMSFLV.m_wRtpDataLen);
        }
        else
        {
            TraceLog6("== %lld == [Recv Audio RtpFrame seq] : %lld - %llu, %lld, len : %lld\n", loCMSFLV.m_i64UserID, loRtpFrame.GetSequenceNumber(), loRtpFrame.GetTimestamp(), nChannelID, loCMSFLV.m_wRtpDataLen);
        }
    }


	//int lnSlice = loCMSFLV.m_i64UserID % DEF_THREAD_COUNT;
	int lnSlice = 0;//loCMSFLV.m_i64UserID & (DEF_THREAD_COUNT - 1);
	{
		//INT64 li64Now = CBaseThread::GetSystemTime();
		//if (li64Now - li64LastCheckTime > 10 * 60 * 1000)
		//{
			//for (int i = 0; i < DEF_THREAD_COUNT; i++)
			//{
				//CCriticalAutoLock loLock(m_CriForFlvMap[i]);
				//for (auto iter = m_mapFLVInfo[i].begin(); iter != m_mapFLVInfo[i].end(); )
				//{
					//if (li64Now - iter->second.m_i64LastRtpTime > 10 * 60 * 1000)
					//{
						//iter->second.m_oAudioBuff.CloseChannel();
						//iter->second.m_oVideoBuff.CloseChannel();
						//m_mapFLVInfo[i].erase(iter++);
					//}
					//else
					//{
						//++iter;
					//}
				//}
			//}
			//li64LastCheckTime = li64Now;
		//}

		CCriticalAutoLock loLock(m_CriForFlvMap[lnSlice]);
		auto pos = m_mapFLVInfo[lnSlice].find(loCMSFLV.m_i64UserID);
		if (pos != m_mapFLVInfo[lnSlice].end())
		{
            INT64 tmpTime = CBaseThread::GetSystemTime();

			if (loCMSFLV.m_byDevType == DEF_AUDIO_DEVICE)
			{
                if (pos->second.m_oAudioBuff.m_nQueueTailPos == pos->second.m_oAudioBuff.m_nQueueHeadPos)
                {
                    TraceLog5("== %lld == [RtpFrame Buff Empty ? Full ? Audio] : %d(%d-%d), recv seq : %lld\n", loCMSFLV.m_i64UserID,pos->second.m_oAudioBuff.GetQueueLen(), pos->second.m_oAudioBuff.m_nQueueTailPos, pos->second.m_oAudioBuff.m_nQueueHeadPos, loRtpFrame.GetSequenceNumber());
                }

				int ret = pos->second.m_oAudioBuff.InsertRtpFrame(&loRtpFrame);
				pos->second.m_i64LastRtpTime = tmpTime;

                if (ret == 0)
                {
                    pos->second.m_lastAudioSeq = loRtpFrame.GetSequenceNumber();
                    pos->second.m_lastRecvAudio += 1;
                    //pos->second.m_lastRecvAudio += loCMSFLV.m_wRtpDataLen;
                    pos->second.m_firstRecvAudio = false;
                }
                else
                {
                    pos->second.m_insertAudioFailed += 1;
                    TraceLog6("== %lld == [InsertRtpFrame Audio Failed] : %d\n", loCMSFLV.m_i64UserID,ret);

                    // TODO 聚范推流一个异常判断，可能影响其他系统推流
                    // 判断是否是新的音频流。根据新包序号与上个收到的包序号大小比对。
                    // 0---cur++++++++++++++++++++++++last---65535
                    // cur last 之间距离在 100 ~ 65536-200 之间，认为异常，清空现有音频数据。
                    if (!pos->second.m_firstRecvAudio)
                    {
                        if ( loRtpFrame.GetSequenceNumber() < pos->second.m_lastAudioSeq  && 
                                ( pos->second.m_lastAudioSeq - loRtpFrame.GetSequenceNumber() > 20 
                                  && pos->second.m_lastAudioSeq - loRtpFrame.GetSequenceNumber() < 65536- 20 ) )
                        {
                            TraceLog0("== %lld == [Before Reset Audio Buff] : %d(%d-%d), recv seq : %lld\n", loCMSFLV.m_i64UserID,pos->second.m_oAudioBuff.GetQueueLen(), pos->second.m_oAudioBuff.m_nQueueTailPos, pos->second.m_oAudioBuff.m_nQueueHeadPos, loRtpFrame.GetSequenceNumber());
                            pos->second.m_oAudioBuff.ResetChannel();
                            pos->second.m_firstRecvAudio = true;
                            TraceLog0("== %lld == [Rest Audio Buff] : last seq : %lld, recv seq : %lld\n", loCMSFLV.m_i64UserID, pos->second.m_lastAudioSeq, loRtpFrame.GetSequenceNumber());
                        }
                    }
                }

                if (pos->second.m_i64LastRtpTime - pos->second.m_i64LastRtpTimeAudio > 30000)   // 30s 输出一次
                {
                    TraceLog0("== %lld == [30s InsertRtpFrame Audio] : %llu, [Failed] : %llu\n", loCMSFLV.m_i64UserID, pos->second.m_lastRecvAudio, pos->second.m_insertAudioFailed);
                    pos->second.m_lastRecvAudio = 0;
                    pos->second.m_insertAudioFailed = 0;
                    pos->second.m_i64LastRtpTimeAudio = pos->second.m_i64LastRtpTime;
                }
			}

			if (loCMSFLV.m_byDevType == DEF_VIDEO_DEVICE)
			{
                if (tmpTime - pos->second.m_i64LastRtpTime > 3000) // 3s 没数据，原数据清空
                {
                    pos->second.m_oVideoBuff.ResetChannel();
                    pos->second.m_oAudioBuff.ResetChannel();
                    pos->second.m_unVideoChannelID = nChannelID;
                    pos->second.m_unFlowID = 0;
                    TraceLog0("== %lld == [%dms NoRtpData]\n", loCMSFLV.m_i64UserID,tmpTime - pos->second.m_i64LastRtpTime);
                }

                if (pos->second.m_oVideoBuff.m_nQueueTailPos == pos->second.m_oVideoBuff.m_nQueueHeadPos)
                {
                    TraceLog0("== %lld == [RtpFrame Buff Empty ? Full ? Video] : %d(%d-%d), recv seq : %lld\n", loCMSFLV.m_i64UserID,pos->second.m_oVideoBuff.GetQueueLen(), pos->second.m_oVideoBuff.m_nQueueTailPos, pos->second.m_oVideoBuff.m_nQueueHeadPos, loRtpFrame.GetSequenceNumber());
                }

                //TraceLog6("== %lld == [RtpFrame Buff Video] : %d(%d-%d)\n", loCMSFLV.m_i64UserID,pos->second.m_oVideoBuff.GetQueueLen(), pos->second.m_oVideoBuff.m_nQueueTailPos, pos->second.m_oVideoBuff.m_nQueueHeadPos);
				if (pos->second.m_unVideoChannelID != nChannelID)
				{
                    TraceLog0("== %lld == [ChannelID change] : %lld -> %lld\n", loCMSFLV.m_i64UserID, pos->second.m_unVideoChannelID, nChannelID);
					pos->second.m_oVideoBuff.ResetChannel();
					pos->second.m_oAudioBuff.ResetChannel();
					pos->second.m_unVideoChannelID = nChannelID;
					pos->second.m_unFlowID = 0;
				}

				int ret = pos->second.m_oVideoBuff.InsertRtpFrame(&loRtpFrame);

                if (ret == 0)
                {
                    pos->second.m_oVideoBuff.m_wVideoHeight = loCMSFLV.m_wHeight;
                    pos->second.m_oVideoBuff.m_wVideoWidth = loCMSFLV.m_wWidth;
                    pos->second.m_lastRecvVideo += 1;
                    //pos->second.m_lastRecvVideo += loCMSFLV.m_wRtpDataLen;
                }
                else
                {
                    TraceLog6("== %lld == [InsertRtpFrame Video Failed] : %d\n", loCMSFLV.m_i64UserID, ret);
                    pos->second.m_insertVideoFailed += 1;
                }

				pos->second.m_i64LastRtpTime = tmpTime;
                if (pos->second.m_i64LastRtpTime - pos->second.m_i64LastRtpTimeVideo > 30000)   // 30s 输出一次
                {
                    TraceLog0("== %lld == [30s InsertRtpFrame Video] : %llu, [Failed] : %llu\n", loCMSFLV.m_i64UserID, pos->second.m_lastRecvVideo, pos->second.m_insertVideoFailed);
                    pos->second.m_lastRecvVideo = 0;
                    pos->second.m_insertVideoFailed = 0;
                    pos->second.m_i64LastRtpTimeVideo = pos->second.m_i64LastRtpTime;
                }
			}
		}
		else
		{
			if (loCMSFLV.m_byDevType != DEF_VIDEO_DEVICE)
			{
				//收到视频包时才生成请求
                TraceLog6("FLVService::DealCMSFLV return : %lld\n", loCMSFLV.m_i64UserID);
				return;
			}
			
    
            TraceLog0("m_mapFLVInfo insert :%lld\n" , loCMSFLV.m_i64UserID );

            //if (m_mapFLVInfo[lnSlice].size() > 10) {
                //return;
            //}
			STRU_FLOW_INFO loFlowInfo;
			pos = m_mapFLVInfo[lnSlice].insert(
                    std::make_pair(loCMSFLV.m_i64UserID,loFlowInfo)).first;
            // 新创建，第一次发送，会等 anJitterTime ms
            int anJitterTime = 1500;
			pos->second.m_oAudioBuff.CreateChannel(0,DEF_AUDIO_DEVICE,0,1200,anJitterTime); 
			pos->second.m_oVideoBuff.CreateChannel(0,DEF_VIDEO_DEVICE,0,400,anJitterTime);
			pos->second.m_oVideoBuff.SetAudioChannel(&pos->second.m_oAudioBuff);
			pos->second.m_unVideoChannelID = nChannelID;
			pos->second.m_oVideoBuff.InsertRtpFrame(&loRtpFrame);
			pos->second.m_i64LastRtpTime = CBaseThread::GetSystemTime();
            TraceLog0("== %lld == [Insert m_mapFLVInfo]\n", loCMSFLV.m_i64UserID);
		}

	}
}

void FLVService::FLVFraming(int aiThreadIndex)
{
	char aszDataBuf[307200];

	BOOL lbNeedSleep = TRUE;
    INT64 last_time = 0;

	INT64 li64LastCheckTime  = CBaseThread::GetSystemTime();

    INT64 full_times = 0;
	while(!m_bStop)
	{
        INT64 curTime  = CBaseThread::GetSystemTime();

        for(int i = 0 ; i < 1/*DEF_THREAD_COUNT*/; i++)
		{
            lbNeedSleep = TRUE;

            aiThreadIndex = i;
			CCriticalAutoLock loLock(m_CriForFlvMap[aiThreadIndex]);
            {
                if (curTime - li64LastCheckTime > 60*1000)
                {
                    TraceLog0("[m_mapFLVInfo]  +++++++++++ size : %lld\n", m_mapFLVInfo[aiThreadIndex].size());
                    TraceLog0("[m_quPtrFLVFrame] +++++++++ size : %lld\n", m_quPtrFLVFrame[0].GetQueueLen());
                    
                    li64LastCheckTime = curTime;
                }

                std::unordered_map<INT64,STRU_FLOW_INFO>& mapFLVInfo 
                    =  m_mapFLVInfo[aiThreadIndex];

                auto iterLoop = mapFLVInfo.begin(); 
                auto iterEnd = mapFLVInfo.end();

                INT64 li64Now = CBaseThread::GetSystemTime();
                for (; iterLoop != iterEnd; /*++iterLoop*/ )
                {
                    UINT uReadLen = 0;
                    BOOL bMark = FALSE;
                    BOOL bBigVideo = FALSE;
                    UINT ulTimestamp = 0;
                    int ret = 0;

                    if (li64Now - iterLoop->second.m_i64LastRtpTime > 10 * 60 * 1000)
                    {
                        iterLoop->second.m_oAudioBuff.CloseChannel();
                        iterLoop->second.m_oVideoBuff.CloseChannel();

                        TraceLog0("== %lld == [Delete m_mapFLVInfo]\n" , iterLoop->first );
                        
                        m_mapFLVInfo[aiThreadIndex].erase(iterLoop++);
                        continue;
                    }

                    INT64 readPackNum = 0;
                    INT64 lostPackNum = 0;

                    if (iterLoop->second.m_oVideoBuff.IsCreated())
                    {
                        ret = iterLoop->second.m_oVideoBuff.ReadRtpFrame(aszDataBuf,307200,uReadLen,bMark,bBigVideo,ulTimestamp, readPackNum, lostPackNum);
                        if (ret == 0 || ret == -1)
                        {
                            lbNeedSleep = FALSE;
                            if (ret == -1)
                            {
                                iterLoop->second.m_i64LostPktNumV += lostPackNum;
                                
                                iterLoop->second.m_i64ReadRtpErrorTimes++;
                                iterLoop->second.m_bLostFrame = TRUE;

                                ++iterLoop;
                                continue;
                            }

                            iterLoop->second.m_i64ReadPktNumV += readPackNum;

                            iterLoop->second.m_i64ReadRtpTotalTimes++;

                            if (bMark)
                            {
                                iterLoop->second.m_bLostFrame = FALSE;
                            }
                            else
                            {
                                if (iterLoop->second.m_bLostFrame)
                                {
                                    ++iterLoop;
                                    continue;
                                }
                            }


                            FLVFrame* lpFLVFrame = new FLVFrame();
                            lpFLVFrame->m_bMark = bMark;
                            lpFLVFrame->m_byDevType = DEF_VIDEO_DEVICE;
                            lpFLVFrame->m_i64UserID = iterLoop->first;
                            lpFLVFrame->m_nLen = uReadLen;
                            lpFLVFrame->m_pData = new char[uReadLen];
                            memcpy(lpFLVFrame->m_pData,aszDataBuf,uReadLen);
                            lpFLVFrame->m_unAudioChannelID = iterLoop->second.m_unAudioChannelID;
                            lpFLVFrame->m_unVideoChannelID = iterLoop->second.m_unVideoChannelID;
                            lpFLVFrame->m_unFlowID = iterLoop->second.m_unFlowID;
                            lpFLVFrame->m_wVideoInfoWidth = iterLoop->second.m_oVideoBuff.m_wVideoWidth;
                            lpFLVFrame->m_wVideoInfoHeigth = iterLoop->second.m_oVideoBuff.m_wVideoHeight;
                            lpFLVFrame->m_unTimestamp = ulTimestamp;
                            
                            if (!m_quPtrFLVFrame[aiThreadIndex].Push(lpFLVFrame))
                            {
                                full_times++;
                                //TraceLog0("%s m_quPtrFLVFrame[%d] is full\n",__FUNCTION__,aiThreadIndex);
                                delete lpFLVFrame;
                            }
                            else
                            {
                                iterLoop->second.m_lastPushRtpVideo += uReadLen;
                                INT64 tmpTime = CBaseThread::GetSystemTime();
                                if (tmpTime - iterLoop->second.m_i64LastPushRtmpTimeVideo > 30000)
                                {
                                    TraceLog0("== %lld == [30s ReadRTP - PushFLV Video] : %lld, [Read-Lost] : %lld-%lld\n", iterLoop->first, iterLoop->second.m_lastPushRtpVideo, iterLoop->second.m_i64ReadPktNumV, iterLoop->second.m_i64LostPktNumV);

                                    iterLoop->second.m_i64LostPktNumV = 0;
                                    iterLoop->second.m_i64ReadPktNumV = 0;
                                    iterLoop->second.m_lastPushRtpVideo = 0;
                                    iterLoop->second.m_i64LastPushRtmpTimeVideo = tmpTime;
                                }
                            }
                        }
                        else
                        {
                            iterLoop->second.m_i64LostPktNumV += lostPackNum;
                            TraceLog6("== %lld == [ReadRtpFrame E Video] :  %d-%d(%d-%d)-%d\n", iterLoop->first,iterLoop->second.m_oVideoBuff.IsCreated(), iterLoop->second.m_oVideoBuff.GetQueueLen(), iterLoop->second.m_oVideoBuff.m_nQueueTailPos, iterLoop->second.m_oVideoBuff.m_nQueueHeadPos, ret);
                        }
                    }
                    else
                    {
                        TraceLog6("== %lld == [ReadRtpFrame E Video] : m_oVideoBuff Is not Created", iterLoop->first);
                    }

                    readPackNum = 0;
                    lostPackNum = 0;

                    if  (iterLoop->second.m_oAudioBuff.IsCreated() &&
                            ( (ret = iterLoop->second.m_oAudioBuff.ReadRtpFrame(aszDataBuf,409600,uReadLen,bMark,bBigVideo,ulTimestamp, readPackNum, lostPackNum)) == 0 )
                        )
                    {
                        iterLoop->second.m_i64ReadPktNumA += readPackNum;

                        lbNeedSleep = FALSE;

                        FLVFrame* lpFLVFrame = new FLVFrame();
                        lpFLVFrame->m_bMark = bMark;
                        lpFLVFrame->m_byDevType = DEF_AUDIO_DEVICE;
                        lpFLVFrame->m_i64UserID = iterLoop->first;
                        lpFLVFrame->m_nLen = uReadLen;
                        lpFLVFrame->m_pData = new char[uReadLen];
                        memcpy(lpFLVFrame->m_pData,aszDataBuf,uReadLen);
                        lpFLVFrame->m_unAudioChannelID = iterLoop->second.m_unAudioChannelID;
                        lpFLVFrame->m_unVideoChannelID = iterLoop->second.m_unVideoChannelID;
                        lpFLVFrame->m_unFlowID = iterLoop->second.m_unFlowID;
                        lpFLVFrame->m_unTimestamp = ulTimestamp;

                        if (!m_quPtrFLVFrame[aiThreadIndex].Push(lpFLVFrame))
                        {
                            full_times++;
                            //TraceLog0("%s m_quPtrFLVFrame[%d]is full\n",__FUNCTION__,aiThreadIndex);
                            delete lpFLVFrame;
                        }
                        else
                        {
                            iterLoop->second.m_lastPushRtpAudio += uReadLen;
                            INT64 tmpTime = CBaseThread::GetSystemTime();
                            if (tmpTime - iterLoop->second.m_i64LastPushRtmpTimeAudio > 30000)
                            {
                                TraceLog0("== %lld == [30s ReadRTP - PushFLV Audio] : %lld, [Read-Lost] : %lld-%lld\n", iterLoop->first, iterLoop->second.m_lastPushRtpAudio, iterLoop->second.m_i64ReadPktNumA, iterLoop->second.m_i64LostPktNumA);

                                iterLoop->second.m_i64LostPktNumA = 0;
                                iterLoop->second.m_i64ReadPktNumA = 0;
                                iterLoop->second.m_lastPushRtpAudio = 0;
                                iterLoop->second.m_i64LastPushRtmpTimeAudio = tmpTime;
                            }
                        }
                    }
                    else
                    {
                        iterLoop->second.m_i64LostPktNumA += lostPackNum;

                        TraceLog6("== %lld == [ReadRtpFrame E Audio] : %d-%d(%d-%d)-%d\n", iterLoop->first,iterLoop->second.m_oAudioBuff.IsCreated(), iterLoop->second.m_oAudioBuff.GetQueueLen(), iterLoop->second.m_oAudioBuff.m_nQueueTailPos, iterLoop->second.m_oAudioBuff.m_nQueueHeadPos, ret);
                    }

                    ++iterLoop;
                }
            }
		}

        if (full_times && CBaseThread::GetSystemTime() - last_time > 10000)
        {
            TraceLog0("full_times:%d\n",full_times);
            last_time = CBaseThread::GetSystemTime();
            full_times = 0;
        }
		CBaseThread::Sleep(5);
	}

    TraceLog0("FLVFraming ================================ thread exit.\n");
}

void FLVService::FLVEncodingSendThread(int aiThreadIndex)
{
	char chFLV[409600];
	std::unordered_map<INT64,FLVEncoder> mapFLVEncoder;
	INT64 li64LastCheckTime  = CBaseThread::GetSystemTime();
	INT64 li64LastUpdateFileTime = 0;
	INT64 dump_interval = 20000;//20 * 1000;
	INT64 check_interval = 120000;//2 * 60 * 1000;
	INT64 last_dump_time = 0;
	int out_flow = 0;
	int bConnResult = CONNSTA_CONN_BREAK;
    long timeDiff = 0;
#ifndef DEF_RECONN_CDN
	std::unordered_map<INT64, int>::iterator iter_conn;
#else
	std::unordered_map<INT64, RtmpSta>::iterator iter_conn;
#endif
	TraceLog0("thread_%d start.\n",aiThreadIndex);

	while(!m_bStop)
	{
		INT64 li64Now = CBaseThread::GetSystemTime();
		if (li64Now - li64LastUpdateFileTime > dump_interval)
		{
            TraceLog0("[mapFLVEncoder] ----------- size : %lld\n", mapFLVEncoder.size());
            TraceLog0("[m_oRtmpMap]    ----------- size : %lld\n", m_oRtmpMap.size());

			char tmpFileName[100]={0};
			char tmpContent[1000] = {0};
			sprintf(tmpFileName,"thread_%d.txt",aiThreadIndex);
			FILE* fp = fopen(tmpFileName,"w");
			if (fp)
			{
				fputs("\n",fp);
				for (auto iter = mapFLVEncoder.begin(); iter != mapFLVEncoder.end(); ++iter)
				{
					sprintf(tmpContent,
                            "[userid:%lld,flowid:%d,ToLastSendTime->:%lld\n]",
			    iter->first,iter->second.m_unFlowID,
			    li64Now - iter->second.m_i64LastSendTime);
					fputs(tmpContent,fp);
				}

				fclose(fp);
			}
            else
            {
                TraceLog0("[tmpFileName]  %s open error : %s\n", tmpFileName, strerror(errno));
            }

			li64LastUpdateFileTime = li64Now;
		}

		if (li64Now - li64LastCheckTime > check_interval)
		{
			for (auto iter = mapFLVEncoder.begin(); iter != mapFLVEncoder.end(); )
			{
				if (li64Now - iter->second.m_i64LastSendTime > check_interval)
				{
					if (iter->second.m_hFLVEncoder)
					{
						FLVClose(iter->second.m_hFLVEncoder);
					}
					m_pRtmpMgr->disconnect(iter->second.m_i64UserID);

                   /* if (iter->second.m_rtmp)
                    {
                        RTMP_Close(iter->second.m_rtmp);
                        RTMP_Free(iter->second.m_rtmp);
                    }
		*/
                    m_lockRtmpMap.Lock();
                    m_oRtmpMap.erase(iter->second.m_i64UserID);
                    m_lockRtmpMap.UnLock();

					mapFLVEncoder.erase(iter++);
                }
                else
				{
					++iter;
				}
			}
			li64LastCheckTime = li64Now;
		}

		FLVFrame* lpFLVFrame = NULL;
		BOOL lbNeedSleep = TRUE;
        INT64   dealCount = 0;
		while(m_quPtrFLVFrame[aiThreadIndex].Pop(lpFLVFrame) && lpFLVFrame)
		{
            ++ dealCount;
			lbNeedSleep = FALSE;

			std::unordered_map<INT64,FLVEncoder>::iterator iter;

			iter = mapFLVEncoder.find(lpFLVFrame->m_i64UserID);
			if (iter == mapFLVEncoder.end())
            {
                FLVEncoder loEncoder;
                iter = mapFLVEncoder.insert(
                        std::make_pair(lpFLVFrame->m_i64UserID,loEncoder)).first;

                iter->second.m_i64UserID = lpFLVFrame->m_i64UserID;
                iter->second.m_hFLVEncoder = FlvCreate(lpFLVFrame->m_unFlowID);
            }

        #ifdef DEF_RECONN_CDN
            m_lockRtmpMap.Lock();
            bConnResult = CONNSTA_CONN_BREAK;
            iter_conn = m_oRtmpMap.find(iter->second.m_i64UserID);
            if (iter_conn != m_oRtmpMap.end())
            {
                bConnResult = iter_conn->second.m_conSta;
                if (bConnResult == CONNSTA_CONN_BREAK)
                {
                    TraceLog0("== %lld == [Reset ConnStatus]\n",iter->second.m_i64UserID);
                    iter->second.m_status = no_init;
                    iter_conn->second.m_conSta = CONNSTA_CONNING;
                }
            }
            //TODO
            //else
            //{
                //// 有可能第一次进来，需要连接
                //// 有可能已经连接，  还没有调用OnConnect 或 OnDisConnect，需要等等再连
                //TraceLog0("FLVService::FLVEncodingSendThread : map insert - ID:%ld\n",iter->second.m_i64UserID);
                //m_oRtmpMap.insert(std::make_pair(iter->second.m_i64UserID, RtmpSta(false, time(NULL))));
            //}
            m_lockRtmpMap.UnLock();
        #endif
            if (iter->second.m_status == no_init)
            {
                iter->second.m_i64ConnTime = li64Now;

                std::string rtmp_url;
                get_push_url(iter->second.m_i64UserID,
                        rtmp_url);

                //TraceLog0("thread:%d,to be init rtmp ,userid:%lld,%s\n",
                        //aiThreadIndex,
                        //iter->second.m_i64UserID,
                        //rtmp_url.c_str());

                TraceLog0("== %lld == [Init Rtmp] : %s\n",iter->second.m_i64UserID, rtmp_url.c_str());
                if (init_rtmp(
                            iter->second.m_i64UserID,
                            const_cast<char*>( rtmp_url.c_str()
                                )
                            )
                        )
                {
                    iter->second.m_status = inited;
                    TraceLog0("== %lld == [Init Rtmp] : inited\n",iter->second.m_i64UserID);
                }
                else
                {
                    //TraceLog0("thread:%d,init rtmp fail,userid:%lld,%s\n",
                            //aiThreadIndex,
                            //iter->second.m_i64UserID,
                            //rtmp_url.c_str());

                    TraceLog0("== %lld == [Init Rtmp] : Failed\n",iter->second.m_i64UserID);
                    goto Release;
                }
            }
        #ifndef DEF_RECONN_CDN
            m_lockRtmpMap.Lock();
            bConnResult = CONNSTA_CONN_BREAK;
            iter_conn = m_oRtmpMap.find(iter->second.m_i64UserID);
            if (iter_conn != m_oRtmpMap.end())
            {
                bConnResult = iter_conn->second;
            }
            m_lockRtmpMap.UnLock();
        #endif

            if (iter->second.m_status == inited && bConnResult==CONNSTA_CONNED)
            {
				if (lpFLVFrame->m_bMark 
                        && lpFLVFrame->m_byDevType == DEF_VIDEO_DEVICE)
                {
                    TraceLog0("== %lld == [Send Header] : Ready\n",iter->second.m_i64UserID);
                    //send header;
                    char buf[1024];
                    int len = 0;

                    int ret = FLVAudioInfo(iter->second.m_hFLVEncoder,
                            48000,2,24000,16);
                    if (ret < 0)
                    {
                        TraceLog0("== %lld == [Send Header] : Release 1: %d\n",iter->second.m_i64UserID, ret);
                        goto Release;
                    }

                    ret = FLVVideoInfo(iter->second.m_hFLVEncoder,
                            lpFLVFrame->m_wVideoInfoWidth,
                            lpFLVFrame->m_wVideoInfoHeigth,12);
                    if (ret < 0)
                    {
                        TraceLog0("== %lld == [Send Header] : Release 2: %d\n",iter->second.m_i64UserID, ret);
                        goto Release;
                    }

					len = FLVWriteHeader(iter->second.m_hFLVEncoder,
                            (unsigned char*)chFLV,1024,
                            (unsigned char*)lpFLVFrame->m_pData,
						lpFLVFrame->m_nLen,
                        lpFLVFrame->m_unTimestamp,iter->second.m_i64UserID);

                    if (len < 0)
                    {
                        TraceLog0("== %lld == [Send Header] : Release 3: %d\n",iter->second.m_i64UserID, ret);
                        goto Release;
                    }

                    if (send_header(iter->second.m_i64UserID,chFLV+296,len - 296,
                                lpFLVFrame->m_unTimestamp))
                    {
                        //TraceLog6("thread_%d,send rtmp header ok,userid:%lld\n",
                                //aiThreadIndex,iter->second.m_i64UserID);
                        
                        TraceLog0("== %lld == [Send Header] : OK, [Timeuse] : %lld\n",iter->second.m_i64UserID, li64Now - iter->second.m_i64ConnTime);
                        iter->second.m_status = sended_header;
                        out_flow += (len - 296);
                        //TraceLog0("send length:%d\n",len);
                        iter->second.m_i64LastSendTime = 
                            CBaseThread::GetSystemTime();
                    }
                    else
                    {
                        iter->second.m_status = no_init;
                        //TraceLog0("thread_%d,send rtmp header fail,userid:%lld\n",
                                //aiThreadIndex,iter->second.m_i64UserID);
                        TraceLog0("== %lld == [Send Header] : Failed\n",iter->second.m_i64UserID);
                    }
                
                }
                else
                {
                    TraceLog0("== %lld == [Send Header] : Not Ready : %d-%d\n",iter->second.m_i64UserID, lpFLVFrame->m_bMark, lpFLVFrame->m_byDevType);
                    goto Release;
                }
            }

            if (iter->second.m_status == sended_header)
            {
                int len = 0;
                if (lpFLVFrame->m_byDevType == DEF_VIDEO_DEVICE)
                {
                    len = FLVWriteVideoFrame(iter->second.m_hFLVEncoder,
                            iter->second.m_unVideoIndex++,
                            (char*)(chFLV), 409600,
                            lpFLVFrame->m_unTimestamp,lpFLVFrame->m_bMark,
                            (unsigned char*)lpFLVFrame->m_pData,
                            lpFLVFrame->m_nLen,iter->second.m_i64UserID);
                }
                else
                {
                    len = FLVWriteAudioFrame(iter->second.m_hFLVEncoder,
                            iter->second.m_unAudioIndex++,
                            (char*)(chFLV),409600,lpFLVFrame->m_unTimestamp,
                            (unsigned char*)lpFLVFrame->m_pData, lpFLVFrame->m_nLen);
                }

                if (len <= 0)
                {
                    //flv encode fail
                    iter->second.m_status = no_init;
                    goto Release;
                }

                if (!send_rtmp_packet(iter->second.m_i64UserID,
                            lpFLVFrame->m_byDevType,lpFLVFrame->m_bMark,
                            chFLV+11,len - 11 - 4,lpFLVFrame->m_unTimestamp))
                {
                    //TraceLog0("thread_%d,send rtmp fail,userid:%lld,devtype:%d\n",
                            //aiThreadIndex,iter->second.m_i64UserID,lpFLVFrame->m_byDevType);
                    TraceLog0("== %lld == [Send RtmpData] : Failed, [Type] : %d\n",iter->second.m_i64UserID, lpFLVFrame->m_byDevType);

                    iter->second.m_status = no_init;
                }
                else
                {
                    TraceLog6("thread_%d,send rtmp suc,userid:%lld,devtype:%d, %d\n",
                            aiThreadIndex,iter->second.m_i64UserID,lpFLVFrame->m_byDevType, lpFLVFrame->m_bMark);

                    iter->second.m_i64LastSendTime = CBaseThread::GetSystemTime();
                    if (lpFLVFrame->m_byDevType == DEF_VIDEO_DEVICE)
                    {
                        iter->second.m_sendVideo += (len - 15);
                        if (iter->second.m_i64LastSendTime - iter->second.m_i64LastSendTimeVideo > 30000)
                        {
                            TraceLog0("== %lld == [30s Send RtmpData Video] : %lld\n",iter->second.m_i64UserID, iter->second.m_sendVideo);
                            iter->second.m_i64LastSendTimeVideo = iter->second.m_i64LastSendTime;
                            iter->second.m_sendVideo = 0;
                        }
                    }
                    else
                    {
                        iter->second.m_sendAudio += (len - 15);
                        if (iter->second.m_i64LastSendTime - iter->second.m_i64LastSendTimeAudio > 30000)
                        {
                            TraceLog0("== %lld == [30s Send RtmpData Audio] : %lld\n",iter->second.m_i64UserID, iter->second.m_sendAudio);
                            iter->second.m_i64LastSendTimeAudio = iter->second.m_i64LastSendTime;
                            iter->second.m_sendAudio = 0;
                        }
                    }
                    
                    out_flow += (len - 15);
                }
            }


Release:
			delete lpFLVFrame;
			lpFLVFrame = NULL;

            if (dealCount > 50000)
            {
                break;
            }
		}

		if (lbNeedSleep)
		{
			CBaseThread::Sleep(1);
		}

        if (li64Now - last_dump_time > 180000)//3 * 60 * 1000))
        {
            TraceLog0("thread_%d,out_flux:%dKB,stream count:%d\n",
                    aiThreadIndex,out_flow/1000/180,mapFLVEncoder.size());

            last_dump_time = li64Now;
            out_flow = 0;
	    //TraceLog0("%lld,%lld\n,",li64Now,last_dump_time);
        }
    }

    TraceLog0("thread_%d exit.\n",aiThreadIndex);
}

bool FLVService::init_rtmp(INT64 ai64StreamID, char* rtmp_url)
{
 	if (NULL == m_pRtmpMgr) {
		return false;
	}
	return m_pRtmpMgr->connect(ai64StreamID, rtmp_url, strlen(rtmp_url));
/*
    if (r)
    {
        RTMP_Close(r);
        RTMP_Free(r);
    }

    r = RTMP_Alloc();

    RTMP_Init(r);
    r->Link.timeout=5;
    RTMP_SetupURL(r,rtmp_url);
    RTMP_EnableWrite(r);
    return (RTMP_Connect(r,NULL) && 
        RTMP_ConnectStream(r,0));
*/
}

bool FLVService::send_header(INT64 ai64StreamID, char* data,
        int len, unsigned int timestamp)
{
	return m_pRtmpMgr->SendAudioData(ai64StreamID, data, 4, timestamp) &&
		m_pRtmpMgr->SendVideoData(ai64StreamID, data + 4, len - 4, true, timestamp, NULL);
	
/*
    return send_rtmp_packet(r,DEF_AUDIO_DEVICE ,data,4,
            timestamp) && 
        send_rtmp_packet(r,DEF_VIDEO_DEVICE,data + 4,len - 4,timestamp);
*/
}

bool FLVService::send_rtmp_packet(INT64 ai64StreamID, int type, bool bKeyMark, 
        char* data, int len, unsigned int timestamp)
{
	if (DEF_AUDIO_DEVICE == type)
	{
	 return m_pRtmpMgr->SendAudioData(ai64StreamID, data, len, timestamp);
	}
	else if (DEF_VIDEO_DEVICE == type)
	{
	 return m_pRtmpMgr->SendVideoData(ai64StreamID, data, len, bKeyMark, timestamp, NULL);
	}

	return false;
/*
    RTMPPacket packet;
    char* body = (char*)malloc(RTMP_MAX_HEADER_SIZE+len);
    packet.m_body = body + RTMP_MAX_HEADER_SIZE;
    packet.m_nBodySize = len;
    memcpy(packet.m_body,data,len);
    packet.m_hasAbsTimestamp = TRUE;
    packet.m_packetType = (type == DEF_AUDIO_DEVICE) ? 
            RTMP_PACKET_TYPE_AUDIO : RTMP_PACKET_TYPE_VIDEO;
    packet.m_nInfoField2 = r->m_stream_id;
    packet.m_nChannel = (type == DEF_AUDIO_DEVICE) ? 
            0x05 : 0x04;
    packet.m_headerType = RTMP_PACKET_SIZE_LARGE;

    packet.m_nTimeStamp = timestamp;

    int ret = 0;
    if (RTMP_IsConnected(r))
    {
        ret =  RTMP_SendPacket(r,&packet,TRUE);
    }

    free(body);

    return ret;
*/
}

void FLVService::get_push_url(INT64 uid,
        std::string& url)
{
    char buf[200]  = {0};
    sprintf(buf,"%s/%lld",m_push_url.c_str(),uid);
    url = std::string(buf);

}
