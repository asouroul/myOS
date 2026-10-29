.section .init
    /* GCC's crtend.o was glued right above this */
    popl %ebp
    ret

.section .fini
    /* GCC's crtend.o was glued right above this */
    popl %ebp
    ret
