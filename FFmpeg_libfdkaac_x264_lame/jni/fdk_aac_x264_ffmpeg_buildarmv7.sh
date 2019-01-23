#!/bin/bash

LIBMEDIA_TARGET_PLATFORM=android-14
NDK_ROOT_PATH=/Applications/android-ndk-r13
TOOLCHAIN_VERSION=4.9
FFMPEG_TMPDIR=/Users/loneswordman/Desktop/ffmpegtemp

export LIBMEDIA_TARGET_PLATFORM LIBMEDIA_TARGET_ARCH_ABI FFMPEG_TMPDIR NDK_ROOT_PATH TOOLCHAIN_VERSION
LIBMEDIA_TARGET_ARCH_ABI=armeabi-v7a #armeabi



build_aac(){
	echo "====build aac==="	
  cd fdk-aac-0.1.5 && ./config-android.sh && cd ..
}

build_x264(){
	echo "====build x264==="
	cd libx264 && ./config-android.sh && cd ..
}

build_lame(){
	echo "====build lame==="
	#cd lame-3.98.4 && ./config-android.sh && cd ..
	cd liblame3.100/jni && ./build_android.sh && cd .. && cd ..
}

build_ffmpeg(){
	echo "====build ffmpeg==="
	cd ffmpeg-3.2.5 && ./config-android.sh && cd ..
}




build_module(){
	echo "==== build libffmpeg with_x264_aac_lame ==="
   cp out/Android.mk  out/${LIBMEDIA_TARGET_ARCH_ABI}/
   ndk-build clean && ndk-build all
}


# build_aac

build_x264

# build_lame

# build_ffmpeg

# build_module