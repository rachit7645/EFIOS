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
		uint32_t ClearColor;

		void Print(const char* str);
		void PutChar(char chr, uint32_t xOff, uint32_t yOff);
		void PutChar(char chr);
		void Clear();
		void ClearChar();
		void Next();

};

extern BasicRenderer* GlobalRenderer;