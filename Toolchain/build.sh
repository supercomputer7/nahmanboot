#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

mkdir -p "$DIR/tarballs"
mkdir -p "$DIR/build"
mkdir -p "$DIR/local"

BINUTILS_VERSION="2.36.1"
BINUTILS_NAME="binutils-$BINUTILS_VERSION"
BINUTILS_PKG="${BINUTILS_NAME}.tar.gz"
BINUTILS_BASE_URL="http://ftp.gnu.org/gnu/binutils"

GCC_VERSION="11.1.0"
GCC_NAME="gcc-$GCC_VERSION"
GCC_PKG="${GCC_NAME}.tar.gz"
GCC_BASE_URL="http://ftp.gnu.org/gnu/gcc"

LOCAL_DIR="$DIR/local/"
BUILD_DIR="$DIR/build/"
TARGET=i686-elf

pushd "$DIR/tarballs"
curl -LO "$BINUTILS_BASE_URL/$BINUTILS_PKG"
curl -LO "$GCC_BASE_URL/$GCC_NAME/$GCC_PKG"

tar xvf "$BINUTILS_PKG"
mkdir -p "$BUILD_DIR/${BINUTILS_NAME}"
    pushd "$BUILD_DIR/${BINUTILS_NAME}" 
    "$DIR/tarballs/${BINUTILS_NAME}/configure" --target=$TARGET --prefix="$LOCAL_DIR" --with-sysroot --disable-nls --disable-werror
    make
    make install
    popd

mkdir -p "$BUILD_DIR/${GCC_NAME}"
tar xvf "$GCC_PKG"
    pushd "$BUILD_DIR/${GCC_NAME}" 
    "$DIR/tarballs/${GCC_NAME}/configure" --target=$TARGET --prefix="$LOCAL_DIR" --disable-nls --enable-languages=c,c++ --without-headers
    make all-gcc
    make all-target-libgcc
    make install-gcc
    make install-target-libgcc
    popd
popd
