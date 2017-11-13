//
// Created by developer on 11/12/17.
//
/**
 * JNI 底层推流
 */
#ifndef NATIVEAPP_JNI_LIVE_MANAGE_H
#define NATIVEAPP_JNI_LIVE_MANAGE_H

#include "v2/VideoCapture.h"
#include "v2/AudioCapture.h"
#include "v2/PrefixHeader.h"
#include "v2/VideoEncoder.h"
#include "v2/AudioEncoder.h"

AudioCapture *audioCapture = NULL;
VideoCapture *videoCapture = NULL;

VideoEncoder *videoEncoder = NULL;
AudioEncoder *audioEncoder = NULL;

/**
 * 初始化采集数据接收
 */
bool audioCaptureInit=false;
bool videoCaptureInit=false;
bool isClose = true;
bool isRelease = false;
bool startStream;
mutex mMutex;
#endif
