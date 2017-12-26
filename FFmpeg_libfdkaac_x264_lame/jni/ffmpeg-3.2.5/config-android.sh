#!/bin/bash
#此脚本用于构建ffmpeg静态库，使用linux版本的ndk，构建系统推荐使用ubuntu14.4 64bit TLS，不要使用cygwin
#set -x

#x264 aac  lame


export TMPDIR=$FFMPEG_TMPDIR

SYSROOT_ARCH_DIRNAME=arch-arm
TOOLCHAIN_PREFIX=${NDK_ROOT_PATH}/toolchains/arm-linux-androideabi-${TOOLCHAIN_VERSION}/prebuilt/darwin-x86_64/bin/arm-linux-androideabi-
TARGET_ARCH_OPTION="--arch=arm --cpu=armv7-a --enable-neon" 


PREFIX=${PWD}/../out/${LIBMEDIA_TARGET_ARCH_ABI}/lib/
SYSROOT=${NDK_ROOT_PATH}/platforms/${LIBMEDIA_TARGET_PLATFORM}/${SYSROOT_ARCH_DIRNAME}



echo "********support X264,fdk-aac,lame codec******"
./configure --prefix=$PREFIX --enable-pic \
--enable-gpl --enable-nonfree \
--enable-cross-compile \
--cross-prefix=${TOOLCHAIN_PREFIX} \
--sysroot=${SYSROOT} \
--target-os=android \
${TARGET_ARCH_OPTION} \
--enable-gpl \
--enable-shared \
--enable-cross-compile \
--enable-neon \
--disable-shared \
--enable-static \
--enable-version3 \
--enable-pthreads \
--enable-small \
--disable-vda \
--disable-iconv \
--enable-encoders \
--enable-libx264 \
--enable-neon \
--enable-yasm \
--enable-libfdk_aac \
--enable-encoder=libx264 \
--enable-encoder=libfdk_aac \
--enable-encoder=mjpeg \
--enable-encoder=png \
--enable-nonfree \
--disable-muxers \
--enable-muxer=adts \
--enable-muxer=h264 \
--enable-muxer=mp4 \
--enable-muxer=flv \
--enable-muxer=pcm_s16le \
--enable-parser=aac \
--enable-parser=aac_latm \
--enable-parser=h264 \
--enable-decoders \
--enable-decoder=mp3 \
--enable-demuxer=mp3 \
--enable-muxer=mp3 \
--enable-encoder=libmp3lame \
--enable-libmp3lame \
--enable-demuxers \
--enable-parsers \
--enable-protocols \
--enable-zlib \
--enable-avfilter \
--disable-outdevs \
--enable-ffprobe \
--enable-ffplay \
--enable-ffmpeg \
--enable-ffserver \
--enable-debug \
--enable-postproc \
--enable-avdevice \
--disable-symver \
--disable-stripping \
--disable-zlib \
--disable-debug \
--extra-cflags="-DANDROID -I${PREFIX}/include" \
--extra-ldflags="-L${PREFIX}/lib -lx264 -lfdk-aac -llame -Wl,-dynamic-linker=/system/bin/linker -lc -lm -llog" 

#-lx264 -lfdk-aac -llame  链接库  -L 只是加入搜索库的路径，-l指定要链接的库 -lx64x64, x264就是库的名字
#make clean and disclean can't delete the bellow 4 files,so delete them manual
rm -f compat/strtod.o compat/strtod.d  compat/msvcrt/snprintf.o compat/msvcrt/snprintf.d
make clean
make

#编译的静态库(.a)放在$PREFIX/lib目录下,模块对外头文件存放在$PREFIX/lib目录下
make install

