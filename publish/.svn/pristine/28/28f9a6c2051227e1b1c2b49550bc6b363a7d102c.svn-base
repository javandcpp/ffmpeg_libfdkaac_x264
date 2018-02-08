

#ifndef _H_RTPDATAFRAME_
#define _H_RTPDATAFRAME_

#ifdef WIN32
	#include <windows.h>
#endif

enum ENUM_PACK_TYPE
{
	e_UserNormalPack,		//正常包,不需要确认
	e_UserPackNeedEcho,		//需要应答确认的
	e_EchoPack,
};

//编解码类型(负载类型)
enum EnumPayloadTypes {
	e_RtpG7231AudioData = 0,               // G.723.1 at 6.3kbps
	e_RtpMP3AudioData = 1,                 // MP3 AUDIO
	e_RtpG7221AudioData = 2,               // G.722 16 24 32kbps
	e_RtpG729aAudioData = 3,               // G729 10kbps
	e_RtpGSM6AudioData = 4,                // GSM 6.1 13 kbps
	e_RtpiLBCAudioData = 5,                //iLBC at 13.3Kbps
	e_RtpAMRWBAudioData = 6,               //ARM WB+
	e_RtpAACAudioData = 7,                 //AAC

	e_RtpXvidVideoData = 21,	           // XVID MPEG4 video
	e_RtpH263VideoData = 22,               // H263 VIDEO
	e_RtpDivxVideoData = 23,               // Divx Mpeg 4 Video
	//X264 Video 这种编码器类型已被X264 Video(new)替换，请大家不要再使用了。2013-1-25 张亚伟
	e_RtpX264VideoData = 24,               // X264 Video
	e_RtpIv32VideoData = 25,               // IV32 Video
	e_RtpH264VideoData = 26,               // X264 Video(new)
	e_RtpWmscreenVideoData = 27,           // Windows Media Screen Video
	e_RtpH265VideoData = 28,               // X265 Video，2013-5-7,zyw add

	e_RtcpSenderReport = 100,
	e_RtcpReceiverReport,
	//e_RtcpSourceDescription,
	e_RtcpGoodbye,
	e_RtcpUserData,    
    e_RtcpPackLostID,
	e_RtcpSystemTime,
};


/*
    0               1               2               3
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |V=2|P|X|  CC   |M|     PT      |       sequence number         |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                           timestamp                           |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |           synchronization source (SSRC) identifier            |
   +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
   |            contributing source (CSRC) identifiers             |
   |                             ....                              |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

    RTP Header Extension（当GetExtendMark()== TRUE时存在）
    0               1               2               3
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |      defined by profile       |           length              |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                        header extension                       |
   |                             ....                              |

*/

class CRtpDataFrame 
{
public:
    CRtpDataFrame(int nDataSize = 1152);
	CRtpDataFrame(const char *pDataBuff,int nBuffLen,int nDataLen);
	~CRtpDataFrame();

	void operator = (CRtpDataFrame &aoRtpFrame);

    //申请内存
	bool MallocFrameBuff(int nBuffLength);
	
	//取得缓存数据
	int GetData(char *pDataBuff,int nBuffLen);

public:
	//版本信息
    unsigned GetVersion() const { return (m_pFrameBuff[0]>>6)&3; } 
    
	//取得附加填充位标志（主要用来标志是否加密）
	BOOL GetFillMark() {return (m_pFrameBuff[0]&0x20) != 0;}     

	//设置附加填充位标志（主要用来标志是否加密）
	void SetFillMark(BOOL abFill); 

	//取得附加填充数据长度
	BYTE GetExDataLen();

public:

	//有效负荷(Payload)标志
    BOOL GetMarker() const { return (m_pFrameBuff[1]&0x80) != 0; }

	//有效负荷(Payload)标志
    void SetMarker(BOOL m);

	//数据（有效负荷）类别
    EnumPayloadTypes GetPayloadType() const { return (EnumPayloadTypes)(m_pFrameBuff[1]&0x7f); }

    //设置数据（有效负荷）类别
	void  SetPayloadType(EnumPayloadTypes t);

public:
	// 取得序列号
    unsigned short GetSequenceNumber() const { return *(unsigned short *)&m_pFrameBuff[2]; } 
    
	// 设置序列号
	void SetSequenceNumber(short n) { *(unsigned short *)&m_pFrameBuff[2] = n; }

public:
	//时间戳
    UINT GetTimestamp() const  { return *(UINT *)&m_pFrameBuff[4]; } 

	//设置时间戳
    void  SetTimestamp(UINT t) { *(UINT *)&m_pFrameBuff[4] = t; }

public:
	//同步源（通道id）标示
    UINT GetSyncSource() const  { return *(UINT *)&m_pFrameBuff[8]; }

	//取得同步源（通道id）标示
    void  SetSyncSource(UINT s) { *(UINT *)&m_pFrameBuff[8] = s; } 

public:
	//提供源数量,在H323中没用
    int GetContribSrcCount() const { return m_pFrameBuff[0]&0xf; }
	
	//设置提供源数量
	void SetContribSrcCount(int nCscCount);

	//提供源标示
    UINT  GetContribSource(int idx) const;	
    
	//设置提供源标示
	void   SetContribSource(int idx, UINT src);
	
public:
	//扩展信息标志
    BOOL GetExtendMark() const   { return (m_pFrameBuff[0]&0x10) != 0; }

	//扩展信息标志
    void SetExtendMark(BOOL ext);
	
	//设置扩展剖面	
	void SetExtProfile(short nExtProfile);	

	//取得扩展剖面
	short GetExtProfile(void);

	//设置扩展剖面长度	
	void SetExtProLen(short nExtProLen);

	//取得扩展剖面长度		
	short GetExtProLen();
	
	//设置扩展填充数据
	void CopyExtProData(const char *pExtProData,short anDataLen);

	//读取扩展填充数据
	short GetExtProData(char *pExtProData,short nBuffLen);

public:
	//返回RTP包头长度
    int GetHeaderSize()     const; 	
    int ReturnHeaderSize()  { return m_nHeadSize; } 	

	//数据（有效负荷）长度
	unsigned short GetPayloadSize() { return m_nPayloadSize; }

	//设置数据（有效负荷）长度
	void  SetPayloadSize(unsigned short sPayloadSize) {m_nPayloadSize = sPayloadSize; } 

	//数据（有效负荷）指针
    char * GetPayloadPtr()     const { return (char *)(m_pFrameBuff+GetHeaderSize()); }

	//帧数据指针
	char * GetFramePtr()     const { return (char *)m_pFrameBuff; }

	//帧数据长度
	int GetFrameDataLen();

	//取得数据BUFF长度
	int GetFrameBuffLen() { return m_nDataBuffLen;}

    void PrintData();
 public:
	bool				m_bSelfMalloc;
	//有效负荷
	unsigned short		m_nPayloadSize;
	//内存长度
	int					m_nDataBuffLen;
	//数据长度 
	int					m_nDataLen;		
	int					m_nHeadSize;		
	//RTP数据包的存放地址
	char*				m_pFrameBuff;

////////////////////////////////////////////////////////////////////
//RTCP
public:
	struct ReceiverReport {				//接收者汇报
		UINT ssrc;      /* data source being reported */
		BYTE fraction;      /* fraction lost since last SR/RR */
		BYTE lost[3];	  /* cumulative number of packets lost (signed!) */
		UINT last_seq;  /* extended last sequence number received */
		UINT jitter;    /* interarrival jitter */
		UINT lsr;       /* last SR packet from this source */
		UINT dlsr;      /* delay since last SR packet */

		unsigned GetLostPackets() const { return (lost[0]<<16U)+(lost[1]<<8U)+lost[2]; }
		void SetLostPackets(unsigned lost);
	};

	struct SenderReport {		//发送者汇报
		UINT ssrc;      /* source this RTCP packet refers to */
		UINT ntp_sec;   /* NTP timestamp */
		UINT ntp_frac;
		UINT rtp_ts;    /* RTP timestamp */
		UINT psent;     /* packets sent */
		UINT osent;     /* octets(byte) sent */ 
	};	

    struct LostPackInfo
    {
		INT64 m_nEndpointID;         //终端ID
		BYTE m_byDeviceType;           //通道类型
        //UINT m_ulSyncSource;			//源通道ID
        //unsigned short m_usSeqNumber;	//丢失的包序号        
    };
};
#endif //_H_RTPDATAFRAME_
