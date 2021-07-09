#pragma once

#include "../BasicRenderer.h"

struct interrupt_frame;
__attribute__((interrupt)) void PageFaultHandler(struct interrupt_frame* frame);