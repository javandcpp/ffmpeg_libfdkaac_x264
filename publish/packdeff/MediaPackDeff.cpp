#include "StdSerialMacro.h"
#include "MediaPackDeff.h"
//////////////////////////////////////////////////////////////////////
//
//				以下包是原是TCP的包现在修改为UDP的包
//
//发言用户的流ID通知（私麦时，服务器转发要过滤老版本用户）
IMPLEMENT_STD_CONSTRUCTOR(STRU_CMSP_PACK_HEAD);
BOOL STRU_CMSP_PACK_HEAD::Serialize(CStdSerialize& aoStdSerialize)
{
    try
    {		
        aoStdSerialize.Serialize(m_i64PackID);
        aoStdSerialize.Serialize(m_lProxyID);
        aoStdSerialize.Serialize(m_i64Session);
		aoStdSerialize.Serialize(m_i64UserID);
		aoStdSerialize.Serialize(m_i64RoomID);
        return TRUE;
    }
    catch(...)
    {
        return FALSE;
    }
}


IMPLEMENT_STD_CONSTRUCTOR(STRU_MS_RDS_RTP_DATA);
IMPLEMENT_STD_PACK_SERIALIZE(STRU_MS_RDS_RTP_DATA, PACK_MS_RDS_RTP_DATA);	
BOOL STRU_MS_RDS_RTP_DATA::Serialize(CStdSerialize& aoStdSerialize)
{
	try
	{		
		m_oPackHead.Serialize(aoStdSerialize);
		aoStdSerialize.Serialize(m_byMicIndex);
		aoStdSerialize.Serialize(m_i64AudioChannelID);
		aoStdSerialize.Serialize(m_i64VideoChannelID);
		aoStdSerialize.Serialize(m_sFrameWidth);
		aoStdSerialize.Serialize(m_sFrameHeight);
		aoStdSerialize.Serialize(m_byMicType);
		aoStdSerialize.Serialize(m_byDevType);
		aoStdSerialize.Serialize(m_nRtpDataLen);


		aoStdSerialize.Serialize(m_szRtpData, m_nRtpDataLen, DEF_MAX_RTP_PACK_LEN);

		return TRUE;
	}
	catch(...)
	{
		return FALSE;
	}
}

IMPLEMENT_STD_PACK_SERIALIZE(STRU_CMS_FLV_DATA,PACK_CMS_FLV_DATA);	
BOOL STRU_CMS_FLV_DATA::Serialize(CStdSerialize& aoStdSerialize)
{
	try
	{		
		aoStdSerialize.Serialize(m_i64UserID);
        aoStdSerialize.Serialize(m_wWidth);
        aoStdSerialize.Serialize(m_wHeight);
        aoStdSerialize.Serialize(m_byDevType);
        aoStdSerialize.Serialize(m_byFPS);

        aoStdSerialize.Serialize(m_wRtpDataLen);
        aoStdSerialize.Serialize(m_szRtpData,m_wRtpDataLen,1152);

		return TRUE;
	}
	catch(...)
	{
		return FALSE;
	}
}
