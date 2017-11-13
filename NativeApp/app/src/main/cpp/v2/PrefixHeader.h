//
// Created by developer on 11/12/17.
//

#ifndef NATIVEAPP_PREFIXHEADER_H
#define NATIVEAPP_PREFIXHEADER_H

#include <iostream>
#include <mutex>
#include "../debug.h"
#include "VideoEncodeArgs.h"
#include "../base_include.h"
#include "AudioEncodeArgs.h"

//测试数据写入文件
#if 0
#define WRITE_YUV_TO_FILE
#define WRITE_PCM_TO_FILE
#endif

using namespace std;

enum CameraID {
    FRONT, BACK
};


#endif //NATIVEAPP_PREFIXHEADER_H
