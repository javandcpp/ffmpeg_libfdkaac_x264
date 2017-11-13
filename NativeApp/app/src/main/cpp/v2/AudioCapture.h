//
// Created by developer on 11/12/17.
//


#ifndef NATIVEAPP_AUDIOCAPTURE_H
#define NATIVEAPP_AUDIOCAPTURE_H

#include "MediaCapture.h"
#include "PrefixHeader.h"

/**
 * 音频数据接收
 */
class AudioCapture : public MediaCapture {

private:
    AudioCapture();

    AudioEncodeArgs *audioEncodeArgs = NULL;
public :
    ~AudioCapture();

    static AudioCapture *Get();

    bool CloseCapture();

    bool StartCapture();

    int Release();

    int PushAudioData(OriginData *originData);

    OriginData *GetAudioData();

    /**
   * 设置编码参数
   */
    void SetAudioEncodeArgs(AudioEncodeArgs *audioEncodeArgs);

    AudioEncodeArgs *GetAudioEncodeArgs();

    /**
     * 获取音频采集状态
     */
    bool GetCaptureState();

    mutex mMutex;
};

#endif //NATIVEAPP_AUDIOCAPTURE_H
