//
// Created by developer on 11/13/17.
//

#ifndef NATIVEAPP_MEDIAENCODER_H
#define NATIVEAPP_MEDIAENCODER_H

#include "PrefixHeader.h"

class MediaEncoder {
protected:
    MediaEncoder();

    virtual ~MediaEncoder();

    void RegisterAVCodec();

    void RegisterAVNetwork();

    virtual int StartEncode() = 0;

    virtual int InitEncode() = 0;

    virtual int CloseEncode() = 0;

    virtual int Release()=0;

    static bool first;

    bool ExitFlag;

    mutex mMutex;

    AudioEncodeArgs audioEncodeArgs;
    VideoEncodeArgs videoEncodeArgs;

};


#endif //NATIVEAPP_MEDIAENCODER_H
