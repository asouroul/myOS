CC = i686-elf-gcc
AS = i686-elf-as

CFLAGS ?= -O2 -g
CFLAGS := $(CFLAGS) -std=gnu99 -ffreestanding -Wall -Wextra

# Ask GCC where it hides its internal constructor files
CRTBEGIN_OBJ := $(shell $(CC) $(CFLAGS) -print-file-name=crtbegin.o)
CRTEND_OBJ := $(shell $(CC) $(CFLAGS) -print-file-name=crtend.o)

# The STRICT linking order
OBJ_LINK_LIST := crti.o $(CRTBEGIN_OBJ) boot.o kernel.o $(CRTEND_OBJ) crtn.o

all: myos.iso

crti.o: crti.s
	$(AS) crti.s -o crti.o

crtn.o: crtn.s
	$(AS) crtn.s -o crtn.o

boot.o: boot.s
	$(AS) boot.s -o boot.o

kernel.o: kernel.c
	$(CC) -c kernel.c -o kernel.o $(CFLAGS)

myos: crti.o crtn.o boot.o kernel.o
	$(CC) -T linker.ld -o myos -ffreestanding -O2 -nostdlib $(OBJ_LINK_LIST) -lgcc

myos.iso: myos
	cp myos isodir/boot/myos
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o myos.iso isodir

run: myos.iso
	qemu-system-i386 -cdrom myos.iso

clean:
	rm -f *.o myos myos.iso
