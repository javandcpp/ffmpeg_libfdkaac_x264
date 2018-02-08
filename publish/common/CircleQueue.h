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

	//��ʼ������
	void Init(int alQueueLen)
	{
		m_pList = new T[alQueueLen + 1];
		//ASSERT(m_pList);
		m_lListLen = alQueueLen + 1;
	};

	//���
	void Clear()
	{
		if(m_pList)
			delete[] m_pList;
		m_pList = NULL;
		m_lListLen = 0;
		m_lHeadIndex = 0;
		m_lTailIndex = 0;
	};

	//�Ƴ�����
	void RemoveAll()
	{
		m_lHeadIndex = 0;
		m_lTailIndex = 0;
	};

	//ѹ��
	bool Push(T aVal)
	{
		bool bResult = false;
		int lNextIndex = (m_lHeadIndex + 1) % m_lListLen;
		//�������
		if(lNextIndex != m_lTailIndex)
		{
			//���пռ�
			m_pList[m_lHeadIndex] = aVal;
			m_lHeadIndex = lNextIndex;
			bResult = true;
		}
		return bResult;
	};

	//����
	bool Pop(T &aVal)
	{
		//�ж��Ƿ񳬹����ֵ
		bool bResult = false;
		if(m_lTailIndex != m_lHeadIndex)
		{
			aVal = m_pList[m_lTailIndex];
			m_lTailIndex = (m_lTailIndex + 1) % m_lListLen;
			bResult = TRUE;
		}
		return bResult;
	};

	//ȡ���г���
	int GetQueueLen()
	{
		int liCount = m_lHeadIndex - m_lTailIndex;
		if(liCount < 0)
			liCount += m_lListLen;
		return liCount;
	};

	//��������ȡ������
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

	//�Ƿ�����
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

	//��󳤶�
	int GetMaxLen()
	{
		return m_lListLen - 1;
	};

	//�洢
	int Save(BYTE* apOutbuf, DWORD adwBufSize)
	{
		BYTE* pDest = apOutbuf;
		int len = 0;
		//��һ�ֽڰ汾��
		if(pDest)
		{
			if(adwBufSize < 1)
				return -1;
			*(BYTE*)pDest = DEF_CIRCLE_QUEUE_VER;
			pDest++;
		}
		len ++;
		
		//�б���
		if(pDest)
		{
			if(adwBufSize < len + sizeof(int))
				return -1;
			*(int*)pDest = m_lListLen;
			pDest+= sizeof(int);
		}
		len += sizeof(int);
		
		//ͷ����
		if(pDest)
		{
			if(adwBufSize < len + sizeof(int))
				return -1;
			*(int*)pDest = m_lHeadIndex;
			pDest+= sizeof(int);
		}
		len += sizeof(int);
		
		//β����
		if(pDest)
		{
			if(adwBufSize < len + sizeof(int))
				return -1;
			*(int*)pDest = m_lTailIndex;
			pDest+= sizeof(int);
		}
		len += sizeof(int);
		
		//����
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

	//�ָ�
	int Restore(BYTE* apInbuf, DWORD adwBufSize)
	{
		if(adwBufSize < 13 || NULL == apInbuf)
		{
			return -1;
		}
		//ȡ�汾
		BYTE ver = *apInbuf;
		if(ver > DEF_CIRCLE_QUEUE_VER)
			return -1;
		apInbuf ++;

		//ȡ����
		int lListLen = *(int*)apInbuf;
		if(lListLen != m_lListLen)
			return -1;
		apInbuf += sizeof(int);

		//ȡͷ
		m_lHeadIndex = *(int*)apInbuf;
		if(m_lHeadIndex >= m_lListLen)
			return -1;
		apInbuf += sizeof(int);

		//ȡβ
		m_lTailIndex = *(int*)apInbuf;
		if(m_lTailIndex >= m_lListLen)
			return -1;
		apInbuf += sizeof(int);

		//ȡ����
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

	//���
	void Clear()
	{
		m_lHeadIndex = 0;
		m_lTailIndex = 0;
	};

	//ѹ��
	bool Push(T aVal)
	{
		bool bResult = false;
		int lNextIndex = (m_lHeadIndex + 1) % m_lListLen;
		//�������
		if(lNextIndex != m_lTailIndex)
		{
			//���пռ�
			m_oList[m_lHeadIndex] = aVal;
			m_lHeadIndex = lNextIndex;
			bResult = TRUE;
		}
		return bResult;
	};

	//����
	bool Pop(T &aVal)
	{
		//�ж��Ƿ񳬹����ֵ
		bool bResult = false;
		if(m_lTailIndex != m_lHeadIndex)
		{
			aVal = m_oList[m_lTailIndex];
			m_lTailIndex = (m_lTailIndex + 1) % m_lListLen;
			bResult = TRUE;
		}
		return bResult;
	};

	//ȡ���г���
	int GetQueueLen()
	{
		int liCount = m_lHeadIndex - m_lTailIndex;
		if(liCount < 0)
			liCount += m_lListLen;
		return liCount;
	};

	//��������ȡ������
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

	//�Ƿ�����
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

	//��󳤶�
	int GetMaxLen()
	{
		return m_lListLen - 1;
	};

	//�洢
	int Save(BYTE* apOutbuf, DWORD adwBufSize)
	{
		BYTE* pDest = apOutbuf;
		int len = 0;
		//��һ�ֽڰ汾��
		if(pDest)
		{
			if(adwBufSize < 1)
				return -1;
			*(BYTE*)pDest = DEF_CIRCLE_QUEUE_VER;
			pDest++;
		}
		len ++;
		
		//�б���
		if(pDest)
		{
			if(adwBufSize < len + sizeof(int))
				return -1;
			*(int*)pDest = m_lListLen;
			pDest+= sizeof(int);
		}
		len += sizeof(int);
		
		//ͷ����
		if(pDest)
		{
			if(adwBufSize < len + sizeof(int))
				return -1;
			*(int*)pDest = m_lHeadIndex;
			pDest+= sizeof(int);
		}
		len += sizeof(int);
		
		//β����
		if(pDest)
		{
			if(adwBufSize < len + sizeof(int))
				return -1;
			*(int*)pDest = m_lTailIndex;
			pDest+= sizeof(int);
		}
		len += sizeof(int);
		
		//����
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

	//�ָ�
	int Restore(BYTE* apInbuf, DWORD adwBufSize)
	{
		if(adwBufSize < 13 || NULL == apInbuf)
		{
			return -1;
		}
		//ȡ�汾
		BYTE ver = *apInbuf;
		if(ver > DEF_CIRCLE_QUEUE_VER)
			return -1;
		apInbuf ++;

		//ȡ����
		int lListLen = *(int*)apInbuf;
		if(lListLen != m_lListLen)
			return -1;
		apInbuf += sizeof(int);

		//ȡͷ
		m_lHeadIndex = *(int*)apInbuf;
		if(m_lHeadIndex >= m_lListLen)
			return -1;
		apInbuf += sizeof(int);

		//ȡβ
		m_lTailIndex = *(int*)apInbuf;
		if(m_lTailIndex >= m_lListLen)
			return -1;
		apInbuf += sizeof(int);

		//ȡ����
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
