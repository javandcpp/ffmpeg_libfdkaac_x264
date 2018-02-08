
#ifndef ___QIJU_IRTMPMANAGER_H_INCLUDED___
#define ___QIJU_IRTMPMANAGER_H_INCLUDED___

#include <string>

//IRtmpManagerEvent�ӿ���ý�������ʵ�֣����ϲ�ʵ��
class IRtmpManagerEvent
{
public:
	//���Linux���棬��������������
	virtual ~IRtmpManagerEvent(){};

public:
	//���ӽ��֪ͨ
    virtual void OnConnect(int64_t streamID, bool abResult) = 0;

	//�����ж�֪ͨ
	virtual void OnDisconnet(int64_t streamID) = 0;
};

//IRtmpManager�ӿ���Rtmp������ʵ�֣����²�ʵ��
class IRtmpManager
{
public:
	//���Linux���棬��������������
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

//��������
IRtmpManager* CreateRtmpManager();

//�ͷŶ���
void ReleaseRtmpManager(IRtmpManager*& apRtmpManager);

std::string GetRtmpAsynVersion();

#endif // ___QIJU_IRTMPMANAGER_H_INCLUDED___
