#include "ServerPackHead.h"

IMPLEMENT_STD_CONSTRUCTOR(STRU_SVR_PACK_HEAD)
BOOL STRU_SVR_PACK_HEAD::Serialize(CStdSerialize &aoStdSerial)
{
	try
	{
		aoStdSerial.Serialize(m_wSvrType);
		aoStdSerial.Serialize(m_nSvrID);

		return TRUE;
	}
	catch ( ... )
	{
		return FALSE;
	}
}
