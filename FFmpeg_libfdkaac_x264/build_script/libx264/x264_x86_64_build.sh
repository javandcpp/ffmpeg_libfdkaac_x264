
NDK=/Applications/android-ndk-r13

PLATFORM=$NDK/platforms/android-21/arch-x86_64/
TOOLCHAIN=$NDK/toolchains/x86_64-4.9/prebuilt/darwin-x86_64
PREFIX=./android/x86_64

function build_one
{
./configure \
--prefix=$PREFIX \
--disable-shared \
--enable-static \
--disable-asm \
--enable-pic \
--enable-strip \
--host=x86_64-linux \
--cross-prefix=$TOOLCHAIN/bin/x86_64-linux-android- \
--sysroot=$PLATFORM \
--extra-cflags="-Os -fpic" \
--extra-ldflags="" \

make clean
make
make install
}

build_one

