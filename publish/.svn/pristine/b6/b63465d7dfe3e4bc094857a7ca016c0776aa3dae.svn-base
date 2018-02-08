
#ifndef ___QIJU_IRTMPMANAGER_H_INCLUDED___
#define ___QIJU_IRTMPMANAGER_H_INCLUDED___

#include <string>

//IRtmpManagerEvent接口由媒体服务器实现，即上层实现
class IRtmpManagerEvent
{
public:
	//解决Linux警告，增加虚析构函数
	virtual ~IRtmpManagerEvent(){};

public:
	//连接结果通知
    virtual void OnConnect(int64_t streamID, bool abResult) = 0;

	//连接中断通知
	virtual void OnDisconnet(int64_t streamID) = 0;
};

//IRtmpManager接口由Rtmp管理器实现，即下层实现
class IRtmpManager
{
public:
	//解决Linux警告，增加虚析构函数
	virtual ~IRtmpManager(){};

public:
    virtual bool Init(IRtmpManagerEvent *pEvent) = 0;
	virtual void Close() = 0;

    virtual bool connect(int64_t streamID, const char* url,int len) = 0;
    virtual void disconnect(int64_t streamID) = 0;

    virtual bool SendVideoData(int64_t streamID, const char* apVideoBuf, int len, bool bKeyMark,unsigned int nTimeStamp,void *meta) = 0;
    virtual bool SendAudioData(int64_t streamID, const char* apAudioBuf, int len,unsigned int nTimeStamp) = 0;

    virtual bool SendMetaInfo(int64_t streamID, const char* apMetaBuf, int len,unsigned int nTimeStamp) = 0;
};

//创建对象
IRtmpManager* CreateRtmpManager();

//释放对象
void ReleaseRtmpManager(IRtmpManager*& apRtmpManager);

std::string GetRtmpAsynVersion();

#endif // ___QIJU_IRTMPMANAGER_H_INCLUDED___
