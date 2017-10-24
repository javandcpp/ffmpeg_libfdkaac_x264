/*
 * Copyright (c) javandoc 2017
 * @author javandoc
 */

package com.guagua.nativeapp;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.widget.Button;
import android.widget.EditText;
import android.widget.LinearLayout;

import com.guagua.nativeapp.jnibridge.FFmpegJavaNativeBridge;

import java.io.File;

import butterknife.BindView;
import butterknife.ButterKnife;
import butterknife.OnClick;

public class TransStreamActivity extends AppCompatActivity {

    @BindView(R.id.etInputUri)
    EditText etInputUri;
    @BindView(R.id.etOutputUri)
    EditText etOutputUri;
    @BindView(R.id.btnTransStream)
    Button btnTransStream;
    @BindView(R.id.activity_trans_stream)
    LinearLayout activityTransStream;
    private boolean transform;
    private File destFile;
    private Button btnTrans;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_trans_stream);
        ButterKnife.bind(this);


        btnTrans = ((Button) findViewById(R.id.btnTransStream));

        etInputUri.setText("rtmp://" + Constants.TRANSFORMA_STEAM.SRC_URI.ip + ":1935/test/live");

//        destFile = new File(Environment.getExternalStorageDirectory().getAbsolutePath(), "testTrans.flv");
//        etOutputUri.setText(destFile.getAbsolutePath().toString().trim());
        etOutputUri.setText("rtmp://" + Constants.TRANSFORMA_STEAM.DES_URI.ip + ":1935/test/live");

    }

    @OnClick({R.id.btnTransStream})
    public void click() {
        if (transform) {
            transform = false;
            //停止转发
            btnTrans.setText("开始转发");
            FFmpegJavaNativeBridge.stopTrans();
        } else {
            transform = true;
            //开始转发
            btnTrans.setText("停止转发");
            new Thread() {
                @Override
                public void run() {
                    FFmpegJavaNativeBridge.transStream(etInputUri.getText().toString().trim(), etOutputUri.getText().toString().trim());
                }
            }.start();
        }
    }

    @Override
    public void finish() {
        super.finish();
        if (transform) {
            FFmpegJavaNativeBridge.stopTrans();
        }
    }


    @Override
    protected void onDestroy() {
        super.onDestroy();
        FFmpegJavaNativeBridge.releaseTransStream();
    }
}
