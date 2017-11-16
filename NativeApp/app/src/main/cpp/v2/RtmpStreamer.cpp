//
// Created by developer on 11/14/17.
//

#include "RtmpStreamer.h"

RtmpStreamer::RtmpStreamer() {

}

RtmpStreamer::~RtmpStreamer() {
    if (NULL != audioStream) {
        av_free(audioStream);
        audioStream = NULL;
    }
    if (NULL != videoStream) {
        av_free(videoStream);
        videoStream = NULL;
    }
    if (NULL != audioCodecContext) {
        avcodec_free_context(&audioCodecContext);
        audioCodecContext = NULL;
    }
    if (NULL != videoCodecContext) {
        avcodec_free_context(&videoCodecContext);
        audioCodecContext = NULL;
    }
    if (NULL != iAvFormatContext) {
        avformat_free_context(iAvFormatContext);
        iAvFormatContext = NULL;
    }
}

RtmpStreamer *RtmpStreamer::Get() {
    static RtmpStreamer rtmpStreamer;
    return &rtmpStreamer;
}

int RtmpStreamer::InitStreamer(const char *url) {
    std::lock_guard<std::mutex> lk(mut);
    this->outputUrl = url;
    int ret = 0;
    ret = avformat_alloc_output_context2(&iAvFormatContext, NULL, "flv", outputUrl);
    if (ret < 0) {
        char buf[1024] = {0};
        av_strerror(ret, buf, sizeof(buf));
        LOG_D(DEBUG, "avformat alloc output context2 failed: %s", buf);

        return -1;
    }
    LOG_D(DEBUG, "Rtmp InitStreamer Success!");
    return 0;
}

int RtmpStreamer::SetAudioEncoder(AudioEncoder *audioEncoder) {
    this->audioEncoder = audioEncoder;
    return this->audioEncoder != NULL ? 0 : -1;
}

int RtmpStreamer::SetVideoEncoder(VideoEncoder *videoEncoder) {
    this->videoEncoder = videoEncoder;
    return this->videoEncoder != NULL ? 0 : -1;
}

int RtmpStreamer::AddStream(AVCodecContext *avCodecContext) {
    std::lock_guard<std::mutex> lk(mut);
    AVStream *pStream = avformat_new_stream(iAvFormatContext, avCodecContext->codec);
    if (!pStream) {
        LOG_D(DEBUG, "avformat_new_stream failed!");
        return -1;
    }
    LOG_D(DEBUG, "avformat new stream success!");
    int ret = 0;
    ret = avcodec_parameters_from_context(pStream->codecpar, avCodecContext);
    if (ret < 0) {
        char buf[1024] = {0};
        av_strerror(ret, buf, sizeof(buf));
        LOG_D(DEBUG, "avcodec_parameters_from_context failed :%s", buf);
        return -1;
    }
    LOG_D(DEBUG, "avcodec_parameters_from_context success!");
    pStream->codecpar->codec_tag = 0;
    if (avCodecContext->codec_type == AVMEDIA_TYPE_VIDEO) {
        LOG_D(DEBUG, "Add video stream success!");
        videoStream = pStream;
        videoCodecContext = avCodecContext;
    } else if (avCodecContext->codec_type == AVMEDIA_TYPE_AUDIO) {
        LOG_D(DEBUG, "Add audio stream success!");
        audioStream = pStream;
        audioCodecContext = avCodecContext;
    }
    return pStream->index;
}


int RtmpStreamer::SendAudioFrame(OriginData *originData, int streamIndex) {
    std::lock_guard<std::mutex> lk(mut1);
    AVRational stime;
    AVRational dtime;

    AVPacket *packet = originData->avPacket;
    packet->stream_index = streamIndex;
#ifdef SHOW_DEBUG_INFO
    LOG_D(DEBUG, "packet index:%d    index:%d", packet->stream_index, streamIndex);
#endif
    stime = audioCodecContext->time_base;
    dtime = audioStream->time_base;
    //push
    packet->pts = av_rescale_q(packet->pts, stime, dtime);
    packet->dts = av_rescale_q(packet->dts, stime, dtime);
    packet->duration = av_rescale_q(packet->duration, stime, dtime);
#ifdef SHOW_DEBUG_INFO
    LOG_D(DEBUG, "writer frame stream Index:%d   size:%d",
          packet->stream_index,
          packet->size);
#endif
    int ret = av_interleaved_write_frame(iAvFormatContext, packet);
    if (ret == 0) {
        LOG_D(DEBUG, "write ++++++++++++++audio frame sucess!");
    } else {
        char buf[1024] = {0};
        av_strerror(ret, buf, sizeof(buf));
        LOG_D(DEBUG, "writer******************* audio frame failed! :%s", buf);
    }
    delete originData;
    return 0;

}

int RtmpStreamer::SendVideoFrame(OriginData *originData, int streamIndex) {
    std::lock_guard<std::mutex> lk(mut1);
    AVRational stime;
    AVRational dtime;

    AVPacket *packet = originData->avPacket;
    packet->stream_index = streamIndex;
#ifdef SHOW_DEBUG_INFO
    LOG_D(DEBUG, "video packet index:%d    index:%d", packet->stream_index, streamIndex);
#endif
    stime = videoCodecContext->time_base;
    dtime = videoStream->time_base;
    packet->pts = originData->pts;
    packet->dts = packet->pts;
    packet->pts = av_rescale_q_rnd(packet->pts, stime, dtime,
                                   (AVRounding) (AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
    packet->dts = av_rescale_q_rnd(packet->dts, stime, dtime,
                                   (AVRounding) (AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
    packet->duration = av_rescale_q(packet->duration, stime, dtime);
#ifdef SHOW_DEBUG_INFO
    LOG_D(DEBUG, "writer frame stream Index:%d   size:%d",
          packet->stream_index,
          packet->size);

#endif
    int ret = av_interleaved_write_frame(iAvFormatContext, packet);
    if (ret == 0) {
        LOG_D(DEBUG, "write ------------------video frame success!");
    } else {
        char buf[1024] = {0};
        av_strerror(ret, buf, sizeof(buf));
        LOG_D(DEBUG, "writer*******************video frame failed! :%s", buf);
    }
    delete originData;
    return 0;

}


/**
 * 音频推流任务
 */
void *RtmpStreamer::PushAudioStreamTask(void *pObj) {
    RtmpStreamer *rtmpStreamer = (RtmpStreamer *) pObj;
    rtmpStreamer->isPushStream = true;
    while (true) {
        if (!rtmpStreamer->isPushStream ||
            rtmpStreamer->audioEncoder->GetAudioCapture()->GetCaptureState()) {
            break;
        }
        if (rtmpStreamer->audioEncoder->aframeQueue.empty()) {
            continue;
        }

        OriginData *audioOriginData = *rtmpStreamer->audioEncoder->aframeQueue.wait_and_pop().get();

        if (audioOriginData->avPacket->size > 0) {
            rtmpStreamer->SendAudioFrame(audioOriginData, rtmpStreamer->audioStreamIndex);
        }
    }
    return 0;
}

/**
* 音频推流任务
*/
void *RtmpStreamer::PushVideoStreamTask(void *pObj) {
    RtmpStreamer *rtmpStreamer = (RtmpStreamer *) pObj;
    rtmpStreamer->isPushStream = true;
    while (true) {
        if (!rtmpStreamer->isPushStream ||
            rtmpStreamer->videoEncoder->GetVideoCapture()->GetCaptureState()) {
            break;
        }
        if (rtmpStreamer->videoEncoder->vframeQueue.empty()) {
            continue;
        }
        OriginData *videoOriginData = *rtmpStreamer->videoEncoder->vframeQueue.wait_and_pop().get();
        if (videoOriginData->avPacket->size > 0) {
            rtmpStreamer->SendVideoFrame(videoOriginData, rtmpStreamer->videoStreamIndex);
        }
    }
    return 0;
}

int RtmpStreamer::StartPushStream() {
    videoStreamIndex = AddStream(videoEncoder->videoCodecContext);
    audioStreamIndex = AddStream(audioEncoder->audioCodecContext);
    pthread_create(&t3, NULL, RtmpStreamer::WriteHead, this);
    pthread_join(t3, NULL);
    pthread_create(&t1, NULL, RtmpStreamer::PushAudioStreamTask, this);
    pthread_create(&t2, NULL, RtmpStreamer::PushVideoStreamTask, this);
    return 0;
}

int RtmpStreamer::ClosePushStream() {
    if (isPushStream) {
        isPushStream = false;
        pthread_join(t1, NULL);
        pthread_join(t2, NULL);
        if (NULL != iAvFormatContext) {
            av_write_trailer(iAvFormatContext);
            avio_close(iAvFormatContext->pb);
        }
    }
    return 0;
}

/**
 * notice:AVStream创建完成开始写头信息
 */
void *RtmpStreamer::WriteHead(void *pObj) {
    RtmpStreamer *rtmpStreamer = (RtmpStreamer *) pObj;
    int ret = 0;
    ret = avio_open(&rtmpStreamer->iAvFormatContext->pb, rtmpStreamer->outputUrl, AVIO_FLAG_WRITE);
    if (ret < 0) {
        char buf[1024] = {0};
        av_strerror(ret, buf, sizeof(buf));
        LOG_D(DEBUG, "avio open failed: %s", buf);
        return 0;
    }
    LOG_D(DEBUG, "avio open success!");
    ret = avformat_write_header(rtmpStreamer->iAvFormatContext, NULL);
    if (ret != 0) {
        char buf[1024] = {0};
        av_strerror(ret, buf, sizeof(buf));
        LOG_D(DEBUG, "avformat write header failed!: %s", buf);
        return 0;
    }
    return 0;
}

int RtmpStreamer::SendFrame(OriginData *pData, int streamIndex) {
    std::lock_guard<std::mutex> lk(mut);
    AVRational stime;
    AVRational dtime;
    AVPacket *packet = pData->avPacket;
    packet->stream_index = streamIndex;
    LOG_D(DEBUG, "video packet index:%d    index:%d", packet->stream_index, streamIndex);
    //判断是音频还是视频
    if (packet->stream_index == videoStreamIndex) {
        stime = videoCodecContext->time_base;
        dtime = videoStream->time_base;
    }
    else if (packet->stream_index == audioStreamIndex) {
        stime = audioCodecContext->time_base;
        dtime = audioStream->time_base;
    }
    else {
        LOG_D(DEBUG, "unknow stream index");
        return -1;
    }
    packet->pts = av_rescale_q(packet->pts, stime, dtime);
    packet->dts = av_rescale_q(packet->dts, stime, dtime);
    packet->duration = av_rescale_q(packet->duration, stime, dtime);
    int ret = av_interleaved_write_frame(iAvFormatContext, packet);

    if (ret == 0) {
        if (streamIndex == audioStreamIndex) {
            LOG_D(DEBUG, "---------->write @@@@@@@@@ frame success------->!");
        } else if (streamIndex == videoStreamIndex) {
            LOG_D(DEBUG, "---------->write ######### frame success------->!");
        }
    } else {
        char buf[1024] = {0};
        av_strerror(ret, buf, sizeof(buf));
        LOG_D(DEBUG, "stream index %d writer frame failed! :%s", streamIndex, buf);
    }
    return 0;
}





