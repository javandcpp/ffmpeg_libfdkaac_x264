//
// Created by developer on 11/6/17.
//

#include "aacencodeandpush.h"

int EncodeAAC::size = 0;

EncodeAAC::EncodeAAC() {

}

EncodeAAC::~EncodeAAC() {
    if (avFormatContext) {
        avformat_free_context(avFormatContext);
        avFormatContext = NULL;
    }

    if (avCodecContext) {
        avcodec_free_context(&avCodecContext);
        avCodecContext = NULL;
    }
    if (outputFrame) {
        av_frame_free(&outputFrame);
        outputFrame = NULL;
    }
}

void *EncodeAAC::startEncode(void *obj) {
    LOG_D(DEBUG, "audio encodec");
    EncodeAAC *encodeAAC = (EncodeAAC *) obj;
    encodeAAC->oAudioFile = fopen("/mnt/sdcard/oaudio1.pcm", "wb+");
    FILE *tempFile = fopen("/mnt/sdcard/oaudio.aac", "wb+");
    int ret = 0;
    int apts = 0;
    AVPacket avPacket;
    memset(&avPacket, 0, sizeof(avPacket));
    for (; ;) {
        if (encodeAAC->exit) {
            break;
        }
        if (encodeAAC->frame_queue.empty()) {
            continue;
        }
        const uint8_t *indata[AV_NUM_DATA_POINTERS] = {0};
        //PCM s16
        uint8_t *picture_buf = *encodeAAC->frame_queue.wait_and_pop().get();//PCM 16bit
        indata[0] = picture_buf;

//        fwrite(indata[0], 1, size, encodeAAC->oAudioFile);
//        fflush(encodeAAC->oAudioFile);
//        LOG_D(DEBUG, "write file size:%d", size);


        swr_convert(encodeAAC->swrContext, encodeAAC->outputFrame->data,
                    encodeAAC->outputFrame->nb_samples, indata, encodeAAC->outputFrame->nb_samples);
        encodeAAC->outputFrame;

        LOG_D(DEBUG, "data[0] line size:%d", encodeAAC->outputFrame->linesize[0]);
        LOG_D(DEBUG, "data[1] line size:%d", encodeAAC->outputFrame->linesize[0]);

        //PCM FLTP
//        fwrite(encodeAAC->outputFrame->data[0], encodeAAC->outputFrame->linesize[0], size,
//               encodeAAC->oAudioFile);
//        fwrite(encodeAAC->outputFrame->data[1], 1, encodeAAC->outputFrame->linesize[0],
//               encodeAAC->oAudioFile);
//        fflush(encodeAAC->oAudioFile);

        LOG_D(DEBUG, "nb_smpales:%d", encodeAAC->outputFrame->nb_samples);
        encodeAAC->outputFrame->pts = apts;
        apts += av_rescale_q(encodeAAC->outputFrame->nb_samples, {1, 48000},
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
        LOG_D(DEBUG, "aac encode size:%d", avPacket.size);
        fwrite(avPacket.data, 1, avPacket.size, tempFile);
        fflush(tempFile);
//
        LOG_D(DEBUG, "write file size:%d", avPacket.size);
        avPacket.pts = av_rescale_q(avPacket.pts, encodeAAC->avCodecContext->time_base,
                                    encodeAAC->avStream->time_base);
        avPacket.dts = av_rescale_q(avPacket.dts, encodeAAC->avCodecContext->time_base,
                                    encodeAAC->avStream->time_base);
        avPacket.duration = av_rescale_q(avPacket.duration, encodeAAC->avCodecContext->time_base,
                                         encodeAAC->avStream->time_base);

        ret = av_interleaved_write_frame(encodeAAC->avFormatContext, &avPacket);
//        av_packet_unref(&avPacket);
        if (ret == 0) {
            LOG_D(DEBUG, "av write frame success!");
        }
    }
    av_write_trailer(encodeAAC->avFormatContext);
    avio_close(encodeAAC->avFormatContext->pb);
    return 0;
}


void EncodeAAC::close() {
    exit = true;
}

int EncodeAAC::initEncoder() {
    //输入参数:
    input_arguments.audio_bit_rate = 40000;
    input_arguments.audio_sample_rate = 48000;//采样率
    input_arguments.avSampleFormat = AV_SAMPLE_FMT_S16;
    input_arguments.nb_samples = 1024;
    input_arguments.channels = av_get_channel_layout_nb_channels(AV_CH_LAYOUT_STEREO);
    input_arguments.channel_layout = av_get_default_channel_layout(input_arguments.channels);

    //输出参数:
    output_arguments.audio_sample_rate = 48000;//采样率
    output_arguments.audio_bit_rate = 40000;
    output_arguments.audio_codec_id = AV_CODEC_ID_AAC;
    output_arguments.channels = av_get_channel_layout_nb_channels(AV_CH_LAYOUT_STEREO);
    output_arguments.channel_layout = av_get_default_channel_layout(output_arguments.channels);
    output_arguments.avSampleFormat = AV_SAMPLE_FMT_FLTP;

    const char *url = "rtmp://192.168.24.132:1935/test/live";
    char *url_space = (char *) malloc(strlen(url) + 1);
    output_arguments.outputUrl = strcpy(url_space, url);
    av_register_all();
    avcodec_register_all();
    avformat_network_init();
    int ret = 0;

    ret = avformat_alloc_output_context2(&avFormatContext, NULL, "flv", output_arguments.outputUrl);
    if (ret < 0) {
        LOG_D(DEBUG, "avformat alloc output context2 failed");
        return -1;
    }

    aVcodec = avcodec_find_encoder(output_arguments.audio_codec_id);
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
    avCodecContext->sample_fmt = AV_SAMPLE_FMT_FLTP;
    avCodecContext->sample_rate = 48000;
    avCodecContext->bit_rate = 40000;
    avCodecContext->codec_id = aVcodec->id;
    avCodecContext->channel_layout = AV_CH_LAYOUT_STEREO;
    avCodecContext->channels = 2;
    avCodecContext->frame_size = 1024;
    avCodecContext->codec_type = AVMEDIA_TYPE_AUDIO;

    ret = avcodec_open2(avCodecContext, aVcodec, NULL);
    if (ret < 0) {
        char buf[1024] = {0};
        av_strerror(ret, buf, sizeof(buf));
        LOG_D(DEBUG, "avcodec_open2  audio failed  :%s", buf);
        return -1;
    }


    avStream = avformat_new_stream(avFormatContext, aVcodec);
    if (!avStream) {
        LOG_D(DEBUG, "avformat new stream error");
        return -1;
    }
    avcodec_parameters_from_context(avStream->codecpar, avCodecContext);
    avStream->codecpar->codec_tag = 0;

//    int buf_size = av_samples_get_buffer_size(NULL, avCodecContext->channels,
//                                              avCodecContext->frame_size,
//                                              avCodecContext->sample_fmt, 0);
//    frame_buf = (uint8_t *) av_malloc(buf_size);



    swrContext = swr_alloc_set_opts(swrContext, AV_CH_LAYOUT_STEREO,
                                    AV_SAMPLE_FMT_FLTP,
                                    48000,
                                    AV_CH_LAYOUT_STEREO,
                                    AV_SAMPLE_FMT_S16,
                                    48000, 0, NULL);

    ret = swr_init(swrContext);
    if (ret != 0) {
        LOG_D(DEBUG, "swr_init failed!");
        return -1;
    }

    outputFrame = av_frame_alloc();
    outputFrame->channels = 2;
    outputFrame->channel_layout = AV_CH_LAYOUT_STEREO;
    outputFrame->nb_samples = 1024;
    outputFrame->sample_rate = 48000;
    outputFrame->format = AV_SAMPLE_FMT_FLTP;

    ret = av_frame_get_buffer(outputFrame, 0);
    if (ret != 0) {
        LOG_D(DEBUG, "av_frame_get_buffer failed!");
        return -1;
    }

    size = av_samples_get_buffer_size(NULL, avCodecContext->channels, avCodecContext->frame_size,
                                      avCodecContext->sample_fmt, 1);
//    uint8_t *frame_buf = (uint8_t *) av_malloc(size);
//    avcodec_fill_audio_frame(outputFrame, avCodecContext->channels, avCodecContext->sample_fmt,
//                             (const uint8_t *) frame_buf, size, 1);


    ret = avio_open(&avFormatContext->pb, output_arguments.outputUrl, AVIO_FLAG_WRITE);
    if (ret < 0) {
        LOG_D(DEBUG, "avio_open failed");
        return -1;
    }

    ret = avformat_write_header(avFormatContext, NULL);
    if (ret < 0) {
        LOG_D(DEBUG, "avformat_write_header failed!");
        return -1;
    }
    pthread_t id1;
    pthread_create(&id1, NULL, EncodeAAC::startEncode, this);
    return 0;
}