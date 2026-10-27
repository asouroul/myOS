# Kernel Initialization & Global Constructor Support
### Overview
In a bare-metal environment, the operating system lacks a standard C library (libc) to handle pre-main initialization. By default, the bootloader jumps directly into the kernel entry point, bypassing any code flagged to run prior to execution. This implementation introduces full System V ABI compliance to explicitly support global constructors and compiler-injected initialization routines.

### Why This is Necessary
Without global constructor support, several advanced language features will fail silently or crash the kernel:

-C++ Global Objects: The constructors for objects declared at the global scope will never execute.

-C Constructor Attributes: Functions tagged with __attribute__((constructor)) (frequently used for automated hardware driver registration) will be ignored.

-Internal Compiler Libraries: Advanced math and stack-smashing protection routines provided by libgcc often rely on initialization scripts running at boot.

### The Implementation Architecture
To resolve this, the build system leverages the GNU linker to construct the _init and _fini functions dynamically during the linking phase.

Because the compiler provides the internal initialization list (crtbegin.o and crtend.o), the kernel provides 
the assembly entry and exit points (crti.s and crtn.s). The Makefile enforces a strict linking order to merge these files into valid executable functions:

1. crti.o: Provides the function headers (push %ebp, movl %esp, %ebp) and opens the .init and .fini sections.

2. crtbegin.o: Provided by GCC; contains the start of the internal constructor lists.

3. boot.o & kernel.o: The core operating system binaries.

4. crtend.o: Provided by GCC; contains the end of the constructor lists.

5. crtn.o: Provides the function footers (popl %ebp, ret) to safely return execution to the caller.

### Boot Sequence Integration
With the _init function fully assembled by the linker, the kernel bootloader (boot.s) is configured to call _init immediately after establishing the stack pointer, guaranteeing all global constructors are executed before control is handed to kernel_main.
