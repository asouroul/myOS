#!/bin/sh
set -e

export MAKE=${MAKE:-make}
export HOST=i686-elf

export AR=${HOST}-ar
export AS=${HOST}-as
export CC=${HOST}-gcc

export PREFIX=/usr
export EXEC_PREFIX=$PREFIX
export BOOTDIR=/boot
export LIBDIR=$EXEC_PREFIX/lib
export INCLUDEDIR=$PREFIX/include

export CFLAGS='-O2 -g'
export CPPFLAGS=''

# Configure the cross-compiler to use the fake hard drive (sysroot)
export SYSROOT="$(pwd)/sysroot"
export CC="$CC --sysroot=$SYSROOT -isystem=$INCLUDEDIR"

# Only build the kernel for now (we will add libc later)
PROJECTS="kernel"
