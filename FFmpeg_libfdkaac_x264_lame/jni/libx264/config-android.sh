# !/bin/bash
#x264 build script

NDK_ROOT_PATH=/Applications/android-ndk-r13
TARGET_HOST=armv7a-linux-androideabi
SYSROOT_ARCH_DIRNAME=arch-arm
OPTIMIZE_OPTION="-O3 -DANDROID -march=armv7-a -mfloat-abi=softfp -mfpu=neon"
TOOLCHAIN_PREFIX=${NDK_ROOT_PATH}/toolchains/arm-linux-androideabi-${TOOLCHAIN_VERSION}/prebuilt/darwin-x86_64/bin/arm-linux-androideabi-


SYSROOT=${NDK_ROOT_PATH}/platforms/${LIBMEDIA_TARGET_PLATFORM}/${SYSROOT_ARCH_DIRNAME}
PREFIX=${PWD}/../out/${LIBMEDIA_TARGET_ARCH_ABI}

./configure --prefix=$PREFIX ${CONFIG_OPTION} \
--disable-opencl -disable-gpac --disable-cli \
--enable-pic \
--enable-static \
--host=${TARGET_HOST} \
--cross-prefix=${TOOLCHAIN_PREFIX} \
--sysroot=$SYSROOT \
--extra-cflags="${OPTIMIZE_OPTION}"  \
--extra-ldflags="-Wl,--no-undefined -Wl,-dynamic-linker=/system/bin/linker -lc -lm -lgcc"

make clean
make
make install
