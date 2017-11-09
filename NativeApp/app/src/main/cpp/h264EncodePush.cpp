//
// Created by developer on 10/31/17.
//

#include "base_include.h"
#include "h264encodeandpush.h"
#include "aacencodeandpush.h"

using namespace libyuv;

#ifdef __cplusplus
extern "C" {
#endif

EncoderH264 *encoderH264 = NULL;
EncodeAAC *encodeAAC = NULL;
/**
 * video
 */
JNIEXPORT jint JNICALL
Java_com_guagua_nativeapp_MediaProcess_encodeH264(JNIEnv *env, jclass type, jbyteArray data_,
                                                  jint length, jint w, jint h, jint orientation) {
    jbyte *src = env->GetByteArrayElements(data_, NULL);
    //I420
    uint8_t *dstI420 = (uint8_t *) malloc(w * h * 3 / 2);
    //I420/mirror
    uint8_t *mirror = (uint8_t *) malloc(w * h * 3 / 2);
    //I420/mirror/rotate
    uint8_t *rotate = (uint8_t *) malloc(w * h * 3 / 2);

    //libyuv NV21 convert I420
    NV21ToI420((const uint8_t *) src, w, (uint8_t *) src + (w * h), w, dstI420, w,
               dstI420 + (w * h),
               w / 2, dstI420 + (w * h * 5 / 4), w / 2, w, h);

    uint8_t *src_y = dstI420;
    uint8_t *src_u = dstI420 + w * h;
    uint8_t *src_v = dstI420 + (w * h * 5 / 4);

    uint8_t *dst_y = mirror;
    uint8_t *dst_u = mirror + w * h;
    uint8_t *dst_v = mirror + (w * h * 5 / 4);

    RotationMode rotationMode = kRotate0;
    if (&encoderH264 != NULL) {
        if (orientation == 1) {
            //libyuv 前置镜像
            I420Mirror(src_y, w, src_u, w / 2, src_v, w / 2, dst_y,
                       w, dst_u, w / 2, dst_v, w / 2, w, h);
            //前置  注意:如果旋转90/270,旋转后,Y U V 行数据量,应该使用height计算
            rotationMode = kRotate90;
            I420Rotate(mirror, w, mirror + (w * h), w / 2, mirror + (w * h * 5 / 4), w / 2, rotate,
                       h, rotate + (w * h), h / 2, rotate + (w * h * 5 / 4), h / 2, w, h,
                       rotationMode);
        } else if (orientation == 0) {
            //后置  旋转90
            rotationMode = kRotate90;
            I420Rotate(src_y, w, src_u, w / 2, src_v, w / 2, rotate, h,
                       rotate + (w * h), h / 2, rotate + (w * h * 5 / 4), h / 2, w, h,
                       rotationMode);
        }

        fwrite(rotate, 1, w * h * 3 / 2, encoderH264->iVideoFile);
        fflush(encoderH264->iVideoFile);
        encoderH264->frame_queue.push(rotate);

        LOG_D(DEBUG, "input write file  w:%d,h:%d", w, h);
    }

    env->ReleaseByteArrayElements(data_, src, 0);
    return 0;
}


/**
 * audio
 */
JNIEXPORT jint JNICALL
Java_com_guagua_nativeapp_MediaProcess_encodeAAC(JNIEnv *env, jclass type, jbyteArray data_,
                                                 jint length) {
    if (encodeAAC != NULL) {
        jbyte *data = env->GetByteArrayElements(data_, NULL);
        uint8_t *buf = (uint8_t *) malloc(length);
        memcpy(buf, (uint8_t *) data, length);
        encodeAAC->frame_queue.push(buf);
        env->ReleaseByteArrayElements(data_, data, 0);
    }
//    fwrite(buf, 1, length, encodeAAC->iAudioFile);
//    fflush(encodeAAC->iAudioFile);
    return 0;
}


JNIEXPORT jint JNICALL
Java_com_guagua_nativeapp_MediaProcess_initEncoder(JNIEnv *env, jclass type) {

    int ret = 0;
    // TODO
//    encoderH264 = new EncoderH264();
    encodeAAC = new EncodeAAC();
//    encoderH264->iVideoFile = fopen("/mnt/sdcard/ivideo.yuv", "wb+");
//    encodeAAC->iAudioFile = fopen("/mnt/sdcard/iaudio.pcm", "wb+");
//    int ret = 0;
//    ret = encoderH264->initEncoder();
//    if (ret < 0) {
//        LOG_E(DEBUG, "initVideoEncoder failed!");
//        delete encoderH264;
//        return -1;
//    }

    ret = encodeAAC->initEncoder();
    if (ret < 0) {
        LOG_E(DEBUG, "initAudioEncoder failed!");
        delete encodeAAC;
        return ret;
    }
//    LOG_E(DEBUG, "initVideoEncoder success!");
    LOG_E(DEBUG, "initAudioEncoder success!");
    return ret;
}


JNIEXPORT void JNICALL
Java_com_guagua_nativeapp_MediaProcess_close(JNIEnv *env, jclass type) {
    if (encoderH264 != NULL) {
        if (encoderH264->oVideoFile) {
            fclose(encoderH264->oVideoFile);
        }
        if (encoderH264->iVideoFile) {
            fclose(encoderH264->iVideoFile);
        }
        encoderH264->close();
        delete encoderH264;
    }

    if (encodeAAC != NULL) {
        if (encodeAAC->oAudioFile) {
            fclose(encodeAAC->oAudioFile);
        }
        if (encodeAAC->iAudioFile) {
            fclose(encodeAAC->iAudioFile);
        }
        encodeAAC->close();
        delete encodeAAC;
    }

}


#ifdef __cplusplus
}
#endif