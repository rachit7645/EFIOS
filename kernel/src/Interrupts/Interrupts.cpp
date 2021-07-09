#include "Interrupts.h"

void PageFaultHandler(struct interrupt_frame* frame) {
	GlobalRenderer -> Print("Page Fault Detected");
	while(true) asm("hlt");
}