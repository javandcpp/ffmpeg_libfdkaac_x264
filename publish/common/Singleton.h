// $_FILEHEADER_BEGIN ***************************
// 八度互联研发部版权所有
// Copyright (C) 八度互联研发部 Corporation.  All Rights Reserved
// 文件名称：Singleton.h
// 创建日期：2010-01-18
// 创建人：刘俊江
// 文件说明：单件模板类
//				 
// $_FILEHEADER_END *****************************

#pragma once
#include <stdlib.h>
#include "CriticalLock.h"


template< typename T >
class CSingleton
{
public:
	static T& Instance();

private:
	static void MakeInstance();
	static void DestroySingleton();

	static T* m_pInstance;

private:
	CSingleton();
	CSingleton(const CSingleton&);

	//临界区
	static CCriticalLock  m_oCriSection;
};

template<class T> CCriticalLock CSingleton<T>::m_oCriSection;
template<class T> T* CSingleton<T>::m_pInstance = 0;

template <class T> 
T& CSingleton<T>::Instance()
{
	if ( !m_pInstance)
	{
		MakeInstance();
	}

	return *m_pInstance;
}

template <class T>
inline void CSingleton<T>::MakeInstance()
{
	CCriticalAutoLock loGuard(m_oCriSection);

	if(!m_pInstance)
	{
		m_pInstance = new T;
		atexit(DestroySingleton);
	}
}

template <class T>
void CSingleton<T>::DestroySingleton()
{
	delete m_pInstance;
	m_pInstance = 0;
}

