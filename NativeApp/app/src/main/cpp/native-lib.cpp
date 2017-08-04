extern "C"{
#include "android/log.h"
#include <jni.h>
#include <string.h>
#include <stdio.h>
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/imgutils.h"
#include "libswscale/swscale.h"
#define LOG_TAG "native"
#define LOG_D(...)   __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG, __VA_ARGS__)


JNIEXPORT jstring JNICALL
Java_com_guagua_nativeapp_MainActivity_nativeString(JNIEnv *evn, jobject obj) {
    avcodec_register_all();
    const char *string = avcodec_configuration();
    char *info = new char[strlen(string)];
    sprintf(info, "%s\n", string);
    return evn->NewStringUTF(info);
}

JNIEXPORT jstring JNICALL
Java_com_guagua_nativeapp_MainActivity_nativeGetVideoInfo(JNIEnv *env, jobject instance,
                                                          jstring path_) {
    const char *path = env->GetStringUTFChars(path_, 0);
    AVFormatContext *pFormatCtx;
    int             i, videoindex;
    AVCodecContext  *pCodecCtx;
    AVCodec         *pCodec;
    AVCodec         *pEncoder;
    AVFrame *pFrame,*pFrameYUV;
    unsigned char *out_buffer;
    AVPacket *packet;
    int y_size;
    int ret, got_picture;
    struct SwsContext *img_convert_ctx;
    int screen_w=0,screen_h=0;
    FILE *fp_yuv;

    av_register_all();
    avformat_network_init();
    pFormatCtx = avformat_alloc_context();


    if(avformat_open_input(&pFormatCtx,path,NULL,NULL)!=0){

        LOG_D("native","-------------open file failed!-----------");
        return env->NewStringUTF("Couldn't open input stream");
    }
    if(avformat_find_stream_info(pFormatCtx,NULL)<0){
        LOG_D("native","can't find stream info");
        return env->NewStringUTF("Couldn't find stream information.");
    }
    videoindex=-1;
    for(i=0; i<pFormatCtx->nb_streams; i++)
        if(pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO){
            videoindex=i;
            break;
        }
    if(videoindex==-1){
        LOG_D("Didn't find a video stream.\n");
        return env->NewStringUTF("Didn't find a video stream");
    }

    pCodecCtx=pFormatCtx->streams[videoindex]->codec;
    pEncoder=avcodec_find_encoder(pCodecCtx->codec_id);
    pCodec=avcodec_find_decoder(pCodecCtx->codec_id);
    if(pCodec==NULL){
        LOG_D("Codec not found.\n");
        return env->NewStringUTF("Codec not found");
    }else{
        LOG_D("encoder :%s",pEncoder->name);
    }

    if(pEncoder!=NULL){
        LOG_D("decoder:%s",pEncoder->name);

    }

    int64_t duration=pFormatCtx->duration/1000/1000;

    LOG_D("duration时长:%lld(秒)",duration);

    LOG_D("文件名:","%s",pFormatCtx->filename);
    LOG_D("bitrate码率:%lld",pFormatCtx->bit_rate);


    if(avcodec_open2(pCodecCtx, pCodec,NULL)<0){
        printf("Could not open codec.\n");
        LOG_D("native","could not open codec");
        return env->NewStringUTF("Could not open codec");
    }


    pFrame=av_frame_alloc();
    pFrameYUV=av_frame_alloc();
    out_buffer=(unsigned char *)av_malloc(av_image_get_buffer_size(AV_PIX_FMT_YUV420P,  pCodecCtx->width, pCodecCtx->height,1));
    av_image_fill_arrays(pFrameYUV->data, pFrameYUV->linesize,out_buffer,
                         AV_PIX_FMT_YUV420P,pCodecCtx->width, pCodecCtx->height,1);

    packet=(AVPacket *)av_malloc(sizeof(AVPacket));
    //Output Info-----------------------------
    LOG_D("native","--------------- File Information ----------------\n");
    av_dump_format(pFormatCtx,0,path,0);
    LOG_D("native","-------------------------------------------------\n");
    img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt,
                                     pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);




    avformat_close_input(&pFormatCtx);

    return env->NewStringUTF(path);
}

JNIEXPORT jstring JNICALL
Java_com_guagua_nativeapp_MainActivity_getStringUTF(JNIEnv *env, jobject instance) {

    return env->NewStringUTF("1111122222");


}
}