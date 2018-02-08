// $_FILEHEADER_BEGIN ***************************
// 八度互联研发部版权所有
// Copyright (C) 八度互联研发部 Corporation.  All Rights Reserved
// 文件名称：LRDSPackDeff.h
// 创建日期：2014-05-18
// 创建人：边召
// 文件说明：本地分发服务器包定义
// $_FILEHEADER_END *****************************
#ifndef __LRDS_PACK_DEFF_H__
#define __LRDS_PACK_DEFF_H__

#include "StdSerializeEx.h"
#include "StdSerialMacro.h"
#include "ServerPackHead.h"

#define	PACK_SVR_LRDS_BASE					   300
#define PACK_SVR_LRDS_SUB_BASE                 (PACK_SVR_LRDS_BASE + 100)

#define	PACK_SVR_LRDS_DATA_FORWARD_ID		    PACK_SVR_LRDS_BASE + 1 //服务器到LRDS数据转发包
#define PACK_SVR_LRDS_QUERY_USER_INFO_RQ        PACK_SVR_LRDS_BASE + 2 //查询用户信息请求
#define PACK_SVR_LRDS_QUERY_USER_INFO_RS        PACK_SVR_LRDS_BASE + 3 //查询用户信息应答
#define PACK_SVR_LRDS_CMSP_DATA_ID  		    PACK_SVR_LRDS_BASE + 4 //LRDS-->CMS透传包
#define PACK_SVR_LRDS_RLSP_DATA_ID              PACK_SVR_LRDS_BASE + 5 //LRDS-->RLSP下推透传包
#define PACK_SVR_LRDS_RLSP_USER_ENTER_ID        PACK_SVR_LRDS_BASE + 6 //LRDS-->RLSP用户进房间
#define PACK_SVR_LRDS_RLSP_USER_LEAVE_ID        PACK_SVR_LRDS_BASE + 7 //LRDS-->RLSP用户出房间
#define PACK_SVR_LRDS_RLSP_USERINFO_CHANGE_ID   PACK_SVR_LRDS_BASE + 8 //LRDS-->RLSP用户信息变化
#define PACK_SVR_LRDS_RLSP_USERLIST_RQ			PACK_SVR_LRDS_BASE + 9 //RLSP请求LRDS用户列表请求
#define PACK_SVR_LRDS_RLSP_USERLIST_RS          PACK_SVR_LRDS_BASE + 10 //LRDS返回RLSP用户列表响应
#define PACK_SVR_LRDS_QUERY_USER_LIST_RQ        PACK_SVR_LRDS_BASE + 11 //RLSP查询LRDS用户们是否在房间请求
#define PACK_SVR_LRDS_QUERY_USER_LIST_RS        PACK_SVR_LRDS_BASE + 12 //LRDS返回RLSP查询用户们是否在房间响应
#define PACK_SVR_LRDS_RLSP_DATA_RQ              PACK_SVR_LRDS_BASE + 13 //RLSP-->LRDS透传客户端请求
#define PACK_SVR_LRDS_RLSP_DATA_RS              PACK_SVR_LRDS_BASE + 14 //LRDS---RLSP透传客户端响应
#define PACK_SVR_LRDS_REG_WITH_ROOM_RQ          PACK_SVR_LRDS_BASE + 15 //按房间注册请求(TCP-Mode Only)
#define PACK_SVR_LRDS_REG_WITH_ROOM_RS          PACK_SVR_LRDS_BASE + 16 //按房间注册应答(TCP-Mode Only)

#define	PACK_LRDS_MAX_COUNT			            32

#define DEF_LRDS_FORWARD_DATA_LEN	         1300

//(LRDS-->CMSP) 透传包，媒体数据下推
struct STRU_SVR_LRDS_CMSP_DATA_ID
{
	STRU_SVR_PACK_HEAD	m_oSvrPackHead;
	INT64				m_i64RoomID;        //目标房间ID
	unsigned int				m_dwMsgType;        //消息类型

	WORD				m_wDataLen;			//数据长度
	BYTE				m_byData[DEF_LRDS_FORWARD_DATA_LEN];//数据
public:
	STRU_SVR_LRDS_CMSP_DATA_ID();
	DECLEAR_STD_PACK_SERIALIZE();
};


#endif//__LRDS_PACK_DEFF_H__
