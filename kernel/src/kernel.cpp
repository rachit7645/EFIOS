#include "BasicRenderer.h"
#include "cstr.h"
#include "EFIMemory.h"
#include "memory.h"
#include "Paging/PageFrameAllocater.h"
#include "Paging/PageMapIndexer.h"
#include "Paging/Paging.h"
#include "Paging/PageTableManager.h"

struct BootInfo {

	FrameBuffer* framebuffer;
	PSF1_FONT* psf1_Font;
	EFI_MEMORY_DESCRIPTOR* mMap;
	uint64_t mMapSize;
	uint64_t mMapDescSize;

};

extern uint64_t _KernelStart;
extern uint64_t _KernelEnd;

extern "C" void _start(BootInfo* bootInfo) {
	
	BasicRenderer newRenderer = BasicRenderer(bootInfo -> framebuffer, bootInfo -> psf1_Font);

	GlobalAllocator = PageFrameAllocater();
	GlobalAllocator.ReadEFIMemoryMap(bootInfo -> mMap, bootInfo -> mMapSize, bootInfo -> mMapDescSize);

	size_t mMapEntries = bootInfo -> mMapSize / bootInfo -> mMapDescSize;

	uint64_t KernelSize = (uint64_t)&_KernelEnd - (uint64_t)&_KernelStart;
	uint64_t KernelPages = (uint64_t)KernelSize /  4096 + 1;
	GlobalAllocator.LockPages(&_KernelStart, KernelPages);

	PageTable* PML4 = (PageTable*)GlobalAllocator.RequestPage();
	memset(PML4, 0, 0x1000);

	PageTableManager pageTableManager = PageTableManager(PML4);

	for (uint64_t t = 0; t < GetMemorySize(bootInfo -> mMap, mMapEntries, bootInfo ->mMapDescSize); t += 0x1000) {
		pageTableManager.MapMemory((void*)t, (void*)t);
	}

	uint64_t fbBase = (uint64_t) bootInfo -> framebuffer -> BaseAddress;
	uint64_t fbSize = (uint64_t) bootInfo -> framebuffer -> BufferSize + 0x1000;
	for (uint64_t t = fbBase; t < fbBase + fbSize; t += 0x1000) {
		pageTableManager.MapMemory((void*)t, (void*)t);
	}

	asm ("mov %0, %%cr3" : : "r" (PML4));

	pageTableManager.MapMemory((void*)0x600000000, (void*)0x80000);

	uint64_t* test = (uint64_t*) 0x600000000;
	*test = 26;

	newRenderer.Print(to_string(*test));

	return;
}