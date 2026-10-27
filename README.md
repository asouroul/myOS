## Cross-Compiler
Creating a GCC cross-compiler for i686-elf on Debian OS (Ubuntu) following https://wiki.osdev.org/GCC_Cross-Compiler

Tools used:  
GCC Compiler
Make  
Bison  
Flex  
GMP  
MPC  
MPFR  
Texinfo  
ISL  

```sudo apt install build-essential bison flex libgmp3-dev libmpc-dev libmpfr-dev texinfo libisl-dev```


## Current Features
* **Bootable 32-bit x86 Kernel**: Custom bootloader and kernel compiled via `i686-elf` cross-compiler, packaged as a GRUB-bootable ISO.
* **VGA Text-Mode Driver**: Memory-mapped display driver interacting directly with hardware port `0xB8000`.
* **Hardware Screen Scrolling**: Automatic memory shifting and row tracking when text reaches the bottom of the display.
* **ANSI Escape Sequence Parser**: Custom state-machine implemented in C to parse and translate embedded string commands.
 - **16-Color Palette Support**: Translates standard ANSI color codes (`\033[3Xm`) into VGA hardware signals.
 - **Screen Wiping**: Supports the `\033[2J` clear-screen ANSI command.
* **Automated Build System**: Fully integrated Makefile for dependency tracking and instant QEMU emulation.
* **Global Constructor Support**: Full System V ABI compliance via `crti`/`crtn` integration, 
allowing the kernel to safely execute advanced C initialization scripts and C++ global objects before `kernel_main` runs.











