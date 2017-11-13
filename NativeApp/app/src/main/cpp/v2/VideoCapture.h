//
// Created by developer on 11/12/17.
//

#ifndef NATIVEAPP_VIDEOCAPTURE_H
#define NATIVEAPP_VIDEOCAPTURE_H

#include "MediaCapture.h"
#include "PrefixHeader.h"


/**
 * 视频数据接收
 */
class VideoCapture : public MediaCapture {

private:
    VideoCapture();

public:
    ~VideoCapture();

    static VideoCapture *Get();

    bool CloseCapture();

    bool StartCapture();

    int Release();

    int PushVideoData(OriginData *originData);

    void SetCameraID(CameraID cameraID);

    CameraID GetCameraID();

    OriginData *GetVideoData();

    mutex mMutex;
    CameraID mCameraId;
};

#endif //NATIVEAPP_VIDEOCAPTURE_H
