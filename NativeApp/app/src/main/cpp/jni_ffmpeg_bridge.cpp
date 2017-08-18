//
// Created by developer on 8/18/17.
//

#include "base_include.h"

AACEncode *aacEncoder;
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
Java_com_guagua_nativeapp_jnibridge_FFmpegJavaNativeBridge_endAACEncode(JNIEnv *env, jclass type) {

    // TODO
    if (aacEncoder != NULL) {
        aacEncoder->user_end();
        aacEncoder=NULL;
    }
}


JNIEXPORT jint JNICALL
Java_com_guagua_nativeapp_jnibridge_FFmpegJavaNativeBridge_releaseAACEncode(JNIEnv *env,
                                                                            jclass type) {
    if (aacEncoder != NULL) {
        aacEncoder->release();
        aacEncoder=NULL;
    }

    // TODO

}

#ifdef __cplusplus
}
#endif

