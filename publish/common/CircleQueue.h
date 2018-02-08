#ifndef __CIRCLE_QUEUE_H__
#define __CIRCLE_QUEUE_H__

#define DEF_CIRCLE_QUEUE_VER			1
#include "TypeDeff.h"

template<class T>
class CCircleQueue
{
	typedef T* PT;

public:
	CCircleQueue()
	{
		m_pList = NULL;
		m_lListLen = 0;
		m_lHeadIndex = 0;
		m_lTailIndex = 0;
	};

	//初始化队列
	void Init(int alQueueLen)
	{
		m_pList = new T[alQueueLen + 1];
		//ASSERT(m_pList);
		m_lListLen = alQueueLen + 1;
	};

	//清空
	void Clear()
	{
		if(m_pList)
			delete[] m_pList;
		m_pList = NULL;
		m_lListLen = 0;
		m_lHeadIndex = 0;
		m_lTailIndex = 0;
	};

	//移除所有
	void RemoveAll()
	{
		m_lHeadIndex = 0;
		m_lTailIndex = 0;
	};

	//压入
	bool Push(T aVal)
	{
		bool bResult = false;
		int lNextIndex = (m_lHeadIndex + 1) % m_lListLen;
		//加入队列
		if(lNextIndex != m_lTailIndex)
		{
			//还有空间
			m_pList[m_lHeadIndex] = aVal;
			m_lHeadIndex = lNextIndex;
			bResult = true;
		}
		return bResult;
	};

	//弹出
	bool Pop(T &aVal)
	{
		//判断是否超过最大值
		bool bResult = false;
		if(m_lTailIndex != m_lHeadIndex)
		{
			aVal = m_pList[m_lTailIndex];
			m_lTailIndex = (m_lTailIndex + 1) % m_lListLen;
			bResult = TRUE;
		}
		return bResult;
	};

	//取队列长度
	int GetQueueLen()
	{
		int liCount = m_lHeadIndex - m_lTailIndex;
		if(liCount < 0)
			liCount += m_lListLen;
		return liCount;
	};

	//根据索引取得数据
	bool GetIndexData(int alIndex, T& aVal)
	{
		int lLen = GetQueueLen();

		if(lLen < 1 || alIndex < 0 || alIndex >= lLen)
		{
			return false;
		}

		aVal = m_pList[(m_lTailIndex + alIndex) % m_lListLen];

		return TRUE;
	};

	//是否满的
	bool IsFull()
	{
		bool bResult = false;
		int lNextIndex = (m_lHeadIndex + 1) % m_lListLen;
		if(lNextIndex == m_lTailIndex)
		{
			bResult = TRUE;
		}
		return bResult;
	};

	//最大长度
	int GetMaxLen()
	{
		return m_lListLen - 1;
	};

	//存储
	int Save(BYTE* apOutbuf, DWORD adwBufSize)
	{
		BYTE* pDest = apOutbuf;
		int len = 0;
		//存一字节版本号
		if(pDest)
		{
			if(adwBufSize < 1)
				return -1;
			*(BYTE*)pDest = DEF_CIRCLE_QUEUE_VER;
			pDest++;
		}
		len ++;
		
		//列表长度
		if(pDest)
		{
			if(adwBufSize < len + sizeof(int))
				return -1;
			*(int*)pDest = m_lListLen;
			pDest+= sizeof(int);
		}
		len += sizeof(int);
		
		//头索引
		if(pDest)
		{
			if(adwBufSize < len + sizeof(int))
				return -1;
			*(int*)pDest = m_lHeadIndex;
			pDest+= sizeof(int);
		}
		len += sizeof(int);
		
		//尾索引
		if(pDest)
		{
			if(adwBufSize < len + sizeof(int))
				return -1;
			*(int*)pDest = m_lTailIndex;
			pDest+= sizeof(int);
		}
		len += sizeof(int);
		
		//数据
		int lDataLen = m_lListLen * sizeof(T);
		if(pDest)
		{
			if((int)adwBufSize < len + lDataLen)
				return -1;
			memcpy(pDest,m_pList,lDataLen);
		}
		len += lDataLen;
		return len;
	};

	//恢复
	int Restore(BYTE* apInbuf, DWORD adwBufSize)
	{
		if(adwBufSize < 13 || NULL == apInbuf)
		{
			return -1;
		}
		//取版本
		BYTE ver = *apInbuf;
		if(ver > DEF_CIRCLE_QUEUE_VER)
			return -1;
		apInbuf ++;

		//取长度
		int lListLen = *(int*)apInbuf;
		if(lListLen != m_lListLen)
			return -1;
		apInbuf += sizeof(int);

		//取头
		m_lHeadIndex = *(int*)apInbuf;
		if(m_lHeadIndex >= m_lListLen)
			return -1;
		apInbuf += sizeof(int);

		//取尾
		m_lTailIndex = *(int*)apInbuf;
		if(m_lTailIndex >= m_lListLen)
			return -1;
		apInbuf += sizeof(int);

		//取数据
		if(lListLen + 13 > (int)adwBufSize)
			return -1;
		memcpy(m_pList,apInbuf, m_lListLen * sizeof(T));

		return DEF_CIRCLE_QUEUE_VER + sizeof(int) + sizeof(int) + sizeof(int) + m_lListLen * sizeof(T);
	};

private:
	PT								m_pList;
	int							m_lListLen;
	int							m_lHeadIndex;
	int							m_lTailIndex;
};

template<class T, int L>
class CCircleFixedQueue
{
public:
	CCircleFixedQueue()
	{
		m_lListLen = L + 1;
		m_lHeadIndex = 0;
		m_lTailIndex = 0;
	};

	//清空
	void Clear()
	{
		m_lHeadIndex = 0;
		m_lTailIndex = 0;
	};

	//压入
	bool Push(T aVal)
	{
		bool bResult = false;
		int lNextIndex = (m_lHeadIndex + 1) % m_lListLen;
		//加入队列
		if(lNextIndex != m_lTailIndex)
		{
			//还有空间
			m_oList[m_lHeadIndex] = aVal;
			m_lHeadIndex = lNextIndex;
			bResult = TRUE;
		}
		return bResult;
	};

	//弹出
	bool Pop(T &aVal)
	{
		//判断是否超过最大值
		bool bResult = false;
		if(m_lTailIndex != m_lHeadIndex)
		{
			aVal = m_oList[m_lTailIndex];
			m_lTailIndex = (m_lTailIndex + 1) % m_lListLen;
			bResult = TRUE;
		}
		return bResult;
	};

	//取队列长度
	int GetQueueLen()
	{
		int liCount = m_lHeadIndex - m_lTailIndex;
		if(liCount < 0)
			liCount += m_lListLen;
		return liCount;
	};

	//根据索引取得数据
	bool GetIndexData(int alIndex, T& aVal)
	{
		int lLen = GetQueueLen();

		if(lLen < 1 || alIndex < 0 || alIndex >= lLen)
		{
			return false;
		}

		aVal = m_oList[(m_lTailIndex + alIndex) % m_lListLen];

		return TRUE;
	};

	//是否满的
	bool IsFull()
	{
		bool bResult = false;
		int lNextIndex = (m_lHeadIndex + 1) % m_lListLen;
		if(lNextIndex == m_lTailIndex)
		{
			bResult = TRUE;
		}
		return bResult;
	};

	//最大长度
	int GetMaxLen()
	{
		return m_lListLen - 1;
	};

	//存储
	int Save(BYTE* apOutbuf, DWORD adwBufSize)
	{
		BYTE* pDest = apOutbuf;
		int len = 0;
		//存一字节版本号
		if(pDest)
		{
			if(adwBufSize < 1)
				return -1;
			*(BYTE*)pDest = DEF_CIRCLE_QUEUE_VER;
			pDest++;
		}
		len ++;
		
		//列表长度
		if(pDest)
		{
			if(adwBufSize < len + sizeof(int))
				return -1;
			*(int*)pDest = m_lListLen;
			pDest+= sizeof(int);
		}
		len += sizeof(int);
		
		//头索引
		if(pDest)
		{
			if(adwBufSize < len + sizeof(int))
				return -1;
			*(int*)pDest = m_lHeadIndex;
			pDest+= sizeof(int);
		}
		len += sizeof(int);
		
		//尾索引
		if(pDest)
		{
			if(adwBufSize < len + sizeof(int))
				return -1;
			*(int*)pDest = m_lTailIndex;
			pDest+= sizeof(int);
		}
		len += sizeof(int);
		
		//数据
		int lDataLen = m_lListLen * sizeof(T);
		if(pDest)
		{
			if((int)adwBufSize < len + lDataLen)
				return -1;
			memcpy(pDest,m_oList,lDataLen);
		}
		len += lDataLen;
		return len;
	};

	//恢复
	int Restore(BYTE* apInbuf, DWORD adwBufSize)
	{
		if(adwBufSize < 13 || NULL == apInbuf)
		{
			return -1;
		}
		//取版本
		BYTE ver = *apInbuf;
		if(ver > DEF_CIRCLE_QUEUE_VER)
			return -1;
		apInbuf ++;

		//取长度
		int lListLen = *(int*)apInbuf;
		if(lListLen != m_lListLen)
			return -1;
		apInbuf += sizeof(int);

		//取头
		m_lHeadIndex = *(int*)apInbuf;
		if(m_lHeadIndex >= m_lListLen)
			return -1;
		apInbuf += sizeof(int);

		//取尾
		m_lTailIndex = *(int*)apInbuf;
		if(m_lTailIndex >= m_lListLen)
			return -1;
		apInbuf += sizeof(int);

		//取数据
		if(lListLen + 13 > (int)adwBufSize)
			return -1;
		memcpy(m_oList,apInbuf, m_lListLen * sizeof(T));

		return DEF_CIRCLE_QUEUE_VER + sizeof(int) + sizeof(int) + sizeof(int) + m_lListLen * sizeof(T);
	};

private:
	T								m_oList[L + 1];
	int							m_lListLen;
	int							m_lHeadIndex;
	int							m_lTailIndex;
};


#endif // __CIRCLE_QUEUE_H__
