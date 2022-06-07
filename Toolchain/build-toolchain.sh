#!/bin/sh

SCRIPT_LOCATION=$(dirname $(realpath $0))
TOOLCHAIN_LOCATION=$SCRIPT_LOCATION/Cross

export PREFIX="$SCRIPT_LOCATION/Cross"
export TARGET=x86_64-elf
export PATH="$PREFIX/bin:$PATH"

mkdir -p $TOOLCHAIN_LOCATION
cd $TOOLCHAIN_LOCATION

# Binutils
BINUTILS_FILE=binutils-2.38.tar.gz
BINUTILS_URL=https://ftp.gnu.org/gnu/binutils/$BINUTILS_FILE

if [ ! -f $BINUTILS_FILE ]; then
	wget $BINUTILS_URL
fi

mkdir -p BinutilsSource
tar -xvf $BINUTILS_FILE -C BinutilsSource --strip-components=1

mkdir -p BinutilsBuild
cd BinutilsBuild
../BinutilsSource/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
make
make install

# GCC
cd $TOOLCHAIN_LOCATION
GCC_FILE=gcc-12.1.0.tar.gz
GCC_URL=https://ftp.gnu.org/gnu/gcc/gcc-12.1.0/$GCC_FILE

if [ ! -f $GCC_FILE ]; then
	wget $GCC_URL
fi

mkdir -p GCCSource
tar -xvf $GCC_FILE -C GCCSource --strip-components=1
cp $SCRIPT_LOCATION/t-x86_64-elf GCCSource/gcc/config/i386/t-x86_64-elf
patch GCCSource/gcc/config.gcc $SCRIPT_LOCATION/config.gcc.patch

mkdir -p GCCBuild
cd GCCBuild
../GCCSource/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers -disable-libssp -with-dwarf2 -with-newlib --disable-__cxa_atexit --disable-threads --disable-shared --disable-sjlj-exceptions --enable-libstdcxx --disable-hosted-libstdcxx --disable-bootstrap
make all-gcc
make all-target-libgcc
make install-gcc
make install-target-libgcc
make all-target-libstdc++-v3
make install-target-libstdc++-v3

