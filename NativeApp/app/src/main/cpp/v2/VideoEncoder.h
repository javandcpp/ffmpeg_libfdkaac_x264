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

public:
    AVCodec *avCodec = NULL;
    AVStream *outStream = NULL;
    AVFrame *vOutFrame = NULL;
    AVCodecContext *avCodecContext = NULL;
    AVFrame *inputYUVFrame = NULL;

    bool isEncoding = false;

    static VideoEncoder *Get();

    static void *EncodeTask(void *obj);

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
     * 获取编码器状态
     */
    bool GetEncodeState();


    /**
     * 前置镜像
     */
    void YUVProcessMirror();


};


#endif //NATIVEAPP_VIDEOENCODER_H
