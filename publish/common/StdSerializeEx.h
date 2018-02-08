#ifndef NETORDERSERIALIZE_HEADER
#define NETORDERSERIALIZE_HEADER

#include "TypeDeff.h"
#ifdef WIN32
#include <windows.h>
#endif

//�������л�ͨѶ������
class CStdSerialize
{
public:
    enum ENUM_TYPE{LOAD,STORE};

public:

    //���������
    CStdSerialize(char * apBuffer,int  alBufLen, ENUM_TYPE abyType);
    ~CStdSerialize();

public:
    //���л�bool���͵�ֵ
	//����ͨѶ�н�ֹʹ��bool�ͣ������ASSERT����Ϊ���Ȳ��̶�������Ҫ����ʹ��BTYE���ʹ���
    void Serialize(bool& abValue);

    //���л�int���͵�ֵ
    void Serialize(int& abValue);

    //���л�BYTE���͵�ֵ
    void Serialize(BYTE& abyValue);

    //���л�short���͵�ֵ
    void Serialize(short& asValue);

    //���л�WORD���͵�ֵ
    void Serialize(WORD& awValue);

    //���л�long���͵�ֵ
	//����ͨѶ�н�ֹʹ��long�ͣ������ASSERT����Ϊ���Ȳ��̶�
    //void Serialize(long& alValue);

    //���л�unsigned long���͵�ֵ
    void Serialize(unsigned int& aulValue);

    //���л�INT64���͵�ֵ
    void Serialize(INT64& ai64Value);

    //���л�float���͵�ֵ
    void Serialize(float& afValue);
    
	//���л�double���͵�ֵ  by ���� 2010-05-25
    void Serialize(double& adbValue);

    //���л������͵�ֵ(��\0��β���ַ���)
    void Serialize(char* apValue,WORD awMaxLen);

#if !__STDC__
	//���л������͵�ֵ(��\0��β���ַ���)
	void Serialize(wchar_t* apValue,WORD awMaxLen);
#endif

	//���л��ַ��������͵�ֵ
    void Serialize(char * apValue, WORD awLen,WORD aiBufferLen);

    //ȡ�ô������
    int GetDataLen();

    //���л�����
    ENUM_TYPE m_byType;

protected:
    unsigned int m_lDataLen; //���ݳ���

    char* m_pBuffer;

    unsigned int m_lBufLen; //buffer����
};

#endif
