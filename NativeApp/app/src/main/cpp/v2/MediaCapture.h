//
// Created by developer on 11/12/17.
//

#ifndef NATIVEAPP_MEDIACAPTURE_H
#define NATIVEAPP_MEDIACAPTURE_H

#include "../threadsafe_queue.cpp"
#include "OriginData.h"
#include "PrefixHeader.h"

class MediaCapture {

protected:
    MediaCapture();

    virtual ~MediaCapture();

    virtual bool CloseCapture() = 0;

    virtual int Release() = 0;

    threadsafe_queue<OriginData *> frame_queue;
    bool ExitCapture;
};

#endif //NATIVEAPP_MEDIACAPTURE_H
