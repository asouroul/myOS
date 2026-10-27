CC = i686-elf-gcc
AS = i686-elf-as

CFLAGS ?= -O2 -g
CFLAGS := $(CFLAGS) -std=gnu99 -ffreestanding -Wall -Wextra

all: myos.iso

boot.o: boot.s
	$(AS) boot.s -o boot.o

kernel.o: kernel.c
	$(CC) -c kernel.c -o kernel.o $(CFLAGS)

myos: boot.o kernel.o
	$(CC) -T linker.ld -o myos -ffreestanding -O2 -nostdlib boot.o kernel.o -lgcc

myos.iso: myos
	cp myos isodir/boot/myos
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o myos.iso isodir

run: myos.iso
	qemu-system-i386 -cdrom myos.iso

clean:
	rm -f *.o myos myos.iso
