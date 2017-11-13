//
// Created by developer on 11/13/17.
//

#ifndef NATIVEAPP_VIDEOENCODER_H
#define NATIVEAPP_VIDEOENCODER_H

#include "MediaEncoder.h"
#include "PrefixHeader.h"
#include "VideoCapture.h"

class VideoEncoder : public MediaEncoder {

private:
    VideoCapture *videoCapture = NULL;
    VideoEncodeArgs *videoEncodeArgs = NULL;
public:
    AVCodec *avCodec = NULL;
    AVStream *outStream = NULL;
    AVFrame *vOutFrame = NULL;
    AVCodecContext *avCodecContext = NULL;

    static VideoEncoder *Get();

    /**
     * 已编码数据队列
     */
    threadsafe_queue<OriginData *> vframeQueue;


    VideoEncoder();

    ~VideoEncoder();

    /**
     * 开启编码
     */
    int StartEncode();

    /**
     * 初始化H264视频编码器
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
    void SetVideoCapture(VideoCapture *videoCapture);

    /**
     * 设置编码参数
     */
    void SetVideoEncodeArgs(VideoEncodeArgs *videoEncodeArgs);


};


#endif //NATIVEAPP_VIDEOENCODER_H
