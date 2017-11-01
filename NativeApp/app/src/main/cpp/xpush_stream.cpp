//
// Created by developer on 10/26/17.
//
#ifdef __cplusplus
 extern "C" {
#endif


#include "include/libavcodec/avcodec.h"
#include "include/libavformat/avformat.h"
#include "include/libavcodec/avcodec.h"
#include "include/libavutil/opt.h"
#include "include/libavutil/frame.h"
#include "include/libavutil/time.h"
#include <jni.h>
#include <android/log.h>
#include "debug.h"


void endPush(AVFormatContext *inAFCtx, AVFormatContext *outAFCtx);

int ErrorPrint(int errorNum, AVFormatContext *inAFCtx, AVFormatContext *outAFCtx) {
    char buf[1024] = {0};
    av_strerror(errorNum, buf, sizeof(buf));
    LOG_E(DEBUG, "error:%s", buf);
    endPush(inAFCtx, outAFCtx);
    return -1;
}

double av_avr2double(AVRational rational) {
    return rational.num == 0 || rational.den == 0 ? 0 : (double) rational.num /
                                                        (double) rational.den;
}


int av_avr2int(AVRational rational) {
    return rational.num == 0 || rational.den == 0 ? 0 : rational.num /
                                                        rational.den;
}

void endPushStream(AVFormatContext *inAFCtx, AVFormatContext *outAFCtx) {
    if (inAFCtx != NULL) {
        avformat_close_input(&inAFCtx);
        avformat_free_context(inAFCtx);
        inAFCtx = NULL;
    }
    if (outAFCtx != NULL) {
        if (outAFCtx->pb) {
            avio_close(outAFCtx->pb);
        }
        avformat_free_context(outAFCtx);
        outAFCtx = NULL;
    }
}

jint JNICALL
Java_com_guagua_nativeapp_jnibridge_FFmpegJavaNativeBridge_xpushstream(JNIEnv *env,
                                                                       jclass type,
                                                                       jstring inputUri_,
                                                                       jstring outputUri_) {
    const char *inputPath = env->GetStringUTFChars(inputUri_, 0);
    const char *outputUri = env->GetStringUTFChars(outputUri_, 0);

    AVFormatContext *inAFCtx = NULL;
    AVFormatContext *outAFCtx = NULL;
    int ret, videoStreamIndex;
    int fps;
    av_register_all();
    avformat_network_init();
    if ((ret = avformat_open_input(&inAFCtx, inputPath, NULL, NULL)) < 0) {
        return ErrorPrint(ret, inAFCtx, outAFCtx);
    }

    LOG_D(DEBUG, "open input");
    if ((ret = avformat_find_stream_info(inAFCtx, NULL)) < 0) {
        return ErrorPrint(ret, inAFCtx, outAFCtx);
    }
    LOG_D(DEBUG, "find stream info");

    av_dump_format(inAFCtx, 0, inputPath, 0);

    if ((ret = avformat_alloc_output_context2(&outAFCtx, NULL, "flv", outputUri)) < 0) {
        return ErrorPrint(ret, inAFCtx, outAFCtx);
    }


    for (int i = 0; i < inAFCtx->nb_streams; i++) {

        AVStream *inStream = inAFCtx->streams[i];
        AVStream *outStream = avformat_new_stream(outAFCtx, (const AVCodec *) inStream->codec);
        ret = avcodec_copy_context(outStream->codec, inStream->codec);

        if (inStream->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoStreamIndex = i;
            fps = av_avr2int(inAFCtx->streams[i]->avg_frame_rate);
        }
        outStream->codec->codec_tag = 0;
        if (ret < 0) {
            return ErrorPrint(ret, inAFCtx, outAFCtx);
        }
    }

    if ((ret = avio_open(&outAFCtx->pb, outputUri, AVIO_FLAG_WRITE)) < 0) {
        return ErrorPrint(ret, inAFCtx, outAFCtx);
    }

    if ((ret = avformat_write_header(outAFCtx, NULL)) < 0) {
        return ErrorPrint(ret, inAFCtx, outAFCtx);
    }

    AVPacket pkt;
    int64_t startTime = av_gettime();
    for (; ;) {
        ret = av_read_frame(inAFCtx, &pkt);
        if (ret != 0) {
            avio_close(outAFCtx->pb);
            break;
        }

        AVRational inTimeBase = inAFCtx->streams[pkt.stream_index]->time_base;
        AVRational outTimeBase = outAFCtx->streams[pkt.stream_index]->time_base;

        LOG_D(DEBUG, "--------------timebase-----------\n"
                "--------------int timebase:%d/%d-----------\n"
                "--------------out timebase:%d/%d-----------\n", inTimeBase.num, inTimeBase.den,
              outTimeBase.num, outTimeBase.den);
        LOG_D(DEBUG, "in r2d:%lf", av_avr2double(inTimeBase));
        LOG_D(DEBUG, "out r2d:%lf", av_avr2double(outTimeBase));
        LOG_D(DEBUG, "---------------before rescale timebase------------------");
        LOG_D(DEBUG, "---------------in pts(秒):%lld   dts(秒):%lld-------------------",
              (int64_t) (pkt.pts * av_avr2double(inTimeBase)),
              (int64_t) (pkt.dts * av_avr2double(inTimeBase)));


        //delay
        if (pkt.stream_index == videoStreamIndex) {
            int64_t hostTime = av_gettime() - startTime;
            int64_t nowTime = (int64_t) (pkt.dts * av_avr2double(inTimeBase) * 1000 * 1000);
            LOG_D(DEBUG, "---------------in nowTime(微秒):%lld   dts(微秒):%lld-------------------",
                  nowTime, hostTime);

            if (nowTime > hostTime) {
                av_usleep(1000 / 15);
            }
        }
        LOG_D(DEBUG, "---------------after rescale timebase------------------");
        pkt.dts = av_rescale_q(pkt.dts, inTimeBase, outTimeBase);
        pkt.pts = av_rescale_q(pkt.pts, inTimeBase, outTimeBase);
        pkt.duration = av_rescale_q(pkt.duration, inTimeBase, outTimeBase);
        pkt.pos = -1;
        LOG_D(DEBUG, "---------------in pts(秒):%lld  pts(微秒):%lld-------------------",
              (int64_t) (pkt.pts * av_avr2double(outTimeBase)),
              (int64_t) (pkt.dts * av_avr2double(outTimeBase)));

        LOG_D(DEBUG, "outPts:%lld", pkt.pts);
        LOG_D(DEBUG, "outDts:%lld", pkt.dts);

        if ((ret = av_interleaved_write_frame(outAFCtx, &pkt)) < 0) {
            LOG_D(DEBUG, "writer frame failed!");
        } else {
            LOG_D(DEBUG, "writer frame success!");
        }
        av_packet_unref(&pkt);
    }
    av_free_packet(&pkt);
    av_write_trailer(outAFCtx);
    endPushStream(inAFCtx, outAFCtx);

    return 0;


}

#ifdef  __cplusplus
}
#endif
