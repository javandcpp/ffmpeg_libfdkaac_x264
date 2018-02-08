// $_FILEHEADER_BEGIN ****************************
// 北京八度互联科技有限公司版权所有
// Copyright (C) Badu Corporation.  All Rights Reserved
// 文件名称：cmsp_kernel.h
// 创建日期：2014-06-01
// 创建人：  junjiang
// 修改人：
// 文件说明：
// $_FILEHEADER_END ******************************
#pragma once

#include "TypeDeff.h"
#include "CriticalLock.h"

class CLocalUdpSession;
class FLVService;

class CKernel 
{
public:
	CKernel();
	~CKernel();

public:
	BOOL Open();
	void Close();


public:
	void DealCMSRtp(char *apRecvData,int aiDataLen);

private:
	static unsigned int WorkThread(void *apParam);
	void Work();

private:
	CCriticalLock						m_oCSThreadCount;
	BOOL									m_bStop;
	long									m_lThreadCount;

private:
	CLocalUdpSession*			m_pLocalUdpSession;
	FLVService*					m_pFLVService;
};

 
