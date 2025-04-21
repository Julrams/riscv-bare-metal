#include "riscv.h"
#ifdef DEBUG
#include "uart.h"
#endif

#define CLINT_BASE 0x2000000

#if __riscv_xlen == 64
#define MTIME (volatile uint64_t *)(CLINT_BASE + 0xbff8)
#define MTIMECMP (volatile uint64_t *)(CLINT_BASE + 0x4000)
typedef uint64_t reg_t;
#else
#define MTIME (volatile uint32_t *)(CLINT_BASE + 0xbff8)
#define MTIMECMP (volatile uint32_t *)(CLINT_BASE + 0x4000)
typedef uint32_t reg_t;
#endif

int count = 0;

void handle_interrupt(reg_t mcause) {
    if ((mcause << 1 >> 1) == 0x7) {
#ifdef DEBUG
        print_s("Timer Interrupt: ");
        print_i(++count);
        print_s("\n");
#endif

        *MTIMECMP = *MTIME + 0xfffff * 5;
        if (count == 10) {
            reg_t mie;
            asm volatile("csrr %0, mie" : "=r"(mie));
            mie &= ~(1 << 7);
            asm volatile("csrw mie, %0" : "=r"(mie));
        }
    } else {
#ifdef DEBUG
        print_s("Unknown interrupt: ");
        print_i(mcause << 1 >> 1);
        print_s("\n");
#endif
        while (1)
            ;
    }
}

void handle_exception(reg_t mcause) {
    reg_t mie;

    if (mcause == 0x8) {
        *MTIMECMP = *MTIME + 0xfffff * 5;

        asm volatile("csrr %0, mie" : "=r"(mie));
        mie |= (1 << 7);
        asm volatile("csrw mie, %0" : "=r"(mie));
    } else {
#ifdef DEBUG
        print_s("Unknown exception: ");
        print_i(mcause << 1 >> 1);
        print_s("\n");
#endif
        while (1)
            ;
    }
}

void handle_trap() {
    reg_t mcause, mepc;
    asm volatile("csrr %0, mcause" : "=r"(mcause));
    asm volatile("csrr %0, mepc" : "=r"(mepc));

    if (mcause >> (sizeof(reg_t) * 8 - 1)) {
        handle_interrupt(mcause);
    } else {
        handle_exception(mcause);
        asm volatile("csrr t0, mepc");
        asm volatile("addi t0, t0, 0x4");
        asm volatile("csrw mepc, t0");
    }
}