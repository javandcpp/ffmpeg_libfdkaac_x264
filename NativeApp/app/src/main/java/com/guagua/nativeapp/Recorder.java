package com.guagua.nativeapp;

import android.os.Bundle;
import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;

import com.guagua.nativeapp.jnibridge.FFmpegJavaNativeBridge;
import com.guagua.nativeapp.recorder.IMediaCallback;
import com.guagua.nativeapp.recorder.NativeAudioRecorder;

public class Recorder extends AppCompatActivity implements View.OnClickListener, IMediaCallback {

    private NativeAudioRecorder nativeAudioRecorder;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_recorder);
        findViewById(R.id.btnFlash).setOnClickListener(this);
        findViewById(R.id.btnStart).setOnClickListener(this);
        findViewById(R.id.btnStop).setOnClickListener(this);
        findViewById(R.id.btnSwitchCamera).setOnClickListener(this);

        nativeAudioRecorder = new NativeAudioRecorder(this);
        try {
            nativeAudioRecorder.setPath(Environment.getExternalStorageDirectory().getAbsolutePath(), "audio.aac");
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    @Override
    public void onClick(View view) {
        switch (view.getId()) {
            case R.id.btnFlash:
                break;
            case R.id.btnStart:
                nativeAudioRecorder.startAudioRecord();
                break;
            case R.id.btnSwitchCamera:
                break;
            case R.id.btnStop:
                FFmpegJavaNativeBridge.endAACEncode();
//                FFmpegJavaNativeBridge.releaseAACEncode();
                nativeAudioRecorder.stopAudioRecord();
                break;
        }
    }

    @Override
    public void onAudioRecordError(int info, String error) {
        Log.d("record", info + ":" + error);
    }

    @Override
    public void receiveAudioData(byte[] sampleBuffer, int result) {
        Log.d("record", result + "");
        if (nativeAudioRecorder.isRecording() && result > 0) {
            FFmpegJavaNativeBridge.encode2AAC(sampleBuffer);
        }
    }
}
