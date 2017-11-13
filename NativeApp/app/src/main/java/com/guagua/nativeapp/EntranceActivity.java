package com.guagua.nativeapp;

import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;

public class EntranceActivity extends AppCompatActivity implements View.OnClickListener {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_entrance);
        findViewById(R.id.MainActivity).setOnClickListener(this);
        findViewById(R.id.SDLActivity).setOnClickListener(this);
        findViewById(R.id.getYUVInfo).setOnClickListener(this);
        findViewById(R.id.recorder).setOnClickListener(this);
        findViewById(R.id.btnPushStream).setOnClickListener(this);
        findViewById(R.id.btnPullStream).setOnClickListener(this);
        findViewById(R.id.btnTransStream).setOnClickListener(this);
        findViewById(R.id.btnDecoder).setOnClickListener(this);

    }

    @Override
    public void onClick(View view) {
        switch (view.getId()) {
            case R.id.MainActivity:
                startActivity(new Intent(this, MainActivity.class));
                break;
            case R.id.SDLActivity:
                startActivity(new Intent(this, SDLPlayerActivity.class));
                break;
            case R.id.getYUVInfo:
                startActivity(new Intent(this, YUVActivity.class));
                break;
            case R.id.recorder:
                startActivity(new Intent(this, Recorder.class));
                break;
            case R.id.btnPushStream:
                startActivity(new Intent(this, PushStreamActivity.class));
                break;
            case R.id.btnPullStream:
                startActivity(new Intent(this, PullStreamActivity.class));
                break;
            case R.id.btnTransStream:
                startActivity(new Intent(this, TransStreamActivity.class));
                break;
            case R.id.btnDecoder:
                startActivity(new Intent(this,VideoDecoderActivity.class));
                break;

        }
    }
}
