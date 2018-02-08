//
//
// 
//
//  @ Project : CMSP
//  @ File Name : LocalUdpSession.h
//  @ Date : 2014/6/3
//  @ Author : junjiang
//
//


#pragma once 
#include "TypeDeff.h"
#include "event2/util.h"

class CKernel;
class CLocalUdpFluxCount
{
public:
	CLocalUdpFluxCount(void);
	~CLocalUdpFluxCount(void);
public:
	//增加流入计数(单位：字节)
	__inline void AddInFluxCount(INT64 ai64InComeFlux) { m_i64InComeFlux += ai64InComeFlux;}
	
	//增加流出计数(单位：字节)
	__inline void AddOutFluxCount(INT64 ai64OutComeFlux) { m_i64OutComeFlux += ai64OutComeFlux;}

	//计算流量(单位：kbps/s)
	int CalcFluxCount(float &afOutputFlux,float &afInputFlux);

	//重置状态
	void ResetCount();

private:
	INT64 m_i64BeginTime;       //开始时间
	INT64 m_i64InComeFlux;      //流入字节数
	INT64 m_i64OutComeFlux;     //流出字节数
};


class CLocalUdpSession
{
public:
	CLocalUdpSession(CKernel*);
	~CLocalUdpSession();

public:   
	//打开连接
	BOOL Open();
	
	//内存输出
	void Dump();

public:
	//查询网络统计情况
	int GetFluxCount(float &afOutputFlux,float &afInputFlux,INT64 &ai64PacketCount);

private:
    void OnRecvUdpData(char *apRecvData,int aiDataLen);

    static unsigned int WorkThread(void*);

    void Work();

    static void read_cb(evutil_socket_t fd ,short what, void *arg);

private:	
    //是否活动	
	BOOL    m_bActive;						

	CLocalUdpFluxCount m_oRDSFluxCount;

private:
	INT64		m_i64PacketCount;

private:
	CKernel* m_pKernel;
};


