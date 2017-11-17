/*
 * Copyright (c) javandoc 2017
 * @author javandoc
 */

package com.guagua.nativeapp;

/**
 * Created by android on 11/16/17.
 */
public enum RotationMode {

    kRotate0(0),  // No rotation.
    kRotate90(90),  // Rotate 90 degrees clockwise.
    kRotate180(180),  // Rotate 180 degrees.
    kRotate270(270);  // Rotate 270 degrees clockwise
    public int angleValue = 0;

    RotationMode(int angle) {
        angleValue = angle;
    }


}
