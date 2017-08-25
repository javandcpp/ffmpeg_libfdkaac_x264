package com.guagua.nativeapp.recorder;

import android.content.Context;

/**
 * Created by android on 8/17/17.
 */

public interface IMediaCallback{


    void onAudioRecordError(int info, String error);

    void onVideoError();


    void receiveAudioData(byte[] sampleBuffer, int result);

    void receiveVideoData(byte[] data);

    void onPrepared();

    Context getContext();
}

