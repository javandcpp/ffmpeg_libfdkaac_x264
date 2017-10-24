package com.guagua.nativeapp.jnibridge;

/**
 * Created by android on 8/17/17.
 */

public class FFmpegJavaNativeBridge {


    /**
     * 编码一帧音频
     * @param data
     * @return
     */
    public static native int encode2AAC(byte []data);

    /**
     * 编码一帧视频
     * @param data
     * @return
     */
    public static native int encodeFrame2H264(byte[] data);

    public static native int prepareInitAACEncode(String path,String fileName);


    public static native int prepareInitFFmpegEncode(String basePath);


    public static native int endRecord();

    public static native int releaseRecord();

    public static native int yuv420Image2YUV(String path,int w,int h);
    public static native int yuvTOGrayYUV(String path,int w,int h);

    /**
     * 推流
     * @param inputUri
     * @param outputUri
     * @return
     */
    public static native int pushStream(String inputUri,String outputUri);

    /**
     * 拉流
     * @return
     */
    public static native int pullStream(String inputUri,String outputUri);


    public static native int transStream(String inputUri,String outputUri);

    public static native int stopPush();
    public static native int stopPull();
    public static native int stopTrans();

    public static native int releaseTransStream();

    public static native int decode(String inputuri);

}
