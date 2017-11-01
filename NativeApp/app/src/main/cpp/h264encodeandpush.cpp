//
// Created by developer on 10/31/17.
//
#include "h264encodeandpush.h"


EncoderH264::EncoderH264() {
//    pthread_mutex_init(&mutex, NULL);
}

EncoderH264::~EncoderH264() {
//    pthread_mutex_destroy(&mutex);
}

void EncoderH264::close() {

    exit = true;
    av_write_trailer(iCtx);
    if (avCodecContext) {
        avcodec_close(avCodecContext);
    }
    if (inputYUV) {
        av_frame_free(&inputYUV);
        inputYUV = NULL;
    }
    if (iCtx) {
        avio_close(iCtx->pb);
        iCtx->pb = NULL;
        avformat_free_context(iCtx);
        iCtx = NULL;
    }
    LOG_I(DEBUG, "视频编码结束");
}


void *EncoderH264::startEncode(void *pa) {
    LOG_D(DEBUG, "encode");
    EncoderH264 *encoderH264 = (EncoderH264 *) pa;
    encoderH264->oVideoFile = fopen("/mnt/sdcard/ovideo.yuv", "wb+");
    int frame_count;
    int in_y_size = encoderH264->arguments.in_width * encoderH264->arguments.in_height;
//    uint8_t *yuvBuffer = (uint8_t *) malloc(in_y_size * 3 / 2);
    int vpts = 0;
    int64_t start_time = av_gettime();
    int frame_index;
    for (; ;) {
        if (encoderH264->exit) {
            break;
        }
        if (encoderH264->frame_queue.empty()) {
            continue;
        }

        uint8_t *picture_buf = *encoderH264->frame_queue.wait_and_pop().get();
//        memcpy(yuvBuffer, pictur_buf, in_y_size * 3 / 2);

        memcpy(encoderH264->outputYUV->data[0], picture_buf, in_y_size);//Y
        memcpy(encoderH264->outputYUV->data[1], picture_buf + in_y_size, in_y_size / 4);//U
        memcpy(encoderH264->outputYUV->data[2], picture_buf + in_y_size * 5 / 4, in_y_size / 4);//V

        encoderH264->outputYUV->linesize[0] = encoderH264->arguments.in_width;
        encoderH264->outputYUV->linesize[1] = encoderH264->arguments.in_width / 2;
        encoderH264->outputYUV->linesize[2] = encoderH264->arguments.in_width / 2;

//        fwrite(pictur_buf,1,in_y_size*3/2,encoderH264->oVideoFile);
//        fflush(encoderH264->oVideoFile);

        LOG_D(DEBUG, "output write to file   w:%d,h:%d", encoderH264->arguments.in_width,
              encoderH264->arguments.in_height);
        encoderH264->outputYUV->pts =vpts;
        vpts++;


        int ret = avcodec_send_frame(encoderH264->avCodecContext, encoderH264->outputYUV);
        LOG_D(DEBUG, "send frame result:%d", ret);
        if (ret != 0) {
            continue;
        }
        ret = avcodec_receive_packet(encoderH264->avCodecContext, &encoderH264->pack);
        if (ret != 0 || encoderH264->pack.size > 0) {
            //cout << "*" << pack.size << flush;
            LOG_D(DEBUG, "pack encode size:%d", encoderH264->pack.size);
        }
        else {
            continue;
        }

//        if (encoderH264->pack.pts == AV_NOPTS_VALUE) {
//            //Write PTS
//            LOG_D(DEBUG, "WRITE pts");
//            AVRational time_base1 = AV_TIME_BASE_Q;
//            //Duration between 2 frames (us)
//            int64_t calc_duration = (double) AV_TIME_BASE / av_q2d(time_base1);
//            //Parameters
//            encoderH264->pack.pts = (double) (frame_index * calc_duration) /
//                                    (double) (av_q2d(time_base1) * AV_TIME_BASE);
//            encoderH264->pack.dts = encoderH264->pack.pts;
//            encoderH264->pack.duration =
//                    (double) calc_duration / (double) (av_q2d(time_base1) * AV_TIME_BASE);
//        }

        AVRational time_base = AV_TIME_BASE_Q;
        AVRational time_base_q = {1, AV_TIME_BASE};
        LOG_D(DEBUG,"pack.dts");
        int64_t pts_time = av_rescale_q(encoderH264->pack.dts, time_base, time_base_q);
        int64_t now_time = av_gettime() - start_time;
        LOG_D(DEBUG,"host:%lld",pts_time- now_time);
        if (pts_time > now_time)
            av_usleep(pts_time - now_time);

        frame_index++;

        //推流
        encoderH264->pack.pts = av_rescale_q_rnd(encoderH264->pack.pts,
                                             encoderH264->avCodecContext->time_base,
                                             encoderH264->vs->time_base,  (AVRounding)(AV_ROUND_NEAR_INF|AV_ROUND_PASS_MINMAX));
        encoderH264->pack.dts = av_rescale_q_rnd(encoderH264->pack.dts,
                                             encoderH264->avCodecContext->time_base,
                                             encoderH264->vs->time_base,  (AVRounding)(AV_ROUND_NEAR_INF|AV_ROUND_PASS_MINMAX));
        encoderH264->pack.duration = av_rescale_q(encoderH264->pack.duration,
                                                  encoderH264->avCodecContext->time_base,
                                                  encoderH264->vs->time_base);
        encoderH264->pack.pos = -1;



        encoderH264->pack.stream_index = encoderH264->vs->index;

        ret = av_interleaved_write_frame(encoderH264->iCtx, &encoderH264->pack);


        LOG_D(DEBUG, "push stream result:%d", ret);
    }
//    delete yuvBuffer;

    return 0;

}


int EncoderH264::initEncoder() {
    arguments.video_bit_rate = 580000;
    arguments.frame_rate = 25;
    arguments.in_height = 480;
    arguments.in_width = 640;


    arguments.in_width = 480;
    arguments.in_height = 640;

    const char *url = "rtmp://192.168.24.132:1935/test/live";

    char *url_space = (char *) malloc(strlen(url) + 1);
    arguments.outputUrl = strcpy(url_space, url);

    av_register_all();
    avformat_network_init();

    //初始化像素格式转换器(NV21 TO I420 )
    scsCtx = sws_getCachedContext(scsCtx,
                                  arguments.in_width, arguments.in_height,
                                  AV_PIX_FMT_NV21,     //源宽、高、像素格式
                                  arguments.in_height, arguments.in_width,
                                  AV_PIX_FMT_YUV420P,//目标宽、高、像素格式
                                  SWS_BICUBIC,  // 尺寸变化使用算法
                                  0, 0, 0
    );

    //初始化输出AVFrame


    outputYUV = av_frame_alloc();
    outputYUV->format = AV_PIX_FMT_YUV420P;
    outputYUV->width = arguments.in_height;
    outputYUV->height = arguments.in_width;
    outputYUV->pts = 0;

    //分配yuv空间
    int ret = av_frame_get_buffer(outputYUV, 32);
    if (ret != 0) {
        char buf[1024] = {0};
        av_strerror(ret, buf, sizeof(buf) - 1);
        LOG_E(DEBUG, "output av_frame_get_buffer:%s", buf);


    }
    inputYUV = av_frame_alloc();
    inputYUV->format = AV_PIX_FMT_NV21;
    inputYUV->width = arguments.in_width;
    inputYUV->height = arguments.in_height;
    inputYUV->pts = 0;

    //分配yuv空间
    ret = av_frame_get_buffer(inputYUV, 32);
    if (ret != 0) {
        char buf[1024] = {0};
        av_strerror(ret, buf, sizeof(buf) - 1);
        LOG_E(DEBUG, "input av_frame_get_buffer:%s", buf);
        return -1;

    }
    //查找编码器
    avCodec = avcodec_find_encoder(AV_CODEC_ID_H264);
    if (!avCodec) {
        LOG_E(DEBUG, "avcodec_find_encoder failed!");
        return -1;
    }
    //创建编码器上下文
    avCodecContext = avcodec_alloc_context3(avCodec);
    if (!avCodecContext) {
        LOG_E(DEBUG, "avcodec_alloc_context3 failed!");
    }

    //c 配置编码器参数
    avCodecContext->flags |= AV_CODEC_FLAG_GLOBAL_HEADER; //全局参数
    avCodecContext->codec_id = avCodec->id;
    avCodecContext->thread_count = 8;

    avCodecContext->bit_rate = 50 * 1024 * 8;//压缩后每秒视频的bit位大小 50kB
    avCodecContext->width = arguments.in_width;
    avCodecContext->height = arguments.in_height;
    avCodecContext->time_base = {1, 25};
    avCodecContext->framerate = {25, 1};

    //画面组的大小，多少帧一个关键帧
    avCodecContext->gop_size = 50;
    avCodecContext->max_b_frames = 0;
    avCodecContext->pix_fmt = AV_PIX_FMT_YUV420P;

    ret = avcodec_open2(avCodecContext, NULL, NULL);
    if (ret != 0) {
        char buf[1024] = {0};
        av_strerror(ret, buf, sizeof(buf) - 1);
        LOG_E(DEBUG, "avcodec_open2 failed:%s", buf);
        return -1;
    }

    LOG_D(DEBUG, "avcodec_open success!");

    ///5 输出封装器和视频流配置
    //a 创建输出封装器上下文
    ret = avformat_alloc_output_context2(&iCtx, 0, "flv", arguments.outputUrl);
    if (ret != 0) {
        char buf[1024] = {0};
        av_strerror(ret, buf, sizeof(buf) - 1);
        LOG_E(DEBUG, "avformat_alloc_output_context2 failed");
        return -1;
    }
    //b 添加视频流
    vs = avformat_new_stream(iCtx, NULL);
    if (!vs) {
        return -1;
    }
    vs->codecpar->codec_tag = 0;
    //从编码器复制参数
    avcodec_parameters_from_context(vs->codecpar, avCodecContext);
    av_dump_format(iCtx, 0, arguments.outputUrl, 1);


    ///打开rtmp 的网络输出IO
    ret = avio_open(&iCtx->pb, arguments.outputUrl, AVIO_FLAG_WRITE);
    if (ret != 0) {
        char buf[1024] = {0};
        av_strerror(ret, buf, sizeof(buf) - 1);
        LOG_D(DEBUG, "avio_open failed:^%s", buf);
        return -1;
    }

    //写入封装头
    ret = avformat_write_header(iCtx, NULL);
    if (ret != 0) {
        char buf[1024] = {0};
        av_strerror(ret, buf, sizeof(buf) - 1);
        LOG_D(DEBUG, "avformat_write_header:^%s", buf);
        return -1;
    }
    memset(&pack, 0, sizeof(pack));
    pthread_t t1;
    pthread_create(&t1, NULL, EncoderH264::startEncode, this);
    return 0;
}
