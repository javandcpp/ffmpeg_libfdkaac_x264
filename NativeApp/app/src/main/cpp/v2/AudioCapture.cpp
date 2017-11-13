//
// Created by developer on 11/12/17.
//

#include "AudioCapture.h"

AudioCapture::AudioCapture() {

}

AudioCapture::~AudioCapture() {
    if(NULL!=audioEncodeArgs){
        delete audioEncodeArgs;
    }
}

AudioCapture *AudioCapture::Get() {
    static AudioCapture audioCapture;
    return &audioCapture;
}

/**
 * 资源回收
 */
int AudioCapture::Release() {

    LOG_D(DEBUG, "Release Audio Capture!");
    return 0;
}

/**
 * 
 */
bool AudioCapture::CloseCapture() {
    mMutex.lock();
    ExitCapture = true;
    mMutex.unlock();
    LOG_D(DEBUG, "Close Audio Capture");
    return ExitCapture;
}


bool AudioCapture::StartCapture() {
    mMutex.lock();
    ExitCapture = false;
    mMutex.unlock();
    LOG_D(DEBUG, "Start Audio Capture");
    return !ExitCapture;
}

/**
 * 往除列添加音频原数据
 */
int AudioCapture::PushAudioData(OriginData *originData) {
    if (ExitCapture)
        return 0;
    originData->pts = av_gettime();
//    LOG_D(DEBUG,"audio pts:%lld  , data size:%d",originData->pts,originData->size);
    frame_queue.push(originData);
    return originData->size;
}

void AudioCapture::SetAudioEncodeArgs(AudioEncodeArgs *audioEncodeArgs) {
    this->audioEncodeArgs = audioEncodeArgs;
}

AudioEncodeArgs* AudioCapture::GetAudioEncodeArgs() {
    return this->audioEncodeArgs;
}

bool AudioCapture::GetCaptureState() {
    return ExitCapture;
}

OriginData *AudioCapture::GetAudioData() {
    if (ExitCapture)
        return NULL;
    return frame_queue.empty() ? NULL : *(frame_queue.wait_and_pop().get());
}

