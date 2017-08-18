# Created by jianxi on 2017/6/4
# https://github.com/mabeijianxi
# mabeijianxi@gmail.com

NDK=/Applications/android-ndk-r13
ANDROID_API=android-14

SYSROOT=$NDK/platforms/$ANDROID_API/arch-arm

ANDROID_BIN=$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/darwin-x86_64/bin

CROSS_COMPILE=${ANDROID_BIN}/arm-linux-androideabi-



CPU=arm-v7a


CFLAGS=""

FLAGS="--enable-static  --host=arm-linux-androideabi --target=android --disable-asm "

export CXX="${CROSS_COMPILE}g++ --sysroot=${SYSROOT}"

export LDFLAGS=" -L$SYSROOT/usr/lib  $CFLAGS "

export CXXFLAGS=$CFLAGS

export CFLAGS=$CFLAGS

export CC="${CROSS_COMPILE}gcc --sysroot=${SYSROOT}"

export AR="${CROSS_COMPILE}ar"

export LD="${CROSS_COMPILE}ld"

export AS="${CROSS_COMPILE}gcc"


./configure $FLAGS \
--enable-pic \
--enable-strip \
--prefix=${pwd}/android/$CPU

$ADDITIONAL_CONFIGURE_FLAG
make clean
make -j16
make install
