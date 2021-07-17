#include "Interrupts.h"
#include "../Panic.h"
#include "../IO.h"

INTERRUPT void PageFaultHandler(struct interrupt_frame* frame) {
	Panic("Page Fault Detected");
	while(true) asm("hlt");
}

INTERRUPT void DoubleFaultHandler(struct interrupt_frame* frame) {
	Panic("Double Fault Detected");
	while(true) asm("hlt");
}

INTERRUPT void GPFaultHandler(struct interrupt_frame* frame) {
	Panic("General Protection Fault Detected");
	while(true) asm("hlt");
}

INTERRUPT void KeyboardHandler(struct interrupt_frame* frame) {
	GlobalRenderer -> Print(" Pressed");
	uint8_t scancode = inb(0x60);
	PICEndMaster();
}

void PICEndMaster() {
	outb(PIC1_COMMAND, PIC_EOI);
}

void PICEndSlave() {
	outb(PIC2_COMMAND, PIC_EOI);
	outb(PIC1_COMMAND, PIC_EOI);
}

void RemapPIC() {
	uint8_t a1, a2;
	a1 = inb(PIC1_DATA);
	io_wait();
	a2 = inb(PIC2_DATA);
	io_wait();

	outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
	io_wait();
	outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
	io_wait();

	outb(PIC1_DATA, 0x20);
    io_wait();
    outb(PIC2_DATA, 0x28);
    io_wait();

    outb(PIC1_DATA, 4);
    io_wait();
    outb(PIC2_DATA, 2);
    io_wait();

    outb(PIC1_DATA, ICW4_8086);
    io_wait();
    outb(PIC2_DATA, ICW4_8086);
    io_wait();

	outb(PIC1_DATA, a1);
	io_wait();
	outb(PIC2_DATA, a2);
	io_wait();

}
