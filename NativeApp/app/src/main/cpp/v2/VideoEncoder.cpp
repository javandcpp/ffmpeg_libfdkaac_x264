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

    LOG_D(DEBUG, "delete VideoEncoder");
}

void VideoEncoder::SetVideoCapture(VideoCapture *videoCapture) {
    this->videoCapture = videoCapture;
}





void VideoEncoder::YUVProcessMirror() {

    return;
}


void *VideoEncoder::EncodeTask(void *obj) {
    VideoEncoder *videoEncoder = (VideoEncoder *) obj;
    videoEncoder->isEncoding = true;
    VideoEncodeArgs *args = videoEncoder->videoCapture->GetVideoEncodeArgs();
    int totalSize = args->in_height * args->in_width * 3 / 2;
    LOG_D(DEBUG, "in_height %d,int_width %d", args->in_height, args->in_width);

#ifdef WRITE_YUV_TO_FILE
    FILE *NV21 = fopen("/mnt/sdcard/ivideo.yuv", "wb+");
#endif
    int64_t startTime = av_gettime();
    LOG_D(DEBUG, "begin video encode");
    while (true) {
        if (videoEncoder->videoCapture->GetCaptureState()) {
            break;
        }
        if (videoEncoder->videoCapture->frame_queue.empty()) {
            continue;
        }
        OriginData *srcData = *(videoEncoder->videoCapture->frame_queue.wait_and_pop().get());
        if (!srcData->size > 0) {
            continue;
        }
        //处理转换

#ifdef WRITE_YUV_TO_FILE
        fwrite(srcData->data, 1, totalSize, NV21);
        fflush(NV21);
#endif
        srcData->Drop();
        LOG_D(DEBUG, "encode video data size:%d", totalSize);
    }

#ifdef WRITE_YUV_TO_FILE
    fclose(NV21);
#endif
    return 0;
}

int VideoEncoder::StartEncode() {
    mMutex.lock();
    pthread_t t1;
    pthread_create(&t1, NULL, VideoEncoder::EncodeTask, this);
    mMutex.unlock();
    LOG_D(DEBUG, "Start Video Encode task!")
    return 0;
}

int VideoEncoder::InitEncode() {
    mMutex.lock();
    int ret = 0;
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


    if (NULL != videoCapture->GetVideoEncodeArgs()) {
        avCodecContext->flags |= AV_CODEC_FLAG_GLOBAL_HEADER; //全局参数
        avCodecContext->codec_id = avCodec->id;
        avCodecContext->thread_count = 8;
        avCodecContext->bit_rate = 50 * 1024 * 8;//压缩后每秒视频的bit位大小 50kB
        avCodecContext->width = videoCapture->GetVideoEncodeArgs()->in_width;
        avCodecContext->height = videoCapture->GetVideoEncodeArgs()->in_height;
        avCodecContext->time_base = {1, videoCapture->GetVideoEncodeArgs()->fps};
        avCodecContext->framerate = {videoCapture->GetVideoEncodeArgs()->fps, 1};
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

    inputYUVFrame = av_frame_alloc();
    inputYUVFrame->format = AV_PIX_FMT_NV21;
    inputYUVFrame->width = videoCapture->GetVideoEncodeArgs()->in_width;
    inputYUVFrame->height = videoCapture->GetVideoEncodeArgs()->in_height;
    inputYUVFrame->pts = 0;

    //分配yuv空间
    ret = av_frame_get_buffer(inputYUVFrame, 32);
    if (ret != 0) {
        char buf[1024] = {0};
        av_strerror(ret, buf, sizeof(buf) - 1);
        LOG_E(DEBUG, "input av_frame_get_buffer:%s", buf);
        return -1;
    }


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

    LOG_D(DEBUG, "Release Video Encode!")
    return 0;
}

bool VideoEncoder::GetEncodeState() {
    return isEncoding;
}

int VideoEncoder::CloseEncode() {
    mMutex.lock();
    isEncoding = false;
    mMutex.unlock();
    LOG_D(DEBUG, "Close Video Encode!")
    return 0;
}
