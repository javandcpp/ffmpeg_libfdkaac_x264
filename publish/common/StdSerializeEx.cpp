#include "TypeDeff.h"
#include "StdSerializeEx.h"
#include "DebugTrace.h"
#include <string.h>

#ifdef WIN32
#include <exception>
#endif

// $_FUNCTION_BEGIN ******************************
// 函数名称: CStdSerialize
// 函数参数: char * apBuffer：缓冲buff
// long alBufLen : buff的长度
// ENUM_TYPE abyType：序列化类型( 保存/加载 )
// 返 回 值:
// 函数说明: 构造
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
// 函数名称: ~CStdSerialize
// 函数参数:
// 返 回 值:
// 函数说明: 析构函数
// $_FUNCTION_END ********************************
CStdSerialize::~CStdSerialize( )
{
}

// $_FUNCTION_BEGIN ******************************
// 函数名称: GetDataLen
// 函数参数:
// 返 回 值: long
// 函数说明: 返回打包后的数据总长度
// $_FUNCTION_END ********************************
int CStdSerialize::GetDataLen( )
{
    return m_lDataLen;
}

// $_FUNCTION_BEGIN ******************************
// 函数名称: Serialize
// 函数参数: bool& abValue( in/out )要序列化的数据
// 返 回 值: void
// 函数说明: 对bool类型数据进行序列化
// $_FUNCTION_END ********************************
void CStdSerialize::Serialize( bool& abValue )
{
	ASSERT(FALSE);
    if ( m_lBufLen < ( m_lDataLen + sizeof( bool ) ) )
        throw( -1 );

    if ( m_byType == LOAD )//读取
    {
		abValue = *(bool*)(m_pBuffer + m_lDataLen);
    }
    else//存储
    {
		*(bool*)(m_pBuffer + m_lDataLen) = abValue;
    }
    m_lDataLen += sizeof( bool );
}
//
//// $_FUNCTION_BEGIN ******************************
//// 函数名称: void Serialize( int& aiValue )
//// 函数参数: int& aiValue( in/out )要序列化的数据
//// 返 回 值: void
//// 函数说明: 对int类型数据进行序列化
//// $_FUNCTION_END ********************************
void CStdSerialize::Serialize( int& aiValue )
{
    if ( m_lBufLen < ( m_lDataLen + sizeof( int ) ) )
        throw( -1 );

    if ( m_byType == LOAD )//读取
    {
		aiValue = *(int*)(m_pBuffer + m_lDataLen);
    }
    else//存储
    {
		*(int*)(m_pBuffer + m_lDataLen) = aiValue;
    }
    m_lDataLen += sizeof( int );
}

// $_FUNCTION_BEGIN ******************************
// 函数名称: Serialize
// 函数参数: unsigned char& ausValue ( in/out )要序列化的数据
// 返 回 值: void
// 函数说明: 对BYTE类型数据进行序列化
// $_FUNCTION_END ********************************
void CStdSerialize::Serialize( BYTE& ausValue )
{
    if ( m_lBufLen < ( m_lDataLen + sizeof( BYTE ) ) )
        throw( -1 );

    if ( m_byType == LOAD )//读取
    {
		ausValue = *(BYTE*)(m_pBuffer + m_lDataLen);
    }
    else//存储
    {
		*(BYTE*)(m_pBuffer + m_lDataLen) = ausValue;
    }
    m_lDataLen += sizeof( BYTE );
}


// $_FUNCTION_BEGIN ******************************
// 函数名称: int Serialize( short& asValue );
// 函数参数: short awValue ( in/out )要序列化的数据
// 返 回 值: void
// 函数说明: 序列化WORD类型的值
// $_FUNCTION_END ********************************
void CStdSerialize::Serialize( short& asValue )
{
    if ( m_lBufLen < ( m_lDataLen + sizeof( short ) ) )
        throw( -1 );

    if ( m_byType == LOAD )//读取
    {
		asValue = *(short*)(m_pBuffer + m_lDataLen);
    }
    else//存储
    {
		*(short*)(m_pBuffer + m_lDataLen) = asValue;
    }
    m_lDataLen += sizeof( short );
}

// $_FUNCTION_BEGIN ******************************
// 函数名称: Serialize
// 函数参数: unsigned short& awValue ( in/out )要序列化的数据
// 返 回 值: void
// 函数说明: 对unsigned short类型数据进行序列化
// $_FUNCTION_END ********************************
void CStdSerialize::Serialize( unsigned short& awValue )
{
    if ( m_lBufLen < ( m_lDataLen + sizeof( unsigned short ) ) )
        throw( -1 );

    if ( m_byType == LOAD )//读取
    {
		awValue = *(unsigned short*)(m_pBuffer + m_lDataLen);
    }
    else//存储
    {
		*(unsigned short*)(m_pBuffer + m_lDataLen) = awValue;
    }
    m_lDataLen += sizeof( unsigned short );
}

// $_FUNCTION_BEGIN ******************************
// 函数名称: Serialize
// 函数参数: long& alValue ( in/out )要序列化的数据
// 返 回 值: void
// 函数说明: 对long类型数据进行序列化
// $_FUNCTION_END ********************************
//void CStdSerialize::Serialize( long& alValue )
//{
	//ASSERT(FALSE);
    //if ( m_lBufLen < ( m_lDataLen + sizeof( long ) ) )
        //throw( -1 );

    //if ( m_byType == LOAD )//读取
    //{
		//alValue = *(long*)(m_pBuffer + m_lDataLen);
    //}
    //else//存储
    //{
		//*(long*)(m_pBuffer + m_lDataLen) = alValue;
    //}
    //m_lDataLen += sizeof( long );
//}

// $_FUNCTION_BEGIN ******************************
// 函数名称: Serialize
// 函数参数: unsigned long& aulValue ( in/out )要序列化的数据
// 返 回 值: void
// 函数说明: 对unsigned long类型数据进行序列化
// $_FUNCTION_END ********************************
void CStdSerialize::Serialize( unsigned int& aulValue )
{
    if ( m_lBufLen < ( m_lDataLen + sizeof( unsigned int ) ) )
        throw( -1 );

    if ( m_byType == LOAD )//读取
    {
		aulValue = *(unsigned int*)(m_pBuffer + m_lDataLen);
    }
    else//存储
    {
		*(unsigned int*)(m_pBuffer + m_lDataLen) = aulValue;
    }
    m_lDataLen += sizeof( unsigned int );
}

// $_FUNCTION_BEGIN ******************************
// 函数名称: Serialize
// 函数参数: INT64& ai64Value ( in/out )要序列化的数据
// 返 回 值: void
// 函数说明: 对INT64类型数据进行序列化
// $_FUNCTION_END ********************************
void CStdSerialize::Serialize( INT64& ai64Value )
{
    if ( m_lBufLen < ( m_lDataLen + sizeof( INT64 ) ) )
        throw( -1 );

    //INT64 li64TmpVal = 0;
    if ( m_byType == LOAD )//读取
    {
		ai64Value = *(INT64*)(m_pBuffer + m_lDataLen);
    }
    else//存储
    {
		*(INT64*)(m_pBuffer + m_lDataLen) = ai64Value;
    }
    m_lDataLen += sizeof( INT64 );
}



// $_FUNCTION_BEGIN ******************************
// 函数名称: Serialize
// 函数参数: float& afValue ( in/out )要序列化的数据
// 返 回 值: void
// 函数说明: 对float类型数据进行序列化
// $_FUNCTION_END ********************************
void CStdSerialize::Serialize( float& afValue )
{
    if ( m_lBufLen < ( m_lDataLen + sizeof( float ) ) )
        throw( -1 );

    //float lfTmpVal = 0.0;
    if ( m_byType == LOAD )//读取
    {
		afValue = *(float*)(m_pBuffer + m_lDataLen);
    }
    else//存储
    {
		*(float*)(m_pBuffer + m_lDataLen) = afValue;
    }
    m_lDataLen += sizeof( float );
}

// $_FUNCTION_BEGIN ******************************
// 函数名称: Serialize
// 函数参数: double& adbValue ( in/out )要序列化的数据
// 返 回 值: void
// 函数说明: 对doubl类型数据进行序列化 by 王辉 2010-05-25
// $_FUNCTION_END ********************************
void CStdSerialize::Serialize(double& adbValue)
{
	if ( m_lBufLen < ( m_lDataLen + sizeof( double ) ) )
		throw( -1 );

	//double adbValue = 0.0;
	if ( m_byType == LOAD )//读取
	{
		adbValue = *(double*)(m_pBuffer + m_lDataLen);
	}
	else//存储
	{
		*(double*)(m_pBuffer + m_lDataLen) = adbValue;
	}
	m_lDataLen += sizeof( double );
}

// $_FUNCTION_BEGIN ******************************
// 函数名称: Serialize
// 函数参数: char * apValue 字符串数据 ( in/out )要序列化的数据
// WORD awBufferLen 容纳此字符串数据的缓存区大小
// 返 回 值: void
// 函数说明: 序列化以0结尾的字符串数据
// $_FUNCTION_END ********************************
void CStdSerialize::Serialize( char * apValue,WORD awMaxLen )
{
    if ( m_lBufLen < ( m_lDataLen+2 ) )
        throw( -1 );

    WORD lwLen = 0;
    if ( m_byType == LOAD )//读取
    {
        //首先读取长度
		lwLen = *(WORD*)&m_pBuffer[m_lDataLen];
        m_lDataLen += 2;

        //读取数据本身
        if ( ( lwLen >= awMaxLen ) || ( ( m_lDataLen+lwLen ) > m_lBufLen ) )
        {
            throw( -1 );
        }
        memcpy( apValue, m_pBuffer + m_lDataLen, lwLen );
        apValue[lwLen] = '\0';
        m_lDataLen += lwLen;
    }
    else//存储
    {
        //首先存储长度
        lwLen = (WORD)strlen( apValue );
        if ( ( lwLen >= awMaxLen ) || ( lwLen+m_lDataLen+2 > m_lBufLen ) )
            throw( -1 );

        *(WORD*)&m_pBuffer[m_lDataLen] = lwLen;
        m_lDataLen += 2;

        //存储数据本身
        memcpy( m_pBuffer+m_lDataLen, apValue, lwLen );
        m_lDataLen += lwLen;
    }
}

#if !__STDC__
// $_FUNCTION_BEGIN ******************************
// 函数名称: Serialize
// 函数参数: char * apValue 字符串数据 ( in/out )要序列化的数据
// WORD awBufferLen 容纳此字符串数据的缓存区大小
// 返 回 值: void
// 函数说明: 序列化以0结尾的字符串数据
// $_FUNCTION_END ********************************
void CStdSerialize::Serialize( wchar_t * apValue,WORD awMaxLen )
{
    if ( m_lBufLen < ( m_lDataLen+2 ) )
        throw( -1 );

    WORD lwLen = 0;
    if ( m_byType == LOAD )//读取
    {
        //首先读取长度
		lwLen = *(WORD*)&m_pBuffer[m_lDataLen];
        m_lDataLen += 2;
		//乘2
		WORD lCharLen = lwLen << 1;
        //读取数据本身
        if ( ( lCharLen >= awMaxLen ) || ( ( m_lDataLen + lCharLen ) > m_lBufLen ) )
        {
            throw( -1 );
        }
        memcpy( apValue, m_pBuffer + m_lDataLen, lCharLen );
        apValue[lwLen] = '\0';
        m_lDataLen += lCharLen;
    }
    else//存储
    {
        //首先存储长度
        lwLen = (WORD)wcslen( apValue );
		//乘2
		WORD lCharLen = lwLen << 1;
        if ( ( lCharLen >= awMaxLen ) || ( lCharLen + m_lDataLen + 2 > m_lBufLen ) )
            throw( -1 );

        *(WORD*)&m_pBuffer[m_lDataLen] = lwLen;
        m_lDataLen += 2;

        //存储数据本身
        memcpy( m_pBuffer + m_lDataLen, apValue, lCharLen );
        m_lDataLen += lCharLen;
    }
}
#endif

// $_FUNCTION_BEGIN ******************************
// 函数名称: Serialize
// 函数参数: char * apValue 数据 ( in/out )要序列化的数据
// WORD& awLen 此数据的真正长度
// WORD awBufferLen 容纳此数据的缓存区大小
// 返 回 值: void
// 函数说明: 序列化二进制数据
// $_FUNCTION_END ********************************
void CStdSerialize::Serialize( char * apValue, WORD awLen,WORD aiBufferLen )
{
    if ( ( awLen > aiBufferLen ) || ( m_lBufLen < ( m_lDataLen+awLen ) ) )
        throw ( -1 );

    if ( m_byType == LOAD )//读取
    {
        //因为外部制定了读取长度，所以不需要对数据长度进行序列化
        memcpy( apValue, m_pBuffer + m_lDataLen, awLen );
    }
    else//存储数据本身
    {
        memcpy( m_pBuffer+m_lDataLen, apValue, awLen );
    }
    m_lDataLen += awLen;
}
