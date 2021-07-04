#include "KernelUtil.h"

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

KernelInfo InitializeKernel(BootInfo* bootInfo) {

	PrepareMemory(bootInfo);

	memset(bootInfo -> framebuffer -> BaseAddress, 0, bootInfo -> framebuffer -> BufferSize);

	return kernelInfo;

}