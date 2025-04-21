#include "main.h"
#include "riscv.h"
#include "trap.h"

extern void kernelvec();
extern void trap_entry();
extern void timervec();

void system_init()
{
    // set M Previous Privilege mode to Supervisor, for mret.
    unsigned long x = r_mstatus();
    x &= ~MSTATUS_MPP_MASK;
    x |= MSTATUS_MPP_S;
    w_mstatus(x);

    // set M Exception Program Counter to main, for mret.
    // requires gcc -mcmodel=medany
    #if __riscv_xlen == 64
        w_mepc((uint64_t)main);
    #else
        w_mepc((uint32_t)main);
    #endif

    // disable paging for now.
    w_satp(0);

    // physical memory protection
    #if __riscv_xlen == 64
        w_pmpcfg0(0xf);
        w_pmpaddr0(0xffffffffffffffff);
    #else
        w_pmpcfg0(0xf);
        w_pmpaddr0(0xffffffff);
    #endif

    // delegate all interrupts and exceptions to supervisor mode.
    w_medeleg(0xffff);
    w_mideleg(0xffff);

    // setup trap_entry
    #if __riscv_xlen == 64
        w_mtvec((uint64_t)trap_entry);
    #else
        w_mtvec((uint32_t)trap_entry);
    #endif

    // setup kernelvec
    #if __riscv_xlen == 64
        w_stvec((uint64_t)kernelvec);
    #else
        w_stvec((uint32_t)kernelvec);
    #endif

    // setup timervec
    #if __riscv_xlen == 64
        w_mtvec((uint64_t)timervec);
    #else
        w_mtvec((uint32_t)timervec);
    #endif

    // keep each CPU's hartid in its tp register, for cpuid().
    int id = r_mhartid();
    w_tp(id);

    // switch to supervisor mode and jump to main().
    asm volatile("mret");
}
