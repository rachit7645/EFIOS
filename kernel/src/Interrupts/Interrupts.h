#pragma once

#include "../BasicRenderer.h"

#define INTERRUPT __attribute__((interrupt))

#define PIC1_COMMAND	0x20
#define PIC1_DATA		0x21
#define PIC2_COMMAND	0xA0
#define PIC2_DATA		0xA1

#define ICW1_INIT		0x10
#define ICW1_ICW4		0x01
#define ICW4_8086		0x01

#define PIC_EOI			0x20

struct interrupt_frame;

INTERRUPT void PageFaultHandler(struct interrupt_frame* frame);
INTERRUPT void DoubleFaultHandler(struct interrupt_frame* frame);
INTERRUPT void GPFaultHandler(struct interrupt_frame* frame);

INTERRUPT void KeyboardHandler(struct interrupt_frame* frame);

void RemapPIC();
void PICEndMaster();
void PICEndSlave();