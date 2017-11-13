//
// Created by developer on 11/12/17.
//

#include "VideoCapture.h"
using namespace libyuv;

VideoCapture::VideoCapture() {

}

VideoCapture::~VideoCapture() {
    if (NULL != videoEncodeArgs) {
        delete videoEncodeArgs;
    }
}

VideoCapture *VideoCapture::Get() {
    static VideoCapture videoCapture;
    return &videoCapture;
}

void VideoCapture::SetCameraID(CameraID cameraID) {
    this->mCameraId = cameraID;
}

CameraID VideoCapture::GetCameraID() {
    return this->mCameraId;
}


int VideoCapture::Release() {

    LOG_D(DEBUG, "Release Video Capture!");
    return 0;
}

/**
 * 关闭采集数据输入
 */
bool VideoCapture::CloseCapture() {
    mMutex.lock();
    ExitCapture = true;
    mMutex.unlock();
    LOG_D(DEBUG, "Close Video Capture");
    return ExitCapture;
}


bool VideoCapture::StartCapture() {
    mMutex.lock();
    ExitCapture = false;
    mMutex.unlock();
    LOG_D(DEBUG, "Start Video Capture");
    return !ExitCapture;
}

/**
 * 往队列中添加视频原数据
 */
int VideoCapture::PushVideoData(OriginData *originData) {
    if (ExitCapture)
        return 0;
    originData->pts = av_gettime();
//    LOG_D(DEBUG,"video pts:%lld  , data size:%d",originData->pts,originData->size);
    frame_queue.push(originData);
    return originData->size;
}

void VideoCapture::SetVideoEncodeArgs(VideoEncodeArgs *videoEncodeArgs) {
    this->videoEncodeArgs = videoEncodeArgs;
}

VideoEncodeArgs* VideoCapture::GetVideoEncodeArgs() {
    return this->videoEncodeArgs;
}

/**
 *从队列中获取视频原数据
 */
OriginData *VideoCapture::GetVideoData() {
    if (ExitCapture)
        return NULL;
    return frame_queue.empty() ? NULL : *(frame_queue.wait_and_pop().get());
}


bool VideoCapture::GetCaptureState() {
    return ExitCapture;
}

void VideoCapture::NV21ProcessYUV420P(int w, int h, uint8_t *src, uint8_t *dst) {
    //I420
//    uint8_t *dstI420 = (uint8_t *) malloc(w * h * 3 / 2);
    //I420/mirror
//    uint8_t *mirror = (uint8_t *) malloc(w * h * 3 / 2);
    //I420/mirror/rotate
//    uint8_t *rotate = (uint8_t *) malloc(w * h * 3 / 2);

    //libyuv NV21 convert I420
    NV21ToI420((const uint8_t *) src, w, (uint8_t *) src + (w * h), w, dst, w,
               dst + (w * h),
               w / 2, dst + (w * h * 5 / 4), w / 2, w, h);

//    uint8_t *src_y = dstI420;
//    uint8_t *src_u = dstI420 + w * h;
//    uint8_t *src_v = dstI420 + (w * h * 5 / 4);

//    uint8_t *dst_y = mirror;
//    uint8_t *dst_u = mirror + w * h;
//    uint8_t *dst_v = mirror + (w * h * 5 / 4);

//    RotationMode rotationMode = kRotate0;
//    if (orientation == 1) {
//        //libyuv 前置镜像
//        I420Mirror(src_y, w, src_u, w / 2, src_v, w / 2, dst_y,
//                   w, dst_u, w / 2, dst_v, w / 2, w, h);
//        //前置  注意:如果旋转90/270,旋转后,Y U V 行数据量,应该使用height计算
//        rotationMode = kRotate90;
//        I420Rotate(mirror, w, mirror + (w * h), w / 2, mirror + (w * h * 5 / 4), w / 2, rotate,
//                   h, rotate + (w * h), h / 2, rotate + (w * h * 5 / 4), h / 2, w, h,
//                   rotationMode);
//    } else if (orientation == 0) {
//        //后置  旋转90
//        rotationMode = kRotate90;
//        I420Rotate(src_y, w, src_u, w / 2, src_v, w / 2, rotate, h,
//                   rotate + (w * h), h / 2, rotate + (w * h * 5 / 4), h / 2, w, h,
//                   rotationMode);
//    }

    return;
}
