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

rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

SRC = $(call rwildcard,$(SRCDIR),*.cpp)
ASM = $(call rwildcard,$(SRCDIR),*.asm)         
OBJS = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRC))
OBJS += $(patsubst $(SRCDIR)/%.asm, $(OBJDIR)/%_asm.o, $(ASM))
DIRS = $(wildcard $(SRCDIR)/*)

all: kernel buildimg

kernel: start $(OBJS) link

$(OBJDIR)/Interrupts/Interrupts.o : $(SRCDIR)/Interrupts/Interrupts.cpp
	@ echo Compiling $^
	@ mkdir -p $(@D)
	$(CC) -mno-red-zone -mgeneral-regs-only -ffreestanding -c $^ -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@ echo Compiling $^
	@ mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $^ -o $@

$(OBJDIR)/%_asm.o: $(SRCDIR)/%.asm
	@ echo Assembling $^
	@ mkdir -p $(@D)
	$(AS) $(ASFLAGS) $^ -f elf64 -o $@

link:
	$(LD) $(LDFLAGS) -o $(BUILDDIR)/kernel.elf $(OBJS)

setup:
	@ mkdir $(BUILDDIR)
	@ mkdir $(SRCDIR)
	@ mkdir $(OBJDIR)

buildimg:
	@ echo Building $(BUILDDIR)/$(OSNAME).img
	dd if=/dev/zero of=$(BUILDDIR)/$(OSNAME).img bs=512 count=93750
	mformat -i $(BUILDDIR)/$(OSNAME).img -f 2880 ::
	mmd -i $(BUILDDIR)/$(OSNAME).img ::/EFI
	mmd -i $(BUILDDIR)/$(OSNAME).img ::/EFI/BOOT
	mcopy -i $(BUILDDIR)/$(OSNAME).img $(BOOTEFI) ::/EFI/BOOT
	mcopy -i $(BUILDDIR)/$(OSNAME).img startup.nsh ::
	mcopy -i $(BUILDDIR)/$(OSNAME).img $(BUILDDIR)/kernel.elf ::
	mcopy -i $(BUILDDIR)/$(OSNAME).img $(BUILDDIR)/zap-light18.psf ::

.PHONY: clean
clean:
	rm -f lib/*.o
	rm -f bin/kernel.elf
	rm -f bin/CustomOS.img

.PHONY: start
start:
	@ echo Using $(AS) as Assembler
	@ echo Using $(CC) as Compiler
	@ echo Using $(LD) as Linker
	@ echo Using $(LDS) as Linker Script

run:
	qemu-system-x86_64 -drive file=$(BUILDDIR)/$(OSNAME).img -m 256M -cpu qemu64 -drive if=pflash,format=raw,unit=0,file="$(OVMFDIR)/OVMF_CODE-pure-efi.fd",readonly=on -drive if=pflash,format=raw,unit=1,file="$(OVMFDIR)/OVMF_VARS-pure-efi.fd" -net none
