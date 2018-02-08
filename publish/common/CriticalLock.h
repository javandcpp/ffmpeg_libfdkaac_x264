#ifndef __CRITICAL_LOCK__H_INCLUDE__
#define __CRITICAL_LOCK__H_INCLUDE__

#ifdef WIN32
	#include <Windows.h>
#else
	#include <pthread.h>
	#include <unistd.h>
#endif

class CCriticalLock
{
private:

#ifdef WIN32
	CRITICAL_SECTION	m_section;
#else
	pthread_mutex_t		m_mutex;
#endif

public:
	CCriticalLock()
	{
#ifdef WIN32
		InitializeCriticalSection(&m_section);
#else
		pthread_mutexattr_t mutex_attr;
		pthread_mutexattr_init(&mutex_attr);   
		pthread_mutexattr_settype(&mutex_attr, PTHREAD_MUTEX_RECURSIVE);   
		pthread_mutex_init(&m_mutex, &mutex_attr);
#endif
	};
	~CCriticalLock()
	{
#ifdef WIN32
		DeleteCriticalSection(&m_section);
#else
		pthread_mutex_destroy(&m_mutex);
#endif
	}
	__inline void Lock()
	{
#ifdef WIN32
		EnterCriticalSection(&m_section);
#else
		pthread_mutex_lock(&m_mutex);
#endif
	}
	__inline void UnLock()
	{
#ifdef WIN32
		LeaveCriticalSection(&m_section);
#else
		pthread_mutex_unlock(&m_mutex);
#endif
	};

private:
	CCriticalLock(const CCriticalLock&);
	CCriticalLock& operator = (const CCriticalLock&);
};

class CCriticalAutoLock
{
public:
	CCriticalAutoLock(CCriticalLock& aCriticalLock) : m_critical_lock(aCriticalLock)
	{
		m_critical_lock.Lock();
	};
	~CCriticalAutoLock()
	{
		m_critical_lock.UnLock();
	};
private:
	CCriticalLock&	m_critical_lock;

private:
	CCriticalAutoLock(const CCriticalAutoLock&);
	CCriticalAutoLock& operator = (const CCriticalAutoLock&);
};

#endif
