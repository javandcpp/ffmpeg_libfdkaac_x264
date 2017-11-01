//
// Created by developer on 10/31/17.
//

#include "base_include.h"
#include "h264encodeandpush.h"

#ifdef __cplusplus
extern "C" {
#endif

EncoderH264 *encoderH264;

//
void detailPic90(uint8_t *d, uint8_t *yuv_temp, int nw, int nh, int w, int h) {
    int deleteW = (nw - h) / 2;
    int deleteH = (nh - w) / 2;
    int i, j;
    for (i = 0; i < h; i++) {
        for (j = 0; j < w; j++) {
            yuv_temp[(h - i) * w - 1 - j] = d[nw * (deleteH + j) + nw - deleteW
                                              - i];
        }
    }
    int index = w * h;
    for (i = deleteW + 1; i < nw - deleteW; i += 2)
        for (j = nh / 2 * 3 - deleteH / 2; j > nh + deleteH / 2; j--)
            yuv_temp[index++] = d[(j - 1) * nw + i];
    for (i = deleteW; i < nw - deleteW; i += 2)
        for (j = nh / 2 * 3 - deleteH / 2; j > nh + deleteH / 2; j--)
            yuv_temp[index++] = d[(j - 1) * nw + i];
}


void detailPic270(uint8_t *d, uint8_t *yuv_temp, int nw, int nh, int w, int h) {
    int deleteW = (nw - h) / 2;
    int deleteH = (nh - w) / 2;
    int i, j;
//处理y 旋转加裁剪
    for (i = 0; i < h; i++) {
        for (j = 0; j < w; j++) {
            yuv_temp[i * w + j] = d[nw * (deleteH + j) + nw - deleteW - i];
        }
    }
//处理u 旋转裁剪加格式转换
    int index = w * h;
    for (i = nw - deleteW - 1; i > deleteW; i -= 2)
        for (j = nh + deleteH / 2; j < nh / 2 * 3 - deleteH / 2; j++)
            yuv_temp[index++] = d[(j) * nw + i];
//处理v 旋转裁剪加格式转换
    for (i = nw - deleteW - 2; i >= deleteW; i -= 2)
        for (j = nh + deleteH / 2; j < nh / 2 * 3 - deleteH / 2; j++)
            yuv_temp[index++] = d[(j) * nw + i];
}

//mirro
void Mirror(uint8_t *yuv_temp, int nw, int nh, int w,
            int h) {
    int deleteW = (nw - h) / 2;
    int deleteH = (nh - w) / 2;
    int i, j;
    int a, b;
    uint8_t temp;
//mirror y
    for (i = 0; i < h; i++) {
        a = i * w;
        b = (i + 1) * w - 1;
        while (a < b) {
            temp = yuv_temp[a];
            yuv_temp[a] = yuv_temp[b];
            yuv_temp[b] = temp;
            a++;
            b--;
        }
    }
//mirror u
    int uindex = w * h;
    for (i = 0; i < h / 2; i++) {
        a = i * w / 2;
        b = (i + 1) * w / 2 - 1;
        while (a < b) {
            temp = yuv_temp[a + uindex];
            yuv_temp[a + uindex] = yuv_temp[b + uindex];
            yuv_temp[b + uindex] = temp;
            a++;
            b--;
        }
    }
//mirror v
    uindex = w * h / 4 * 5;
    for (i = 0; i < h / 2; i++) {
        a = i * w / 2;
        b = (i + 1) * w / 2 - 1;
        while (a < b) {
            temp = yuv_temp[a + uindex];
            yuv_temp[a + uindex] = yuv_temp[b + uindex];
            yuv_temp[b + uindex] = temp;
            a++;
            b--;
        }
    }
}


JNIEXPORT jint JNICALL
Java_com_guagua_nativeapp_MediaProcess_encodeH264(JNIEnv *env, jclass type, jbyteArray data_,
                                                  jint length, jint w, jint h, jint orientation) {
    jbyte *data = env->GetByteArrayElements(data_, NULL);

    uint8_t *buf = (uint8_t *) malloc(w * h * 3 / 2);

    if (&encoderH264 != NULL) {
        if (orientation == 1) {//前置  旋转270/镜像
            detailPic270((uint8_t *) data, buf, w, h, h, w);
            Mirror(buf, w, h, h, w);
        } else if (orientation == 0) {//后置  旋转90
            detailPic90((uint8_t *) data, buf, w, h, h, w);
        }

//        fwrite(buf, 1, w * h * 3 / 2, encoderH264->iVideoFile);
//        fflush(encoderH264->iVideoFile);
//
        encoderH264->frame_queue.push(buf);
//        LOG_D(DEBUG, "input write file  w:%d,h:%d",w,h);
    }

//    env->ReleaseByteArrayElements(data_, data, 0);
    return 0;
}


JNIEXPORT jint JNICALL
Java_com_guagua_nativeapp_MediaProcess_initEncoder(JNIEnv *env, jclass type) {

    // TODO
    encoderH264 = new EncoderH264();
    encoderH264->iVideoFile = fopen("/mnt/sdcard/ivideo.yuv", "wb+");
    int ret = encoderH264->initEncoder();
    if (ret < 0) {
        LOG_E(DEBUG, "initEncoder failed!");
        return -1;
    }
    LOG_E(DEBUG, "initEncoder success!");
    return 0;
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

}


#ifdef __cplusplus
}
#endif