#include "KernelUtil.h"

extern "C" void _start(BootInfo* bootInfo) {

	KernelInfo kernelInfo = InitializeKernel(bootInfo);
	PageTableManager* pageTableManager = kernelInfo.pageTableManager;

	GlobalRenderer -> Print("Kernel Initialized Successfully");

	asm ("int $0x0E");

	while (true) asm("hlt");
}	