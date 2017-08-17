package com.guagua.nativeapp;

import android.app.Activity;
import android.os.Bundle;
import android.os.Environment;
import android.widget.TextView;

import java.io.File;

public class YUVActivity extends Activity {

    private TextView tvYuvInfo;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_yuv);
        File destfile = new File(Environment.getExternalStorageDirectory(), "video_1.mkv");
        tvYuvInfo = (TextView) findViewById(R.id.tvYUVInfo);
        tvYuvInfo.setText(getYuvInfo(destfile.getAbsolutePath()));
    }


    public native String getYuvInfo(String path);


}
