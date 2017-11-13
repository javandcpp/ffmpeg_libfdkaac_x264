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

    VideoEncodeArgs *videoEncodeArgs = NULL;
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


    /**
   * 设置编码参数
   */
    void SetVideoEncodeArgs(VideoEncodeArgs *videoEncodeArgs);

    VideoEncodeArgs *GetVideoEncodeArgs();

    /**
     * 获取视频采集状态
     */
    bool GetCaptureState();

    /**
   * nv21源数据处理(旋转)
   */
    void NV21ProcessYUV420P(int w, int h, uint8_t *src, uint8_t *dst);
};

#endif //NATIVEAPP_VIDEOCAPTURE_H
