#ifndef __H_StdSerializeMocro_H_
#define __H_StdSerializeMocro_H_

//
#ifndef WIN32
#include <string.h>
#endif
//
/*********************************************************************************************/
//ȱʡ����ʵ��
/*********************************************************************************************/
#define IMPLEMENT_STD_CONSTRUCTOR(class_name) \
class_name::class_name( void  ) \
{ \
	memset(this ,0, sizeof(class_name) ); \
} 

//
	/*********************************************************************************************/
	//ȱʡ�������ʵ��
	/*********************************************************************************************/
#define IMPLEMENT_STD_SERIALIZE_PACK(class_name,package_serial_number) \
	int \
class_name::Pack( char* apBuffer , int alLen  ) \
{ \
	try \
	{ \
		CStdSerialize loSerialize(apBuffer, alLen, CStdSerialize::STORE ); \
		WORD lwPackType=package_serial_number; \
		loSerialize.Serialize( lwPackType  ); \
		if( Serialize(loSerialize) == FALSE  ) \
		{ \
			return 0; \
		} \
		return loSerialize.GetDataLen(); \
	} \
	catch( ...  ) \
	{ \
		return 0; \
	} \
}

/*********************************************************************************************/
//ȱʡ�������
/*********************************************************************************************/
#define IMPLEMENT_STD_SERIALIZE_UNPACK(class_name) \
	BOOL \
class_name::UnPack( char* apBuffer , int alLen  ) \
{ \
	try \
	{ \
		CStdSerialize loSerialize( apBuffer, alLen, CStdSerialize::LOAD  ); \
		WORD lwPackType ; \
		loSerialize.Serialize(lwPackType); \
		if ( Serialize(loSerialize) == FALSE  ) \
		{ \
			return FALSE; \
		} \
		return TRUE; \
	} \
	catch( ...  ) \
	{ \
		return FALSE; \
	} \
}

/*********************************************************************************************/
//ȱʡ���죬����������������
/*********************************************************************************************/
#define DECLEAR_STD_PACK_SERIALIZE() \
	public:\
int    Pack(char * apNetBuffer ,int alBuffLen);\
BOOL   UnPack(char * apNetBuffer,int alBuffLen);\
BOOL   Serialize(CStdSerialize & aoStdSerialize);


/*********************************************************************************************/
//ȱʡ���죬������������ʵ��
/*********************************************************************************************/
#define IMPLEMENT_STD_PACK_SERIALIZE(class_name,package_serial_number) \
	IMPLEMENT_STD_SERIALIZE_PACK(class_name,package_serial_number) \
IMPLEMENT_STD_SERIALIZE_UNPACK(class_name) \


/*********************************************************************************************/
/*********************************************************************************************/
/*********************************************************************************************/
/*********************************************************************************************/
/*********************************************************************************************/
//�����ݴ������buffer��
/*********************************************************************************************/
#define IMPLEMENT_STD_SERIALIZE_PACK_BUFFER(class_name) \
	int \
class_name::PackBuffer( char* apBuffer , int alLen  ) \
{ \
	try \
	{ \
		CStdSerialize loSerialize(apBuffer, alLen, CStdSerialize::STORE ); \
		if( Serialize(loSerialize) == FALSE  ) \
		{ \
			return 0; \
		} \
		return loSerialize.GetDataLen(); \
	} \
	catch( ...  ) \
	{ \
		return 0; \
	} \
}

/*********************************************************************************************/
//�Ӱ�buffer�н������
/*********************************************************************************************/
#define IMPLEMENT_STD_SERIALIZE_UNPACK_BUFFER(class_name) \
	BOOL \
class_name::UnPackBuffer( char* apBuffer , int alLen  ) \
{\
	try \
	{ \
		CStdSerialize loSerialize( apBuffer, alLen, CStdSerialize::LOAD  ); \
		if ( Serialize(loSerialize) == FALSE  ) \
		{ \
			return FALSE; \
		} \
		return TRUE; \
	} \
	catch( ...  ) \
	{ \
		return FALSE; \
	} \
}

/*********************************************************************************************/
//����buffer�������������л��궨��
/*********************************************************************************************/
#define DECLEAR_STD_PACK_SERIALIZE_BUFFER() \
	public:\
int    PackBuffer(char * apBuffer ,int alBuffLen);\
BOOL   UnPackBuffer(char * apBuffer,int alBuffLen);\
BOOL   Serialize(CStdSerialize & aoStdSerialize);

/*********************************************************************************************/
//����buffer�������������л��궨��
/*********************************************************************************************/
#define IMPLEMENT_STD_PACK_SERIALIZE_BUFFER(class_name) \
	IMPLEMENT_STD_SERIALIZE_PACK_BUFFER(class_name) \
IMPLEMENT_STD_SERIALIZE_UNPACK_BUFFER(class_name) \

#endif //__H_StdSerializeMocro_H_

