/*
 * Copyright (c) javandoc 2017
 * @author javandoc
 */

package com.guagua.nativeapp;

import android.app.Activity;
import android.content.Context;
import android.os.Bundle;
import android.os.Environment;
import android.util.DisplayMetrics;
import android.view.SurfaceView;
import android.view.View;
import android.view.WindowManager;
import android.widget.RelativeLayout;
import android.widget.TextView;

import com.guagua.nativeapp.recorder.IMediaCallback;
import com.guagua.nativeapp.recorder.NativeMediaRecorder;

public class Recorder2 extends Activity implements View.OnClickListener, IMediaCallback {

    private SurfaceView surfaceView;
    private TextView tvVideo;
    private TextView tvAudio;
    private RelativeLayout videoParent;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON); // 防止锁屏
        setContentView(R.layout.activity_recorder);
        findViewById(R.id.btnFlash).setOnClickListener(this);
        findViewById(R.id.btnStart).setOnClickListener(this);
        findViewById(R.id.btnStop).setOnClickListener(this);
        videoParent = (RelativeLayout) findViewById(R.id.rlLayout);
        findViewById(R.id.btnSwitchCamera).setOnClickListener(this);
        tvAudio = ((TextView) findViewById(R.id.tvAudioInfo));
        tvVideo = ((TextView) findViewById(R.id.tvVideoInfo));


        NativeMediaRecorder nativeMediaRecorder = new NativeMediaRecorder(this);

        SurfaceView surfaceView = new SurfaceView(this);

        DisplayMetrics displayMetrics = new DisplayMetrics();
        getWindowManager().getDefaultDisplay().getRealMetrics(displayMetrics);
        surfaceView.setLayoutParams(new RelativeLayout.LayoutParams(displayMetrics.widthPixels, displayMetrics.heightPixels));
        try {
            nativeMediaRecorder.setSurfaceHolder(surfaceView.getHolder());
            nativeMediaRecorder.setPath(Environment.getExternalStorageDirectory().getAbsolutePath(), "audio.pcm");
            nativeMediaRecorder.prepare();
            nativeMediaRecorder.startRecord();
        } catch (Exception e) {
            e.printStackTrace();
        }
        videoParent.addView(surfaceView);


    }

    @Override
    protected void onResume() {
        super.onResume();
    }

    @Override
    protected void onPause() {
        super.onPause();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
    }

    @Override
    public void onClick(View view) {
        switch (view.getId()) {
            case R.id.btnFlash:
                break;
            case R.id.btnStart:

                break;
            case R.id.btnSwitchCamera:
                break;
            case R.id.btnStop:
                break;
        }
    }

    @Override
    public void onAudioRecordError(int info, String error) {

    }

    @Override
    public void onVideoError() {

    }

    @Override
    public void receiveAudioData(byte[] sampleBuffer, int result) {

    }

    @Override
    public void receiveVideoData(byte[] data) {

    }

    @Override
    public void onPrepared() {

    }

    @Override
    public Context getContext() {
        return null;
    }
}
