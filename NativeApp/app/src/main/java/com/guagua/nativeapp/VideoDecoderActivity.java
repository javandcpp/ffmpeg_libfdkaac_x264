/*
 * Copyright (c) javandoc 2017
 * @author javandoc
 */

package com.guagua.nativeapp;

import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;

import com.guagua.nativeapp.jnibridge.FFmpegJavaNativeBridge;

import java.io.File;

public class VideoDecoderActivity extends AppCompatActivity implements View.OnClickListener {

    private EditText etInput;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_video_decoder);


        etInput = ((EditText) findViewById(R.id.etInputUri));
        etInput.setText(Environment.getExternalStorageDirectory() + File.separator + "capture.mp4");
        ((Button) findViewById(R.id.btnDecode)).setOnClickListener(this);
    }

    @Override
    public void onClick(View view) {
        switch (view.getId()) {
            case R.id.btnDecode:
                new Thread() {
                    @Override
                    public void run() {
                        FFmpegJavaNativeBridge.decode(etInput.getText().toString().trim());
                    }
                }.start();
                break;
        }
    }
}
