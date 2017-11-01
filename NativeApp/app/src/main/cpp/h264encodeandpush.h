//
// Created by developer on 10/31/17.
//

#include "base_include.h"
#include "arguments.h"
#include <iostream>
#include <stdexcept>
#include <exception>
/**
 * android 摄像头采集与推流(暂无音频)
 */

using namespace std;
class EncoderH264 {

public:
    EncoderH264();

    virtual ~EncoderH264();

public:


    int initEncoder();

    static void *startEncode(void *obj);

    void close();

    threadsafe_queue<uint8_t *> frame_queue;
    UserArguments arguments;


    bool exit = false;

    //像素格式转换上下文
    SwsContext *scsCtx = NULL;

    //输出的数据结构
    AVFrame *outputYUV = NULL;
    AVFrame *inputYUV = NULL;

    AVPacket pack;
    AVStream *vs = NULL;
    FILE *oVideoFile = NULL;
    FILE *iVideoFile = NULL;

    //编码器上下文
    AVCodecContext *avCodecContext = NULL;

    //rtmp flv 封装器
    AVFormatContext *iCtx = NULL;

    AVCodec *avCodec;

};

