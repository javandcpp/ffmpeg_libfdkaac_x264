/*
 * Copyright (c) javandoc 2017
 * @author javandoc
 */

package com.guagua.nativeapp;

/**
 * Created by android on 11/16/17.
 */

public class LibVideoProcess {

    /**
     * nv21 convert to I420
     *
     * @param in_width
     * @param in_height
     * @param srcData
     * @param dstData
     * @return
     */
    public static native int NV21TOI420(int in_width, int in_height,
                                        byte[] srcData,
                                        byte[] dstData);

    /**
     * mirror I420
     */
    public static native int MirrorI420(int in_width, int in_height,
                                        byte[] srcData,
                                        byte[] dstData);

    /**
     * rotate I420 by RotationMode
     */
    public static native int RotateI420(int in_width, int in_hegith,
                                        byte[] srcData,
                                        byte[] dstData, int rotationValue);

}
