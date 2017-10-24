/*
 * Copyright (c) javandoc 2017
 * @author javandoc
 */

package com.guagua.nativeapp;

import android.app.Activity;
import android.os.Bundle;
import android.view.SurfaceView;

import com.guagua.nativeapp.recorder.CaptureManager;

import java.io.File;

public class CaptureAndPushActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_capture_and_push);
        SurfaceView surfaceView = (SurfaceView) findViewById(R.id.surfaceView);
        CaptureManager captureManager = new CaptureManager(this,new File(""), surfaceView, 360, 640);
    }
}
