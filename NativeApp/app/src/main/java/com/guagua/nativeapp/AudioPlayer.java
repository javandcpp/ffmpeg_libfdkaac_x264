/*
 * Copyright (c) javandoc 2017
 * @author javandoc
 */

package com.guagua.nativeapp;

/**
 * Created by android on 11/7/17.
 */

import java.io.FileOutputStream;


import java.io.File;

import android.media.AudioFormat;
import android.media.AudioManager;
import android.media.AudioTrack;

public class AudioPlayer extends Thread {
    private AudioPlayer rtpMobilePlayer;
    private AudioTrack audioTrack;
    private boolean isRunning;
    private boolean isPause;
    private int anRequirLength = 1024 * 8;//需要读取的长度
    private byte[] aszPackBuff;

    public AudioPlayer() {
    }

    public void initAudio() {
        isRunning = true;
        isPause = false;
        aszPackBuff = new byte[anRequirLength];//缓冲区
        int streamType = AudioManager.STREAM_MUSIC;
        int sampleRateInHz = 48000;
        int channelConfig = AudioFormat.CHANNEL_OUT_STEREO;
        int audioFormat = AudioFormat.ENCODING_PCM_16BIT;
        int bufferSizeInBytes = 1024 * 100;
        int mode = AudioTrack.MODE_STREAM;
        audioTrack = new AudioTrack(streamType, sampleRateInHz, channelConfig, audioFormat, bufferSizeInBytes, mode);
    }

    public void setAudioPause(boolean pause) {
        isPause = pause;
    }

    public boolean isAudioPause() {
        return isPause;
    }

    public void audioStop() {
        if (audioTrack != null) {
            audioTrack.stop();
        }
        isRunning = false;
    }

    public void release() {
        if (audioTrack != null) {
            audioTrack.release();
        }
        isRunning = false;
    }

    @Override
    public void run() {

        audioTrack.play();
        while (isRunning) {
            super.run();
            int lenght = 0;

            if (lenght > 0 && !isPause) {
//				System.out.println("AudioStat play");
                audioTrack.write(aszPackBuff, 0, lenght);
            }
        }
    }
}


