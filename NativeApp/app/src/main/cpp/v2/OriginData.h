//
// Created by developer on 11/12/17.
//

#ifndef NATIVEAPP_ORIGINDATA_H
#define NATIVEAPP_ORIGINDATA_H

#include <iostream>
#include "PrefixHeader.h"
using namespace std;

class OriginData{
public:
    OriginData();
    ~OriginData();

    int64_t pts;
    AVFrame *frame=NULL;
    uint8_t *data=NULL;
    uint32_t size;
};

#endif //NATIVEAPP_ORIGINDATA_H
