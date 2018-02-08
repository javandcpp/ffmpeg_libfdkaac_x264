#include "kernel.h"
#include "DebugTrace.h"
#include "LocalUdpSession.h"
#include "flv_service.h"
#include "ConfigManager.h"

CKernel::CKernel()
{
	m_pLocalUdpSession = new CLocalUdpSession(this);
	m_pFLVService = new FLVService();
}

CKernel::~CKernel()
{
	Close();

	delete m_pFLVService;
	delete m_pLocalUdpSession;

}

BOOL CKernel::Open()
{	
	TraceLog0("%s\n", GetRtmpAsynVersion().c_str());
	if(!m_pFLVService->Open())
	{
		TraceLog0("%s m_pFLVService->Open()Ê§°Ü\n",__FUNCTION__);
		return FALSE;
	}

	if (!m_pLocalUdpSession->Open())
	{
		TraceLog0("%s m_pLocalUdpSession->Open()Ê§°Ü\n",__FUNCTION__);
		return FALSE;
	}

	m_bStop = FALSE;

	CBaseThread loThread;
	loThread.BeginThread(WorkThread,this);

	return TRUE;
}

void CKernel::Close()
{
	m_bStop = TRUE;

	//m_pLocalUdpSession->Close();
	m_pFLVService->Close();

	while(m_lThreadCount)
	{
		CBaseThread::Sleep(10);
	}
	
}

unsigned int CKernel::WorkThread(void *apParam)
{
	CKernel *lpThis = (CKernel*)apParam;
	if (lpThis)
	{
		lpThis->m_oCSThreadCount.Lock();
		lpThis->m_lThreadCount ++;
		lpThis->m_oCSThreadCount.UnLock();

		lpThis->Work();

		lpThis->m_oCSThreadCount.Lock();
		lpThis->m_lThreadCount --;
		lpThis->m_oCSThreadCount.UnLock();
	}
	return 0;
}


void CKernel::Work()
{
	INT64 li64LoadFileTime = CBaseThread::GetSystemTime();
	INT64 li64DumpTime = 0;
	INT64 li64LastPacketCount = 0;
	INT64 li64CurPacketCount = 0;

	while(!m_bStop)
	{
		if (CBaseThread::GetSystemTime() - li64LoadFileTime > 3000)
		{
			CSingletonCfgManager::Instance().Load("server.ini");
			li64LoadFileTime = CBaseThread::GetSystemTime();
		}


		if (CBaseThread::GetSystemTime() - li64DumpTime > 5000)
		{
			if (m_pLocalUdpSession)
			{
				float lfIncoming = 0;
				float lfOutcoming = 0;
				m_pLocalUdpSession->GetFluxCount(lfOutcoming,lfIncoming,li64CurPacketCount);

				//TraceLog0("%s,Udp packet_count:%I64d  Udp In:%.2f(kbs),Out:%.2f(kbs)\n",__FUNCTION__,
                        //li64CurPacketCount - li64LastPacketCount,lfIncoming,lfOutcoming);

				li64LastPacketCount = li64CurPacketCount;
			}
			li64DumpTime = CBaseThread::GetSystemTime();
		}


		CBaseThread::Sleep(1000);
	}
}

void CKernel::DealCMSRtp(char *apRecvData,int aiDataLen)
{
	m_pFLVService->DealRecvUdp(apRecvData,aiDataLen);
}
