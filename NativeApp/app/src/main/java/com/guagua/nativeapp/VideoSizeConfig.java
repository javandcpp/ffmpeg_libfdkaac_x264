/*
 * Copyright (c) javandoc 2017
 * @author javandoc
 */

package com.guagua.nativeapp;

/**
 * Created by android on 10/31/17.
 */

public class VideoSizeConfig {


    public short srcFrameWidth = 640;
    public short srcFrameHeight = 480;


    public short desWidth = 368;
    public short desHeight = 640;

    @Override
    public String toString() {
        return "VideoSizeConfig{" +
                "srcFrameWidth=" + srcFrameWidth +
                ", srcFrameHeight=" + srcFrameHeight +
                ", desWidth=" + desWidth +
                ", desHeight=" + desHeight +
                '}';
    }


}
