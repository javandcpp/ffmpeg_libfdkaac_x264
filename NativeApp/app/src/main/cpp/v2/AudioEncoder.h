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
    AudioEncodeArgs *audioEncodeArgs = NULL;
public:
    AVCodec *avCodec = NULL;
    AVStream *outStream = NULL;
    AVFrame *vOutFrame = NULL;
    AVCodecContext *avCodecContext = NULL;

    static AudioEncoder *Get();

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
     * 设置编码参数
     */
    void SetAudioEncodeArgs(AudioEncodeArgs *audioEncodeArgs);


};


#endif //NATIVEAPP_AUDIOENCODER_H
