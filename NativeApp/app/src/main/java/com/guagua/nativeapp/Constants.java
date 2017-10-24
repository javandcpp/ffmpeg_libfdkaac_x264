/*
 * Copyright (c) javandoc 2017
 * @author javandoc
 */

package com.guagua.nativeapp;

/**
 * Created by android on 8/31/17.
 */

public interface Constants {
//    String IP_ADDRESS="192.168.1.105";//home

    String IP_ADDRESS="192.168.24.132";//company linux

//    String IP_ADDRESS="192.168.24.153";//company mac

//    String SRC_ADDRESS="192.168.24.132";
//    String DST_ADDRESS="192.168.24.153";

    String SRC_ADDRESS="192.168.1.100";
    String DST_ADDRESS="192.168.1.104";


    enum TRANSFORMA_STEAM{

        SRC_URI(SRC_ADDRESS),DES_URI(DST_ADDRESS);

        public final String ip;

        TRANSFORMA_STEAM(String ip) {
            this.ip=ip;
        }
    }


}
