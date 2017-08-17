package com.guagua.nativeapp.recorder;

/**
 * Created by android on 8/17/17.
 */

public interface IMediaCallback {



    void onAudioRecordError(int info,String error);


    void receiveAudioData(byte[] sampleBuffer, int result);
}
