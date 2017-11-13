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


public :
    ~AudioCapture();

    static AudioCapture *Get();

    bool CloseCapture();

    bool StartCapture();

    int Release();

    int PushAudioData(OriginData *originData);

    OriginData *GetAudioData();

    mutex mMutex;
};

#endif //NATIVEAPP_AUDIOCAPTURE_H
