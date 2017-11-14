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

    FILE *PCM = fopen("/mnt/sdcard/iaudio.pcm", "wb+");
    LOG_D(DEBUG, "begin audio encode");
    audioEncoder->audioCapture->frame_queue.clear();
    int64_t beginTime = av_gettime();
    while (true) {
        //线程中断标记
        if (audioEncoder->audioCapture->GetCaptureState()) {
            break;
        }
        //获取音频采集中的对列中的数据
        if (audioEncoder->audioCapture->frame_queue.empty()) {
            continue;
        }
        OriginData *srcData = *audioEncoder->audioCapture->frame_queue.wait_and_pop().get();
        if (srcData->size <= 0 || !srcData->data) {
            continue;
        }
        int ret = 0;
        ret = avcodec_fill_audio_frame(audioEncoder->outputFrame,
                                       audioEncoder->avCodecContext->channels,
                                       audioEncoder->avCodecContext->sample_fmt, srcData->data,
                                       4096, 0);
        if (ret < 0) {
            LOG_D(DEBUG, "fill frame failed!");
            continue;
        }
#ifdef PTS_INFO
        audioEncoder->outputFrame->pts = srcData->pts - beginTime;
        LOG_D(DEBUG, "audio pts:%lld", audioEncoder->outputFrame->pts);
#endif
        ret = avcodec_send_frame(audioEncoder->avCodecContext, audioEncoder->outputFrame);
        if (ret != 0) {
            LOG_D(DEBUG, "send frame failed!");
            continue;
        }
        ret = avcodec_receive_packet(audioEncoder->avCodecContext, &audioEncoder->avPacket);
        if (ret != 0) {
            LOG_D(DEBUG, "receive packet failed!");
            continue;
        }
        LOG_D(DEBUG, "Audio frame encode success!");

#ifdef WRITE_PCM_TO_FILE
        fwrite(audioEncoder->outputFrame->data[0], 1, 4096, PCM);//V
        fflush(PCM);
#endif
    }
#ifdef WRITE_PCM_TO_FILE
    fclose(PCM);
#endif
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
    avCodecContext->time_base = {1, 1000000};//AUDIO VIDEO 两边时间基数要相同
    avCodecContext->channel_layout = av_get_default_channel_layout(avCodecContext->channels);


    outputFrame = av_frame_alloc();
    outputFrame->channels = avCodecContext->channels;
    outputFrame->channel_layout = av_get_default_channel_layout(outputFrame->channels);
    outputFrame->format = avCodecContext->sample_fmt;
    outputFrame->nb_samples = 1024;
    ret = av_frame_get_buffer(outputFrame, 0);
    if (ret != 0) {
        LOG_D(DEBUG, "av_frame_get_buffer failed!");
        return -1;
    }
    LOG_D(DEBUG, "av_frame_get_buffer success!");

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