/*
 * Copyright (c) javandoc 2017
 * @author javandoc
 */

package com.guagua.nativeapp;

/**
 * Created by android on 10/31/17.
 */

public class MediaProcess {

    public static native int encodeH264(byte[] data,int length,int w,int h,int orientation);
    public static native int encodeAAC(byte[] data,int length);
    public static native int initEncoder();
    public static native void close();
}
