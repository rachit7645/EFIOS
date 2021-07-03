#include "BasicRenderer.h"
#include "cstr.h"
#include "EFIMemory.h"
#include "memory.h"
#include "PageFrameAllocater.h"

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
	PageFrameAllocater newAllocater;

	size_t mMapEntries = bootInfo -> mMapSize / bootInfo -> mMapDescSize;
	size_t memorySizeBytes = GetMemorySize(bootInfo -> mMap, mMapEntries, bootInfo -> mMapDescSize);
	
	newAllocater.ReadEFIMemoryMap(bootInfo -> mMap, bootInfo -> mMapSize, bootInfo -> mMapDescSize);

	uint64_t KernelSize = (uint64_t)&_KernelEnd - (uint64_t)&_KernelStart;
	uint64_t KernelPages = (uint64_t)KernelSize /  4096 + 1;
	newAllocater.LockPages(&_KernelStart, KernelPages);

	newRenderer.Print("Free Ram: ");
	newRenderer.Print(to_string(newAllocater.getFreeRam() / 1024));
	newRenderer.Print(" KB\n");
	newRenderer.Print("Used Ram: ");
	newRenderer.Print(to_string(newAllocater.getUsedRam() / 1024));
	newRenderer.Print(" KB\n");
	newRenderer.Print("Reserved Ram: ");
	newRenderer.Print(to_string(newAllocater.getReservedRam() / 1024));
	newRenderer.Print(" KB\n");

	for (size_t i = 0; i < 20; i++) {
		void* address = newAllocater.RequestPage();
		newRenderer.Print(to_hstring((uint64_t)address));
		newRenderer.Print("\n");
	}

	return;
}