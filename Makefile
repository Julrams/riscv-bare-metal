SHELL = /bin/sh
COMPILER_PATH = /opt/riscv/bin/

# Default to RV64 if ARCH is not set
ARCH ?= riscv64

# Set the appropriate GCC prefix based on the ARCH variable
ifeq ($(ARCH), riscv64)
GCC_PREFIX = riscv64-unknown-elf
CFLAGS += -mcmodel=medany
else ifeq ($(ARCH), riscv32)
GCC_PREFIX = riscv32-unknown-elf
endif

CC = $(COMPILER_PATH)$(GCC_PREFIX)-gcc
LD = $(COMPILER_PATH)$(GCC_PREFIX)-ld
GDB = $(COMPILER_PATH)$(GCC_PREFIX)-gdb
OBJCOPY = $(COMPILER_PATH)$(GCC_PREFIX)-objcopy

IDIR = include
SDIR = src
BDIR = build
CFLAGS += -Wall -g -I $(IDIR) -O0
SFLAGS = -g -I $(IDIR)

S_SRCS = $(wildcard $(SDIR)/*.s)
C_SRCS = $(wildcard $(SDIR)/*.c)
S_OBJS = $(S_SRCS:$(SDIR)/%.s=$(BDIR)/%_asm.o)
C_OBJS = $(C_SRCS:$(SDIR)/%.c=$(BDIR)/%.o)
ifeq ($(ARCH), riscv64)
SCAT = etc/qemu_64.ld
else ifeq ($(ARCH), riscv32)
SCAT = etc/qemu_32.ld
endif

all: clean kernel.img

kernel.img: kernel.elf
	$(OBJCOPY) kernel.elf -O binary kernel.img

kernel.elf: $(S_OBJS) $(SCAT) $(C_OBJS)
	$(LD) -T $(SCAT) -o kernel.elf $(S_OBJS) $(C_OBJS)

$(BDIR)/%.o: $(SDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BDIR)/%_asm.o: $(SDIR)/%.s
	$(CC) $(SFLAGS) -c $< -o $@

clean:
	rm -f $(BDIR)/*_asm.o $(BDIR)/*.o kernel.elf kernel.img

run: all
	qemu-system-$(ARCH) -M virt -kernel kernel.img -bios none -serial stdio -display none

debug: all
	terminator -e "qemu-system-$(ARCH) -M virt -kernel kernel.img -bios none -serial stdio -s -S" --new-tab
	terminator -e "$(GDB) -x debug.txt" --new-tab
