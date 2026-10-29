#!/bin/sh
set -e
. ./build.sh

mkdir -p isodir
mkdir -p isodir/boot
mkdir -p isodir/boot/grub

# Copy the kernel from our fake hard drive into the ISO folder
cp sysroot/boot/myos.kernel isodir/boot/myos.kernel

# We can use the grub.cfg we already have in the root folder!
cp grub.cfg isodir/boot/grub/grub.cfg

grub-mkrescue -o myos.iso isodir
