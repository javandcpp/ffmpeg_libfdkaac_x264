//
// Created by developer on 8/18/17.
//

#include "base_include.h"

AACEncode *aacEncoder;
H264Encode *h264Encoder;
#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jint JNICALL
Java_com_guagua_nativeapp_jnibridge_FFmpegJavaNativeBridge_encode2AAC(JNIEnv *env, jclass type,
                                                                      jbyteArray data_) {
    if (aacEncoder != NULL) {
        jbyte *elements = env->GetByteArrayElements(data_, 0);
        int i = aacEncoder->sendOneFrame((uint8_t *) elements);
        env->ReleaseByteArrayElements(data_, elements, 0);
        return i;
    }

    return -1;
}

JNIEXPORT jint JNICALL
Java_com_guagua_nativeapp_jnibridge_FFmpegJavaNativeBridge_prepareInitFFmpegEncode(JNIEnv *env,
                                                                                   jclass type,
                                                                                   jstring basePath_) {
    const char *basePath = env->GetStringUTFChars(basePath_, 0);
    UserArguments *userArguments = (UserArguments *) malloc(sizeof(UserArguments));


    const char *audioFileName = "audio.aac";
    const char *videoFileName = "video.h264";


    char *audioPath = (char *) malloc(strlen(basePath) + 1 + strlen(audioFileName) + 1);
    char *videoPath = (char *) malloc(strlen(basePath) + 1 + strlen(videoFileName) + 1);


    strcpy(audioPath, basePath);
    strcat(audioPath, "/");
    strcat(audioPath, audioFileName);

    strcpy(videoPath, basePath);
    strcat(videoPath, "/");
    strcat(videoPath, videoFileName);

//    strcpy(audioPath)

    userArguments->video_path = videoPath;
    userArguments->audio_path = audioPath;
    userArguments->video_bit_rate = 580000;
    userArguments->frame_rate = 20;


    userArguments->in_height = 1080;
    userArguments->in_width = 1920;
    userArguments->out_height = 1080;
    userArguments->out_width = 1920;

    userArguments->audio_bit_rate = 64000;
    userArguments->audio_sample_rate = 44100;
    userArguments->env = env;
    userArguments->java_class = type;
    userArguments->env->GetJavaVM(&userArguments->javaVM);
    aacEncoder = new AACEncode(userArguments);
    h264Encoder = new H264Encode(userArguments);

    int a_code = aacEncoder->initAudioEncoder();
    int v_code = h264Encoder->initVideoEncoder();

    return a_code & v_code;
}


JNIEXPORT jint JNICALL
Java_com_guagua_nativeapp_jnibridge_FFmpegJavaNativeBridge_prepareInitAACEncode(JNIEnv *env,
                                                                                jclass type,
                                                                                jstring path_,
                                                                                jstring fileName_) {
    const char *path = env->GetStringUTFChars(path_, 0);
    const char *fileName = env->GetStringUTFChars(fileName_, 0);

    UserArguments *userArguments = (UserArguments *) malloc(sizeof(UserArguments));
    userArguments->media_base_path = path;
    userArguments->media_name = fileName;
    char *audio_path_alloc = (char *) malloc(strlen(path) + 1);
    userArguments->audio_path = strcpy(audio_path_alloc, path);
    userArguments->audio_bit_rate = 64000;
    userArguments->audio_sample_rate = 44100;
    userArguments->env = env;
    userArguments->java_class = type;
    userArguments->env->GetJavaVM(&userArguments->javaVM);
    aacEncoder = new AACEncode(userArguments);
    int a_code = aacEncoder->initAudioEncoder();

    if (a_code == 0) {
        return 0;
    } else {
        return -1;
    }
}


JNIEXPORT jint JNICALL
Java_com_guagua_nativeapp_jnibridge_FFmpegJavaNativeBridge_endRecord(JNIEnv *env, jclass type) {


    if (h264Encoder != NULL) {
        h264Encoder->user_end();
        h264Encoder = NULL;
    }
    // TODO
    if (aacEncoder != NULL) {
        aacEncoder->user_end();
        aacEncoder = NULL;
    }
    return 0;
}


JNIEXPORT jint JNICALL
Java_com_guagua_nativeapp_jnibridge_FFmpegJavaNativeBridge_releaseRecord(JNIEnv *env,
                                                                         jclass type) {

    if (NULL != h264Encoder) {
        h264Encoder->release();
        h264Encoder = NULL;
    }
    if (NULL != aacEncoder) {
        aacEncoder->release();
        aacEncoder = NULL;
    }

    return 0;
    // TODO

}

JNIEXPORT jint JNICALL
Java_com_guagua_nativeapp_jnibridge_FFmpegJavaNativeBridge_encodeFrame2H264(JNIEnv *env,
                                                                            jclass type,
                                                                            jbyteArray data_) {
    if (NULL != h264Encoder) {
        jbyte *data = env->GetByteArrayElements(data_, NULL);
        int i = h264Encoder->sendOneFrame((uint8_t *) data);
        env->ReleaseByteArrayElements(data_, data, 0);
        return i;
    }
    // TODO


    return 0;
}


JNIEXPORT jint JNICALL
Java_com_guagua_nativeapp_jnibridge_FFmpegJavaNativeBridge_yuv420Image2YUV(JNIEnv *env, jclass type,
                                                                           jstring path_, jint w,
                                                                           jint h) {
    const char *path = env->GetStringUTFChars(path_, 0);

    FILE *fHandle = fopen(path, "wb+");
    FILE *y = fopen("/mnt/sdcard/image.y", "rb");
    FILE *u = fopen("/mnt/sdcard/image.u", "rb");
    FILE *v = fopen("/mnt/sdcard/image.v", "rb");

    if (fHandle == NULL) {
        LOG_E(DEBUG, "file open error!");
        return -1;
    }
//合并YUV
//    unsigned char *bufferY = (unsigned char *) malloc(w * h * 3 / 2);
//    fread(bufferY, 1, w * h, y);
//    fread(bufferY + w * h, 1, w * h / 4, u);
//    fread(bufferY + w * h * 5 / 4, 1, w * h / 4, v);
//
//    fwrite(bufferY, 1, w * h * 3 / 2, fHandle);

    //提取YUV
    unsigned char *buffer = (unsigned char *) malloc(w * h * 3 / 2);
    fread(buffer, 1, w * h * 3 / 2, fHandle);
    fwrite(buffer, 1, w * h, y);
    fwrite(buffer + w * h, 1, w * h / 4, u);//write u
    fwrite(buffer + w * h * 5 / 4, 1, w * h / 4, v);//write v

    /**
     * y=w*h;
     * u=w*h/4;
     * v=w*h/4;
     *
     *
     * total size=y+u+v=w*h*3/2;
     *
     *
     *
     *
     *
     *
     *
     *
     */


    fclose(y);
    fclose(u);
    fclose(v);
    fclose(fHandle);
    return 0;
}

#ifdef __cplusplus
}
#endif

