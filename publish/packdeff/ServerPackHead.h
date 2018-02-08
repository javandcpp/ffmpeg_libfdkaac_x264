#ifndef __DEF_SERVER_PACK_HEAD_H__
#define __DEF_SERVER_PACK_HEAD_H__

#include "StdSerializeEx.h"
#include "StdSerialMacro.h"

struct STRU_SVR_PACK_HEAD
{
	WORD	m_wSvrType;			//参见服务器类型枚举定义,这里指的是包的定义者的服务器类型
	int		m_nSvrID;           //具体的服务器ID

public:
	STRU_SVR_PACK_HEAD();
	BOOL Serialize(CStdSerialize &aoStdSerial);
};

#endif//__DEF_SERVER_PACK_HEAD_H__
