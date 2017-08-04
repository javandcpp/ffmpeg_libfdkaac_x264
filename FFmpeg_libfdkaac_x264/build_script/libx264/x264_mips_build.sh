
NDK=/Applications/android-ndk-r13

PLATFORM=$NDK/platforms/android-14/arch-mips/
TOOLCHAIN=$NDK/toolchains/mipsel-linux-android-4.9/prebuilt/darwin-x86_64
PREFIX=./android/mips

function build_one
{
./configure \
--prefix=$PREFIX \
--disable-shared \
--enable-static \
--disable-asm \
--enable-pic \
--enable-strip \
--sysroot=$PLATFORM \
--host=mipsel-linux \
--cross-prefix=$TOOLCHAIN/bin/mipsel-linux-android- \
--sysroot=$PLATFORM \
--extra-cflags="-Os -fpic" \

make clean
make
make install
}

build_one
