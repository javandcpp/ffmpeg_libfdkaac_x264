#ifndef NETORDERSERIALIZE_HEADER
#define NETORDERSERIALIZE_HEADER

#include "TypeDeff.h"
#ifdef WIN32
#include <windows.h>
#endif

//用来序列化通讯包的类
class CStdSerialize
{
public:
    enum ENUM_TYPE{LOAD,STORE};

public:

    //构造和析构
    CStdSerialize(char * apBuffer,int  alBufLen, ENUM_TYPE abyType);
    ~CStdSerialize();

public:
    //序列化bool类型的值
	//网络通讯中禁止使用bool型，会出现ASSERT，因为长度不固定，如需要可以使用BTYE类型代替
    void Serialize(bool& abValue);

    //序列化int类型的值
    void Serialize(int& abValue);

    //序列化BYTE类型的值
    void Serialize(BYTE& abyValue);

    //序列化short类型的值
    void Serialize(short& asValue);

    //序列化WORD类型的值
    void Serialize(WORD& awValue);

    //序列化long类型的值
	//网络通讯中禁止使用long型，会出现ASSERT，因为长度不固定
    //void Serialize(long& alValue);

    //序列化unsigned long类型的值
    void Serialize(unsigned int& aulValue);

    //序列化INT64类型的值
    void Serialize(INT64& ai64Value);

    //序列化float类型的值
    void Serialize(float& afValue);
    
	//序列化double类型的值  by 王辉 2010-05-25
    void Serialize(double& adbValue);

    //序列化串类型的值(以\0结尾的字符串)
    void Serialize(char* apValue,WORD awMaxLen);

#if !__STDC__
	//序列化串类型的值(以\0结尾的字符串)
	void Serialize(wchar_t* apValue,WORD awMaxLen);
#endif

	//序列化字符数组类型的值
    void Serialize(char * apValue, WORD awLen,WORD aiBufferLen);

    //取得打包长度
    int GetDataLen();

    //序列化类型
    ENUM_TYPE m_byType;

protected:
    unsigned int m_lDataLen; //数据长度

    char* m_pBuffer;

    unsigned int m_lBufLen; //buffer长度
};

#endif
