//
// Created by developer on 11/12/17.
//

#include "VideoCapture.h"

VideoCapture::VideoCapture() {

}

VideoCapture::~VideoCapture() {

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

    LOG_D(DEBUG,"Release Video Capture!");
    return 0;
}

/**
 * 关闭采集数据输入
 */
bool VideoCapture::CloseCapture() {
    mMutex.lock();
    ExitCapture = true;
    mMutex.unlock();
    LOG_D(DEBUG,"Close Video Capture");
    return ExitCapture;
}


bool VideoCapture::StartCapture() {
    mMutex.lock();
    ExitCapture = false;
    mMutex.unlock();
    LOG_D(DEBUG,"Start Video Capture");
    return !ExitCapture;
}

/**
 * 往队列中添加视频原数据
 */
int VideoCapture::PushVideoData(OriginData *originData) {
    if (ExitCapture)
        return 0;
    frame_queue.push(originData);
    return originData->size;
}

/**
 *从队列中获取视频原数据
 */
OriginData *VideoCapture::GetVideoData() {
    if (ExitCapture)
        return NULL;
    return frame_queue.empty() ? NULL : *(frame_queue.wait_and_pop().get());
}
