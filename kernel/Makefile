OSNAME = CustomOS

GNUEFI = ../gnu-efi
OVMFDIR = ../OVMFbin
LDS = kernel.ld

CC = x86_64-elf-g++
AS = nasm
LD = x86_64-elf-ld

WFLAGS = -Wall -Wextra -Wpedantic
CFLAGS = -ffreestanding -fshort-wchar -mno-red-zone $(WFLAGS)
ASFLAGS = 
LDFLAGS = -T $(LDS) -static -Bsymbolic

SRCDIR := src
OBJDIR := lib
BUILDDIR = bin
BOOTEFI := $(GNUEFI)/x86_64/bootloader/main.efi

NO_COL  := \033[0m
RED     := \033[0;31m
GREEN   := \033[0;32m
BLUE    := \033[0;34m
YELLOW  := \033[0;33m
BLINK   := \033[33;5m

rwildcard = $(foreach d, $(wildcard $(1:=/*)), $(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

define print
  @ printf "$(GREEN)$(1) $(YELLOW)$(2)$(GREEN) -> $(BLUE)$(3)$(BLUE)\n"
endef

SRC = $(call rwildcard,$(SRCDIR),*.cpp)
ASM = $(call rwildcard,$(SRCDIR),*.asm)         
OBJS = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRC))
OBJS += $(patsubst $(SRCDIR)/%.asm, $(OBJDIR)/%_asm.o, $(ASM))
DIRS = $(wildcard $(SRCDIR)/*)

all: kernel buildimg

kernel: start $(OBJS) link

$(OBJDIR)/Interrupts/Interrupts.o : $(SRCDIR)/Interrupts/Interrupts.cpp
	@ $(call print,Compiling:,$<,$@)
	@ mkdir -p $(@D)
	$(CC) -mno-red-zone -mgeneral-regs-only -ffreestanding -c $^ -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@ $(call print,Compiling:,$<,$@)
	@ mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $^ -o $@

$(OBJDIR)/%_asm.o: $(SRCDIR)/%.asm
	@ $(call print,Assembling:,$<,$@)
	@ mkdir -p $(@D)
	$(AS) $(ASFLAGS) $^ -f elf64 -o $@

link:
	@ printf "$(GREEN)Linking $(YELLOW)kernel.elf$(BLUE)\n"
	$(LD) $(LDFLAGS) -o $(BUILDDIR)/kernel.elf $(OBJS)

setup:
	@ mkdir $(BUILDDIR)
	@ mkdir $(SRCDIR)
	@ mkdir $(OBJDIR)

buildimg:
	@ printf "$(GREEN)Building $(BUILDDIR)/$(OSNAME).img$(BLUE)\n"
	dd if=/dev/zero of=$(BUILDDIR)/$(OSNAME).img bs=512 count=93750
	mformat -i $(BUILDDIR)/$(OSNAME).img -f 2880 ::
	mmd -i $(BUILDDIR)/$(OSNAME).img ::/EFI
	mmd -i $(BUILDDIR)/$(OSNAME).img ::/EFI/BOOT
	mcopy -i $(BUILDDIR)/$(OSNAME).img $(BOOTEFI) ::/EFI/BOOT
	mcopy -i $(BUILDDIR)/$(OSNAME).img startup.nsh ::
	mcopy -i $(BUILDDIR)/$(OSNAME).img $(BUILDDIR)/kernel.elf ::
	mcopy -i $(BUILDDIR)/$(OSNAME).img $(BUILDDIR)/zap-light18.psf ::

.PHONY: clean start
clean:
	rm -f lib/*.o
	rm -f bin/kernel.elf
	rm -f bin/CustomOS.img

start:
	@ printf "$(YELLOW)Using $(AS) as Assembler\n"
	@ printf "Using $(CC) as Compiler\n"
	@ printf "Using $(LD) as Linker\n"
	@ printf "Using $(LDS) as Linker Script$(BLUE)\n"

run:
	qemu-system-x86_64 -drive file=$(BUILDDIR)/$(OSNAME).img -m 256M -cpu qemu64 -drive if=pflash,format=raw,unit=0,file="$(OVMFDIR)/OVMF_CODE-pure-efi.fd",readonly=on -drive if=pflash,format=raw,unit=1,file="$(OVMFDIR)/OVMF_VARS-pure-efi.fd" -net none
