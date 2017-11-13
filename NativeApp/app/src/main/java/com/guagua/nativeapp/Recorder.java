package com.guagua.nativeapp;

import android.app.Activity;
import android.os.Bundle;
import android.util.DisplayMetrics;
import android.view.SurfaceView;
import android.view.View;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.RelativeLayout;
import android.widget.Switch;
import android.widget.TextView;
import android.widget.Toast;

import com.guagua.nativeapp.recorder.NativeMediaRecorder;

public class Recorder extends Activity implements View.OnClickListener {

    private NativeMediaRecorder nativeAudioRecorder;
    private SurfaceView surfaceView;
    private TextView tvVideo;
    private TextView tvAudio;
    private PushStreamer pushStreamer;
    private RelativeLayout videoParent;
    private Button btnStart;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON); // 防止锁屏
        setContentView(R.layout.activity_recorder);
        final Switch swflash = (Switch) findViewById(R.id.swFlash);
        swflash.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton compoundButton, boolean b) {
                if(!pushStreamer.Light()){
                    swflash.setChecked(false);
                    Toast.makeText(getApplicationContext(),"前置摄像头,不支持闪光灯!",Toast.LENGTH_SHORT).show();
                }
            }
        });
        ((Switch) findViewById(R.id.swBeauty)).setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton compoundButton, boolean b) {

            }
        });
        btnStart = (Button) findViewById(R.id.btnStart);
        btnStart.setOnClickListener(this);
        videoParent = (RelativeLayout) findViewById(R.id.rlLayout);
        findViewById(R.id.btnSwitchCamera).setOnClickListener(this);
        tvAudio = ((TextView) findViewById(R.id.tvAudioInfo));
        tvVideo = ((TextView) findViewById(R.id.tvVideoInfo));

        SurfaceView surfaceView = new SurfaceView(this);
        pushStreamer = new PushStreamer(this, surfaceView);
        DisplayMetrics displayMetrics = new DisplayMetrics();
        getWindowManager().getDefaultDisplay().getRealMetrics(displayMetrics);
        surfaceView.setLayoutParams(new RelativeLayout.LayoutParams(displayMetrics.widthPixels, displayMetrics.heightPixels));
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
    protected void onStop() {
        super.onStop();
        pushStreamer.destroy();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
    }

    @Override
    public void onClick(View view) {
        switch (view.getId()) {
            case R.id.btnStart:
                if (pushStreamer.isSpeak()) {
                    pushStreamer.stopSpeak();
                    btnStart.setText("开始直播");
                } else {
                    pushStreamer.startSpeak();
                    btnStart.setText("停止直播");
                }
                break;
            case R.id.btnSwitchCamera:
                pushStreamer.SwitchCamera();
                break;
        }
    }

}
