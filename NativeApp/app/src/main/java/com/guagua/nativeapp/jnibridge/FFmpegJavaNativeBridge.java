package com.guagua.nativeapp.jnibridge;

/**
 * Created by android on 8/17/17.
 */

public class FFmpegJavaNativeBridge {



    public static native int encode2AAC(byte []data);


    public static native int prepareInitAACEncode(String path,String fileName);


    public static native int endAACEncode();

    public static native int releaseAACEncode();




}
