#include "KernelUtil.h"
#include "GDT/GDT.h"
#include "Interrupts/IDT.h"
#include "Interrupts/Interrupts.h"
#include "IO.h"

KernelInfo kernelInfo;
PageTableManager pageTableManager = NULL;

void PrepareMemory(BootInfo* bootInfo) {
	
	size_t mMapEntries = bootInfo -> mMapSize / bootInfo -> mMapDescSize;

	GlobalAllocator = PageFrameAllocater();
	GlobalAllocator.ReadEFIMemoryMap(bootInfo -> mMap, bootInfo -> mMapSize, bootInfo -> mMapDescSize);

	uint64_t KernelSize = (uint64_t)&_KernelEnd - (uint64_t)&_KernelStart;
	uint64_t KernelPages = (uint64_t)KernelSize /  4096 + 1;
	GlobalAllocator.LockPages(&_KernelStart, KernelPages);

	PageTable* PML4 = (PageTable*)GlobalAllocator.RequestPage();
	memset(PML4, 0, 0x1000);

	pageTableManager = PageTableManager(PML4);

	for (uint64_t t = 0; t < GetMemorySize(bootInfo -> mMap, mMapEntries, bootInfo ->mMapDescSize); t += 0x1000) {
		pageTableManager.MapMemory((void*)t, (void*)t);
	}

	uint64_t fbBase = (uint64_t) bootInfo -> framebuffer -> BaseAddress;
	uint64_t fbSize = (uint64_t) bootInfo -> framebuffer -> BufferSize + 0x1000;
	GlobalAllocator.LockPages((void*)fbBase, fbSize / 0x1000 + 1);
	for (uint64_t t = fbBase; t < fbBase + fbSize; t += 0x1000) {
		pageTableManager.MapMemory((void*)t, (void*)t);
	}

	asm ("mov %0, %%cr3" : : "r" (PML4));

	kernelInfo.pageTableManager = &pageTableManager;

}

void PrepareGDT() {

	GDTDescriptor gdtDescriptor;
	gdtDescriptor.Size = sizeof(GDT) - 1;
	gdtDescriptor.Offset = (uint64_t)&DefaultGDT;
	LoadGDT(&gdtDescriptor);

}

IDTR idtr; 
void PrepareInterrupts() {
	idtr.Limit = 0x0FFF;
	idtr.Offset = (uint64_t) GlobalAllocator.RequestPage();

	IDTDescEntry* int_PageFault = (IDTDescEntry*) (idtr.Offset + 0xE * sizeof(IDTDescEntry));
	int_PageFault -> SetOffset((uint64_t)PageFaultHandler);
	int_PageFault -> type_attr = IDT_TA_InterruptGate;
	int_PageFault -> selector = 0x08;

	IDTDescEntry* int_DoubleFault = (IDTDescEntry*) (idtr.Offset + 0x8 * sizeof(IDTDescEntry));
	int_DoubleFault -> SetOffset((uint64_t)DoubleFaultHandler);
	int_DoubleFault -> type_attr = IDT_TA_InterruptGate;
	int_DoubleFault -> selector = 0x08;

	IDTDescEntry* int_GPFault = (IDTDescEntry*) (idtr.Offset + 0xD * sizeof(IDTDescEntry));
	int_GPFault -> SetOffset((uint64_t)GPFaultHandler);
	int_GPFault -> type_attr = IDT_TA_InterruptGate;
	int_GPFault -> selector = 0x08;

	IDTDescEntry* int_KeyboardHandler = (IDTDescEntry*) (idtr.Offset + 0x21 * sizeof(IDTDescEntry));
	int_KeyboardHandler -> SetOffset((uint64_t)KeyboardHandler);
	int_KeyboardHandler -> type_attr = IDT_TA_InterruptGate;
	int_KeyboardHandler -> selector = 0x08;

	asm ("lidt %0" : : "m" (idtr));

	RemapPIC();

	outb(PIC1_DATA, 0b11111101);
	outb(PIC2_DATA, 0b11111101);

	asm("sti");

}

BasicRenderer renderer = BasicRenderer(NULL, NULL);
KernelInfo InitializeKernel(BootInfo* bootInfo) {

	renderer = BasicRenderer(bootInfo -> framebuffer, bootInfo -> psf1_Font);
	GlobalRenderer = &renderer;

	PrepareGDT();
	PrepareMemory(bootInfo);
	// Clear the whole screen
	memset(bootInfo -> framebuffer -> BaseAddress, 0, bootInfo -> framebuffer -> BufferSize);

	PrepareInterrupts();

	return kernelInfo;

}
