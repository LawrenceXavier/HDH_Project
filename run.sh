#!/bin/sh

# ROOT=/home/thangtq/Work/HDH_Project
ROOT=$PWD
LIBSRC=$ROOT/lib

TARGET=decstation-ultrix
HOST=i686-pc-linux-gnu
PREFIX=$ROOT/build/cross
# GCCDIR=lib/gcc-2.95.3
# BINDIR=lib/binutils-2.11.2
OBJGCC=$ROOT/build/gcc-obj
OBJBIN=$ROOT/build/bin-obj

cd $LIBSRC
tar -xzvf binutils-2.11.2.tar.gz
tar -xzvf gcc-2.95.3.tar.gz

mkdir $ROOT/build
mkdir $OBJGCC
mkdir $OBJBIN
mkdir $PREFIX

cd $OBJBIN
$LIBSRC/binutils-2.11.2/configure --host=$HOST --target=$TARGET --prefix=$PREFIX -v
# $BINDIR/make all install CC=gcc34
make CC=gcc34
make install

mkdir $PREFIX/decstation-ultrix/sys-include

cd $OBJGCC
$LIBSRC/gcc-2.95.3/configure --host=$HOST \
    --target=$TARGET \
    --prefix=$PREFIX \
    --program-prefix=cross \
    --with-gnu-as \
    --with-gnu-ld \
    --with-as=$PREFIX/decstation-ultrix/bin/as \
    --with-ld=$PREFIX/decstation-ultrix/bin/ld \
    --enable-languages=c \
    --disable-multilib \
    --disable-libgcj

make CC=gcc34
make -k install 


