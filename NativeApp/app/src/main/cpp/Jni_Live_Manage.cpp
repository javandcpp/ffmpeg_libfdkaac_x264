//
// Created by developer on 11/12/17.
//
/**
 * JNI 底层推流
 */
#include <jni.h>
#include "jni_Live_Manage.h"


#ifdef __cplusplus
extern "C" {
#endif

/**
 * 关闭底层采集、编码、推流
 */
JNIEXPORT jint JNICALL
Java_com_guagua_nativeapp_LiveManager_Close(JNIEnv *env, jclass type) {
    //关闭采集
    if (NULL != audioCapture) {
        audioCapture->CloseCapture();
    }
    if (NULL != videoCapture) {
        videoCapture->CloseCapture();
    }
    //关闭编码
    if (NULL != videoEncoder) {
        videoEncoder->CloseEncode();
    }
    if (NULL != audioEncoder) {
        audioEncoder->CloseEncode();
    }
    //关闭推流
    isClose = true;
    startStream = false;
    LOG_D(DEBUG, "jni close");
    return 0;
}

/**
 * 底层资源回收
 */
JNIEXPORT jint JNICALL
Java_com_guagua_nativeapp_LiveManager_Release(JNIEnv *env, jclass type) {
    isRelease = true;
    if (NULL != audioEncoder) {

    }
    if (NULL != videoEncoder) {
    }

    if (NULL != audioCapture) {
    }
    if (NULL != videoCapture) {
    }

    return 0;
}

/**
 * 接收APP层数据,AAC编码
 */
JNIEXPORT jint JNICALL
Java_com_guagua_nativeapp_LiveManager_EncodeAAC(JNIEnv *env, jclass type, jbyteArray audioBuffer_,
                                                jint length) {
    if (audioCaptureInit&&!isClose) {
        jbyte *audioSrc = env->GetByteArrayElements(audioBuffer_, 0);
        uint8_t *audioDstData = (uint8_t *) malloc(length);
        memcpy(audioDstData, audioSrc, length);
        OriginData *audioOriginData = new OriginData();
        audioOriginData->size = length;
        audioOriginData->data = audioDstData;
        audioCapture->PushAudioData(audioOriginData);
        env->ReleaseByteArrayElements(audioBuffer_, audioSrc, 0);
    }
    return 0;
}
/**
 * 接收APP层数据,H264编码
 */
JNIEXPORT jint JNICALL
Java_com_guagua_nativeapp_LiveManager_EncodeH264(JNIEnv *env, jclass type, jbyteArray videoBuffer_,
                                                 jint length, jint i) {
    if (videoCaptureInit&&!isClose) {
        jbyte *videoSrc = env->GetByteArrayElements(videoBuffer_, 0);
        uint8_t *videoDstData = (uint8_t *) malloc(length);
        memcpy(videoDstData, videoSrc, length);
        OriginData *videoOriginData = new OriginData();
        videoOriginData->size = length;
        videoOriginData->data = videoDstData;
        videoCapture->PushVideoData(videoOriginData);
        env->ReleaseByteArrayElements(videoBuffer_, videoSrc, 0);
    }
    return 0;
}


/**
 * 初始化音频编码器
 */
JNIEXPORT jint JNICALL
Java_com_guagua_nativeapp_LiveManager_InitAudioEncoder(JNIEnv *env, jclass type) {

    if (audioCaptureInit) {
        audioEncoder = AudioEncoder::Get();
        audioEncoder->SetAudioCapture(audioCapture);
        return audioEncoder->InitEncode();

    }
    LOG_D(DEBUG, "jni InitAudioEncoder failed!");
    return -1;
}
/**
 * 初始化视频编码器
 */
JNIEXPORT jint JNICALL
Java_com_guagua_nativeapp_LiveManager_InitVideoEncoder(JNIEnv *env, jclass type) {
    if (videoCaptureInit) {
        videoEncoder = VideoEncoder::Get();
        videoEncoder->SetVideoCapture(videoCapture);
        return videoEncoder->InitEncode();
    }
    LOG_D(DEBUG, "jni InitVideoEncoder failed!");
    return -1;
}

/**
 * 初始化采集
 */
JNIEXPORT jint JNICALL
Java_com_guagua_nativeapp_LiveManager_InitAudioCapture(JNIEnv *env, jclass type, jint channles,
                                                       jint SampleRate, jint SampleBitRate) {
    audioCapture = AudioCapture::Get();
    AudioEncodeArgs *audioEncodeArgs = (AudioEncodeArgs *) malloc(sizeof(AudioEncodeArgs));
    audioEncodeArgs->avSampleFormat = AV_SAMPLE_FMT_S16;
    audioEncodeArgs->sampleRate = SampleRate;
    audioEncodeArgs->bitRate = SampleBitRate;
    audioEncodeArgs->channels = channles;
    if (audioEncodeArgs->channels == 1) {
        audioEncodeArgs->ch_layout = AV_CH_LAYOUT_MONO;
    } else if (audioEncodeArgs->channels == 2) {
        audioEncodeArgs->ch_layout = AV_CH_LAYOUT_STEREO;
    }
    audioEncodeArgs->nb_samples = 1024;
    audioCapture->SetAudioEncodeArgs(audioEncodeArgs);
    audioCaptureInit = true;
    isRelease = false;
    return 0;
}

/**
 * 初始化采集
 */
JNIEXPORT jint JNICALL
Java_com_guagua_nativeapp_LiveManager_InitVideoCapture(JNIEnv *env, jclass type, jint inWidth,
                                                       jint inHeight, jint outWidth, jint outHeight,
                                                       jint fps, jboolean mirror) {
    videoCapture = VideoCapture::Get();
    VideoEncodeArgs *videoEncodeArgs = (VideoEncodeArgs *) malloc(sizeof(VideoEncodeArgs));
    videoEncodeArgs->fps = fps;
    videoEncodeArgs->in_height = inHeight;
    videoEncodeArgs->in_width = inWidth;
    videoEncodeArgs->out_height = outHeight;
    videoEncodeArgs->out_width = outWidth;
    videoEncodeArgs->mirror = mirror;
    videoCapture->SetVideoEncodeArgs(videoEncodeArgs);
    videoCaptureInit = true;
    isRelease = false;
    return 0;
}


/**
 * 开始推流
 */
JNIEXPORT jint JNICALL
Java_com_guagua_nativeapp_LiveManager_StartPush(JNIEnv *env, jclass type, jstring url_) {
    if (videoCaptureInit && audioCaptureInit) {
        startStream = true;
        isClose = false;
        videoCapture->StartCapture();
        audioCapture->StartCapture();
        const char *url = env->GetStringUTFChars(url_, 0);
        LOG_D(DEBUG, "stream URL:%s", url);
        if (NULL != videoEncoder) {
            videoEncoder->StartEncode();
        }
        if (NULL != audioEncoder) {
            audioEncoder->StartEncode();
        }
        env->ReleaseStringUTFChars(url_, url);
    }
    return 0;
}


/**
 * 添加视频滤镜
 */
JNIEXPORT jint JNICALL
Java_com_guagua_nativeapp_LiveManager_Filter(JNIEnv *env, jclass type, jobject filterType,
                                             jint filterValue) {

    // TODO
    return 0;
}

JNIEXPORT void JNICALL
Java_com_guagua_nativeapp_LiveManager_SetCameraID(JNIEnv *env, jclass type, jint cameraID) {
    if (videoCaptureInit && !isClose && !isRelease) {
        if (cameraID == CameraID::FRONT) {
            videoCapture->SetCameraID(CameraID::FRONT);
        } else if (cameraID == CameraID::BACK) {
            videoCapture->SetCameraID(CameraID::BACK);
        }
    }
    return;
}

#ifdef __cplusplus
}
#endif
