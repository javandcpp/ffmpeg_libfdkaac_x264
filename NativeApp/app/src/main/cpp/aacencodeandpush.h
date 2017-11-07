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

class EncodeAAC{


public:
    EncodeAAC();

    virtual ~EncodeAAC();

    bool exit;

    int initEncoder();

    static void *startEncode(void *obj);

    void close();

    threadsafe_queue<uint8_t *> frame_queue;
    UserArguments output_arguments;
    UserArguments input_arguments;

    FILE *oAudioFile = NULL;
    FILE *iAudioFile = NULL;

    AVCodec* aVcodec=NULL;
    AVCodecContext* avCodecContext=NULL;

    AVFormatContext* avFormatContext=NULL;

    AVFrame* outputFrame=NULL;
    AVPacket packet;


    uint8_t *frame_buf=NULL;

    SwrContext* swrContext=NULL;
    static int size;
    AVStream* avStream=NULL;

};




#endif //NATIVEAPP_AACENCODEANDPUSH_H
