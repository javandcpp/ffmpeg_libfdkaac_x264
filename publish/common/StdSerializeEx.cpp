#include "TypeDeff.h"
#include "StdSerializeEx.h"
#include "DebugTrace.h"
#include <string.h>

#ifdef WIN32
#include <exception>
#endif

// $_FUNCTION_BEGIN ******************************
// ��������: CStdSerialize
// ��������: char * apBuffer������buff
// long alBufLen : buff�ĳ���
// ENUM_TYPE abyType�����л�����( ����/���� )
// �� �� ֵ:
// ����˵��: ����
// $_FUNCTION_END ********************************
CStdSerialize::CStdSerialize( char * apBuffer,int  alBufLen, ENUM_TYPE abyType )
{
#ifndef __WXDEBUG__
	ASSERT(apBuffer != NULL);
	ASSERT(alBufLen > 0);
#endif

    m_pBuffer = apBuffer;
    m_byType = abyType;
    m_lBufLen = alBufLen;
    m_lDataLen = 0;
}

// $_FUNCTION_BEGIN ******************************
// ��������: ~CStdSerialize
// ��������:
// �� �� ֵ:
// ����˵��: ��������
// $_FUNCTION_END ********************************
CStdSerialize::~CStdSerialize( )
{
}

// $_FUNCTION_BEGIN ******************************
// ��������: GetDataLen
// ��������:
// �� �� ֵ: long
// ����˵��: ���ش����������ܳ���
// $_FUNCTION_END ********************************
int CStdSerialize::GetDataLen( )
{
    return m_lDataLen;
}

// $_FUNCTION_BEGIN ******************************
// ��������: Serialize
// ��������: bool& abValue( in/out )Ҫ���л�������
// �� �� ֵ: void
// ����˵��: ��bool�������ݽ������л�
// $_FUNCTION_END ********************************
void CStdSerialize::Serialize( bool& abValue )
{
	ASSERT(FALSE);
    if ( m_lBufLen < ( m_lDataLen + sizeof( bool ) ) )
        throw( -1 );

    if ( m_byType == LOAD )//��ȡ
    {
		abValue = *(bool*)(m_pBuffer + m_lDataLen);
    }
    else//�洢
    {
		*(bool*)(m_pBuffer + m_lDataLen) = abValue;
    }
    m_lDataLen += sizeof( bool );
}
//
//// $_FUNCTION_BEGIN ******************************
//// ��������: void Serialize( int& aiValue )
//// ��������: int& aiValue( in/out )Ҫ���л�������
//// �� �� ֵ: void
//// ����˵��: ��int�������ݽ������л�
//// $_FUNCTION_END ********************************
void CStdSerialize::Serialize( int& aiValue )
{
    if ( m_lBufLen < ( m_lDataLen + sizeof( int ) ) )
        throw( -1 );

    if ( m_byType == LOAD )//��ȡ
    {
		aiValue = *(int*)(m_pBuffer + m_lDataLen);
    }
    else//�洢
    {
		*(int*)(m_pBuffer + m_lDataLen) = aiValue;
    }
    m_lDataLen += sizeof( int );
}

// $_FUNCTION_BEGIN ******************************
// ��������: Serialize
// ��������: unsigned char& ausValue ( in/out )Ҫ���л�������
// �� �� ֵ: void
// ����˵��: ��BYTE�������ݽ������л�
// $_FUNCTION_END ********************************
void CStdSerialize::Serialize( BYTE& ausValue )
{
    if ( m_lBufLen < ( m_lDataLen + sizeof( BYTE ) ) )
        throw( -1 );

    if ( m_byType == LOAD )//��ȡ
    {
		ausValue = *(BYTE*)(m_pBuffer + m_lDataLen);
    }
    else//�洢
    {
		*(BYTE*)(m_pBuffer + m_lDataLen) = ausValue;
    }
    m_lDataLen += sizeof( BYTE );
}


// $_FUNCTION_BEGIN ******************************
// ��������: int Serialize( short& asValue );
// ��������: short awValue ( in/out )Ҫ���л�������
// �� �� ֵ: void
// ����˵��: ���л�WORD���͵�ֵ
// $_FUNCTION_END ********************************
void CStdSerialize::Serialize( short& asValue )
{
    if ( m_lBufLen < ( m_lDataLen + sizeof( short ) ) )
        throw( -1 );

    if ( m_byType == LOAD )//��ȡ
    {
		asValue = *(short*)(m_pBuffer + m_lDataLen);
    }
    else//�洢
    {
		*(short*)(m_pBuffer + m_lDataLen) = asValue;
    }
    m_lDataLen += sizeof( short );
}

// $_FUNCTION_BEGIN ******************************
// ��������: Serialize
// ��������: unsigned short& awValue ( in/out )Ҫ���л�������
// �� �� ֵ: void
// ����˵��: ��unsigned short�������ݽ������л�
// $_FUNCTION_END ********************************
void CStdSerialize::Serialize( unsigned short& awValue )
{
    if ( m_lBufLen < ( m_lDataLen + sizeof( unsigned short ) ) )
        throw( -1 );

    if ( m_byType == LOAD )//��ȡ
    {
		awValue = *(unsigned short*)(m_pBuffer + m_lDataLen);
    }
    else//�洢
    {
		*(unsigned short*)(m_pBuffer + m_lDataLen) = awValue;
    }
    m_lDataLen += sizeof( unsigned short );
}

// $_FUNCTION_BEGIN ******************************
// ��������: Serialize
// ��������: long& alValue ( in/out )Ҫ���л�������
// �� �� ֵ: void
// ����˵��: ��long�������ݽ������л�
// $_FUNCTION_END ********************************
//void CStdSerialize::Serialize( long& alValue )
//{
	//ASSERT(FALSE);
    //if ( m_lBufLen < ( m_lDataLen + sizeof( long ) ) )
        //throw( -1 );

    //if ( m_byType == LOAD )//��ȡ
    //{
		//alValue = *(long*)(m_pBuffer + m_lDataLen);
    //}
    //else//�洢
    //{
		//*(long*)(m_pBuffer + m_lDataLen) = alValue;
    //}
    //m_lDataLen += sizeof( long );
//}

// $_FUNCTION_BEGIN ******************************
// ��������: Serialize
// ��������: unsigned long& aulValue ( in/out )Ҫ���л�������
// �� �� ֵ: void
// ����˵��: ��unsigned long�������ݽ������л�
// $_FUNCTION_END ********************************
void CStdSerialize::Serialize( unsigned int& aulValue )
{
    if ( m_lBufLen < ( m_lDataLen + sizeof( unsigned int ) ) )
        throw( -1 );

    if ( m_byType == LOAD )//��ȡ
    {
		aulValue = *(unsigned int*)(m_pBuffer + m_lDataLen);
    }
    else//�洢
    {
		*(unsigned int*)(m_pBuffer + m_lDataLen) = aulValue;
    }
    m_lDataLen += sizeof( unsigned int );
}

// $_FUNCTION_BEGIN ******************************
// ��������: Serialize
// ��������: INT64& ai64Value ( in/out )Ҫ���л�������
// �� �� ֵ: void
// ����˵��: ��INT64�������ݽ������л�
// $_FUNCTION_END ********************************
void CStdSerialize::Serialize( INT64& ai64Value )
{
    if ( m_lBufLen < ( m_lDataLen + sizeof( INT64 ) ) )
        throw( -1 );

    //INT64 li64TmpVal = 0;
    if ( m_byType == LOAD )//��ȡ
    {
		ai64Value = *(INT64*)(m_pBuffer + m_lDataLen);
    }
    else//�洢
    {
		*(INT64*)(m_pBuffer + m_lDataLen) = ai64Value;
    }
    m_lDataLen += sizeof( INT64 );
}



// $_FUNCTION_BEGIN ******************************
// ��������: Serialize
// ��������: float& afValue ( in/out )Ҫ���л�������
// �� �� ֵ: void
// ����˵��: ��float�������ݽ������л�
// $_FUNCTION_END ********************************
void CStdSerialize::Serialize( float& afValue )
{
    if ( m_lBufLen < ( m_lDataLen + sizeof( float ) ) )
        throw( -1 );

    //float lfTmpVal = 0.0;
    if ( m_byType == LOAD )//��ȡ
    {
		afValue = *(float*)(m_pBuffer + m_lDataLen);
    }
    else//�洢
    {
		*(float*)(m_pBuffer + m_lDataLen) = afValue;
    }
    m_lDataLen += sizeof( float );
}

// $_FUNCTION_BEGIN ******************************
// ��������: Serialize
// ��������: double& adbValue ( in/out )Ҫ���л�������
// �� �� ֵ: void
// ����˵��: ��doubl�������ݽ������л� by ���� 2010-05-25
// $_FUNCTION_END ********************************
void CStdSerialize::Serialize(double& adbValue)
{
	if ( m_lBufLen < ( m_lDataLen + sizeof( double ) ) )
		throw( -1 );

	//double adbValue = 0.0;
	if ( m_byType == LOAD )//��ȡ
	{
		adbValue = *(double*)(m_pBuffer + m_lDataLen);
	}
	else//�洢
	{
		*(double*)(m_pBuffer + m_lDataLen) = adbValue;
	}
	m_lDataLen += sizeof( double );
}

// $_FUNCTION_BEGIN ******************************
// ��������: Serialize
// ��������: char * apValue �ַ������� ( in/out )Ҫ���л�������
// WORD awBufferLen ���ɴ��ַ������ݵĻ�������С
// �� �� ֵ: void
// ����˵��: ���л���0��β���ַ�������
// $_FUNCTION_END ********************************
void CStdSerialize::Serialize( char * apValue,WORD awMaxLen )
{
    if ( m_lBufLen < ( m_lDataLen+2 ) )
        throw( -1 );

    WORD lwLen = 0;
    if ( m_byType == LOAD )//��ȡ
    {
        //���ȶ�ȡ����
		lwLen = *(WORD*)&m_pBuffer[m_lDataLen];
        m_lDataLen += 2;

        //��ȡ���ݱ���
        if ( ( lwLen >= awMaxLen ) || ( ( m_lDataLen+lwLen ) > m_lBufLen ) )
        {
            throw( -1 );
        }
        memcpy( apValue, m_pBuffer + m_lDataLen, lwLen );
        apValue[lwLen] = '\0';
        m_lDataLen += lwLen;
    }
    else//�洢
    {
        //���ȴ洢����
        lwLen = (WORD)strlen( apValue );
        if ( ( lwLen >= awMaxLen ) || ( lwLen+m_lDataLen+2 > m_lBufLen ) )
            throw( -1 );

        *(WORD*)&m_pBuffer[m_lDataLen] = lwLen;
        m_lDataLen += 2;

        //�洢���ݱ���
        memcpy( m_pBuffer+m_lDataLen, apValue, lwLen );
        m_lDataLen += lwLen;
    }
}

#if !__STDC__
// $_FUNCTION_BEGIN ******************************
// ��������: Serialize
// ��������: char * apValue �ַ������� ( in/out )Ҫ���л�������
// WORD awBufferLen ���ɴ��ַ������ݵĻ�������С
// �� �� ֵ: void
// ����˵��: ���л���0��β���ַ�������
// $_FUNCTION_END ********************************
void CStdSerialize::Serialize( wchar_t * apValue,WORD awMaxLen )
{
    if ( m_lBufLen < ( m_lDataLen+2 ) )
        throw( -1 );

    WORD lwLen = 0;
    if ( m_byType == LOAD )//��ȡ
    {
        //���ȶ�ȡ����
		lwLen = *(WORD*)&m_pBuffer[m_lDataLen];
        m_lDataLen += 2;
		//��2
		WORD lCharLen = lwLen << 1;
        //��ȡ���ݱ���
        if ( ( lCharLen >= awMaxLen ) || ( ( m_lDataLen + lCharLen ) > m_lBufLen ) )
        {
            throw( -1 );
        }
        memcpy( apValue, m_pBuffer + m_lDataLen, lCharLen );
        apValue[lwLen] = '\0';
        m_lDataLen += lCharLen;
    }
    else//�洢
    {
        //���ȴ洢����
        lwLen = (WORD)wcslen( apValue );
		//��2
		WORD lCharLen = lwLen << 1;
        if ( ( lCharLen >= awMaxLen ) || ( lCharLen + m_lDataLen + 2 > m_lBufLen ) )
            throw( -1 );

        *(WORD*)&m_pBuffer[m_lDataLen] = lwLen;
        m_lDataLen += 2;

        //�洢���ݱ���
        memcpy( m_pBuffer + m_lDataLen, apValue, lCharLen );
        m_lDataLen += lCharLen;
    }
}
#endif

// $_FUNCTION_BEGIN ******************************
// ��������: Serialize
// ��������: char * apValue ���� ( in/out )Ҫ���л�������
// WORD& awLen �����ݵ���������
// WORD awBufferLen ���ɴ����ݵĻ�������С
// �� �� ֵ: void
// ����˵��: ���л�����������
// $_FUNCTION_END ********************************
void CStdSerialize::Serialize( char * apValue, WORD awLen,WORD aiBufferLen )
{
    if ( ( awLen > aiBufferLen ) || ( m_lBufLen < ( m_lDataLen+awLen ) ) )
        throw ( -1 );

    if ( m_byType == LOAD )//��ȡ
    {
        //��Ϊ�ⲿ�ƶ��˶�ȡ���ȣ����Բ���Ҫ�����ݳ��Ƚ������л�
        memcpy( apValue, m_pBuffer + m_lDataLen, awLen );
    }
    else//�洢���ݱ���
    {
        memcpy( m_pBuffer+m_lDataLen, apValue, awLen );
    }
    m_lDataLen += awLen;
}
