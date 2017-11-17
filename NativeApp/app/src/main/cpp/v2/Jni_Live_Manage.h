//
// Created by developer on 11/12/17.
//
/**
 * JNI 底层推流
 */
#ifndef NATIVEAPP_JNI_LIVE_MANAGE_H
#define NATIVEAPP_JNI_LIVE_MANAGE_H

#include "VideoCapture.h"
#include "AudioCapture.h"
#include "PrefixHeader.h"
#include "VideoEncoder.h"
#include "AudioEncoder.h"
#include "RtmpStreamer.h"
#include "AudioCapture.h"

AudioCapture *audioCapture = NULL;
VideoCapture *videoCapture = NULL;

VideoEncoder *videoEncoder = NULL;
AudioEncoder *audioEncoder = NULL;

RtmpStreamer *rtmpStreamer=NULL;

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
