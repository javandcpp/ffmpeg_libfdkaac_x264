#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "kernel.h"
#include "ConfigManager.h"
#include "DebugTrace.h"
#include "BaseThread.h"
#include <signal.h>

#include <string>
using std::string;

string VERSTR = "2017041201";

static string m_lastLogFile;
static int    m_lastDay = 0;

static bool CreateDebugInfo()
{
    char szBinPath[500] = {0};

    //获取工作路径
    char* p_cur_path = getcwd(szBinPath,499);

    //重新加载配置
    if (false == CSingletonCfgManager::Instance().Load("server.ini"))
    {
        return FALSE;
    }


    //得到是否将进行trace的设置
    BOOL bTraceEnable = CSingletonCfgManager::Instance().GetValue("DEBUG", "TraceEnable");
  
    //得到调试级别
    int lTraceLevel = CSingletonCfgManager::Instance().GetValue("DEBUG", "TraceLevel");
    
    //设置TRACE级别
    CDebugTrace::SetTraceLevel(lTraceLevel);

    //将TRACE日志写入文件的
    BOOL bWriteTraceToFile = CSingletonCfgManager::Instance().GetValue("DEBUG", "AppendToFile");

    //初始化TRACE选项
    DWORD dwTraceOption = CDebugTrace::GetTraceOptions() | CDebugTrace::Timestamp |  \
                          CDebugTrace::PrintToConsole & ~CDebugTrace::LogLevel & ~CDebugTrace::FileAndLine;

    //将TRACE日志写入文件的
    if(bWriteTraceToFile)
    {
        dwTraceOption |= CDebugTrace::AppendToFile;
    }
    else
    {
        dwTraceOption &= ~CDebugTrace::AppendToFile;
    }

    //设置TRACE选项
    CDebugTrace::SetTraceOptions(dwTraceOption);

    //生成TRACE文件名
    time_t ltmNow;
    time(&ltmNow);

    //当前系统时间
    struct tm * pTime = localtime(&ltmNow);

    char szLogFileName[1024] = { 0  };
    //生成日志文件名称
    snprintf(szLogFileName, 1023, "%s/log/%s_%04d_%02d_%02d_%02d_%02d.log",
            szBinPath,"publish-rtmp", (1900 + pTime->tm_year),
            (1 + pTime->tm_mon), pTime->tm_mday, pTime->tm_hour, pTime->tm_min);

    m_lastLogFile = szLogFileName;
    m_lastDay     = pTime->tm_mday;

    //调用"BeginTrace"为了锁住TRACE，防止别的线程再访问日志文件名
    CDebugTrace& loDebugTrace = CDebugTrace::BeginTrace(0, __FILE__, __LINE__);

    //设置日志文件名
    CDebugTrace::SetLogFileName(szLogFileName);

    //释放锁,调用结束输出,解锁并输出此次动作
    CDebugTrace::EndTrace(loDebugTrace);

    return true;
}

static INT64 m_i64ConfigUpdateInterval = 10*1000;
static INT64 m_i64LastLoadConfigTime   = 0;

//更新日志级别
static BOOL CheckUpdateLogLevel()
{
	//重新加载配置
	if (CBaseThread::GetSystemTime() - m_i64LastLoadConfigTime < m_i64ConfigUpdateInterval)
	{
		return FALSE;
	}

	//更新最后加载时间
	m_i64LastLoadConfigTime = CBaseThread::GetSystemTime();

	//重新加载配置
	if (false == CSingletonCfgManager::Instance().Load("server.ini"))
	{
		return FALSE;
	}

	//得到调试级别
	int lTraceLevel = CSingletonCfgManager::Instance().GetValue("DEBUG", "TraceLevel");

	//设置TRACE级别
	CDebugTrace::SetTraceLevel(lTraceLevel);

	TraceLog0("当前日志级别调整为[%ld].\n", lTraceLevel);

	return TRUE;
}

static INT64 m_i64ResetLogFileInterval = 60*1000;
static INT64 m_i64LastResetLogFileTime = 0;

//重新日志文件
static BOOL CheckResetLogFile()
{
	if (CBaseThread::GetSystemTime() - m_i64LastResetLogFileTime < m_i64ResetLogFileInterval)
	{
		return FALSE;
	}

    m_i64LastResetLogFileTime = CBaseThread::GetSystemTime();
    
    bool   newLogFile = false;

    time_t ltmNow;
    time(&ltmNow);
    //当前系统时间
    struct tm * pTime = localtime(&ltmNow);
    if (pTime->tm_mday != m_lastDay)        // 另一天
    {
        m_lastDay     = pTime->tm_mday;
        newLogFile = true;
    }
    
    struct stat logStat;
    if (stat(m_lastLogFile.c_str(), &logStat) == -1) 
    {
        TraceLog0("CheckResetLogFile::生成新日志文件,此文件结束写入.\n");
        return FALSE;
    }
    if (logStat.st_size >= 500*1024*1024)    // 500M
    {
        newLogFile = true;
    }
    
    if (!newLogFile)
    {
        return FALSE;
    }

	TraceLog0("CheckResetLogFile::生成新日志文件,此文件结束写入 : %s.\n", m_lastLogFile.c_str());

	//重置调试信息
	CreateDebugInfo();

	TraceLog0("CheckResetLogFile::此文件为新生成日志文件 : %s.\n", m_lastLogFile.c_str());
    TraceLog0("Version : %s\n", VERSTR.c_str());
#ifdef DEF_RECONN_CDN
    TraceLog0("Define DEF_RECONN_CDN.");
#endif

	return TRUE;
}

BOOL MaintainWork()
{
	//定时更新日志级别
	CheckUpdateLogLevel();
	//定时重新日志文件
	CheckResetLogFile();

	return TRUE;
}

int main()
{
    //setbuf(stdout, NULL);
    

    signal(SIGPIPE, SIG_IGN);
    char path[500];

    char* p_cur_path = getcwd(path,499);

    if(!CreateDebugInfo())
    {
        return 0;
    }

    TraceLog0("Version : %s\n", VERSTR.c_str());
#ifdef DEF_RECONN_CDN
    TraceLog0("Define DEF_RECONN_CDN.");
#endif

    CKernel loKernel;
    if(!loKernel.Open())
    {
        return 0;
    }

    while(1)
    {
        CBaseThread::Sleep(10);
        //定时维护工作
		if(FALSE == MaintainWork())
		{
			break;
		}
    }

    return 0;
}


