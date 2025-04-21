#ifndef RISCV_H
#define RISCV_H

#if __riscv_xlen == 64
#include "rv64.h"
#else
#include "rv32.h"
#endif

#endif // RISCV_H