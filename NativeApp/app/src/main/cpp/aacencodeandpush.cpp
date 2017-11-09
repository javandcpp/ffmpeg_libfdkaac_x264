//
// Created by developer on 11/6/17.
//

#include "aacencodeandpush.h"
//int EncodeAAC::size = 0;

EncodeAAC::EncodeAAC() {
    pthread_mutex_init(&mutex, NULL);
}

EncodeAAC::~EncodeAAC() {
    pthread_mutex_destroy(&mutex);
    LOG_D(DEBUG, "~EncodeAAC");
}

void *EncodeAAC::startEncode(void *obj) {
    LOG_D(DEBUG, "audio encodec");
    EncodeAAC *encodeAAC = (EncodeAAC *) obj;
    encodeAAC->oAudioFile = fopen("/mnt/sdcard/oaudio.pcm", "wb+");
    FILE *tempFile = fopen("/mnt/sdcard/oaudio.aac", "wb+");
    int ret = 0;
    int apts = 0;
    ret = avio_open(&encodeAAC->avFormatContext->pb, encodeAAC->outputUrl,AVIO_FLAG_WRITE);
    if (ret < 0) {
        LOG_D(DEBUG, "avio_open failed");
        return (void*)-1;
    }
    LOG_D(DEBUG, "avio open success");
    ret = avformat_write_header(encodeAAC->avFormatContext, NULL);
    if (ret < 0) {
        LOG_D(DEBUG, "avformat_write_header failed!");
        return (void*)-1;
    }
    LOG_D(DEBUG, "write header success!");

    AVPacket avPacket = {0};
    for (; ;) {
        if (encodeAAC->exit) {
            break;
        }
        if (encodeAAC->frame_queue.empty()) {
            continue;
        }
        const uint8_t *indata[AV_NUM_DATA_POINTERS] = {0};
        //PCM s16
        uint8_t *buf = *encodeAAC->frame_queue.wait_and_pop().get();//PCM 16bit
#ifdef FDK_CODEC
        ret = avcodec_fill_audio_frame(encodeAAC->outputFrame, encodeAAC->avCodecContext->channels,
                                       encodeAAC->avCodecContext->sample_fmt, buf, BUFFER_SIZE, 0);
        if (ret < 0) {
            LOG_D(DEBUG, "fill frame failed!");
            continue;
        }
#else
        //重采样为AM_SAMPLE_FMT_FLTP
        indata[0] = buf;
        swr_convert(encodeAAC->swrContext, encodeAAC->outputFrame->data,
                    encodeAAC->outputFrame->nb_samples, indata,
                    encodeAAC->outputFrame->nb_samples);
#endif
        encodeAAC->outputFrame->pts = apts;
        apts += av_rescale_q(encodeAAC->outputFrame->nb_samples,
                             (AVRational) {1, 48000},
                             encodeAAC->avCodecContext->time_base);

        ret = avcodec_send_frame(encodeAAC->avCodecContext, encodeAAC->outputFrame);
        if (ret != 0) {
            LOG_D(DEBUG, "send frame failed!");
            continue;
        }
        ret = avcodec_receive_packet(encodeAAC->avCodecContext, &avPacket);
        if (ret != 0) {
            LOG_D(DEBUG, "receive packet failed!");
            continue;
        }
//        fwrite(avPacket.data, 1, avPacket.size, tempFile);
//        fflush(tempFile);
        LOG_D(DEBUG, "frame size:%d", encodeAAC->avCodecContext->frame_size);
        LOG_D(DEBUG, "packet pts:%lld", avPacket.pts);

        avPacket.pts = av_rescale_q_rnd(avPacket.pts, encodeAAC->avCodecContext->time_base,
                                        encodeAAC->avStream->time_base,
                                        (AVRounding) (AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
        avPacket.dts = av_rescale_q_rnd(avPacket.dts, encodeAAC->avCodecContext->time_base,
                                        encodeAAC->avStream->time_base,
                                        (AVRounding) (AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
        avPacket.duration = av_rescale_q(avPacket.duration, encodeAAC->avCodecContext->time_base,
                                         encodeAAC->avStream->time_base);

        avPacket.stream_index = encodeAAC->avStream->index;
        LOG_D(DEBUG, "as timebase  %d,%d", encodeAAC->avStream->time_base.num,
              encodeAAC->avStream->time_base.den);
        LOG_D(DEBUG, "av timebase  %d,%d", encodeAAC->avCodecContext->time_base.num,
              encodeAAC->avCodecContext->time_base.den);

        ret = av_interleaved_write_frame(encodeAAC->avFormatContext, &avPacket);
        if (ret == 0) {
            LOG_D(DEBUG, "write frame success!");
        }
        av_packet_unref(&avPacket);
    }

    av_write_trailer(encodeAAC->avFormatContext);
    LOG_D(DEBUG, "av_write_trailer");
    avio_close(encodeAAC->avFormatContext->pb);
    if (encodeAAC->avFormatContext) {
        avformat_free_context(encodeAAC->avFormatContext);
        encodeAAC->avFormatContext = NULL;
    }
    if (encodeAAC->avCodecContext) {
        avcodec_close(encodeAAC->avCodecContext);
        avcodec_free_context(&encodeAAC->avCodecContext);
        encodeAAC->avCodecContext = NULL;
    }
    if (encodeAAC->outputFrame) {
        av_frame_free(&encodeAAC->outputFrame);
        encodeAAC->outputFrame = NULL;
    }
    LOG_D(DEBUG, "avio_close");
    LOG_D(DEBUG, "end encode");
    return 0;
}

void EncodeAAC::close() {
    exit = true;
    LOG_D(DEBUG, "close");
}

int EncodeAAC::initEncoder() {
    struct timeval start;
    struct timeval end;
    gettimeofday(&start, NULL);
    outputUrl = "rtmp://192.168.24.132:1935/test/live";
    av_register_all();
    avformat_network_init();
    int ret = 0;

#ifdef FDK_CODEC
    //fdk_aac
    aVcodec = avcodec_find_encoder_by_name("libfdk_aac");
#else
    //ffmpeg aac
    aVcodec = avcodec_find_encoder(AV_CODEC_ID_AAC);
#endif

#if DEBUG
    LOG_D(DEBUG, "configure:%s", avcodec_configuration());
#endif

    if (!aVcodec) {
        LOG_D(DEBUG, "avcodec find encoder NULL");
        return -1;
    }
    avCodecContext = avcodec_alloc_context3(aVcodec);
    if (!avCodecContext) {
        LOG_D(DEBUG, "avcodec_alloc_context3 failed");
        return -1;
    }
    avCodecContext->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
#ifdef FDK_CODEC
    avCodecContext->sample_fmt = AV_SAMPLE_FMT_S16;
#else
    avCodecContext->sample_fmt=AV_SAMPLE_FMT_FLTP;
#endif
    avCodecContext->sample_rate = 48000;
    avCodecContext->thread_count = 8;
    avCodecContext->bit_rate = 64000;
    avCodecContext->channels = CHANNELS;
    avCodecContext->channel_layout = av_get_default_channel_layout(avCodecContext->channels);

    ret = avcodec_open2(avCodecContext, aVcodec, NULL);
    if (ret < 0) {
        char buf[1024] = {0};
        av_strerror(ret, buf, sizeof(buf));
        LOG_D(DEBUG, "avcodec_open2  audio failed  :%s", buf);
        return -1;
    }

    ret = avformat_alloc_output_context2(&avFormatContext, NULL, "flv",
                                         outputUrl);
    if (ret < 0) {
        LOG_D(DEBUG, "avformat alloc output context2 failed");
        return -1;
    }
    LOG_D(DEBUG, "avformat alloc output context2 success!");

    avStream = avformat_new_stream(avFormatContext, aVcodec);
    if (!avStream) {
        LOG_D(DEBUG, "avformat new stream error");
        return -1;
    }
    LOG_D(DEBUG, "avformat new stream success");
    avcodec_parameters_from_context(avStream->codecpar, avCodecContext);
    avStream->codecpar->codec_tag = 0;

    //FFMPEG AAC需要重采样
#ifdef AAC_CODEC
    swrContext = swr_alloc_set_opts(swrContext, av_get_default_channel_layout(CHANNELS),
                                    AV_SAMPLE_FMT_FLTP,
                                    48000,
                                    av_get_default_channel_layout(CHANNELS),
                                    AV_SAMPLE_FMT_S16,
                                    48000, 0, NULL);
      ret = swr_init(swrContext);

    if (ret != 0) {
        LOG_D(DEBUG, "swr_init failed!");
        return -1;
    }
#endif
    outputFrame = av_frame_alloc();
    outputFrame->channels = CHANNELS;
    outputFrame->channel_layout = av_get_default_channel_layout(outputFrame->channels);
    outputFrame->format = avCodecContext->sample_fmt;
    outputFrame->nb_samples = 1024;
    ret = av_frame_get_buffer(outputFrame, 0);
    if (ret != 0) {
        LOG_D(DEBUG, "av_frame_get_buffer failed!");
        return -1;
    }
    LOG_D(DEBUG, "av_frame_get_buffer success!");

    gettimeofday(&end, NULL);
    LOG_D(DEBUG, "host time millseconds:%d", end.tv_usec / 1000);
    pthread_t id1;
    pthread_create(&id1, NULL, EncodeAAC::startEncode, this);
    return 0;
}