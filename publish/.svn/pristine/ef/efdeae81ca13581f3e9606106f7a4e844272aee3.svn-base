//$_FILEHEADER_BEGIN **************************** 
// 北京八度互联科技有限公司版权所有
// Copyright (C) Badu Corporation.  All Rights Reserved
// 文件名称：
// 创建日期：2010年6月3日 星期四
// 创建人：   刘俊江
// 文件说明：
//			配置信息管理类,读取配置文件,访问配置每个section中node的value
//			
//			要求配置文件的格式如下
//									[section]
//									node = value
//			GetValue(section,node)返回值可以直接转化为 char*或int
//	使用步骤
//			1 Load file in current path filename Load(filename);
//			2 GetValue(section,node)
//
//
//
//
// $_FILEHEADER_END ******************************

#ifndef __CONFIG_MANAGER_H
#define __CONFIG_MANAGER_H

#include <map>
#include <list>
#include <string>
#include "CriticalLock.h"
#include "Singleton.h"
#include "TypeDeff.h"

class CItem
{
public:
	CItem(const char* aStrKey = "",const char* aStrValue = "0");
	bool FindByKey(const char* strKey);

	operator int();
	//operator long();
	//operator std::string();
	operator  std::string();

private:
    std::string	m_strValue;
    std::string	m_strKey;
};

class CCfgManager
{
public:
	CItem GetValue(const char* aSectionName, const char* aszNodeName);
	bool Load(const char* aszFileName);

private:
	bool LoadCore(const char* azFileName);
    std::string Trim(std::string& aStr);

private:
    std::map< std::string,std::list<CItem> > m_oSeciontNode;
	CCriticalLock m_oCriForSeciontNode;
};

typedef CSingleton<CCfgManager> CSingletonCfgManager;

#endif //__CONFIG_MANAGER_H
