/*
 * Copyright (c) javandoc 2017
 * @author javandoc
 */

package com.guagua.nativeapp.recorder;

import android.app.Activity;
import android.content.Context;
import android.graphics.ImageFormat;
import android.hardware.Camera;
import android.hardware.Camera.Size;
import android.os.Build;
import android.util.Log;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

import com.guagua.nativeapp.CameraUtils;
import com.guagua.nativeapp.DeviceUtils;
import com.guagua.nativeapp.DisplayUtil;
import com.guagua.nativeapp.StringUtils;
import com.guagua.nativeapp.WrapCameraSize;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

import static android.content.ContentValues.TAG;
import static com.guagua.nativeapp.recorder.NativeMediaRecorder.SMALL_VIDEO_HEIGHT;
import static com.guagua.nativeapp.recorder.NativeMediaRecorder.SMALL_VIDEO_WIDTH;
import static com.guagua.nativeapp.recorder.NativeMediaRecorder.mSupportedPreviewWidth;

/**
 * Created by android on 9/25/17.
 */

public class CaptureManager implements SurfaceHolder.Callback, Camera.PreviewCallback {


    private int degrees;
    private Context mContext;
    private boolean NEED_FULL_SCREEN;
    private final File dstFile;
    private final SurfaceView mSurfaceView;
    private int captureWidth;
    private int captureHeight;
    private int curentCameraId;
    private boolean preview;
    private SurfaceHolder mSurfaceViewHolder;
    private boolean surfaceCreated;
    private boolean surfaceDestroyed;
    private Camera.Parameters mParameters;
    private int mFrameRate;

    private int MAX_FRAME_RATE = 20;
    private List<Camera.Size> mSupportedPreviewSizes;
    private Camera camera;

    public CaptureManager(Context context, File dstFile, SurfaceView surfaceView, int captrueWidth, int captureHeight) {
        this.mContext = context;
        this.dstFile = dstFile;
        this.mSurfaceView = surfaceView;
        this.captureWidth = captrueWidth;
        this.captureHeight = captureHeight;
        mSurfaceViewHolder = mSurfaceView.getHolder();
        mSurfaceViewHolder.addCallback(this);
        if (!DeviceUtils.hasHoneycomb()) {
            mSurfaceViewHolder.setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS);
        }
        initCapture();
    }

    private void initCapture() {
        int numberOfCameras = Camera.getNumberOfCameras();//获取摄像头数量
        if (numberOfCameras == 2) {
            curentCameraId = Camera.CameraInfo.CAMERA_FACING_FRONT;
        } else {
            curentCameraId = Camera.CameraInfo.CAMERA_FACING_BACK;
        }
    }

    public void startPreview() {
        if (surfaceCreated && !surfaceDestroyed && null != mSurfaceViewHolder && !preview) {
            preview = true;
            camera = Camera.open(Camera.CameraInfo.CAMERA_FACING_BACK);
            mParameters = camera.getParameters();
            List<Integer> supportedPreviewFrameRates = mParameters.getSupportedPreviewFrameRates();
            List<Integer> supportePreviewFormats = mParameters.getSupportedPreviewFormats();
            List<Camera.Size> supportedVideoSizes = mParameters.getSupportedVideoSizes();
            mSupportedPreviewSizes = mParameters.getSupportedPreviewSizes();
//            prepareCameraParaments();
            setCameraDisplayOrientation((Activity) mContext,Camera.CameraInfo.CAMERA_FACING_BACK,camera);
            setCameraSize(mParameters,1920,1080);
            camera.setParameters(mParameters);
            setPreviewCallback();

            try {
                camera.setPreviewDisplay(mSurfaceViewHolder);
                camera.setDisplayOrientation(90);
                camera.setErrorCallback(new Camera.ErrorCallback() {
                    @Override
                    public void onError(int i, Camera camera) {
                        Log.d("capture", i + "");
                    }
                });
                camera.startPreview();


            } catch (IOException e) {
                e.printStackTrace();
            }


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

        float previewRate = DisplayUtil.getScreenRate(mContext);
        Camera.Size pictureSize = CameraUtils.getInstance().getPropPictureSize(
                mParameters.getSupportedPictureSizes(), previewRate, 800);
        mParameters.setPictureSize(pictureSize.width, pictureSize.height);
        Camera.Size previewSize = CameraUtils.getInstance().getPropPreviewSize(
                mParameters.getSupportedPreviewSizes(), previewRate, 800);
        mParameters.setPreviewSize(previewSize.width, previewSize.height);

        // 设置输出视频流尺寸，采样率
        mParameters.setPreviewFormat(ImageFormat.YV12);

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
//        if (!DeviceUtils.isDevice("GT-N7100", "GT-I9308", "GT-I9300")) {
//            mParameters.set("cam_mode", 1);
//            mParameters.set("cam-mode", 1);
//        }
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

    /**
     * 检测是否支持指定特性
     */
    private boolean isSupported(List<String> list, String key) {
        return list != null && list.contains(key);
    }


    @Override
    public void surfaceCreated(SurfaceHolder surfaceHolder) {
        surfaceCreated = true;
        surfaceDestroyed = false;
        startPreview();
    }

    @Override
    public void surfaceChanged(SurfaceHolder surfaceHolder, int i, int i1, int i2) {
        mSurfaceViewHolder = surfaceHolder;
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder surfaceHolder) {
        surfaceCreated = false;
        surfaceDestroyed = true;
    }

    @Override
    public void onPreviewFrame(byte[] bytes, Camera camera) {
//            if (mRecording&&null!=iMediaCallback) {
//                iMediaCallback.receiveVideoData(data);
//            }
        Log.d("tag", "capture");
        camera.addCallbackBuffer(bytes);
    }


    public void setCameraDisplayOrientation(Activity activity, int cameraIo, Camera camera) {

        Camera.CameraInfo info = new Camera.CameraInfo();
        Camera.getCameraInfo(cameraIo, info);
        int rotation = activity.getWindowManager().getDefaultDisplay().getRotation();
        int degress = 0;
        switch (rotation) {
            case Surface.ROTATION_0:
                degress = 0;
                break;
            case Surface.ROTATION_90:
                degress = 90;
                break;
            case Surface.ROTATION_180:
                degress = 180;
                break;
            case Surface.ROTATION_270:
                degress = 270;
                break;
        }
        int result;
        if (info.facing == Camera.CameraInfo.CAMERA_FACING_BACK) {
            result = (info.orientation + degrees) % 360;
            // 就是摄像头需要顺时针转过多少度才能恢复自然方向
            result = (360 - result) % 360;
        } else { // back-facing
            result = (info.orientation - degrees + 360) % 360;
        }
        camera.setDisplayOrientation(result);
        switch (result) {
            case 0:
            case 180:
                setCameraSize(camera.getParameters(), DisplayUtil.getScreenWidthAndHeight(mContext).widthPixels, DisplayUtil.getScreenWidthAndHeight(mContext).heightPixels);
                break;
            case 90:
            case 270:
                setCameraSize(camera.getParameters(), DisplayUtil.getScreenWidthAndHeight(mContext).widthPixels, DisplayUtil.getScreenWidthAndHeight(mContext).heightPixels);
                break;
        }
    }

    public void setCameraSize(Camera.Parameters parameters, int width, int height) {
        Map<String, List<Camera.Size>> allSizes = new HashMap<>();
        String typePreview = "typePreview";
        String typePicture = "typePicture";
        allSizes.put(typePreview, parameters.getSupportedPreviewSizes());
        allSizes.put(typePicture, parameters.getSupportedPictureSizes());
        Iterator iterator = allSizes.entrySet().iterator();
        while (iterator.hasNext()) {
            Map.Entry<String, List<Size>> entry = (Map.Entry<String, List<Size>>) iterator.next();
            List<Size> sizes = entry.getValue();
            if (sizes == null || sizes.isEmpty()) continue;
            ArrayList<WrapCameraSize> wrapCameraSizes = new ArrayList<>(sizes.size());
            for (Size size : sizes) {
                WrapCameraSize wrapCameraSize = new WrapCameraSize();
                wrapCameraSize.setWidth(size.width);
                wrapCameraSize.setHeight(size.height);
                wrapCameraSize.setD(Math.abs((size.width - width)) + Math.abs((size.height - height)));
                if (size.width == width && size.height == height) {
                    if (typePreview.equals(entry.getKey())) {
                        parameters.setPreviewSize(size.width, size.height);
                    } else if (typePicture.equals(entry.getKey())) {
                        parameters.setPictureSize(size.width, size.height);
                    }
                    Log.d(TAG, "best size: width=" + size.width + ";height=" + size.height);
                    break;
                }
                wrapCameraSizes.add(wrapCameraSize);
            }
            Log.d(TAG, "wrapCameraSizes.size()=" + wrapCameraSizes.size());
            Size resultSize = null;
            if (typePreview.equals(entry.getKey())) {
                resultSize = parameters.getPreviewSize();
            } else if (typePicture.equals(entry.getKey())) {
                resultSize = parameters.getPictureSize();
            }
            if (resultSize == null || (resultSize.width != width && resultSize.height != height)) {
                //找到相机Preview Size 和 Picture Size中最适合的大小
                if (wrapCameraSizes.isEmpty()) continue;
                WrapCameraSize minCameraSize = Collections.min(wrapCameraSizes);
                while (!(minCameraSize.getWidth() >= width && minCameraSize.getHeight() >= height)) {
                    wrapCameraSizes.remove(minCameraSize);
                    if (wrapCameraSizes.isEmpty()) break;
                    minCameraSize = null;
                    minCameraSize = Collections.min(wrapCameraSizes);
                }
                Log.d(TAG, "best min size: width=" + minCameraSize.getWidth() + ";height=" + minCameraSize.getHeight());
                if (typePreview.equals(entry.getKey())) {
                    parameters.setPreviewSize(minCameraSize.getWidth(), minCameraSize.getHeight());
                } else if (typePicture.equals(entry.getKey())) {
                    parameters.setPictureSize(minCameraSize.getWidth(), minCameraSize.getHeight());
                }
                String mode = getAutoFocusMode();
                if (StringUtils.isNotEmpty(mode)) {
                    parameters.setFocusMode(mode);
                }
                if (isSupported(mParameters.getSupportedWhiteBalance(), "auto"))
                    mParameters.setWhiteBalance("auto");

                //是否支持视频防抖
                if ("true".equals(mParameters.get("video-stabilization-supported")))
                    mParameters.set("video-stabilization", "true");

                mParameters.setPreviewFormat(ImageFormat.YV12);


            }
            iterator.remove();
        }
    }
}
