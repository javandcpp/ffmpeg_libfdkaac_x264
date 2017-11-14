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
    int totalSize = args->out_width * args->out_height * 3 / 2;
    int ySize = args->out_width * args->out_height;
    LOG_D(DEBUG, "in_height %d,int_width %d", args->out_height, args->out_width);

#ifdef WRITE_YUV_TO_FILE
    FILE *I420 = fopen("/mnt/sdcard/ivideo.yuv", "wb+");
#endif

    videoEncoder->videoCapture->frame_queue.clear();
    int64_t beginTime = av_gettime();
    LOG_D(DEBUG, "begin video encode");
    while (true) {
        if (videoEncoder->videoCapture->GetCaptureState()) {
            break;
        }
        if (videoEncoder->videoCapture->frame_queue.empty()) {
            continue;
        }
        OriginData *srcData = *videoEncoder->videoCapture->frame_queue.wait_and_pop().get();
        if (srcData->size <= 0 || !srcData->data) {
            continue;
        }

        /**
         * 方式一：
         * I420手动填充AVFrame,需要注意ySize =width*height;
         */
//        memcpy(videoEncoder->outputYUVFrame->data[0], srcData->data, ySize);//Y
//        memcpy(videoEncoder->outputYUVFrame->data[1], srcData->data + ySize, ySize / 4);//U
//        memcpy(videoEncoder->outputYUVFrame->data[2], srcData->data + (ySize * 5 / 4),
//               ySize / 4);
//        videoEncoder->outputYUVFrame->linesize[0] = videoEncoder->avCodecContext->width;
//        videoEncoder->outputYUVFrame->linesize[1] = videoEncoder->avCodecContext->width / 2;
//        videoEncoder->outputYUVFrame->linesize[2] = videoEncoder->avCodecContext->width / 2;

        /**
         * 方式二:  deprecated
         */

//        avpicture_fill((AVPicture *) videoEncoder->outputYUVFrame, srcData->data, AV_PIX_FMT_YUV420P, videoEncoder->avCodecContext->width,
//                       videoEncoder->avCodecContext->height);

        /**
         * 方式三:
         */
        av_image_fill_arrays(videoEncoder->outputYUVFrame->data,
                             videoEncoder->outputYUVFrame->linesize, srcData->data,
                             AV_PIX_FMT_YUV420P, videoEncoder->avCodecContext->width,
                             videoEncoder->avCodecContext->height, 1);
#ifdef ENCODE_INFO
        LOG_D(DEBUG, "linesize[0]=%d", videoEncoder->outputYUVFrame->linesize[0]);
        LOG_D(DEBUG, "linesize[1]=%d", videoEncoder->outputYUVFrame->linesize[1]);
        LOG_D(DEBUG, "linesize[2]=%d", videoEncoder->outputYUVFrame->linesize[2]);
#endif
#ifdef PTS_INFO
        videoEncoder->outputYUVFrame->pts = srcData->pts - beginTime;
        LOG_D(DEBUG, "video pts:%lld", videoEncoder->outputYUVFrame->pts);
#endif
        //编码
        int ret = 0;
        ret = avcodec_send_frame(videoEncoder->avCodecContext, videoEncoder->outputYUVFrame);
        if (ret != 0) {
            LOG_D(DEBUG, "avcodec send frame failed");
            continue;
        }
        ret = avcodec_receive_packet(videoEncoder->avCodecContext, &videoEncoder->videoPacket);
        if (ret != 0) {
            LOG_D(DEBUG, "avcodec recieve packet failed");
            continue;
        }
        srcData->Drop();

        //TODO 编码完成(数据传递问题不知道有没有问题)
        srcData->avPacket = &videoEncoder->videoPacket;
        srcData->pts = videoEncoder->outputYUVFrame->pts;
        srcData->size = videoEncoder->videoPacket.size;
        videoEncoder->vframeQueue.push(srcData);
        LOG_D(DEBUG, "Video frame encode success!");


#ifdef WRITE_YUV_TO_FILE
        fwrite(videoEncoder->outputYUVFrame->data[0], 1, ySize, I420);//Y
        fwrite(videoEncoder->outputYUVFrame->data[1], 1, ySize / 4, I420);//U
        fwrite(videoEncoder->outputYUVFrame->data[2], 1, ySize / 4, I420);//V
        fflush(I420);
#endif
    }

#ifdef WRITE_YUV_TO_FILE
    fclose(I420);
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
        avCodecContext->width = videoCapture->GetVideoEncodeArgs()->out_height;
        avCodecContext->height = videoCapture->GetVideoEncodeArgs()->out_width;
        avCodecContext->time_base = {1, videoCapture->GetVideoEncodeArgs()->fps};
        avCodecContext->framerate = {videoCapture->GetVideoEncodeArgs()->fps, 1};
        avCodecContext->gop_size = 50;
        avCodecContext->time_base = {1, 1000000};//AUDIO VIDEO 两边时间基数要相同
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

    outputYUVFrame = av_frame_alloc();
    outputYUVFrame->format = AV_PIX_FMT_YUV420P;
    outputYUVFrame->width = avCodecContext->width;//480
    outputYUVFrame->height = avCodecContext->height;//640

    //分配yuv空间
    ret = av_frame_get_buffer(outputYUVFrame, 32);
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
