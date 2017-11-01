/*
 * Copyright (c) javandoc 2017
 * @author javandoc
 */

package com.guagua.nativeapp;

import android.os.Bundle;
import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;

import com.guagua.nativeapp.jnibridge.FFmpegJavaNativeBridge;

import java.io.File;


/**
 * 推流
 */
public class PushStreamActivity extends AppCompatActivity implements View.OnClickListener {

    private Button btnPushStream;
    private EditText etOutputUri;
    private EditText etInputUri;
    private boolean isPush;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_push_stream);

        etInputUri = ((EditText) findViewById(R.id.etInputUri));
        etOutputUri = ((EditText) findViewById(R.id.etOutputUri));
        btnPushStream = ((Button) findViewById(R.id.btnPushStream));
        btnPushStream.setOnClickListener(this);


        File inputFile = new File(Environment.getExternalStorageDirectory().getAbsolutePath(), "ffmpeg.flv");
        Log.d("debug", inputFile.exists() + "");
        etInputUri.setText(inputFile.getAbsolutePath());
        etOutputUri.setText("rtmp://" + Constants.IP_ADDRESS + ":1935/test/live");
    }

    @Override
    public void onClick(View view) {
        if (isPush) {
            isPush = false;
            btnPushStream.setText("startPush");
//            FFmpegJavaNativeBridge.stopPush();
        } else {
            isPush = true;
            btnPushStream.setText("stopPush");
            new Thread() {
                @Override
                public void run() {
                    FFmpegJavaNativeBridge.xpushstream(etInputUri.getText().toString().trim(), etOutputUri.getText().toString().trim());
                }
            }.start();
        }
    }
}
