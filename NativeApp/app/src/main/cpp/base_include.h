//
// Created by developer on 8/17/17.
//


#ifndef NATIVEAPP_BASE_INCLUDE_H
#define NATIVEAPP_BASE_INCLUDE_H


extern "C"{
#include "include/libavcodec/avcodec.h"
#include "include/libavformat/avformat.h"
#include "include/libavcodec/avcodec.h"
#include "include/libavutil/opt.h"
#include "include/libavutil/frame.h"
}

#include <jni.h>
#include <iostream>
#include <string.h>
#include <android/log.h>
#include "debug.h"
#include "threadsafe_queue.cpp"
#include "arguments.h"
#include "jni_encode_aac.h"

#define START 0
#define END 1
#define RELEASE 1


#endif //NATIVEAPP_BASE_INCLUDE_H
