/*
 * Copyright (c) javandoc 2017
 * @author javandoc
 */

package com.guagua.nativeapp;

import android.app.Activity;
import android.content.SharedPreferences;
import android.os.Environment;
import android.os.Handler;
import android.telephony.TelephonyManager;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

import com.capture.avcapture.AudioCaptureInterface;
import com.capture.avcapture.VideoCaptureInterface;
import com.capture.avcapture.impl.AudioCapture;
import com.capture.avcapture.impl.VideoCapture;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.RandomAccessFile;
import java.util.List;

import static android.hardware.Camera.CameraInfo.CAMERA_FACING_BACK;

//obj.m_byCodeType = (byte) mediaCacheConfig.getInt(MediaConstants.AUDIO_CODE_TYPE, 7);
//        obj.m_dwSamplesPerSec = mediaCacheConfig.getInt(MediaConstants.AUDIO_SAMPLE_RATE, 48000);
//        obj.m_wBandWidth = (short) mediaCacheConfig.getInt(MediaConstants.AUDIO_BANDWIDTH, 24);
//        obj.m_wBitsPerSample = ((short) mediaCacheConfig.getInt(MediaConstants.AUDIO_BITSPERSAMPLE, 16));
//        obj.m_wChannels = ((short) mediaCacheConfig.getInt(MediaConstants.AUDIO_CHANNELS, 2));
//        obj.m_wPerPackTimes = ((short) mediaCacheConfig.getInt(MediaConstants.AUDIO_PER_PACKTIMES, 80));
//
//
//        videoConfig.m_byCodeType = ((byte) mediaCacheConfig.getInt(MediaConstants.VIDEO_CODE_TYPE, 26));
//        videoConfig.m_byVideoQuality = ((byte) mediaCacheConfig.getInt(MediaConstants.VIDEO_QUALITY, 100));
//        videoConfig.m_sColorSpace = ((short) mediaCacheConfig.getInt(MediaConstants.VIDEO_COLOR_SPACE, 24));
//        videoConfig.m_sFramesPerSec = ((short) mediaCacheConfig.getInt(MediaConstants.VIDEO_Frames_PS, 15));

/**
 * Created by android on 10/31/17.
 */

public class PushStreamer implements SurfaceHolder.Callback {

    public final SurfaceHolder mSurfaceHolder;
    public int[] m_aiBufferLength;
    public int requireLength = 1024 * 8;
    public AudioCaptureInterface m_oAudioCapture = new AudioCapture();
    public VideoCaptureInterface m_oVideoCapture = new VideoCapture();
    public int frameCount;
    public List<VideoCaptureInterface.CameraDeviceInfo> s_lCameraInfoList;
    public AudioThread m_oAudioThread = null;
    public VideoThread m_oVideoThread = null;
    public int maxZoomLevel;
    public int currentZoomLevel;
    public boolean isLight;
    public short m_sCameraID = 0;
    public boolean destroy = false;
    public boolean mBeautyRelease;
    public int FTHandler;
    //默认为前
    public int curCameraType = VideoCaptureInterface.CameraDeviceType.CAMERA_FACING_FRONT;
    public boolean isChangeEffect = true;
    public String TAG = this.getClass().getSimpleName();

    public short currentMicIndex;
    public SharedPreferences mediaCacheConfig;
    public FileOutputStream stream;
    public long pauseTime;
    public int currentPhoneState = TelephonyManager.CALL_STATE_IDLE;
    public byte currentDevType;
    public long mBeautyHandler;
    public boolean mBeautyGpuUseful;
    public boolean mBeautyInit;
    public boolean singleCamera;
    public VideoSizeConfig mVideoSizeConfig = new VideoSizeConfig();
    private Handler handler = new Handler();
    private final File videoFile;
    private final File audioFile;
    private FileOutputStream videoOps;
    private FileOutputStream audioOps;
    private final byte[] yuvBuf;

    private static final Object i1 = new Object();
    private RandomAccessFile file;
    private boolean nativeInt;
    private boolean speak;

    public PushStreamer(Activity context, SurfaceView surfaceView) {
        mSurfaceHolder = surfaceView.getHolder();
        File externalStorageDirectory = Environment.getExternalStorageDirectory();
        surfaceView.getHolder().addCallback(this);

        yuvBuf = new byte[mVideoSizeConfig.srcFrameHeight * mVideoSizeConfig.srcFrameWidth * 3 / 2];
        videoFile = new File(externalStorageDirectory, "video.yuv");
        audioFile = new File(externalStorageDirectory, "audio.pcm");
        try {
            if (videoFile.exists()) {
                videoFile.delete();
            }
            if (audioFile.exists()) {
                audioFile.delete();
            }
            videoFile.createNewFile();
            audioFile.createNewFile();
            videoOps = new FileOutputStream(videoFile);
            audioOps = new FileOutputStream(audioFile);
        } catch (Exception e) {
            e.printStackTrace();
        }
        if (!InitNative()) {
            Log.e("init native", "init failed!");
        }
    }

    private boolean initCapture() {
        int ret = 0;
        ret = LiveManager.InitAudioCapture(2, 48000, 16);
        if (ret < 0) {
            Log.e("initNative", "init audio capture failed!");
            return false;
        }
        ret = LiveManager.InitVideoCapture(mVideoSizeConfig.srcFrameWidth, mVideoSizeConfig.srcFrameHeight, 640, 480, 25, true);
        if (ret < 0) {
            Log.e("initNative", "init video capture failed!");
            return false;
        }
        return true;
    }

    private boolean initEncoder() {
        int ret = 0;
        ret = LiveManager.InitAudioEncoder();
        if (ret < 0) {
            Log.e("initNative", "init AudioEncoder failed!");
            return false;
        }
        ret = LiveManager.InitVideoEncoder();
        if (ret < 0) {
            Log.e("initNative", "init VideoEncoder failed!");
            return false;
        }
        return true;
    }

    /**
     * 初始化底层采集与编码器
     */
    private boolean InitNative() {
        if(!initCapture()){
            return false;
        }
        if(!initEncoder()){
            return false;
        }
        Log.d("initNative", "native init success!");
        nativeInt = true;
        return nativeInt;
    }

    private boolean startPushStream(String pushUrl) {
        if (nativeInt) {
            int ret = 0;
            ret = LiveManager.StartPush(pushUrl);
            if (ret < 0) {
                Log.d("initNative", "native push failed!");
                return false;
            }
            return true;
        }
        return false;
    }


    public void startSpeak() {
        if (!speak) {
            if (startPushStream("rtmp://192.168.24.153:1935/test/live")) {
                speak = true;
            }
        }
    }

    public boolean isSpeak() {
        return speak;
    }


    public void stopSpeak() {
        if (speak) {
            speak = false;
            LiveManager.Close();
        }
    }


    public boolean destroy() {
        if (!destroy) {
            destroy = true;
            closeCapture();
            LiveManager.Release();
            try {
                if (videoOps != null) {
                    videoOps.close();
                }
                if (audioOps != null) {
                    audioOps.close();
                }
            } catch (Exception e) {

            }
        }
        return false;
    }


    public void closeAudioVideoCapture() {
        if (null != m_oAudioCapture && m_oAudioCapture.IsStartAudioCapture()) {
            if (m_oVideoThread != null) {

                m_oAudioThread.stopThread();
                try {
                    m_oAudioThread.join();
                } catch (Exception e) {
                    // TODO Auto-generated catch block
                    e.printStackTrace();
                }
            }
        }
        if (null != m_oVideoCapture && m_oVideoCapture.IsStartVideoCapture()) {
            if (m_oVideoThread != null) {
                m_oVideoThread.stopThread();
                try {
                    m_oVideoThread.join();
                } catch (Exception e) {
                    // TODO Auto-generated catch block
                    e.printStackTrace();
                }
            }
        }
        if (null != m_oAudioCapture) {
            m_oAudioCapture.CloseAudioDevice();
        }
        if (null != m_oVideoCapture) {
            m_oVideoCapture.CloseVideoDevice();
        }
    }

    /**
     * 关闭采集
     *
     * @return
     */
    public int closeCapture() {
        if (nativeInt) {
            LiveManager.Close();
        }
        closeAudioVideoCapture();
        return 0;
    }


    public void onActivityResume() {
        if (!destroy) {

//            createCapture(currentMicIndex);
//            if (m_oVideoThread == null
//                    || !m_oVideoThread.isAlive()) {
//                m_oVideoThread = new VideoThread();
//                m_oVideoThread.start();
//            }
//
//            if (null == m_oAudioThread || !m_oAudioThread
//                    .isAlive()) {
//                m_oAudioThread = new AudioThread();
//                m_oAudioThread.start();
//            }
        }
    }

    public void onActivityPause() {
        if (!destroy) {
//            closeAudioVideoCapture();
        }
    }


    /**
     * 开始采信
     *
     * @return
     */
    public int createCapture(short micIndex) {
        m_aiBufferLength = new int[1];
        try {
            AudioCaptureInterface.OpenAudioDeviceReturn retAudio = m_oAudioCapture.OpenAudioDevice(
                    48000,
                    2,
                    16, m_aiBufferLength);

            if (retAudio == AudioCaptureInterface.OpenAudioDeviceReturn.OPEN_DEVICE_SUCCESS) {
                if (null == m_oAudioThread || m_oAudioThread.isAlive() == false) {
                    m_oAudioThread = new AudioThread();
                    m_oAudioCapture.StartAudioCapture();
                    m_oAudioThread.start();
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
            return 0;
        }
        switchCamera(curCameraType, micIndex);
        return 0;
    }

    @Override
    public void surfaceCreated(SurfaceHolder surfaceHolder) {
        createCapture((short) 0);
    }

    @Override
    public void surfaceChanged(SurfaceHolder surfaceHolder, int i, int i1, int i2) {

    }

    @Override
    public void surfaceDestroyed(SurfaceHolder surfaceHolder) {

    }


    /**
     * 视频采集线程
     */
    class VideoThread extends Thread {

        public volatile boolean m_bExit = false;
        public int scalePix = 0;
        byte[] m_nv21Data = new byte[mVideoSizeConfig.srcFrameWidth
                * mVideoSizeConfig.srcFrameHeight * 3 / 2];
        byte[] m_CutData = new byte[(mVideoSizeConfig.desWidth + scalePix)
                * (mVideoSizeConfig.desHeight) * 3 / 2];
        //        byte[] m_ScaleData = new byte[mVideoSizeConfig.desWidth
//                * mVideoSizeConfig.desHeight * 3 / 2];
        byte[] m_RoatepData = new byte[mVideoSizeConfig.srcFrameWidth
                * mVideoSizeConfig.srcFrameHeight * 3 / 2];
        byte[] m_MirrorData = new byte[(mVideoSizeConfig.desWidth + scalePix)
                * mVideoSizeConfig.desHeight * 3 / 2];
        byte[] m_420pData = new byte[(mVideoSizeConfig.desWidth + scalePix)
                * mVideoSizeConfig.desHeight * 3 / 2];
        //        byte[] argbBuf = new byte[(mVideoSizeConfig.desWidth + scalePix)
//                * mVideoSizeConfig.desHeight * 4];
        byte[] smallCut = new byte[368 * 640 * 3 / 2];


        @Override
        public void run() {
            // TODO Auto-generated method stub
            super.run();

            VideoCaptureInterface.GetFrameDataReturn ret;
            while (!m_bExit) {
                try {
                    Thread.sleep(1, 10);
                    if (m_bExit) {
                        break;
                    }
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }

                ret = m_oVideoCapture.GetFrameData(m_nv21Data,
                        m_nv21Data.length);
                if (ret == VideoCaptureInterface.GetFrameDataReturn.RET_SUCCESS) {
                    frameCount++;
                    encodeVideo(m_nv21Data, m_nv21Data.length, mVideoSizeConfig.srcFrameWidth, mVideoSizeConfig.srcFrameHeight);
                    Log.d("video size:-------->", m_nv21Data.length + "");
                }
            }
        }


        public void stopThread() {
            m_bExit = true;
        }
    }


    /**
     * 音频采集线程
     */
    class AudioThread extends Thread {

        public volatile boolean m_bExit = false;

        @Override
        public void run() {
            // TODO Auto-generated method stub
            super.run();

            int[] dataLength;
            byte[] audioBuffer;
            AudioCaptureInterface.GetAudioDataReturn ret;

            dataLength = new int[1];
            audioBuffer = new byte[m_aiBufferLength[0]];


            while (!m_bExit) {
                try {
                    Thread.sleep(1, 10);
                    if (m_bExit) {
                        break;
                    }
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                try {
                    ret = m_oAudioCapture.GetAudioData(audioBuffer,
                            m_aiBufferLength[0], dataLength);
                    if (ret == AudioCaptureInterface.GetAudioDataReturn.RET_SUCCESS) {
                        encodeAudio(audioBuffer, dataLength[0]);
                        Log.d("audio size:-------->", dataLength[0] + "");
                    }

                } catch (Exception e) {
                    e.printStackTrace();
                    stopThread();
                }
            }
        }

        public void stopThread() {
            m_bExit = true;
            if (null != stream) {
                try {
                    stream.flush();
                    stream.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
    }

    public void encodeVideo(byte[] videoBuffer, int length, int w, int h) {
        try {
            //   videoOps.write(videoBuffer);
            //  videoOps.flush();
            if (curCameraType == VideoCaptureInterface.CameraDeviceType.CAMERA_FACING_FRONT) {
                LiveManager.EncodeH264(videoBuffer, length, 1);
            } else if (curCameraType == VideoCaptureInterface.CameraDeviceType.CAMERA_FACING_BACK) {
                LiveManager.EncodeH264(videoBuffer, length, 0);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }


    public void encodeAudio(byte[] audioBuffer, int length) {
        try {
//            audioOps.write(audioBuffer);
//            audioOps.flush();
            LiveManager.EncodeAAC(audioBuffer, length);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }


    // 切换摄像头
    public void switchCamera(int type, short micIndex) {
        try {
            if (s_lCameraInfoList == null) {
                s_lCameraInfoList = m_oVideoCapture.EnumerateCameraDevice();
            }
            int[] iBufferLen = new int[1];
            int[] iOpenResult = new int[1];
            if (s_lCameraInfoList.size() == 1) {
                singleCamera = true;
                if (s_lCameraInfoList.get(0).m_nType != type) {
                    curCameraType = CAMERA_FACING_BACK;
                }
            } else {
                for (int i = 0; i < s_lCameraInfoList.size(); i++) {
                    if (s_lCameraInfoList.get(i).m_nType == type) {
                        break;
                    }
                }
            }
            openCamera(iBufferLen, curCameraType);
        } catch (Exception e) {
            e.printStackTrace();
        } finally {

        }
    }


    public boolean openCamera(int[] iBufferLen, int i) {
        m_oVideoCapture.CloseVideoDevice();
        VideoCaptureInterface.OpenVideoDeviceReturn retVideo = null;
        retVideo = m_oVideoCapture
                .OpenVideoDevice(
                        i,
                        mVideoSizeConfig.srcFrameWidth,
                        mVideoSizeConfig.srcFrameHeight,
                        25,
                        iBufferLen);

        if (retVideo == VideoCaptureInterface.OpenVideoDeviceReturn.OPEN_DEVICE_SUCCESS) {
            maxZoomLevel = s_lCameraInfoList.get(i).m_nMaxZoom;
            currentZoomLevel = 0;
            isLight = false;
            m_sCameraID = (short) i;
            m_oVideoCapture.SetSurfaceHolder(mSurfaceHolder);
            m_oVideoCapture.StartVideoCapture();
            if (m_oVideoThread == null
                    || m_oVideoThread.isAlive() == false) {
                m_oVideoThread = new VideoThread();
                m_oVideoThread.start();
            }
            return true;
        }
        return false;
    }

    /**
     * 闪光灯
     *
     * @param
     */
    public boolean Light() {
        if (curCameraType == VideoCaptureInterface.CameraDeviceType.CAMERA_FACING_FRONT) {
            return false;
        }
        if (null != m_oVideoCapture) {
            isLight = !isLight;
            m_oVideoCapture.TurnFlash(isLight);
        }
        return true;

    }

    /**
     * 切换摄像头
     */
    public void SwitchCamera() {
        if (singleCamera) {
            return;
        }
        if (curCameraType == VideoCaptureInterface.CameraDeviceType.CAMERA_FACING_FRONT) {
            curCameraType = CAMERA_FACING_BACK;
            LiveManager.SetCameraID(LiveManager.CameraID.BACK.ordinal());
        } else {
            curCameraType = VideoCaptureInterface.CameraDeviceType.CAMERA_FACING_FRONT;
            LiveManager.SetCameraID(LiveManager.CameraID.FRONT.ordinal());
        }
        switchCamera(curCameraType, currentMicIndex);
    }


}
