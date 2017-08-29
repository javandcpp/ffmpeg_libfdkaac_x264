package com.guagua.nativeapp;

import android.os.Bundle;
import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import com.guagua.nativeapp.jnibridge.FFmpegJavaNativeBridge;

import java.io.File;

public class MainActivity extends AppCompatActivity implements View.OnClickListener, SurfaceHolder.Callback {

    private Button btnVideoDecode;
    private File destfile;
    private Button btnPlay;
    private File srcfile;
    private SurfaceView surfaceView;
    private boolean surfaceCreated;
    private File srcYUVFile;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        TextView tvInfo = (TextView) findViewById(R.id.tvInfo);
        srcfile = new File(Environment.getExternalStorageDirectory(), "video_1.mkv");
        destfile = new File(Environment.getExternalStorageDirectory(), "video_2.yuv");
        srcYUVFile = new File(Environment.getExternalStorageDirectory(), "image.yuv");
        tvInfo.setText(nativeGetVideoInfo(srcfile.getAbsolutePath()));


        ((EditText) findViewById(R.id.etInputUri)).setText(srcfile.getAbsolutePath());
        ((EditText) findViewById(R.id.etOutputUri)).setText(destfile.getAbsolutePath());

        btnVideoDecode = ((Button) findViewById(R.id.btnVideoDecorder));
        btnPlay = ((Button) findViewById(R.id.btnPlay));
        findViewById(R.id.btGetImageYUV).setOnClickListener(this);
        ((EditText) findViewById(R.id.etyuvPath)).setText(srcYUVFile.getAbsolutePath());
        surfaceView = (SurfaceView) findViewById(R.id.surfaceView);
        findViewById(R.id.btToGrayYUV).setOnClickListener(this);
        ((EditText) findViewById(R.id.etyuvPath1)).setText(srcYUVFile.getAbsolutePath());
        surfaceView.getHolder().addCallback(this);
        btnVideoDecode.setOnClickListener(this);
        btnPlay.setOnClickListener(this);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
    }


    public native String getStringUTF();

    public native String nativeString();

    public native String nativeGetVideoInfo(String path);

    public native int nativeDecode(String inputUri, String outputUri);

    public native int nativePlayer(String playUri, Object surface);

    @Override
    public void onClick(View view) {
        switch (view.getId()) {
            case R.id.btnVideoDecorder:
                nativeDecode(srcfile.getAbsolutePath(), destfile.getAbsolutePath());
                break;
            case R.id.btnPlay:
//                new Thread(new Runnable() {
//                    @Override
//                    public void run() {
                nativePlayer(srcfile.getAbsolutePath(), surfaceView.getHolder().getSurface());
//                    }
//                }).start();
                break;
            case R.id.getYUVInfo:
                break;
            case R.id.btGetImageYUV:

                FFmpegJavaNativeBridge.yuv420Image2YUV(srcYUVFile.getAbsolutePath(), 500, 332);
                break;
            case R.id.btToGrayYUV:
                FFmpegJavaNativeBridge.yuvTOGrayYUV(srcYUVFile.getAbsolutePath(), 500, 332);
                break;
        }
    }

    @Override
    public void surfaceCreated(final SurfaceHolder surfaceHolder) {

    }

    @Override
    public void surfaceChanged(SurfaceHolder surfaceHolder, int i, int i1, int i2) {

    }

    @Override
    public void surfaceDestroyed(SurfaceHolder surfaceHolder) {

    }
}
