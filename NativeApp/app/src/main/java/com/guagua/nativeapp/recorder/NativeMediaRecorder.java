package com.guagua.nativeapp.recorder;

import android.annotation.SuppressLint;
import android.annotation.TargetApi;
import android.graphics.ImageFormat;
import android.hardware.Camera;
import android.media.AudioFormat;
import android.media.AudioRecord;
import android.media.MediaRecorder;
import android.os.Build;
import android.os.Environment;
import android.util.Log;
import android.view.SurfaceHolder;

import com.guagua.nativeapp.DeviceUtils;
import com.guagua.nativeapp.StringUtils;
import com.guagua.nativeapp.jnibridge.FFmpegJavaNativeBridge;
import com.guagua.nativeapp.nativehandler.NativeCrashHandler;

import java.io.DataOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.Collections;
import java.util.List;

/**
 * Created by android on 8/17/17.
 */

public class NativeMediaRecorder extends BaseMediaRecorder implements SurfaceHolder.Callback, Camera.PreviewCallback {

    public static int SMALL_VIDEO_HEIGHT = 480;
    public static int SMALL_VIDEO_WIDTH = 360;
    /**
     * 最大帧率
     */
    protected static int MAX_FRAME_RATE = 20;
    /**
     * 最小帧率
     */
    protected static int MIN_FRAME_RATE = 8;

    protected static int CAPTURE_THUMBNAILS_TIME = 1;

    private final IMediaCallback iMediaCallback;
    private AudioRecord mAudioRecord;
    private Thread recordThread;
    private DataOutputStream dataOutputStream;
    private File mDestDir;
    private boolean mRecording;
    private boolean mSurfaceCreated;
    private boolean mPrepared;
    private SurfaceHolder mSurfaceHolder;
    private boolean mStartPreview;
    private int mCameraId=Camera.CameraInfo.CAMERA_FACING_FRONT;
    private Camera camera;
    private Camera.Parameters mParameters;
    public List<Camera.Size> mSupportedPreviewSizes;
    private int mFrameRate;
    public static int mSupportedPreviewWidth;
    private boolean NEED_FULL_SCREEN;

    public NativeMediaRecorder(IMediaCallback iMediaCallback) {
        this.iMediaCallback = iMediaCallback;
        NativeCrashHandler nativeCrashHandler = new NativeCrashHandler();
        nativeCrashHandler.registerForNativeCrash(iMediaCallback.getContext());
    }

    public void setPath(String path, String fileName) throws Exception {
        mDestDir = new File(path, fileName);
        if (mDestDir.exists()) {
            mDestDir.delete();
        }

        mDestDir.createNewFile();

    }

    @SuppressLint("NewApi")
    @TargetApi(Build.VERSION_CODES.GINGERBREAD)
    public static boolean isSupportFrontCamera() {
        if (!DeviceUtils.hasGingerbread()) {
            return false;
        }
        int numberOfCameras = Camera.getNumberOfCameras();
        if (2 == numberOfCameras) {
            return true;
        }
        return false;
    }


    /**
     * 开始预览
     */
    public void prepare() {
        mPrepared = true;
        if (mSurfaceCreated) {
            startPreview();
        }
    }

    public void stopPreview() {
        if (camera != null) {
            try {
                camera.stopPreview();
                camera.setPreviewCallback(null);
                // camera.lock();
                camera.release();
            } catch (Exception e) {
                Log.e("tag", "stopPreview...");
            }
            camera = null;
        }
        mPrepared = false;
        mStartPreview = false;
    }


    private void startPreview() {
        if (mStartPreview || mSurfaceHolder == null || !mPrepared)
            return;
        else
            mStartPreview = true;

        try {

            if (mCameraId == Camera.CameraInfo.CAMERA_FACING_BACK)
                camera = Camera.open();
            else
                camera = Camera.open(mCameraId);

            camera.setDisplayOrientation(90);
            try {
                camera.setPreviewDisplay(mSurfaceHolder);
            } catch (IOException e) {
                if (iMediaCallback != null) {
                    iMediaCallback.onVideoError();
                }
                Log.e("tag", "setPreviewDisplay fail " + e.getMessage());
            }

            //设置摄像头参数
            mParameters = camera.getParameters();
            mSupportedPreviewSizes = mParameters.getSupportedPreviewSizes();//	获取支持的尺寸
            prepareCameraParaments();
            camera.setParameters(mParameters);
            setPreviewCallback();
            camera.startPreview();

//            onStartPreviewSuccess();
            if (iMediaCallback != null)
                iMediaCallback.onPrepared();
        } catch (Exception e) {
            e.printStackTrace();
            if (iMediaCallback != null) {
                iMediaCallback.onVideoError();
            }
            Log.e("tag", "startPreview fail :" + e.getMessage());
        }

    }

    /**
     * 设置回调
     */
    protected void setPreviewCallback() {
        Camera.Size size = mParameters.getPreviewSize();
        if (size != null) {
            int buffSize = size.width * size.height * 3 / 2;
            try {
                camera.addCallbackBuffer(new byte[buffSize]);
                camera.addCallbackBuffer(new byte[buffSize]);
                camera.addCallbackBuffer(new byte[buffSize]);
                camera.setPreviewCallbackWithBuffer(this);
            } catch (OutOfMemoryError e) {
                Log.e("tag", "startPreview...setPreviewCallback...", e);
            }
            Log.e("tag", "startPreview...setPreviewCallbackWithBuffer...width:" + size.width + " height:" + size.height);
        } else {
            camera.setPreviewCallback(this);
        }
    }


    protected void prepareCameraParaments() {
        if (mParameters == null)
            return;
        List<Integer> rates = mParameters.getSupportedPreviewFrameRates();
        if (rates != null) {
            if (rates.contains(MAX_FRAME_RATE)) {
                mFrameRate = MAX_FRAME_RATE;
            } else {
                boolean findFrame = false;
                Collections.sort(rates);
                for (int i = rates.size() - 1; i >= 0; i--) {
                    if (rates.get(i) <= MAX_FRAME_RATE) {
                        mFrameRate = rates.get(i);
                        findFrame = true;
                        break;
                    }
                }
                if (!findFrame) {
                    mFrameRate = rates.get(0);
                }
            }
        }

        mParameters.setPreviewFrameRate(mFrameRate);
        // mParameters.setPreviewFpsRange(15 * 1000, 20 * 1000);
        boolean findWidth = false;
        for (int i = mSupportedPreviewSizes.size() - 1; i >= 0; i--) {
            Camera.Size size = mSupportedPreviewSizes.get(i);
            if (size.height == SMALL_VIDEO_HEIGHT) {

                mSupportedPreviewWidth = size.width;
                checkFullWidth(mSupportedPreviewWidth, SMALL_VIDEO_WIDTH);
                findWidth = true;
                break;
            }
        }
        if (!findWidth) {
            Log.e("tag", "传入高度不支持或未找到对应宽度,请按照要求重新设置，否则会出现一些严重问题");
            mSupportedPreviewWidth = 640;
            checkFullWidth(640, 360);
            SMALL_VIDEO_HEIGHT = 480;
        }
        mParameters.setPreviewSize(640,368);

        // 设置输出视频流尺寸，采样率
        mParameters.setPreviewFormat(ImageFormat.NV21);

        //设置自动连续对焦
        String mode = getAutoFocusMode();
        if (StringUtils.isNotEmpty(mode)) {
            mParameters.setFocusMode(mode);
        }

        //设置人像模式，用来拍摄人物相片，如证件照。数码相机会把光圈调到最大，做出浅景深的效果。而有些相机还会使用能够表现更强肤色效果的色调、对比度或柔化效果进行拍摄，以突出人像主体。
        //		if (mCameraId == Camera.CameraInfo.CAMERA_FACING_FRONT && isSupported(mParameters.getSupportedSceneModes(), Camera.Parameters.SCENE_MODE_PORTRAIT))
        //			mParameters.setSceneMode(Camera.Parameters.SCENE_MODE_PORTRAIT);

        if (isSupported(mParameters.getSupportedWhiteBalance(), "auto"))
            mParameters.setWhiteBalance("auto");

        //是否支持视频防抖
        if ("true".equals(mParameters.get("video-stabilization-supported")))
            mParameters.set("video-stabilization", "true");

        //		mParameters.set("mRecording-hint", "false");
        //
        //		mParameters.setFocusMode(Camera.Parameters.FOCUS_MODE_CONTINUOUS_PICTURE);
        if (!DeviceUtils.isDevice("GT-N7100", "GT-I9308", "GT-I9300")) {
            mParameters.set("cam_mode", 1);
            mParameters.set("cam-mode", 1);
        }
    }


    public void setSurfaceHolder(SurfaceHolder surfaceHolder) {
        if (null != surfaceHolder) {
            surfaceHolder.addCallback(this);
            if (!DeviceUtils.hasHoneycomb()) {
                surfaceHolder.setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS);
            }
        }
    }

    public boolean isRecording() {
        return mRecording;
    }

    public void stopAudioRecord() {
        if (null != recordThread) {
            recordThread.interrupt();
            try {
                recordThread.join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            recordThread = null;
        }
    }

    public void startRecord() {
        synchronized (this) {
            stopAudioRecord();
            int minBufferSize = AudioRecord.getMinBufferSize(44100, AudioFormat.CHANNEL_IN_MONO, AudioFormat.ENCODING_PCM_16BIT);
            if (AudioRecord.ERROR_BAD_VALUE == minBufferSize) {
                iMediaCallback.onAudioRecordError(BaseMediaRecorder.AUDIO_RECORD_ERROR_GET_MIN_BUFFER_SIZE_NOT_SUPPORT, "parameters are not supported by the hardware.");
                return;
            }
            File destDir = new File(Environment.getExternalStorageDirectory().getAbsolutePath(), "media");
            if (!destDir.exists()) {
                destDir.mkdirs();
            }

            FFmpegJavaNativeBridge.prepareInitFFmpegEncode(destDir.getAbsolutePath());
            mAudioRecord = new AudioRecord(MediaRecorder.AudioSource.MIC, 44100, AudioFormat.CHANNEL_IN_MONO, AudioFormat.ENCODING_PCM_16BIT, minBufferSize);

            if (null == mAudioRecord) {
                iMediaCallback.onAudioRecordError(BaseMediaRecorder.AUDIO_RECORD_ERROR_CREATE_FAILED, "new AudioRecord failed.");
                return;
            }
            recordThread = new Thread() {
                @Override
                public void run() {
                    try {
                        dataOutputStream = new DataOutputStream(new FileOutputStream(mDestDir, true));
                        mAudioRecord.startRecording();
                    } catch (Exception e) {
                        mRecording = false;
                        iMediaCallback.onAudioRecordError(BaseMediaRecorder.AUDIO_RECORD_ERROR_UNKNOWN, "startRecording failed.");
                        return;
                    }
                    byte[] sampleBuffer = new byte[2048];
                    try {
                        while (!Thread.currentThread().isInterrupted()) {
                            int result = mAudioRecord.read(sampleBuffer, 0, 2048);
                            if (result > 0) {
                                dataOutputStream.write(sampleBuffer, 0, result);
                                Log.d("record audio:",result+"");
                                mRecording = true;
                                iMediaCallback.receiveAudioData(sampleBuffer, result);
                            }
                        }
                    } catch (Exception e) {
                        String message = "";
                        if (e != null)
                            message = e.getMessage();
                        mRecording = false;
                        iMediaCallback.onAudioRecordError(BaseMediaRecorder.AUDIO_RECORD_ERROR_UNKNOWN, message);

                    }
                    try {
                        if (null != dataOutputStream) {
                            dataOutputStream.close();
                            dataOutputStream = null;
                        }
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                    mRecording = false;
                    mAudioRecord.release();
                    mAudioRecord = null;


                }
            };
            recordThread.start();
        }
    }

    @Override
    public void surfaceCreated(SurfaceHolder surfaceHolder) {
        mSurfaceHolder = surfaceHolder;
        mSurfaceCreated = true;
        if (mPrepared && !mStartPreview) {
            startPreview();
        }
    }

    @Override
    public void surfaceChanged(SurfaceHolder surfaceHolder, int i, int i1, int i2) {
        this.mSurfaceHolder = surfaceHolder;
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder surfaceHolder) {
        mSurfaceHolder = null;
        mSurfaceCreated = false;
    }

    /**
     * 视频采集数据
     *
     * @param data
     * @param camera
     */
    @Override
    public void onPreviewFrame(byte[] data, Camera camera) {
        if (mRecording && null != iMediaCallback) {
            iMediaCallback.receiveVideoData(data);
        }
        Log.e("tag","capture");
        camera.addCallbackBuffer(data);
    }

    /**
     * 检测是否支持指定特性
     */
    private boolean isSupported(List<String> list, String key) {
        return list != null && list.contains(key);
    }


    private void checkFullWidth(int trueValue, int falseValue) {
        if (NEED_FULL_SCREEN) {
            SMALL_VIDEO_WIDTH = trueValue;
        } else {
            SMALL_VIDEO_WIDTH = falseValue;
        }
    }

    /**
     * 连续自动对焦
     */
    private String getAutoFocusMode() {
        if (mParameters != null) {
            //持续对焦是指当场景发生变化时，相机会主动去调节焦距来达到被拍摄的物体始终是清晰的状态。
            List<String> focusModes = mParameters.getSupportedFocusModes();
            if ((Build.MODEL.startsWith("GT-I950") || Build.MODEL.endsWith("SCH-I959") || Build.MODEL.endsWith("MEIZU MX3")) && isSupported(focusModes, "continuous-picture")) {
                return "continuous-picture";
            } else if (isSupported(focusModes, "continuous-video")) {
                return "continuous-video";
            } else if (isSupported(focusModes, "auto")) {
                return "auto";
            }
        }
        return null;
    }
}
