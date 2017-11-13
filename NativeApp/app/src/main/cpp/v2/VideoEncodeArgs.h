//
// Created by developer on 11/13/17.
//

#ifndef NATIVEAPP_VIDEOENCODEARGS_H
#define NATIVEAPP_VIDEOENCODEARGS_H

#include <iostream>
using namespace std;
struct VideoEncodeArgs{

    uint32_t in_width;
    uint32_t in_height;
    uint32_t out_width;
    uint32_t out_height;
    uint8_t mirror;
    uint32_t fps;

};

#endif //NATIVEAPP_VIDEOENCODEARGS_H
