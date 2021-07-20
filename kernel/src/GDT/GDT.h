#pragma once

#include <stdint.h>

// The GDTDescriptor, used by lgdt
struct GDTDescriptor {
	uint16_t Size;
	uint64_t Offset;
}__attribute__((packed));

// A GDTEntry
struct GDTEntry {
	uint16_t Limit0;
	uint16_t Base0;
	uint8_t Base1;
	uint8_t AccessByte;
	uint8_t Limit1_Flags;
	uint8_t Base2;
}__attribute__((packed));

// A GDT, needs to be aligned for performance
struct GDT {
	GDTEntry Null;
	GDTEntry KernelCodeSegment;
	GDTEntry KernelDataSegment;
	GDTEntry UserNull;
	GDTEntry UserCode;
	GDTEntry UserData;
}__attribute__((packed)) __attribute__((aligned(0x1000)));

extern GDT DefaultGDT;
/* Extern assembly function.
Needs to be called by the C calling convention 
so that the linker can find it. */
extern "C" void LoadGDT(GDTDescriptor* gdtDescriptor);