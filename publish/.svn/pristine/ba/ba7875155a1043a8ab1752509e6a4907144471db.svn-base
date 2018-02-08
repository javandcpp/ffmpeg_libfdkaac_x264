#pragma once

#include <memory>
#include <list>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unordered_map>
#include <string>
#include "TypeDeff.h"
#include "CircleQueue.h"
#include "flv.h"
#include "CriticalLock.h"
#include "BaseThread.h"
#include "RtmpTransBuff.h"
#include "LRDSPackDeff.h"
#include "MediaPackDeff.h"
//#include "rtmp.h"
//#include "rtmp_sys.h"
#include "IRtmpManager.h"

#define DEF_THREAD_COUNT (2 << 4)

#define DEF_RECONN_CDN

enum rtmp_status
{
    no_init,
    inited,
    sended_header
};

struct STRU_FLOW_INFO
{
	INT64	m_i64UserID;
	UINT	m_unVideoChannelID;
	UINT	m_unAudioChannelID;
	BYTE	m_byDevType;
	UINT	m_unFlowID;
	INT64	m_i64LastRtpTime;
	INT64	m_i64LastRtpTimeVideo;
	INT64	m_i64LastRtpTimeAudio;
	INT64	m_i64LastPushRtmpTimeVideo;
	INT64	m_i64LastPushRtmpTimeAudio;
    INT64   m_insertAudioFailed;
    INT64   m_insertVideoFailed;
	INT64	m_i64ConnectionID;
	INT64	m_i64ReadPktNumV;
	INT64	m_i64LostPktNumV;
	INT64	m_i64ReadPktNumA;
	INT64	m_i64LostPktNumA;
	//void*	m_hFLVEncoder;
	UINT	m_unAudioIndex;
	UINT	m_unVideoIndex;
	BOOL	m_bSendedMark;
	BOOL	m_bLostFrame;
	BOOL	m_bNeedSendHeader;
	INT64	m_i64LastResetTime;
	INT64	m_i64LastSendTime;
	INT64	m_i64ReadRtpErrorTimes;
	INT64	m_i64ReadRtpTotalTimes;
	UINT	m_unVedioTimestamp;
	UINT	m_unAudioTimeStamp;

    unsigned long  m_lastRecvVideo;
    unsigned long  m_lastRecvAudio;
    unsigned long  m_lastPushRtpVideo;
    unsigned long  m_lastPushRtpAudio;

	CRtmpTransBuff m_oAudioBuff;
	CRtmpTransBuff m_oVideoBuff;

    unsigned short m_lastAudioSeq;
    bool           m_firstRecvAudio;

	STRU_FLOW_INFO()
	{
		m_bLostFrame = TRUE;
		m_i64UserID = 0;
		m_unAudioChannelID = 0;
		m_bSendedMark = FALSE;
		m_bNeedSendHeader = TRUE;
		m_byDevType = -1;
		//m_hFLVEncoder = NULL;
		m_i64ConnectionID = 0;
		m_i64LastResetTime = 0;
		m_i64LastRtpTime = 0;
	    m_i64LastRtpTimeVideo = 0;
	    m_i64LastRtpTimeAudio = 0;
        m_i64LastPushRtmpTimeVideo = 0;
        m_i64LastPushRtmpTimeAudio = 0;
        m_insertAudioFailed = 0;
        m_insertVideoFailed = 0;
		m_unAudioIndex = 0;
		m_unFlowID = 0;
		m_unVideoIndex = 0;
		m_unVideoChannelID = 0;
		m_i64LastSendTime = 0;
		m_i64ReadRtpErrorTimes = 0;
		m_i64ReadRtpTotalTimes = 0;
		m_unVedioTimestamp = 0;
		m_unAudioTimeStamp = 0;

        m_lastRecvVideo = 0;
        m_lastRecvAudio = 0;
        m_lastPushRtpVideo  = 0;
        m_lastPushRtpAudio  = 0;

        m_i64ReadPktNumV = 0;
        m_i64LostPktNumV = 0;
        m_i64ReadPktNumA = 0;
        m_i64LostPktNumA = 0;
        m_lastAudioSeq   = 0;
        m_firstRecvAudio = true;
	}
};

struct STRU_USERID_FLOWID
{
	INT64 m_i64UserID;
	UINT  m_unFlowID;
	STRU_USERID_FLOWID()
	{
		m_i64UserID = 0;
		m_unFlowID = 0;
	}
};

class FLVService : public IRtmpManagerEvent
{
public:
	FLVService();
	~FLVService();

public:
	BOOL Open();
	void Close();
	void DealRecvUdp(char *apRecvData,int aiDataLen );

	virtual void OnConnect(int64_t streamID, bool abResult);
	virtual void OnDisconnet(int64_t streamID);

private:
	static unsigned int FLVEncodeThread(void* apVoid);
	void FLVEncode(int aiThreadIndex);


	static unsigned int FLVFramingThread(void* apVoid);
	void FLVFraming(int aiThreadIndex);

	void DealCMSRtp(char *apRecvData,int aiDataLen);
	void DealLRDSData(char *apRecvData,int aiDataLen);
	void DealCMSFLV(STRU_CMS_FLV_DATA&);
	void DealCMSFLV(STRU_MS_RDS_RTP_DATA&);
private:
	STRU_CMS_FLV_DATA* m_arrayFLVData[DEF_THREAD_COUNT];
	CCircleQueue<int> m_quFLVPool[DEF_THREAD_COUNT];
	CCircleQueue<int> m_quFLV[DEF_THREAD_COUNT];

private:
	int m_iThreadCount;
	CCriticalLock m_oCriForThreadCount;
	BOOL m_bStop;

private:
	INT64 m_i64OutFlux[DEF_THREAD_COUNT];

	int m_nTcpFlux;
	INT64 m_i64UdpInFlux;
	INT64 m_i64UdpPacketCount;

private:
	std::unordered_map<INT64,STRU_FLOW_INFO> m_mapFLVInfo[DEF_THREAD_COUNT];
	CCriticalLock m_CriForFlvMap[DEF_THREAD_COUNT];

private:
	struct FLVFrame
	{
		INT64	m_i64UserID;
		char*	m_pData;
		int		m_nLen;		
		BOOL	m_bMark;
		UINT	m_unVideoChannelID;
		UINT	m_unAudioChannelID;
		BYTE	m_byDevType;
		UINT	m_unFlowID;
		WORD	m_wVideoInfoWidth;
		WORD	m_wVideoInfoHeigth;
		UINT	m_unTimestamp;

		FLVFrame()
		{
			m_i64UserID = 0;
			m_pData = 0;
			m_nLen = 0;
			m_bMark = FALSE;
			m_unAudioChannelID = m_unVideoChannelID = 0;
			m_unFlowID = 0;
			m_byDevType = 0;
			m_wVideoInfoWidth = 0;
			m_wVideoInfoHeigth = 0;
			m_unTimestamp = 0;
		}

		~FLVFrame()
		{
			delete []m_pData;
			m_pData = 0;
			m_i64UserID = 0;
			m_nLen = 0;
			m_bMark = FALSE;
			m_unAudioChannelID = m_unVideoChannelID = 0;
			m_unFlowID = 0;
			m_byDevType = 0;
			m_wVideoInfoWidth = 0;
			m_wVideoInfoHeigth = 0;
			m_unTimestamp = 0;
		}
	};

	CCircleQueue<FLVFrame*> m_quPtrFLVFrame[DEF_THREAD_COUNT];

	struct FLVEncoder
	{
		INT64	m_i64UserID;
		void*	m_hFLVEncoder;
		UINT	m_unAudioIndex;
		UINT	m_unVideoIndex;
		INT64	m_i64ConnectionID;
		WORD	m_wVideoInfoWidth;
		WORD	m_wVideoInfoHeight;
		UINT	m_unFlowID;
		INT64	m_i64LastSendTime;
		INT64	m_i64LastSendTimeVideo;
		INT64	m_i64LastSendTimeAudio;
        unsigned long m_sendVideo;
        unsigned long m_sendAudio;

       // RTMP*   m_rtmp;
        rtmp_status m_status;

        INT64   m_i64ConnTime;

		FLVEncoder()
		{
           // m_rtmp=NULL;
            m_status = no_init;
			m_i64ConnectionID = 0;
			m_hFLVEncoder = NULL;
			m_unAudioIndex = 0;
			m_unVideoIndex = 0;
			m_i64UserID = 0;
			m_wVideoInfoHeight = 0;
			m_wVideoInfoWidth = 0;
			m_i64LastSendTime = CBaseThread::GetSystemTime();
		    m_i64LastSendTimeVideo = 0;
		    m_i64LastSendTimeAudio = 0;
            m_sendVideo = 0;
            m_sendAudio = 0;
            m_i64ConnTime = 0;
		}
	};

    struct RtmpSta
    {
        int     m_conSta;
        time_t  m_t;
        bool    m_reCon;

        RtmpSta(int s, time_t t, bool r=false)
        {
            m_conSta = s;
            m_t      = t;
            m_reCon  = r;
        }
    };
    
private:
	static unsigned int FLVEncodingSendThread(void* apVoid);
	void FLVEncodingSendThread(int aiThreadIndex);

private:
    std::string m_push_url;
    void get_push_url(INT64,std::string&);

    int m_id;
    IRtmpManager* m_pRtmpMgr;
#ifndef DEF_RECONN_CDN
    std::unordered_map<INT64, int> m_oRtmpMap;
#else
    std::unordered_map<INT64, RtmpSta> m_oRtmpMap;
#endif
	CCriticalLock m_lockRtmpMap;

    time_t m_startTime;
private:
    bool init_rtmp(INT64 ai64StreamID, char*);
    bool send_header(INT64 ai64StreamID, char*, int len, unsigned int timestamp);
    bool send_rtmp_packet(INT64 ai64StreamID, int type, bool bKeyMark, char*, int len, unsigned int timestamp);
};
