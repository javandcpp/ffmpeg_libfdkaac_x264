#include "LRDSPackDeff.h"

//----------------------------------顶层包开始--------------------------------
// 服务器到RDS数据转发包, 数据流向：SVR->LRDS


IMPLEMENT_STD_CONSTRUCTOR( STRU_SVR_LRDS_CMSP_DATA_ID )
IMPLEMENT_STD_PACK_SERIALIZE( STRU_SVR_LRDS_CMSP_DATA_ID, PACK_SVR_LRDS_CMSP_DATA_ID )
BOOL STRU_SVR_LRDS_CMSP_DATA_ID::Serialize( CStdSerialize& aoStdSerialize )
{
	try
	{
		m_oSvrPackHead.Serialize( aoStdSerialize );
		aoStdSerialize.Serialize( m_i64RoomID );		
		aoStdSerialize.Serialize( m_dwMsgType );
		aoStdSerialize.Serialize( m_wDataLen );
		aoStdSerialize.Serialize( (char*)m_byData, m_wDataLen, DEF_LRDS_FORWARD_DATA_LEN );
		return TRUE;
	}
	catch(...)
	{
		return FALSE;
	}
}

