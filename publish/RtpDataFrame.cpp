
#include <stdio.h>

#include <string.h>
#include <algorithm>
#include "TypeDeff.h"
#include "RtpDataFrame.h"
#include "DebugTrace.h"

/////////////////////////////////////////////////////////////////////////////

CRtpDataFrame::CRtpDataFrame(int nDataSize)  
{
	m_nDataLen = nDataSize + 16;
	m_nDataBuffLen = nDataSize + 16;	
	m_pFrameBuff = new char[m_nDataBuffLen];

	m_bSelfMalloc = true;
	memset(m_pFrameBuff,0,16);    
	m_pFrameBuff[0] = '\xC0';

	m_nPayloadSize = 0;

    m_nHeadSize = 0;
}

CRtpDataFrame::CRtpDataFrame(const char *pDataBuff,int nBuffLen,int nDataLen)
{
	ASSERT(pDataBuff != NULL && nBuffLen >= nDataLen);

	m_nDataLen = nDataLen;
	m_nDataBuffLen = nBuffLen;	
	m_pFrameBuff = (char *)pDataBuff;

    m_nHeadSize = GetHeaderSize();
	m_nPayloadSize = nDataLen - GetHeaderSize();

	m_bSelfMalloc = FALSE;
}

CRtpDataFrame::~CRtpDataFrame()
{	
	if (m_pFrameBuff && m_bSelfMalloc)
	{
		delete []m_pFrameBuff;
		m_pFrameBuff = NULL;
	}

	m_nDataBuffLen = 0;
}

void CRtpDataFrame::operator = (CRtpDataFrame &aoRtpFrame)
{	
	if (m_nDataBuffLen < aoRtpFrame.GetFrameDataLen())
	{
		if (m_bSelfMalloc)
		{
			delete []m_pFrameBuff;
			m_pFrameBuff = NULL;
		}
		m_pFrameBuff = new char[aoRtpFrame.m_nDataBuffLen];	
		m_nDataBuffLen = aoRtpFrame.m_nDataBuffLen;

		m_bSelfMalloc = TRUE;
	}
	m_nPayloadSize = aoRtpFrame.m_nPayloadSize;
    int il = aoRtpFrame.GetFrameDataLen(); 
    if (il > aoRtpFrame.m_nDataBuffLen)
    {
        TraceLog0("[aoRtpFrame]   m_nHeadSize :%d, m_nPayloadSize:%d,  m_nDataBuffLen:%d, m_nDataLen : %d\n", 
                    aoRtpFrame.m_nHeadSize, aoRtpFrame.m_nPayloadSize,aoRtpFrame.m_nDataBuffLen, aoRtpFrame.m_nDataLen);
    }
	memcpy(m_pFrameBuff,aoRtpFrame.m_pFrameBuff,il);
}

bool CRtpDataFrame::MallocFrameBuff(int nBuffLength)
{
	if (m_nDataBuffLen < nBuffLength)
	{
		char *pBuffPtr = new char[nBuffLength];	
		memcpy(pBuffPtr,m_pFrameBuff,GetFrameDataLen());
		m_nDataBuffLen = nBuffLength;

		if (m_bSelfMalloc)
		{
			delete []m_pFrameBuff;
		}
		m_pFrameBuff = pBuffPtr;
		m_bSelfMalloc = TRUE;
	}
	return true;
}

//取得缓存数据
int CRtpDataFrame::GetData(char *pDataBuff, int nBuffLen)
{
	if(NULL == pDataBuff)
		return m_nDataLen;

	int iLen = std::min(m_nDataLen,nBuffLen);
	if(iLen <= 0)
		return 0;

	memcpy(pDataBuff,m_pFrameBuff,iLen);
	return iLen;
}

//设置是否有扩展剖面标志
void CRtpDataFrame::SetExtendMark(BOOL bExtPro)
{
	if (bExtPro)
		m_pFrameBuff[0] |= 0x10;
	else
		m_pFrameBuff[0] &= 0xef;
}

//设置扩展剖面
void CRtpDataFrame::SetExtProfile(short nExtProfile)		
{
	ASSERT(GetExtendMark());
	ASSERT(m_nDataBuffLen >= (16 + (GetContribSrcCount() << 2)));

	*(short *)&m_pFrameBuff[12 + (GetContribSrcCount() << 2)] = nExtProfile; 	
}

//取得扩展剖面
short CRtpDataFrame::GetExtProfile(void)		
{
	ASSERT(GetExtendMark());
	return (*(short *)&m_pFrameBuff[12 + (GetContribSrcCount() << 2)]); 
}

//设置扩展剖面长度
void CRtpDataFrame::SetExtProLen(short nExtenLen)	
{
	ASSERT(nExtenLen >= 0);
	ASSERT(GetExtendMark());
	ASSERT(m_nDataBuffLen >= (16 + (GetContribSrcCount() << 2) + nExtenLen));

	*(short *)&m_pFrameBuff[14 + (GetContribSrcCount() << 2)] = nExtenLen;
}

//取得扩展剖面长度
short CRtpDataFrame::GetExtProLen()	
{	
	ASSERT(GetExtendMark());
	return *(short *)&m_pFrameBuff[14 + (GetContribSrcCount() << 2)]; 	
}

//设置扩展剖面数据
void CRtpDataFrame::CopyExtProData(const char *pExtProData,short nDataLen)
{
	ASSERT(GetExtendMark());
	ASSERT(nDataLen <= GetExtProLen());
	ASSERT(m_nDataBuffLen >= (16 + (GetContribSrcCount() << 2) + nDataLen));	
	
	if (m_nDataBuffLen >= (16 + (GetContribSrcCount() << 2)  + nDataLen))
	{
		memcpy(m_pFrameBuff + 16 + (GetContribSrcCount() << 2),pExtProData,nDataLen);
	}	
}

//取得扩展剖面数据
short CRtpDataFrame::GetExtProData(char *pExtProData,short nBuffLen)
{
	ASSERT(pExtProData != NULL);
	ASSERT(nBuffLen >= GetExtProLen());
	ASSERT(GetExtendMark());

	if (nBuffLen >= GetExtProLen())
	{
		short nDataLen = GetExtProLen();
		memcpy(pExtProData,m_pFrameBuff + 16 + (GetContribSrcCount() << 2),nDataLen);
		return nDataLen;
	}
	return 0;
}

void CRtpDataFrame::SetMarker(BOOL m)
{
	if (m)
		m_pFrameBuff[1] |= 0x80;
	else
		m_pFrameBuff[1] &= 0x7f;
}

void CRtpDataFrame::SetPayloadType(EnumPayloadTypes t)
{
	ASSERT(t <= 0x7f);

	m_pFrameBuff[1] &= 0x80;
	m_pFrameBuff[1] |= t;
}

int CRtpDataFrame::GetHeaderSize()     const
{ 		
	if (!GetExtendMark())	//无扩展信息
		return (12 + (GetContribSrcCount() << 2)); 

	//有扩展信息		
	return (*(short *)&m_pFrameBuff[14 + (GetContribSrcCount() << 2)] + 16 + (GetContribSrcCount() << 2));
}

//设置提供源数量
void CRtpDataFrame::SetContribSrcCount(int nCscCount)
{
	ASSERT(nCscCount < 0xF);

	m_pFrameBuff[0] &= 0xf0;
	m_pFrameBuff[0] |= nCscCount;
}

UINT CRtpDataFrame::GetContribSource(int idx) const
{
	ASSERT(idx < GetContribSrcCount());
	return ((UINT *)&m_pFrameBuff[12])[idx];
}

//提供源
void CRtpDataFrame::SetContribSource(int idx, UINT src)
{
	ASSERT(idx < GetContribSrcCount());

	((UINT *)&m_pFrameBuff[12])[idx] = src;
}

//帧数据长度
int CRtpDataFrame::GetFrameDataLen() 
{
	return (GetHeaderSize() + GetPayloadSize());
/*
	if (!GetFillMark())
	{
		return (GetHeaderSize() + GetPayloadSize());
	}

	int nPayloadSize = GetHeaderSize() + GetPayloadSize();
	BYTE nFillLen = (*(BYTE *)&m_pFrameBuff[nPayloadSize - 1]);

	return nPayloadSize + nFillLen;
*/
}

//填充位信息
void CRtpDataFrame::SetFillMark(BOOL bFillMark) 
{
	if (bFillMark)
		m_pFrameBuff[0] |= 0x20;
	else
		m_pFrameBuff[0] &= 0xdf;
}     

//取得附加填充数据长度
BYTE CRtpDataFrame::GetExDataLen()
{
	if (GetFillMark())
	{
		int nFrameDataLen = GetHeaderSize() + GetPayloadSize();

		ASSERT(m_nDataBuffLen >= nFrameDataLen);
		return *(BYTE *)&m_pFrameBuff[nFrameDataLen - 1];
	}
	return 0;
}
/////////////////////////////////////////////////////////////////////////////
//rtcp
void CRtpDataFrame::ReceiverReport::SetLostPackets(unsigned packets)
{
   lost[0] = (BYTE)(packets >> 16);
   lost[1] = (BYTE)(packets >> 8);
   lost[2] = (BYTE)packets;
}

void CRtpDataFrame::PrintData()
{
    char mapPrint[1024]={0};  // 能打印头部数据就行
    int pos = 0;
    for (int i=0; i<m_nDataLen; ++i)
    {
        int ret = snprintf(mapPrint+pos, 1020-pos, "\\%o", (unsigned char)(m_pFrameBuff[i]) );
        if (ret < 1020-pos)
        {
            pos += ret;
        }
        else if (ret >= 1020-pos)
        {
            // 1020-pos空间不够，写不下，已经写到1020位置，且1020-1位置写入的是'\0'
            // 最后 ... 表示，内容没写完
            mapPrint[1019] = '.'; mapPrint[1020] = '.'; mapPrint[1021] = '.'; 
            break;
        }
        else
        {
            // 最后 xxx 表示，出错
            mapPrint[1019] = 'x'; mapPrint[1020] = 'x'; mapPrint[1021] = 'x';
            break;
        }
    }
    TraceLog0("CRtpDataFrame::PrintData -------------- \n%s\n", mapPrint);
}
