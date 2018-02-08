#include <memory.h>
#include <string>
#include <algorithm>
#ifdef WIN32
#include <direct.h>
#else
#include <sys/stat.h>
#endif
#include <iostream>
#include <fstream>
#include "TypeDeff.h"
#include "ConfigManager.h"

CItem::CItem(const char * aStrKey, const char * aStrValue)
: m_strValue(aStrValue)
, m_strKey(aStrKey)
{
}


//CItem::operator WORD()
//{
	//return atoi(m_strValue.c_str());
//}

CItem::operator int()
{
	return atoi(m_strValue.c_str());
}

//CItem::operator const std::string()
//{
	//return m_strValue.c_str();
//}
CItem::operator std::string() 
{
    return m_strValue;
}

bool CItem::FindByKey(const char * strKey)
{
	return m_strKey == std::string(strKey);
}

CItem CCfgManager::GetValue(const char * aSectionName, const char * aszNodeName)
{
	CItem item("", "0");

	std::string strSectionName(aSectionName);
	std::transform(strSectionName.begin(), strSectionName.end(), strSectionName.begin(), ::tolower);

	std::string strNodeName(aszNodeName);
	std::transform(strNodeName.begin(), strNodeName.end(), strNodeName.begin(), ::tolower);

	CCriticalAutoLock loLock(m_oCriForSeciontNode);

	std::map<std::string,std::list<CItem> >::iterator iter = m_oSeciontNode.find(strSectionName);
	if ( iter == m_oSeciontNode.end() )
	{
		return item;
	}

	std::list<CItem>::iterator iterLst = find_if(iter->second.begin(), iter->second.end(), std::bind2nd(std::mem_fun_ref(&CItem::FindByKey), strNodeName.c_str()));
	if ( iterLst == iter->second.end() )
	{
		return item;
	}

	return (*iterLst);
}

bool CCfgManager::Load(const char* aszFileName)
{
	char lacFileName[1024] = { 0 };
	getcwd(lacFileName, 1023);

	if (0 == lacFileName[0])
	{
		return false;
	}
	
	strcat(lacFileName, "/");
	strcat(lacFileName, aszFileName);
	
	try
	{
		CCriticalAutoLock loLock(m_oCriForSeciontNode);
		LoadCore(lacFileName);
	}
	catch(...)
	{
		return false;
	}

	return true;
}

bool CCfgManager::LoadCore(const char* azFileName)
{
	std::ifstream fConfig(azFileName);

	if (!fConfig)
	{
		return false;
	}

	//first clear
	m_oSeciontNode.clear();

	std::map< std::string,std::list<CItem> >::iterator curIter = m_oSeciontNode.end();

	char szLine[500] = { 0 };

	while (fConfig.getline(szLine, 500))
	{
		std::string strLine(szLine);

		size_t posStart, posEnd;

		if ( std::string::npos != (posStart = strLine.find("[")) )
		{
			strLine.erase(0,strLine.find_first_not_of(" "));
			strLine.erase(strLine.find_last_not_of(" ") + 1);

			if ( std::string::npos != (posEnd = strLine.find("]") ) )
			{
				std::string strTemp = strLine.substr(posStart + 1,posEnd - posStart - 1);
				transform(strTemp.begin(), strTemp.end(), strTemp.begin(), ::tolower);
				std::string lStrTempSection = Trim(strTemp);
				curIter = m_oSeciontNode.insert(make_pair(lStrTempSection,std::list<CItem>())).first;
			}
		}

		if (std::string::npos != (posStart = strLine.find("=")) )
		{
			if (curIter == m_oSeciontNode.end())
			{
				continue;
			}

			std::string key(strLine.substr(0,posStart));
			transform(key.begin(), key.end(), key.begin(), ::tolower);
			std::string lStrKey = Trim(key);

			std::string value(strLine.substr(posStart+1));
			std::string lStrValue = Trim(value);

			CItem loIterm(lStrKey.c_str(),lStrValue.c_str());
			curIter->second.push_back(loIterm);
		}
	}

	return true;
}

std::string CCfgManager::Trim(std::string& aStr)
{
	if (aStr.length() == 0)
	{
		return "";
	}

	replace(aStr.begin(),aStr.end(),0x09,0x20);

	//find SPACE
	size_t stStart = aStr.find_first_not_of(0x20);
	if(stStart == std::string::npos)
	{
		stStart = 0;	
	}

	size_t stEnd = aStr.find_first_of(0x20, stStart);
	if (stEnd == std::string::npos)
	{
		stEnd = aStr.length();
	}

	std::string lStrTemp = aStr.substr(stStart, stEnd - stStart);

	return lStrTemp;
}
