#pragma once

#include "Math.h"
#include "FrameBuffer.h"
#include "SimpleFont.h"

class BasicRenderer {

	public:

		BasicRenderer(FrameBuffer* TargetFrameBuffer, PSF1_FONT* PSF1_Font);

		Point CursorPosition;
		FrameBuffer* TargetFrameBuffer;
		PSF1_FONT* PSF1_Font;
		uint32_t Colour;

		void Print(const char* str);
		void PutChar(char chr, uint32_t xOff, uint32_t yOff);

};