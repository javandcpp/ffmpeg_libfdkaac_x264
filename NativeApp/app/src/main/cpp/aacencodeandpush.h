//
// Created by developer on 11/6/17.
//


#include "base_include.h"
#include "arguments.h"

#include <iostream>
#include <stdexcept>
#include <exception>

#ifndef NATIVEAPP_AACENCODEANDPUSH_H
#define NATIVEAPP_AACENCODEANDPUSH_H


//aac编码器切换
#if 1
#define AAC_CODEC AAC
#else
#define FDK_CODEC FDK_AAC
#endif

//通道切换
#if 0
#define CHANNELS  1
#define CH_LAYOUT AV_CH_LAYOUT_MONO
#define BUFFER_SIZE 4096
#else
#define CHANNELS  2
#define CH_LAYOUT AV_CH_LAYOUT_STEREO
#define BUFFER_SIZE 4096
#endif

class EncodeAAC {


public:
    EncodeAAC();

    virtual ~EncodeAAC();

    bool exit;

    int initEncoder();

    static void *startEncode(void *obj);

    const char* outputUrl;

    void close();

    threadsafe_queue<uint8_t *> frame_queue;

    FILE *oAudioFile = NULL;
    FILE *iAudioFile = NULL;

    AVCodec *aVcodec = NULL;
    AVCodecContext *avCodecContext = NULL;

    AVFormatContext *avFormatContext = NULL;

    AVFrame *outputFrame = NULL;
    AVPacket packet;


    uint8_t *frame_buf = NULL;
    pthread_mutex_t mutex;
    SwrContext *swrContext = NULL;
//    static int size;
    AVStream *avStream = NULL;

};


#endif //NATIVEAPP_AACENCODEANDPUSH_H
