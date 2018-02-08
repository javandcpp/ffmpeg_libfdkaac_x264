//
//
// 
//
//  @ Project : CMSP
//  @ File Name : LogicProcess.h
//  @ Date : 2014/6/2
//  @ Author : junjiang
//
//
#include "LocalUdpSession.h"
#include "ConfigManager.h"
#include "BaseThread.h"
#include "DebugTrace.h"
#include "kernel.h"
#include <event.h>
#include <string>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

CLocalUdpFluxCount::CLocalUdpFluxCount(void)
{
	CSingletonCfgManager::Instance().Load("server.ini");

	m_i64BeginTime = CBaseThread::GetSystemTime();
	m_i64InComeFlux = 0;;
	m_i64OutComeFlux = 0;
}

CLocalUdpFluxCount::~CLocalUdpFluxCount(void)
{
}

//计算流量(单位：kbps/s)
int CLocalUdpFluxCount::CalcFluxCount(float &afOutputFlux,float &afInputFlux)
{
	int nTimeCap = (int)(CBaseThread::GetSystemTime() - m_i64BeginTime);
	if (nTimeCap > 0)
	{
		afOutputFlux = (float)m_i64OutComeFlux*8000/1024/nTimeCap;
		afInputFlux = (float)m_i64InComeFlux*8000/1024/nTimeCap;
		//TRACE1("UDP网络情况统计:上行:%.2f(kbps),下行:%.2f(kbps).\n",afOutputFlux,afInputFlux);		
	}
	if (nTimeCap > 5000)
	{
		ResetCount();
	}
	return nTimeCap;
}

//重置状态
void CLocalUdpFluxCount::ResetCount()
{
	m_i64BeginTime = CBaseThread::GetSystemTime();
	m_i64InComeFlux = 0;
	m_i64OutComeFlux = 0;
}

CLocalUdpSession::CLocalUdpSession(CKernel* apKernel)
{
	//初始化报序号为一个2个字节的随机数
	srand((long)time(NULL));
    m_bActive = FALSE; //是活动
	

	m_pKernel = apKernel;

	m_i64PacketCount = 0;
}

CLocalUdpSession::~CLocalUdpSession()
{
}

//打开连接
BOOL CLocalUdpSession::Open()
{
    CBaseThread loThread;
    loThread.BeginThread(WorkThread,this);
    return TRUE;
}


//数据处理函数
void CLocalUdpSession::OnRecvUdpData(char *apRecvData,int aiDataLen)
{
	try
	{
		m_oRDSFluxCount.AddInFluxCount(aiDataLen);          //接收字节数
		WORD awSvrType = *(WORD*)(apRecvData);
		//if (awSvrType == PACK_CMS_FLV_DATA)
		{
			m_pKernel->DealCMSRtp(apRecvData,aiDataLen);

			m_i64PacketCount++;
		}
	}
	catch(...)
	{
		TraceLog1("%s exception\n",__FUNCTION__);
	}
}


//查询网络统计情况
int CLocalUdpSession::GetFluxCount(float &afOutputFlux,float &afInputFlux,INT64 &ai64PacketCount)
{
	ai64PacketCount = m_i64PacketCount;
	int nTimeGap = m_oRDSFluxCount.CalcFluxCount(afOutputFlux,afInputFlux);	
    if (nTimeGap > 0 )
    {
        return nTimeGap;
    }

    return 0;
}

unsigned int CLocalUdpSession::WorkThread(void* apVoid)
{
    CLocalUdpSession* lpThis = (CLocalUdpSession*)(apVoid);
    if(lpThis)
    {
        lpThis->Work();
    }
}

void CLocalUdpSession::Work()
{
    std::string strIP = (std::string)CSingletonCfgManager::Instance().GetValue("server",
            "ip");

    int port = (int)CSingletonCfgManager::Instance().GetValue("server"
            ,"port");

    int udp_socket = socket(AF_INET,SOCK_DGRAM,0);
    if(udp_socket == -1)
    {
        TraceLog0("%s udp_socket == -1\n",__FUNCTION__);
    }

    int flags = fcntl(udp_socket,F_GETFL);
    flags |= O_NONBLOCK;
    fcntl(udp_socket,F_SETFL,flags);

    int reuse = 1;
    setsockopt(udp_socket,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(reuse));

    struct sockaddr_in svr_addr;
    memset(&svr_addr,0,sizeof(struct sockaddr_in));
    svr_addr.sin_family = AF_INET;
    svr_addr.sin_addr.s_addr = inet_addr(strIP.c_str());
    svr_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    svr_addr.sin_port = htons(port);

    if (::bind(udp_socket,(struct sockaddr*)(&svr_addr),sizeof(svr_addr)) == -1)
    {
        return;
    }

    struct event_base *base = event_base_new();
    struct event *read_event = event_new(base,udp_socket,EV_READ|EV_PERSIST,
            CLocalUdpSession::read_cb,this);

    event_add(read_event,NULL);

    TraceLog0("________________IP:%s____________port:%d____________________________\n",strIP.c_str(),port);

    event_base_dispatch(base);
}

void CLocalUdpSession::read_cb(evutil_socket_t fd ,short what, void *arg)
{
    char recv_buf[2048];
    struct sockaddr_in from;
    socklen_t len_from = 0;
    socklen_t  len = recvfrom(fd,recv_buf,2048,0,(struct sockaddr*)(&from),&len_from);
    if (len == -1)
    {
        TraceLog0("%s recvfrom return -1\n",__FUNCTION__);
        return;
    }

    CLocalUdpSession *lpThis = (CLocalUdpSession*)(arg);
    if (lpThis)
    {
        lpThis->OnRecvUdpData(recv_buf,len);
    }
}
