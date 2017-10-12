#!/bin/sh

ROOT=/home/thangtq/Work/HDH_Project
TARGET=decstation-ultrix
HOST=i686-pc-linux-gnu
PREFIX=$ROOT/build/cross
GCCDIR=$ROOT/lib/gcc-2.95.3
BINDIR=$ROOT/lib/binutils-2.11.2
OBJGCC=$ROOT/build/gcc-obj
OBJBIN=$ROOT/build/bin-obj

mkdir $ROOT/build
mkdir $OBJGCC
mkdir $OGJBIN
mkdir $PREFIX

cd $OBJBIN
$BINDIR/configure --host=$HOST --target=$TARGET --prefix=$PREFIX -v
# $BINDIR/make all install CC=gcc34
make CC=gcc34
make install

mkdir $PREFIX/decstation-ultrix/sys-include

cd $OBJGCC
$GCCDIR/configure --host=$HOST \
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


