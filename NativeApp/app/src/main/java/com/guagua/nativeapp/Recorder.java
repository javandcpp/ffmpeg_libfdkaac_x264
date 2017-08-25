package com.guagua.nativeapp;

import android.content.Context;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.SurfaceView;
import android.view.View;
import android.view.WindowManager;
import android.widget.TextView;

import com.guagua.nativeapp.jnibridge.FFmpegJavaNativeBridge;
import com.guagua.nativeapp.recorder.IMediaCallback;
import com.guagua.nativeapp.recorder.NativeMediaRecorder;

public class Recorder extends AppCompatActivity implements View.OnClickListener, IMediaCallback {

    private NativeMediaRecorder nativeAudioRecorder;
    private SurfaceView surfaceView;
    private TextView tvVideo;
    private TextView tvAudio;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON); // 防止锁屏
        setContentView(R.layout.activity_recorder);
        findViewById(R.id.btnFlash).setOnClickListener(this);
        findViewById(R.id.btnStart).setOnClickListener(this);
        findViewById(R.id.btnStop).setOnClickListener(this);
        findViewById(R.id.btnSwitchCamera).setOnClickListener(this);
        surfaceView = (SurfaceView) findViewById(R.id.surfaceView);
       // surfaceView.setZOrderMediaOverlay(true);
        tvAudio = ((TextView) findViewById(R.id.tvAudioInfo));
        tvVideo = ((TextView) findViewById(R.id.tvVideoInfo));


        try {
//            nativeAudioRecorder.setPath(Environment.getExternalStorageDirectory().getAbsolutePath(), "audio.aac");
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    @Override
    protected void onResume() {
        super.onResume();
        if (null == nativeAudioRecorder) {
            nativeAudioRecorder = new NativeMediaRecorder(this);
            nativeAudioRecorder.setSurfaceHolder(surfaceView.getHolder());
        }
        nativeAudioRecorder.prepare();
    }

    @Override
    protected void onPause() {
        super.onPause();
        nativeAudioRecorder.stopPreview();
    }

    @Override
    public void onClick(View view) {
        switch (view.getId()) {
            case R.id.btnFlash:
                break;
            case R.id.btnStart:
                nativeAudioRecorder.startRecord();
                break;
            case R.id.btnSwitchCamera:
                break;
            case R.id.btnStop:
                nativeAudioRecorder.stopAudioRecord();
                FFmpegJavaNativeBridge.endRecord();
                FFmpegJavaNativeBridge.releaseRecord();
                break;
        }
    }

    @Override
    public void onAudioRecordError(int info, String error) {
        Log.e("record", info + ":" + error);
    }

    @Override
    public void onVideoError() {
        Log.e("record", "video error");
    }

    @Override
    public void receiveAudioData(byte[] data, int result) {
        Log.d("receive audio", result + "");
      //  tvAudio.setText("receive audio len:"+result);
        if (nativeAudioRecorder.isRecording() && result > 0) {
            FFmpegJavaNativeBridge.encode2AAC(data);
        }
    }

    @Override
    public void receiveVideoData(byte[] data) {
        Log.d("receive video", data.length + "");
     //   tvVideo.setText("receive video len:"+data.length);
        if (nativeAudioRecorder.isRecording()&&data.length>0) {
            FFmpegJavaNativeBridge.encodeFrame2H264(data);
        }
    }

    @Override
    public void onPrepared() {
//        final int w = DeviceUtils.getScreenWidth(this);
//        ((RelativeLayout.LayoutParams) surfaceView.getLayoutParams()).topMargin = (int) (w / (SMALL_VIDEO_HEIGHT / (SMALL_VIDEO_WIDTH * 1.0f)));
//        int width = w;
//        int height = (int) (w * ((NativeMediaRecorder.mSupportedPreviewWidth * 1.0f) / SMALL_VIDEO_HEIGHT));
//        //
//        RelativeLayout.LayoutParams lp = (RelativeLayout.LayoutParams) surfaceView
//                .getLayoutParams();
//        lp.width = width;
//        lp.height = height;
//        surfaceView.setLayoutParams(lp);
    }

    @Override
    public Context getContext() {
        return this;
    }

}
