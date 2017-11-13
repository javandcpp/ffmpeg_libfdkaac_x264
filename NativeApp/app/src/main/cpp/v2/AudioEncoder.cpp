//
// Created by developer on 11/13/17.
//

#include "AudioEncoder.h"

AudioEncoder *AudioEncoder::Get() {
    static AudioEncoder audioEncoder;
    return &audioEncoder;
}

AudioEncoder::AudioEncoder() {

}

AudioEncoder::~AudioEncoder() {
    LOG_D(DEBUG, "delete AudioEncoder");
}

void AudioEncoder::SetAudioCapture(AudioCapture *audioCapture) {
    this->audioCapture = audioCapture;
}



void *AudioEncoder::EncodeTask(void *p) {
    AudioEncoder *audioEncoder = (AudioEncoder *) p;
    audioEncoder->isEncoding = true;
    AudioEncodeArgs *args = audioEncoder->audioCapture->GetAudioEncodeArgs();

    LOG_D(DEBUG,"begin audio encode");
    int64_t startTime = av_gettime();
    while (true) {
        //线程中断标记
        if (audioEncoder->audioCapture->GetCaptureState()) {
            break;
        }
        if(audioEncoder->aframeQueue.empty()){
            continue;
        }

    }

    return 0;
}

int AudioEncoder::StartEncode() {
    mMutex.lock();
    pthread_t t1;
    pthread_create(&t1, NULL, AudioEncoder::EncodeTask, this);
    mMutex.unlock();
    LOG_D(DEBUG, "Start Audio Encode task!")
    return 0;
}


int AudioEncoder::InitEncode() {
    mMutex.lock();
    avCodec = avcodec_find_encoder_by_name("libfdk_aac");
    int ret = 0;
    if (!avCodec) {
        LOG_D(DEBUG, "aac encoder not found!")
        return -1;
    }
    avCodecContext = avcodec_alloc_context3(avCodec);
    if (!avCodecContext) {
        LOG_D(DEBUG, "avcodec alloc context3 failed!");
        return -1;
    }
    avCodecContext->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
    avCodecContext->sample_fmt = AV_SAMPLE_FMT_S16;
    avCodecContext->sample_rate = audioCapture->GetAudioEncodeArgs()->sampleRate;
    avCodecContext->thread_count = 8;
    avCodecContext->bit_rate = audioCapture->GetAudioEncodeArgs()->bitRate;
    avCodecContext->channels = audioCapture->GetAudioEncodeArgs()->channels;
    avCodecContext->frame_size = audioCapture->GetAudioEncodeArgs()->nb_samples;
    avCodecContext->channel_layout = av_get_default_channel_layout(avCodecContext->channels);

    ret = avcodec_open2(avCodecContext, NULL, NULL);
    if (ret != 0) {
        char buf[1024] = {0};
        av_strerror(ret, buf, sizeof(buf));
        LOG_D(DEBUG, "avcodec open failed! info:%s", buf);
        return -1;
    }
    LOG_D(DEBUG, "open audio codec success!");
    mMutex.unlock();
    LOG_D(DEBUG, "Complete init Audio Encode!")
    return 0;
}

int AudioEncoder::Release() {
    LOG_D(DEBUG, "Release Audio Encode!");
    return 0;
}

int AudioEncoder::CloseEncode() {
    mMutex.lock();
    isEncoding = false;
    mMutex.unlock();
    LOG_D(DEBUG, "Close Audio Encode!")
    return 0;
}

bool AudioEncoder::GetEncodeState() {
    return isEncoding;
}