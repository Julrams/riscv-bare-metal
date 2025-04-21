.equ REGBYTES, 8
.equ STACK_SIZE,  ((1 << 12) - 128)

.section .text.start

.globl _start
_start:
    csrr   t0, mhartid
    lui    t1, 0
    beq    t0, t1, 2f

1:  wfi
    j      1b

2:
    # initialize global pointer
    .option push
    .option norelax
    la gp, _gp
    .option pop

    # initialize stack pointer
    .if __riscv_xlen == 32
        la sp, stack_top
    .else
        la sp, stack_top
    .endif

    call   system_init

.section .bss
    .align 3
    .global _gp
_gp:
    .space 4

    .align 3
    .global stack_top
stack_top:
    .space STACK_SIZE
