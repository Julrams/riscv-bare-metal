#ifndef RV32_H
#define RV32_H
#include <stdint.h>

// Machine Status Register, mstatus

#define MSTATUS_MPP_MASK (3 << 11)  // previous mode.
#define MSTATUS_MPP_M (3 << 11)
#define MSTATUS_MPP_S (1 << 11)
#define MSTATUS_MPP_U (0 << 11)
#define MSTATUS_MIE (1 << 3)  // machine-mode interrupt enable.

// which hart (core) is this?
static inline uint32_t r_mhartid() {
    uint32_t x;
    asm volatile("csrr %0, mhartid" : "=r"(x));
    return x;
}

static inline uint32_t r_mstatus() {
    uint32_t x;
    asm volatile("csrr %0, mstatus" : "=r"(x));
    return x;
}

static inline void w_mstatus(uint32_t x) {
    asm volatile("csrw mstatus, %0" : : "r"(x));
}

// machine exception program counter, holds the
// instruction address to which a return from
// exception will go.
static inline void w_mepc(uint32_t x) {
    asm volatile("csrw mepc, %0" : : "r"(x));
}

// Supervisor Status Register, sstatus

#define SSTATUS_SPP (1 << 8)   // Previous mode, 1=Supervisor, 0=User
#define SSTATUS_SPIE (1 << 5)  // Supervisor Previous Interrupt Enable
#define SSTATUS_UPIE (1 << 4)  // User Previous Interrupt Enable
#define SSTATUS_SIE (1 << 1)   // Supervisor Interrupt Enable
#define SSTATUS_UIE (1 << 0)   // User Interrupt Enable

static inline uint32_t r_sstatus() {
    uint32_t x;
    asm volatile("csrr %0, sstatus" : "=r"(x));
    return x;
}

static inline void w_sstatus(uint32_t x) {
    asm volatile("csrw sstatus, %0" : : "r"(x));
}

// Supervisor Interrupt Pending
static inline uint32_t r_sip() {
    uint32_t x;
    asm volatile("csrr %0, sip" : "=r"(x));
    return x;
}

static inline void w_sip(uint32_t x) {
    asm volatile("csrw sip, %0" : : "r"(x));
}

static inline void w_pmpcfg0(uint32_t x) {
    asm volatile("csrw pmpcfg0, %0" : : "r"(x));
}

static inline void w_pmpaddr0(uint32_t x) {
    asm volatile("csrw pmpaddr0, %0" : : "r"(x));
}

// Supervisor Interrupt Enable
#define SIE_SEIE (1 << 9)  // external
#define SIE_STIE (1 << 5)  // timer
#define SIE_SSIE (1 << 1)  // software
static inline uint32_t r_sie() {
    uint32_t x;
    asm volatile("csrr %0, sie" : "=r"(x));
    return x;
}

static inline void w_sie(uint32_t x) {
    asm volatile("csrw sie, %0" : : "r"(x));
}

// Machine-mode Interrupt Enable
#define MIE_MEIE (1 << 11)  // external
#define MIE_MTIE (1 << 7)   // timer
#define MIE_MSIE (1 << 3)   // software
static inline uint32_t r_mie() {
    uint32_t x;
    asm volatile("csrr %0, mie" : "=r"(x));
    return x;
}

static inline void w_mie(uint32_t x) {
    asm volatile("csrw mie, %0" : : "r"(x));
}

// machine exception program counter, holds the
// instruction address to which a return from
// exception will go.
static inline void w_sepc(uint32_t x) {
    asm volatile("csrw sepc, %0" : : "r"(x));
}

static inline uint32_t r_sepc() {
    uint32_t x;
    asm volatile("csrr %0, sepc" : "=r"(x));
    return x;
}

// Machine Exception Delegation
static inline uint32_t r_medeleg() {
    uint32_t x;
    asm volatile("csrr %0, medeleg" : "=r"(x));
    return x;
}

static inline void w_medeleg(uint32_t x) {
    asm volatile("csrw medeleg, %0" : : "r"(x));
}

// Machine Interrupt Delegation
static inline uint32_t r_mideleg() {
    uint32_t x;
    asm volatile("csrr %0, mideleg" : "=r"(x));
    return x;
}

static inline void w_mideleg(uint32_t x) {
    asm volatile("csrw mideleg, %0" : : "r"(x));
}

// Supervisor Trap-Vector Base Address
// low two bits are mode.
static inline void w_stvec(uint32_t x) {
    asm volatile("csrw stvec, %0" : : "r"(x));
}

static inline uint32_t r_stvec() {
    uint32_t x;
    asm volatile("csrr %0, stvec" : "=r"(x));
    return x;
}

// Machine-mode interrupt vector
static inline void w_mtvec(uint32_t x) {
    asm volatile("csrw mtvec, %0" : : "r"(x));
}

// use riscv's sv32 page table scheme.
#define SATP_SV32 (1L << 31)

#define MAKE_SATP(pagetable) (SATP_SV32 | (((uint32_t)pagetable) >> 12))

// supervisor address translation and protection;
// holds the address of the page table.
static inline void w_satp(uint32_t x) {
    asm volatile("csrw satp, %0" : : "r"(x));
}

static inline uint32_t r_satp() {
    uint32_t x;
    asm volatile("csrr %0, satp" : "=r"(x));
    return x;
}

// Supervisor Scratch register, for early trap handler in trampoline.S.
static inline void w_sscratch(uint32_t x) {
    asm volatile("csrw sscratch, %0" : : "r"(x));
}

static inline void w_mscratch(uint32_t x) {
    asm volatile("csrw mscratch, %0" : : "r"(x));
}

// Supervisor Trap Cause
static inline uint32_t r_scause() {
    uint32_t x;
    asm volatile("csrr %0, scause" : "=r"(x));
    return x;
}

// Supervisor Trap Value
static inline uint32_t r_stval() {
    uint32_t x;
    asm volatile("csrr %0, stval" : "=r"(x));
    return x;
}

// Machine-mode Counter-Enable
static inline void w_mcounteren(uint32_t x) {
    asm volatile("csrw mcounteren, %0" : : "r"(x));
}

static inline uint32_t r_mcounteren() {
    uint32_t x;
    asm volatile("csrr %0, mcounteren" : "=r"(x));
    return x;
}

// machine-mode cycle counter
static inline uint32_t r_time() {
    uint32_t x;
    asm volatile("csrr %0, time" : "=r"(x));
    return x;
}

// enable device interrupts
static inline void intr_on() {
    w_sie(r_sie() | SIE_SEIE | SIE_STIE | SIE_SSIE);
    w_sstatus(r_sstatus() | SSTATUS_SIE);
}

// disable device interrupts
static inline void intr_off() { w_sstatus(r_sstatus() & ~SSTATUS_SIE); }

// are device interrupts enabled?
static inline int intr_get() {
    uint32_t x = r_sstatus();
    return (x & SSTATUS_SIE) != 0;
}

static inline uint32_t r_sp() {
    uint32_t x;
    asm volatile("mv %0, sp" : "=r"(x));
    return x;
}

// read and write tp, the thread pointer, which holds
// this core's hartid (core number), the index into cpus[].
static inline uint32_t r_tp() {
    uint32_t x;
    asm volatile("mv %0, tp" : "=r"(x));
    return x;
}

static inline void w_tp(uint32_t x) { asm volatile("mv tp, %0" : : "r"(x)); }

static inline uint32_t r_ra() {
    uint32_t x;
    asm volatile("mv %0, ra" : "=r"(x));
    return x;
}

// flush the TLB.
static inline void sfence_vma() {
    // the zero, zero means flush all TLB entries.
    asm volatile("sfence.vma zero, zero");
}

#define PGSIZE 4096  // bytes per page
#define PGSHIFT 12   // bits of offset within a page

#define PGROUNDUP(sz) (((sz) + PGSIZE - 1) & ~(PGSIZE - 1))
#define PGROUNDDOWN(a) (((a)) & ~(PGSIZE - 1))

#define PTE_V (1 << 0)  // valid
#define PTE_R (1 << 1)
#define PTE_W (1 << 2)
#define PTE_X (1 << 3)
#define PTE_U (1 << 4)  // 1 -> user can access

// shift a physical address to the right place for a PTE.
#define PA2PTE(pa) ((((uint32_t)pa) >> 12) << 10)

#define PTE2PA(pte) (((pte) >> 10) << 12)

#define PTE_FLAGS(pte) ((pte)&0x3FF)

// extract the three 9-bit page table indices from a virtual address.
#define PXMASK 0x1FF  // 9 bits
#define PXSHIFT(level) (PGSHIFT + (9 * (level)))
#define PX(level, va) ((((uint32_t)(va)) >> PXSHIFT(level)) & PXMASK)

// one beyond the highest possible virtual address.
// MAXVA is actually one bit less than the max allowed by
// Sv32, to avoid having to sign-extend virtual addresses
// that have the high bit set.
#define MAXVA (1L << (9 + 9 + 12 - 1))

typedef uint32_t pte_t;
typedef uint32_t *pagetable_t;  // 1024 PTEs

#endif // RV32_H