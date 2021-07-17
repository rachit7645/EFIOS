#include "KernelUtil.h"

extern "C" NORETURN void _start(BootInfo* bootInfo) {

	KernelInfo kernelInfo = InitializeKernel(bootInfo);
	PageTableManager* pageTableManager = kernelInfo.pageTableManager;

	GlobalRenderer -> Print("Kernel Initialized Successfully\n");

	while (true) asm("hlt");
}	