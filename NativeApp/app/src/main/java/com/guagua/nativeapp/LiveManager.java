/*
 * Copyright (c) javandoc 2017
 * @author javandoc
 */

package com.guagua.nativeapp;

/**
 * 媒体处理(直播推流)
 * Created by android on 11/12/17.
 */

public class LiveManager {

    enum CameraID {
        FRONT, BACK
    }

    /**
     * 初始化音频编码器
     */
    public static native int InitAudioEncoder();

    /**
     * 初始化视频编码器
     */
    public static native int InitVideoEncoder();

    /**
     * 初始化音频采集
     *
     * @param channles
     * @param SampleRate
     * @param SampleBitRate
     * @return
     */
    public static native int InitAudioCapture(int channles, int SampleRate, int SampleBitRate);

    /**
     * 初始化视频采集
     *
     * @param inWidth
     * @param inHeight
     * @param outWidth
     * @param outHeight
     * @param fps
     * @param mirror
     * @return
     */
    public static native int InitVideoCapture(int inWidth, int inHeight, int outWidth, int outHeight, int fps, boolean mirror);

    /**
     * 开启推流
     *
     * @param url
     * @return
     */
    public static native int StartPush(String url);

    /**
     * 关闭推流
     *
     * @return
     */
    public static native int Close();

    /**
     * 底层资源回收与释放
     *
     * @return
     */
    public static native int Release();

    /**
     * 视频滤镜
     *
     * @param filterType
     * @param filterValue
     * @return
     */
    public static native int Filter(FilterType filterType, int filterValue);

    /**
     * 上传音频数据编码AAC
     *
     * @param audioBuffer
     * @param length
     * @return
     */
    public static native int EncodeAAC(byte[] audioBuffer, int length);

    /**
     * 上传视频数据编码H264
     *
     * @param videoBuffer
     * @param length
     * @param i
     * @return
     */
    public static native int EncodeH264(byte[] videoBuffer, int length, int i);


    /**
     * 设置摄像头
     *
     * @param cameraID
     */
    public static native void SetCameraID(int cameraID);
}
