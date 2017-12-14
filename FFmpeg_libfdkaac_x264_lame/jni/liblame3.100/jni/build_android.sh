#!/bin/bash

ndk_build(){
	echo "==== ndk build lame ==="
   ndk-build clean && ndk-build
   
   #sudo chmod 777 ../obj/local/${LIBMEDIA_TARGET_ARCH_ABI}/liblame.a
   
   sudo cp ../obj/local/${LIBMEDIA_TARGET_ARCH_ABI}/liblame.a ../../out/${LIBMEDIA_TARGET_ARCH_ABI}/lib/
}

ndk_build
