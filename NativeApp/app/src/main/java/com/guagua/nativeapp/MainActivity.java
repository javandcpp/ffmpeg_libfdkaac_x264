package com.guagua.nativeapp;

import android.os.Bundle;
import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.widget.TextView;

import com.guagua.nativeapp.R;

import java.io.File;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        TextView tvInfo = (TextView) findViewById(R.id.tvInfo);
        File file=new File(Environment.getExternalStorageDirectory(),"video_1.mkv");
        tvInfo.setText(nativeGetVideoInfo(file.getAbsolutePath()));
    }

    static {
        System.loadLibrary("native-lib");
    }


    public native String getStringUTF();

    public native String nativeString();

    public native String nativeGetVideoInfo(String path);
}
