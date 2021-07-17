#pragma once

#include "BasicRenderer.h"
#include "cstr.h"
#include "Paging/PageTableManager.h"

#define NORETURN __attribute__((noreturn))

struct BootInfo {

	FrameBuffer* framebuffer;
	PSF1_FONT* psf1_Font;
	EFI_MEMORY_DESCRIPTOR* mMap;
	uint64_t mMapSize;
	uint64_t mMapDescSize;

};

extern uint64_t _KernelStart;
extern uint64_t _KernelEnd;

struct KernelInfo {
	PageTableManager* pageTableManager;
};

KernelInfo InitializeKernel(BootInfo* bootInfo);