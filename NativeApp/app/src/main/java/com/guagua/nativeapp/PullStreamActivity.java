/*
 * Copyright (c) javandoc 2017
 * @author javandoc
 */

package com.guagua.nativeapp;

import android.os.Bundle;
import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;

import com.guagua.nativeapp.jnibridge.FFmpegJavaNativeBridge;

import java.io.File;

public class PullStreamActivity extends AppCompatActivity implements View.OnClickListener {

    private EditText etInputUri;
    private EditText etOutputUri;
    private File destFile;
    private boolean isPull;
    private Button btnPullStream;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_pull_stream);
        btnPullStream = (Button) findViewById(R.id.btnPullStream);
        btnPullStream.setOnClickListener(this);

        etInputUri = (EditText) findViewById(R.id.etInputUri);
        etOutputUri = (EditText) findViewById(R.id.etOutputUri);

        etInputUri.setText("rtmp://" + Constants.IP_ADDRESS + ":1935/test/live");
        destFile = new File(Environment.getExternalStorageDirectory().getAbsolutePath(), "testLive.flv");
        etOutputUri.setText(destFile.getAbsolutePath().toString().trim());
    }

    @Override
    public void onClick(View view) {
        if (isPull) {
            FFmpegJavaNativeBridge.stopPull();
            isPull = false;
            btnPullStream.setText("startPull");
        } else {
            btnPullStream.setText("stopPull");
            new Thread() {
                @Override
                public void run() {
                    isPull = true;
                    FFmpegJavaNativeBridge.pullStream(etInputUri.getText().toString().trim(), etOutputUri.getText().toString());
                }
            }.start();
        }
    }
}
