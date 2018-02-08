#pragma once


//TCP控制包
#define  PACK_MADIA_PACK_BASE 30

#define	 PACK_CL_MS_LOGIN_RQ				PACK_MADIA_PACK_BASE + 0
#define	 PACK_CL_MS_LOGIN_RS				PACK_MADIA_PACK_BASE + 1
#define	 PACK_CL_MS_RADIO_RES_RQ			PACK_MADIA_PACK_BASE + 2
#define	 PACK_CL_MS_RADIO_RES_RS			PACK_MADIA_PACK_BASE + 3
#define  PACK_CL_MS_NAT_ADDR_ID				PACK_MADIA_PACK_BASE + 5
#define	 PACK_CL_MS_NODE_LIST_ID			PACK_MADIA_PACK_BASE + 6

#define	 PACK_CL_MS_STATE_REPORT_RQ			PACK_MADIA_PACK_BASE + 7
#define	 PACK_CL_MS_STATE_REPORT_RS			PACK_MADIA_PACK_BASE + 8
#define	 PACK_CL_MS_STATE_REPORT_V2			PACK_MADIA_PACK_BASE + 9

#define	 PACK_CL_CL_RTP_PACK_LOST_ID		PACK_MADIA_PACK_BASE + 10
#define	 PACK_CL_CL_RADIO_ACT_RQ			PACK_MADIA_PACK_BASE + 11
#define	 PACK_CL_CL_RADIO_ACT_RS			PACK_MADIA_PACK_BASE + 12
#define	 PACK_CL_CL_ADDR_EXCH_ID            PACK_MADIA_PACK_BASE + 13
#define	 PACK_CL_CL_STATE_REPORT_RQ         PACK_MADIA_PACK_BASE + 14
#define	 PACK_CL_CL_STATE_REPORT_RS         PACK_MADIA_PACK_BASE + 15
#define  PACK_CL_CL_INTER_PHONE_ACT_ID      PACK_MADIA_PACK_BASE + 16

#define	 PACK_CMS_CMS_PING_RQ				PACK_MADIA_PACK_BASE + 25
#define	 PACK_CMS_CMS_PING_RS				PACK_MADIA_PACK_BASE + 26

#define  PACK_CMS_CMS_PROXY_NOTIFY_RQ		PACK_MADIA_PACK_BASE + 27
#define  PACK_CMS_CMS_PROXY_NOTIFY_RS		PACK_MADIA_PACK_BASE + 28

#define  PACK_CL_CMS_PING_RQ				PACK_MADIA_PACK_BASE + 50
#define  PACK_CL_CMS_PING_RS				PACK_MADIA_PACK_BASE + 51

#define	 PACK_CL_MS_STATE_REPORT_V3			PACK_MADIA_PACK_BASE + 60

#define PACK_CL_MS_SPEAKER_CHANNEL_RQ		PACK_MADIA_PACK_BASE + 61 //上下麦请求
#define PACK_CL_MS_SPEAKER_CHANNEL_RS		PACK_MADIA_PACK_BASE + 62 
#define PACK_CL_MS_MIC_RECV_STATE_CHG_RQ	PACK_MADIA_PACK_BASE + 63 //更改变麦的收听状态请求
#define PACK_CL_MS_MIC_RECV_STATE_CHG_RS	PACK_MADIA_PACK_BASE + 64 
#define PACK_MS_RDS_RTP_DATA			    PACK_MADIA_PACK_BASE + 65 //向RDS上传RTP流
#define PACK_CL_MS_REGISTER_RQ				PACK_MADIA_PACK_BASE + 66 //用户注册请求
#define PACK_CL_MS_REGISTER_RS				PACK_MADIA_PACK_BASE + 67 //用户注册应答
#define PACK_CL_MS_ALIVE_ID					PACK_MADIA_PACK_BASE + 68 //用户存活包
#define PACK_CL_MS_UNREGISTER_RQ			PACK_MADIA_PACK_BASE + 69 //用户退出包
#define PACK_CL_MS_STOP_SERVICE				PACK_MADIA_PACK_BASE + 70 //停止服务
#define	 PACK_CMS_FLV_DATA					PACK_MADIA_PACK_BASE + 71

#define	 DEF_MEDIA_PACK_COUNT 255
#define  DEF_MAX_RTP_PACK_LEN 1560

#include "ConstDeff.h"
#include "StdSerializeEx.h"
#include "StdSerialMacro.h"


//登录请求
struct STRU_CL_MS_LOGIN_RQ
{
	int	m_lRoomID;			 //房间id	
	INT64   m_i64UserID;		 //用户ID
	int	m_nSessionKey;       //会话密钥

	BYTE    m_byNatType;		 //NAT类型
	BYTE    m_byIspType;		 //ISP类型
	WORD    m_wBandWidth;        //网络带宽(KBPS)
	WORD    m_wClientVer;

	BYTE    m_byMicType;         // 麦类型   ENUM_MIC_TYPE
	short   m_sMicIndex;         // 麦序号

	char	m_szVerifyKey[DEF_MD5_PWD_LEN];		// 登录CMS校验码 CASProxy架构调整时增加

public:
	STRU_CL_MS_LOGIN_RQ();
	int    Pack(char *apBuffer ,int    alLen);
	BOOL   UnPack(char * apBuffer,int    alLen);
	BOOL   Serialize(CStdSerialize & aoStdSerialize);
};

enum ENUM_VTS_MODE
{
	e_VtsMode_Cs = 0,	//传统cs模式
	e_VtsMode_P2p = 1,	//完全p2p
	e_VtsMode_Multiplex = 2, //混合式
};
//登录应答
struct STRU_CL_MS_LOGIN_RS
{
	int	m_lRoomID;			 //房间id	
	INT64	m_i64UserID;		 //用户ID

	BYTE    m_byResult;			 //结果 
	int	m_lNatIpAddr;       //外网地址
	WORD	m_usNatPort;	    //用户的语音端口
	BYTE    m_byVtsMode;		//转发模式
	WORD    m_nContribSrcCount; //提供源数量
	int    m_lJitterTime;		//延迟时间

	BYTE    m_byMicType;         // 麦类型   ENUM_MIC_TYPE
	short   m_sMicIndex;         // 麦序号

public:
	STRU_CL_MS_LOGIN_RS();
	int    Pack(char *apBuffer ,int    alLen);
	BOOL   UnPack(char * apBuffer,int    alLen);
	BOOL   Serialize(CStdSerialize & aoStdSerialize);
};


//NAT地址变化通知包
struct STRU_CL_MS_NAT_ADDR_ID
{
	int    	m_lRoomID;
	INT64		m_i64UserID;

	int		m_lNatIpAddr;       //外网地址
	WORD		m_usNatPort;	    //用户的语音端口

public:
	STRU_CL_MS_NAT_ADDR_ID();
	int    Pack(char *apBuffer ,int    alLen);
	BOOL   UnPack(char * apBuffer,int    alLen);
	BOOL   Serialize(CStdSerialize & aoStdSerialize);
} ;

//语音节点信息
struct STRU_AUDIO_NODE_INFO
{
	INT64	m_i64UserID;        //用户ID
	BYTE    m_byIspType;		//ISP类型
	BYTE    m_byNatType;        //网络类型
	int    m_lNatIpAddr;       //外网地址
	WORD	m_usNatPort;	    //用户的语音端口

	void Serialize(CStdSerialize& aoStdSerialize);
};

//服务节点列表(由服务器向下发)
struct STRU_CL_MS_NODE_LIST_ID
{
	int    	m_lRoomID;
	INT64		m_i64UserID;

	short		m_sLayerIndex;		//所在层
	BYTE		m_byNodeCount;		//节点数量
	STRU_AUDIO_NODE_INFO m_szNodeList[20]; //节点列表

public:
	STRU_CL_MS_NODE_LIST_ID();
	int    Pack(char *apBuffer ,int    alLen);
	BOOL   UnPack(char * apBuffer,int    alLen);
	BOOL   Serialize(CStdSerialize & aoStdSerialize);
};


enum ENUM_MEDIA_RADIO_RES_RQ_TYPE
{
	cl_Ms_BeginRecvRadioRQ,  //开始接收语音请求（m_lOtherParam：用户带宽）
	cl_Ms_StopRecvRadioID,   //停止接收语音请求（m_lOtherParam：所在层）
	cl_Ms_RecruitNodeRQ,     //补充服务节点请求（m_lOtherParam：所在层）
	//cl_Ms_NatAliveID,        //NAT存活（m_lOtherParam: MAKELONG(网络带宽,当前收听状态)） 
};

//广播资源申请
struct STRU_CL_MS_RADIO_RES_RQ
{
	int	m_lRoomID;			 //房间id	
	INT64   m_i64UserID;		 //用户ID
	int	m_nSessionKey;       //会话密钥

	BYTE	m_byOptType;		 //操作类型
	unsigned int   m_dwOtherParam;		 //辅助参数( 见ENUM_MEDIA_USER_OPERATE_TYPE 定义 )

	BYTE    m_byMicType;         // 麦类型   ENUM_MIC_TYPE
	short   m_sMicIndex;         // 麦序号
public:
	STRU_CL_MS_RADIO_RES_RQ();
	int    Pack(char *apBuffer ,int    alLen);
	BOOL   UnPack(char * apBuffer,int    alLen);
	BOOL   Serialize(CStdSerialize & aoStdSerialize);
};


//广播资源应答
struct STRU_CL_MS_RADIO_RES_RS
{
	int	m_lRoomID;			 //房间id	
	INT64	m_i64UserID;		 //用户ID

	BYTE	m_byOptType;		 //操作类型
	unsigned int   m_dwOtherParam;		 //操作辅助参数
	BYTE    m_byResult;			 //结果 

	BYTE    m_byMicType;         // 麦类型   ENUM_MIC_TYPE
	short   m_sMicIndex;         // 麦序号
public:
	STRU_CL_MS_RADIO_RES_RS();
	int    Pack(char *apBuffer ,int    alLen);
	BOOL   UnPack(char * apBuffer,int    alLen);
	BOOL   Serialize(CStdSerialize & aoStdSerialize);
};


//状态汇报请求
struct STRU_CL_MS_STATE_REPORT_RQ
{
	int	m_lRoomID;			 //房间id	
	INT64   m_i64UserID;		 //用户ID
	int	m_nSessionKey;       //会话密钥

	WORD    m_wBandWidth;        //网络带宽(KBPS)
	unsigned int   m_dwTimeStamp;        //时间戳(用与计算一跳时间，接收方需要把该参数带回来)	
	WORD    m_wListenState;		 //收听状态
	//short   m_sLayerIndex;		 //所在层

	BYTE    m_byMicType;         // 麦类型   ENUM_MIC_TYPE
	short   m_sMicIndex;         // 麦序号
public:
	STRU_CL_MS_STATE_REPORT_RQ();
	int    Pack(char *apBuffer ,int    alLen);
	BOOL   UnPack(char * apBuffer,int    alLen);
	BOOL   Serialize(CStdSerialize & aoStdSerialize);
};


//状态汇报请求
struct STRU_CL_MS_STATE_REPORT_V2
{
	int	m_lRoomID;			 //房间id	
	INT64   m_i64UserID;		 //用户ID
	int	m_nSessionKey;       //会话密钥

	WORD    m_wBandWidth;        //网络带宽(KBPS)
	unsigned int   m_dwTimeStamp;        //时间戳(用与计算一跳时间，接收方需要把该参数带回来)	
	WORD    m_wListenState;		 //收听状态
	short   m_sLayerIndex;		 //所在层
	WORD    m_wPingTime;		 //ping时间

	BYTE    m_byMicType;         // 麦类型   ENUM_MIC_TYPE
	short   m_sMicIndex;         // 麦序号
public:
	STRU_CL_MS_STATE_REPORT_V2();
	int    Pack(char *apBuffer ,int    alLen);
	BOOL   UnPack(char * apBuffer,int    alLen);
	BOOL   Serialize(CStdSerialize & aoStdSerialize);
};

//通道的丢包率信息,用于客户端向服务器汇报自身数据状态
struct STRU_CL_CHANNEL_STATE
{
	int m_lChannelID;			//通道id
	int m_lRecvedPackCount;	//收到包个数
	int m_lDropPackCount;		//收到的无效包个数
	int m_lResendPackCount;	//重发请求的次数
	int m_lLostPackCount;		//丢包未补包成功个数

public:
	STRU_CL_CHANNEL_STATE();
	BOOL   Serialize(CStdSerialize & aoStdSerialize);
};



//向服务器ping应答
struct STRU_CL_MS_PING_RS
{
	int	m_lSvrID;			 //cms SvrID
	int	m_lRoomID;			 //房间id	

	INT64   m_i64UserID;		 //用户ID

	unsigned int   m_dwTimeStamp;        //时间戳(用与计算一跳时间，接收方需要把该参数带回来)	

public:
	STRU_CL_MS_PING_RS();
	int    Pack(char *apBuffer ,int    alLen);
	BOOL   UnPack(char * apBuffer,int    alLen);
	BOOL   Serialize(CStdSerialize & aoStdSerialize);
};


//数据包丢失通知
struct STRU_CL_CL_RTP_PACK_LOST_ID
{
	INT64		m_i64UserID;		 //本人ID
	//BYTE		m_byDeviceType;		 //设备类型
	int     	m_lChannelID;		 //通道ID
	WORD	    m_usSeqNumber;	     //丢失的包序号        
public:
	STRU_CL_CL_RTP_PACK_LOST_ID();
	int    Pack(char *apBuffer ,int    alLen);
	BOOL   UnPack(char * apBuffer,int    alLen);
	BOOL   Serialize(CStdSerialize & aoStdSerialize);
};

enum ENUM_MEDIA_RADIO_ACT_TYPE
{
	cl_cl_Act_BeginRtpTrans,            //开始RTP数据中转(m_lOtherParam:流编号)	
	cl_cl_Act_StopRtpTrans,             //停止RTP数据中转(m_lOtherParam:流编号)	
	cl_cl_Act_LevelChanged,             //所在层发生变化
	cl_cl_Act_UserExit,                 //用户退出
};

//向其他用户请求操作
struct STRU_CL_CL_RADIO_ACT_RQ
{
	int		m_lRoomID;			 //房间id	
	INT64		m_i64UserID;		 //当前发言人ID
	int		m_nSessionKey;       //会话密钥

	INT64		m_i64DstUserId;		 //被处理用户ID
	BYTE		m_byOptType;		 //操作类型
	unsigned int     	m_lOtherParam;		 //其他参数

public:
	STRU_CL_CL_RADIO_ACT_RQ();
	int    Pack(char *apBuffer ,int    alLen);
	BOOL   UnPack(char * apBuffer,int    alLen);
	BOOL   Serialize(CStdSerialize & aoStdSerialize);
};


//向其他用户请求操作应答
struct STRU_CL_CL_RADIO_ACT_RS
{
	int		m_lRoomID;			 //房间id	
	INT64		m_i64UserID;		 //当前发言人ID
	int		m_nSessionKey;       //会话密钥

	INT64		m_i64DstUserId;			//被处理用户ID
	BYTE		m_byOptType;			//操作类型
	unsigned int     	m_lOtherParam;			//其他参数
	BYTE		m_byResult;				//结果(对方是否接受等)	

public:
	STRU_CL_CL_RADIO_ACT_RS();
	int    Pack(char *apBuffer ,int    alLen);
	BOOL   UnPack(char * apBuffer,int    alLen);
	BOOL   Serialize(CStdSerialize & aoStdSerialize);
};


//地址交互中转包
struct	STRU_CL_CL_ADDR_EXCH_ID
{
	int	m_lRoomID;				//房间id	
	INT64	m_i64UserID;			//来源用户ID
	int    m_nSessionKey;          //会话密钥
	INT64   m_i64DstUserID;			//目的用户ID
	
	WORD	m_wDataLen;				//信息大小
	char	m_szDataBuff[DEF_GENERAL_DATA_LEN+1];	//信息    

    unsigned int	m_dwSrcCmsID;			//来源Cmsid, cms代理逻辑实现新增字段
public:
	//STRU_CL_CL_ADDR_EXCH_ID();
	int    Pack(char *apBuffer ,int    alLen);
	BOOL   UnPack(char * apBuffer,int    alLen);
	BOOL   Serialize(CStdSerialize & aoStdSerialize);
};

//状态汇报通知
struct STRU_CL_CL_STATE_REPORT_RQ
{
	INT64		m_i64UserID;		 //本人ID
	int        m_lOutputFlux;        //发送带宽数(自上次汇报)
	
	WORD        m_wBandWidth;        //网络带宽(KBPS)
	unsigned int       m_dwTimeStamp;        //时间戳(用与计算一跳时间，接收方需要把该参数带回来)	
	short       m_sLayerIndex;		 //所在层
	WORD        m_wP2pState;		 //两人之间的P2P状态
public:
	int    Pack(char *apBuffer ,int    alLen);
	BOOL   UnPack(char * apBuffer,int    alLen);
	BOOL   Serialize(CStdSerialize & aoStdSerialize);
};


//状态汇报通知
struct STRU_CL_CL_STATE_REPORT_RS
{
    INT64		m_i64UserID;		 //本人ID

    unsigned int       m_dwTimeStamp;        //时间戳(用与计算一跳时间，接收方需要把该参数带回来)	
public:
    //STRU_CL_CL_STATE_REPORT_RS();
    int    Pack(char *apBuffer ,int    alLen);
    BOOL   UnPack(char * apBuffer,int    alLen);
    BOOL   Serialize(CStdSerialize & aoStdSerialize);
};


enum ENUM_INTER_PHONE_ACT_TYPE
{
	enmu_Inter_Phone_Channel_ID,     //RTP通道ID
	enmu_Inter_Phone_Close_ID,       //对讲机关闭
};

//对讲机通道ID
struct STRU_CL_CL_INTER_PHONE_ACT_ID
{
	int    	m_lRoomID;
	INT64		m_i64UserID;
	int        m_lSessionKey;

	INT64		m_i64DstUserId;		 //被处理用户ID
	BYTE		m_byOptType;		 //操作类型
	unsigned int     	m_lOtherParam;		 //其他参数
public:
	//STRU_CL_CL_INTER_PHONE_ACT_ID();
	int    Pack(char *apBuffer ,int    alLen);
	BOOL   UnPack(char * apBuffer,int    alLen);
	BOOL   Serialize(CStdSerialize & aoStdSerialize);
};

//CMS 间ping请求包
struct STRU_CMS_CMS_PING_RQ
{
	int		m_lSrcCmsID;		//源Cmsid
	INT64		m_i64ServiceSign;//服务标示
	
	int		m_lDstCmsID;	//目标cmsid
	unsigned int		m_dwSendTime;	//ping包发送时间

	int    Pack(char *apBuffer ,int    alLen);
	BOOL   UnPack(char * apBuffer,int    alLen);
	BOOL   Serialize(CStdSerialize & aoStdSerialize);
};

//CMS 间ping应答包
struct STRU_CMS_CMS_PING_RS
{
	int		m_lSrcCmsID;		//源Cmsid
	INT64		m_i64ServiceSign;//服务标示

	int		m_lDstCmsID;	//目标cmsid
	BYTE		m_byResult;		//0:对应组找不到 1:正常
	unsigned int		m_dwSendTime;	//ping包发送时间

	int    Pack(char *apBuffer ,int    alLen);
	BOOL   UnPack(char * apBuffer,int    alLen);
	BOOL   Serialize(CStdSerialize & aoStdSerialize);
};



//CMS 间PROXY地址通知包
struct STRU_CMS_CMS_PROXY_NOTIFY_RQ
{
	int		m_lSrcCmsID;		//源Cmsid
	INT64		m_i64SrcVtsKey;//服务标示
	
	int		m_lDstCmsID;	//目标cmsid
    INT64		m_i64DstVtsKey;//服务标示

    BYTE        m_byProxyIspType;    //proxy网络类型
	int		m_lProxyAddr;	//proxy地址
    WORD        m_wProxyPort;   //proxy端口

	int    Pack(char *apBuffer ,int    alLen);
	BOOL   UnPack(char * apBuffer,int    alLen);
	BOOL   Serialize(CStdSerialize & aoStdSerialize);
};

//CMS 间PROXY应答包
struct STRU_CMS_CMS_PROXY_NOTIFY_RS
{
	int		m_lSrcCmsID;		//源Cmsid
	INT64		m_i64SrcVtsKey;//服务标示

	int		m_lDstCmsID;	//目标cmsid
    INT64		m_i64DstVtsKey;//服务标示

	BYTE		m_byResult;		//0:对应组找不到 1:正常
    BYTE        m_byProxyIspType;    //proxy网络类型
	int		m_lProxyAddr;	//proxy地址
    WORD        m_wProxyPort;   //proxy端口

	int    Pack(char *apBuffer ,int    alLen);
	BOOL   UnPack(char * apBuffer,int    alLen);
	BOOL   Serialize(CStdSerialize & aoStdSerialize);
};

//////////////////////////////////////////////////////////////////////
//
//				以下包是原是TCP的包现在修改为UDP的包   junjiang
//
//发言用户的流ID通知（私麦时，服务器转发要过滤老版本用户）
struct STRU_CMSP_PACK_HEAD
{
	INT64  m_i64PackID;       //包ID 
	int   m_lProxyID;		  //代理ID
	INT64  m_i64Session;      //代理SessionKey
	INT64  m_i64UserID;       //用户ID 
	INT64  m_i64RoomID;       //房间ID
public:
	STRU_CMSP_PACK_HEAD();
	BOOL Serialize(CStdSerialize &aoStdSerial);
};

struct STRU_CL_MS_SPEAKER_CHANNEL_RQ
{
	STRU_CMSP_PACK_HEAD m_oPackHead;
	
	BYTE	m_byMicIndex;			//麦序号
	INT64   m_i64AudioChannelID;	//语音流ID
	INT64   m_i64VideoChannelID;	//视频流id
	short	m_sFrameWidth;			//视频宽度
	short	m_sFrameHeight;			//视频高
	BYTE	m_byChgType;			//0:上麦否则下麦

	//added by zht	2010.03.25
	BYTE    m_byMicType;			//麦类型

public:
	STRU_CL_MS_SPEAKER_CHANNEL_RQ();
	DECLEAR_STD_PACK_SERIALIZE();
};

struct STRU_CL_MS_SPEAKER_CHANNEL_RS
{
	STRU_CMSP_PACK_HEAD m_oPackHead;
	
	BYTE	m_byMicIndex;         //麦序号
	INT64   m_i64AudioChannelID;   //语音流ID
	INT64   m_i64VideoChannelID;   //视频流id

	BYTE    m_byResult;			//0:成功否则失败

public:
	STRU_CL_MS_SPEAKER_CHANNEL_RS();
	DECLEAR_STD_PACK_SERIALIZE();
};

//对应以前的TCP（CL_CAS_RES_RQ  -> STRU_CAS_CMS_CHG_USER_ID ）
struct STRU_CL_MS_MIC_RECV_STATE_CHG_RQ
{
	STRU_CMSP_PACK_HEAD m_oPackHead;

	BYTE		m_byAudioRecvState;		//语音接收状态(默认全部接收)
	BYTE		m_byVideoRecvState;		//视频接收状态(默认全部不接收)

	int        m_lUserIP;				//用户ip地址，用于刷新防火墙白名单
	unsigned short m_usMediaState;		//用户媒体状态
public:
	STRU_CL_MS_MIC_RECV_STATE_CHG_RQ();
	DECLEAR_STD_PACK_SERIALIZE();
};

struct STRU_CL_MS_MIC_RECV_STATE_CHG_RS
{
	STRU_CMSP_PACK_HEAD m_oPackHead;

	BYTE		m_byResult;			//0代表成功

public:
	STRU_CL_MS_MIC_RECV_STATE_CHG_RS();
	DECLEAR_STD_PACK_SERIALIZE();
};

struct STRU_MS_RDS_RTP_DATA
{
	STRU_CMSP_PACK_HEAD m_oPackHead;
	
	BYTE	m_byMicIndex;			//麦序号
	INT64   m_i64AudioChannelID;	//语音流ID
	INT64   m_i64VideoChannelID;	//视频流id
	short	m_sFrameWidth;			//视频宽度
	short	m_sFrameHeight;			//视频高
	BYTE    m_byMicType;			//麦类型
	BYTE	m_byDevType;			//设备类型

	WORD	m_nRtpDataLen;
	char	m_szRtpData[DEF_MAX_RTP_PACK_LEN];
public:
	STRU_MS_RDS_RTP_DATA();
	DECLEAR_STD_PACK_SERIALIZE();
};

struct STRU_CL_MS_REGISTER_RQ
{
	INT64	m_i64PackID;			//包ID
	int	m_lRoomID;				//房间id	
	INT64   m_i64UserID;			//用户ID
	int	m_nSessionKey;			//会话密钥
	BYTE    m_byMicType;			// 麦类型   ENUM_MIC_TYPE
	short   m_sMicIndex;			// 麦序号

	WORD    m_wClientVer;

public:
	STRU_CL_MS_REGISTER_RQ();
	int    Pack(char *apBuffer ,int    alLen);
	BOOL   UnPack(char * apBuffer,int    alLen);
	BOOL   Serialize(CStdSerialize & aoStdSerialize);
};

struct STRU_CL_MS_REGISTER_RS
{
	INT64	m_i64PackID;			//包ID
	int	m_lRoomID;				//房间id	
	INT64   m_i64UserID;			//用户ID
	INT64	m_i64SessionKey;		//会话密钥
	BYTE	m_byResult;				//0成功否则失败

public:
	STRU_CL_MS_REGISTER_RS();
	int    Pack(char *apBuffer ,int    alLen);
	BOOL   UnPack(char * apBuffer,int    alLen);
	BOOL   Serialize(CStdSerialize & aoStdSerialize);
};

struct STRU_CL_MS_ALIVE_ID
{
	int	m_lRoomID;				//房间id	
	INT64   m_i64UserID;			//用户ID
	INT64	m_i64SessionKey;		//会话密钥
	INT64   m_i64AudioChannelID;	//语音流ID
	INT64   m_i64VideoChannelID;	//视频流id
public:
	STRU_CL_MS_ALIVE_ID();
	int    Pack(char *apBuffer ,int    alLen);
	BOOL   UnPack(char * apBuffer,int    alLen);
	BOOL   Serialize(CStdSerialize & aoStdSerialize);
};

struct STRU_CL_MS_UNREGISTER_RQ
{
	int	m_lRoomID;				//房间id	
	INT64   m_i64UserID;			//用户ID
	INT64	m_i64SessionKey;		//会话密钥

public:
	STRU_CL_MS_UNREGISTER_RQ();
	int    Pack(char *apBuffer ,int    alLen);
	BOOL   UnPack(char * apBuffer,int    alLen);
	BOOL   Serialize(CStdSerialize & aoStdSerialize);
};

struct STRU_CL_MS_STOP_SERVICE
{
	int	m_lRoomID;				//房间id	
	INT64   m_i64UserID;			//用户ID
	INT64	m_i64SessionKey;		//会话密钥
	int	m_lErrorCode;			//错误码

public:
	STRU_CL_MS_STOP_SERVICE();
	int    Pack(char *apBuffer ,int    alLen);
	BOOL   UnPack(char * apBuffer,int    alLen);
	BOOL   Serialize(CStdSerialize & aoStdSerialize);
};

struct STRU_CMS_FLV_DATA
{
	INT64	m_i64UserID;
	WORD	m_wWidth;
	WORD	m_wHeight;
	BYTE	m_byDevType;
	BYTE	m_byFPS;
	WORD	m_wRtpDataLen;
	char	m_szRtpData[1152];

	int    Pack(char *apBuffer ,int    alLen);
	BOOL   UnPack(char * apBuffer,int    alLen);
	BOOL   Serialize(CStdSerialize & aoStdSerialize);
};
