//
// Created by developer on 11/13/17.
//

#include "VideoEncoder.h"


VideoEncoder *VideoEncoder::Get() {
    static VideoEncoder videoEncoder;
    return &videoEncoder;
}

VideoEncoder::VideoEncoder() {

}

VideoEncoder::~VideoEncoder() {
    if (NULL != avCodecContext) {
        avcodec_free_context(&avCodecContext);
        avCodecContext = NULL;
    }
    LOG_D(DEBUG,"delete VideoEncoder");
}

void VideoEncoder::SetVideoCapture(VideoCapture *videoCapture) {
    this->videoCapture = videoCapture;
}

void VideoEncoder::SetVideoEncodeArgs(VideoEncodeArgs *videoEncodeArgs) {
    this->videoEncodeArgs = videoEncodeArgs;
}

int VideoEncoder::StartEncode() {
    mMutex.lock();

    mMutex.unlock();
    LOG_D(DEBUG, "Start Video Encode!")
    return 0;
}

int VideoEncoder::InitEncode() {
    mMutex.lock();
    int ret = 0;
    ExitFlag = false;
    //find libfdk_aac avcodec
    avCodec = avcodec_find_encoder(AV_CODEC_ID_H264);
    if (!avCodec) {
        LOG_D(DEBUG, "avcodec not found!");
        return -1;
    }
    avCodecContext = avcodec_alloc_context3(avCodec);
    if (!avCodecContext) {
        LOG_D(DEBUG, "avcodec alloc context failed!");
        return -1;
    }
    LOG_D(DEBUG, "avcodec alloc context success!");
    if (NULL != videoEncodeArgs) {
        avCodecContext->flags |= AV_CODEC_FLAG_GLOBAL_HEADER; //全局参数
        avCodecContext->codec_id = avCodec->id;
        avCodecContext->thread_count = 8;
        avCodecContext->bit_rate = 50 * 1024 * 8;//压缩后每秒视频的bit位大小 50kB
        avCodecContext->width = videoEncodeArgs->in_width;
        avCodecContext->height = videoEncodeArgs->in_height;
        avCodecContext->time_base = {1, (int) (videoEncodeArgs->fps)};
        avCodecContext->framerate = {(int) (videoEncodeArgs->fps), 1};
        avCodecContext->gop_size = 50;
        avCodecContext->pix_fmt = AV_PIX_FMT_YUV420P;

    }
    av_opt_set(avCodecContext->priv_data, "preset", "medium", 0);//preset 与编码速度和质量相关
    av_opt_set(avCodecContext->priv_data, "tune", "zerolatency", 0);//设置编码延迟为立即编码
    /**
     *选项: ultrafast, superfast, veryfast, faster, fast, medium, slow, slower, veryslow and placebo.
     * 固定码率150K,
     * 设置slow时：编码速度：245 fps 4ms
     * medium 时：编码速度：335 fps 2ms,
     * veryslow 时：编码速度：140 fps 7ms
     */

    AVDictionary *opts = NULL;
    av_dict_set(&opts, "profile", "baseline", NULL);//I/P帧
    ret = avcodec_open2(avCodecContext, avCodec, &opts);
    //avcodec open failed! info: Invalid argument
    if (ret != 0) {
        char buf[1024] = {0};
        av_strerror(ret, buf, sizeof(buf));
        LOG_D(DEBUG, "avcodec open failed! info: %s", buf);
        return -1;
    }
    LOG_D(DEBUG, "open video avcodec success!");
    mMutex.unlock();
    LOG_D(DEBUG, "Complete Init Video Encode!")
    return 0;
}

int VideoEncoder::Release() {

    LOG_D(DEBUG,"Release Video Encode!")
    return 0;
}

int VideoEncoder::CloseEncode() {
    mMutex.lock();
    ExitFlag = true;
    mMutex.unlock();
    LOG_D(DEBUG, "Close Video Encode!")
    return 0;
}
