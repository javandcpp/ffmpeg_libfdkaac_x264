//
// Created by developer on 11/13/17.
//

#ifndef NATIVEAPP_AUDIOENCODER_H
#define NATIVEAPP_AUDIOENCODER_H

#include "MediaEncoder.h"
#include "AudioCapture.h"

class AudioEncoder : public MediaEncoder {

private:
    AudioCapture *audioCapture = NULL;

public:
    AVCodec *avCodec = NULL;
    AVStream *outStream = NULL;
    AVFrame *outputFrame = NULL;
    AVCodecContext *avCodecContext = NULL;
    AVPacket avPacket={0};


    static AudioEncoder *Get();


    bool isEncoding=false;

    static void* EncodeTask(void *p);

    /**
      * 音频已编码数据队列
      */
    threadsafe_queue<OriginData *> aframeQueue;

    AudioEncoder();

    ~AudioEncoder();

    /**
     * 开启编码
     */
    int StartEncode();

    /**
     * 初始化视频编码器
     */
    int InitEncode();

    /**
    * 关闭编码器
    */
    int CloseEncode();

    /**
     * 资源回收
     */
    int Release();

    /**
     * 设置数据采集
     */
    void SetAudioCapture(AudioCapture *audioCapture);



    /**
    * 获取编码器状态
    */
    bool GetEncodeState();

};


#endif //NATIVEAPP_AUDIOENCODER_H
