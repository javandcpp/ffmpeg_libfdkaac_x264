#ifndef __DEF_SERVER_PACK_HEAD_H__
#define __DEF_SERVER_PACK_HEAD_H__

#include "StdSerializeEx.h"
#include "StdSerialMacro.h"

struct STRU_SVR_PACK_HEAD
{
	WORD	m_wSvrType;			//�μ�����������ö�ٶ���,����ָ���ǰ��Ķ����ߵķ���������
	int		m_nSvrID;           //����ķ�����ID

public:
	STRU_SVR_PACK_HEAD();
	BOOL Serialize(CStdSerialize &aoStdSerial);
};

#endif//__DEF_SERVER_PACK_HEAD_H__
