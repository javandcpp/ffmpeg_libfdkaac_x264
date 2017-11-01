package com.guagua.nativeapp;

import android.app.Activity;
import android.os.Bundle;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.view.WindowManager;
import android.widget.RelativeLayout;
import android.widget.TextView;

import com.guagua.nativeapp.recorder.NativeMediaRecorder;

public class Recorder extends Activity implements View.OnClickListener {

    private NativeMediaRecorder nativeAudioRecorder;
    private SurfaceView surfaceView;
    private TextView tvVideo;
    private TextView tvAudio;
    private PushStreamer pushStreamer;
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

        SurfaceView surfaceView=new SurfaceView(this);
        pushStreamer = new PushStreamer(this,surfaceView);
        DisplayMetrics displayMetrics=new DisplayMetrics();
        getWindowManager().getDefaultDisplay().getRealMetrics(displayMetrics);
        surfaceView.setLayoutParams(new RelativeLayout.LayoutParams(displayMetrics.widthPixels,displayMetrics.heightPixels));

        surfaceView.getHolder().addCallback(new SurfaceHolder.Callback() {
            @Override
            public void surfaceCreated(SurfaceHolder surfaceHolder) {
                pushStreamer.startSpeak();
                Log.d("video","surfacecreate");
            }

            @Override
            public void surfaceChanged(SurfaceHolder surfaceHolder, int i, int i1, int i2) {

            }

            @Override
            public void surfaceDestroyed(SurfaceHolder surfaceHolder) {

            }
        });
        videoParent.addView(surfaceView);

    }

    @Override
    protected void onResume() {
        super.onResume();
        pushStreamer.onActivityResume();
    }

    @Override
    protected void onPause() {
        super.onPause();
        pushStreamer.onActivityPause();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        pushStreamer.destroy();
    }

    @Override
    public void onClick(View view) {
        switch (view.getId()) {
            case R.id.btnFlash:
                pushStreamer.Light();
                break;
            case R.id.btnStart:

                break;
            case R.id.btnSwitchCamera:
                pushStreamer.SwitchCamera();
                break;
            case R.id.btnStop:
                break;
        }
    }

}
