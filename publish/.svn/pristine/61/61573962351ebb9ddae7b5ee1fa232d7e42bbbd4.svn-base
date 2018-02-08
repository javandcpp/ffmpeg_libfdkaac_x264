
// $_FILEHEADER_BEGIN ****************************
// 
// Copyright (C) Badu Corporation.  All Rights Reserved
// 文件名称：BaseThread.cpp
// 创建日期：2005-04-05
// 创建人： 张宏涛
// 文件说明：线程基础函数库
// $_FILEHEADER_END ******************************


#include "BaseThread.h"
#include "DebugTrace.h"
#include <stdio.h>
#include <string.h>

#include <sys/types.h> 
#include <sys/timeb.h>
#include <sys/time.h>

#ifdef DEF_USE_MEDIA_TIMER
#include <MMSystem.h>
#pragma comment(lib, "Winmm.lib")
#else
#include <signal.h>
#endif //DEF_USE_MEDIA_TIMER

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBaseThread::CBaseThread()
{

}

CBaseThread::~CBaseThread()
{

}

bool CBaseThread::BeginThread(unsigned int (*apStartFunc)(void *),void *apParam)
{
    STRU_THREAD_INFO *lpThreadInfo = NULL;
    lpThreadInfo = new struct STRU_THREAD_INFO;
    if (NULL == lpThreadInfo)
    {
        return FALSE;
    }
    lpThreadInfo->mpUserParam = apParam;
    lpThreadInfo->mpThreadFunc = apStartFunc;

#ifdef WIN32

    unsigned int luThreadID = 0;
    m_hThreadHandle = _beginthreadex(NULL,0,Win32ThreadFunc,lpThreadInfo,0,&luThreadID);
    if (-1 == m_hThreadHandle)
    {
        return FALSE;
    }

    TRACE4("CBaseThread::BeginThread, luThreadID = %u !\n", luThreadID);

    CloseHandle((HANDLE)m_hThreadHandle);
    return TRUE;

#else
    sigset_t signal_mask;
    sigemptyset(&signal_mask);
    sigaddset(&signal_mask,SIGPIPE);
    int rc = pthread_sigmask(SIG_BLOCK,&signal_mask,NULL);

    if (rc != 0)
    {
	    printf("block sigpipe error\n");
    }


    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setscope(&attr,PTHREAD_SCOPE_SYSTEM);
    pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
    pthread_create(&m_thread,&attr,LinuxThreadFunc,lpThreadInfo);
    return TRUE;

#endif
}

#ifdef WIN32
unsigned int CBaseThread::Win32ThreadFunc(void * lpParam)
{
    STRU_THREAD_INFO *lpThreadInfo = (STRU_THREAD_INFO *)lpParam;

    ASSERT(lpThreadInfo != NULL);
    ASSERT(lpThreadInfo->mpThreadFunc != NULL);

	lpThreadInfo->mpThreadFunc(lpThreadInfo->mpUserParam);

	if (lpThreadInfo != NULL)
	{
		delete lpThreadInfo;
		lpThreadInfo = NULL;
	}
    _endthreadex(0);
    return 1;
}

#else

void * CBaseThread::LinuxThreadFunc(void * lpParam)
{
    struct STRU_THREAD_INFO *lpThreadInfo = (struct STRU_THREAD_INFO *)lpParam;

    ASSERT(lpThreadInfo != NULL);
    ASSERT(lpThreadInfo->mpThreadFunc != NULL);

    lpThreadInfo->mpThreadFunc(lpThreadInfo->mpUserParam);

	if (lpThreadInfo != NULL)
	{
		delete lpThreadInfo;
		lpThreadInfo = NULL;
	}
    return NULL;
}
#endif

#ifdef DEF_USE_MEDIA_TIMER
void WINAPI OnTimeEvent(UINT auTimerID, UINT auMsg, DWORD dwUser, DWORD dwl,DWORD dw2)
{
	HANDLE hEvnet = (HANDLE)dwUser;
	if (hEvnet)
	{
		SetEvent(hEvnet);
	}
	timeKillEvent(auTimerID);
}
#endif

//锟斤拷锟斤拷
void CBaseThread::Sleep(DWORD dwMilliseconds)
{
#ifdef _WIN32
#ifndef DEF_USE_MEDIA_TIMER
    ::Sleep(dwMilliseconds);
#else
	HANDLE hSleepEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (NULL != hSleepEvent)
	{
		timeSetEvent(dwMilliseconds, 1, OnTimeEvent, (DWORD_PTR)hSleepEvent, TIME_ONESHOT);

		WaitForSingleObject(hSleepEvent, dwMilliseconds);
		CloseHandle(hSleepEvent);
	}
	else
	{
		::Sleep(dwMilliseconds);
	}

#endif //DEF_USE_MEDIA_TIMER

#else
	DWORD ldwSleep = 1000*dwMilliseconds;
  	usleep(ldwSleep);
#endif	//_WIN32
}


INT64 CBaseThread::GetSystemTime()
{
    struct timeb loTimeb;
    memset(&loTimeb, 0 , sizeof(timeb));
    ftime(&loTimeb);
    return ((INT64)loTimeb.time * 1000) + loTimeb.millitm;
    //struct timeval tm;
    //gettimeofday(&tm,NULL);
    //return ((INT64)tm.tv_sec * 1000) + tm.tv_usec;
}

