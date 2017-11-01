/*
 * Copyright (c) javandoc 2017
 * @author javandoc
 */

package com.guagua.nativeapp;

import android.app.Application;

/**
 * Created by android on 10/31/17.
 */

public class MyApplication extends Application {
    static {
        System.loadLibrary("Media");
    }

}
