#include "BasicRenderer.h"

BasicRenderer* GlobalRenderer;

BasicRenderer::BasicRenderer(FrameBuffer* TargetFrameBuffer, PSF1_FONT* PSF1_Font) 
{
	this -> TargetFrameBuffer = TargetFrameBuffer;
	this -> PSF1_Font = PSF1_Font;
	this -> Colour = 0xFFFFFFFF;
	this -> CursorPosition = {0, 0};

}

void BasicRenderer::Clear() {

	uint64_t fbBase = (uint64_t) TargetFrameBuffer -> BaseAddress;
	uint64_t bytesPerScanLine = TargetFrameBuffer -> PixelsPerScanLine * 4;
	uint64_t fbHeight = TargetFrameBuffer -> Height;

	for (size_t verticalScanLine = 0; verticalScanLine < fbHeight; verticalScanLine++) {
		uint64_t pixPtrBase = fbBase + (bytesPerScanLine * verticalScanLine);
		for (uint32_t* pixPtr = (uint32_t*)pixPtrBase; pixPtr < (uint32_t*)(pixPtrBase + bytesPerScanLine); pixPtr++) {
			*pixPtr = ClearColor;
		}
	}
}

void BasicRenderer::ClearChar() {
	if (CursorPosition.X == 0) {
		CursorPosition.X = TargetFrameBuffer -> Width;
		CursorPosition.Y -= 18;
		if (CursorPosition.Y < 0) CursorPosition.Y = 0;
	}

	uint32_t xOff = CursorPosition.X;
	uint32_t yOff = CursorPosition.Y;

	uint32_t* pixPtr = (uint32_t*) TargetFrameBuffer -> BaseAddress;

	for(uint64_t y = yOff; y < yOff + PSF1_Font -> psf1_Header -> charsize; y++) {
		for(uint64_t x = xOff - 8; x < xOff; x++) {
			*(uint64_t*) (pixPtr + x + (y * TargetFrameBuffer -> PixelsPerScanLine)) = ClearColor;
		}
	}

	CursorPosition.X -= 8;

	if (CursorPosition.X < 0) {
		CursorPosition.X = TargetFrameBuffer -> Width;
		CursorPosition.Y -= 16;
		if (CursorPosition.Y < 0) CursorPosition.Y = 0;
	}

}

void BasicRenderer::Print(const char* str) {

	char* chr = (char*)str;
	while (*chr != 0) {

		if (*chr == '\n') {
			GlobalRenderer -> Next();
			chr++;
			continue;
		}

		PutChar(*chr, CursorPosition.X, CursorPosition.Y);
		CursorPosition.X += 8;
		if (CursorPosition.X + 8 > TargetFrameBuffer -> Width) {
			CursorPosition.X = 0;
			CursorPosition.Y += 18;
		}
		chr++;
	}

}

void BasicRenderer::PutChar(char chr, uint32_t xOff, uint32_t yOff)
{

	uint32_t* pixPtr = (uint32_t*) TargetFrameBuffer -> BaseAddress;
	char* fontPtr = (char*)PSF1_Font -> glyphBuffer + (chr * PSF1_Font -> psf1_Header -> charsize);

	for(uint64_t y = yOff; y < yOff + PSF1_Font -> psf1_Header -> charsize; y++) {
		for(uint64_t x = xOff; x < xOff + 8; x++) {
			if((*fontPtr & (0b10000000 >> (x - xOff))) > 0) {
				*(uint64_t*) (pixPtr + x + (y * TargetFrameBuffer -> PixelsPerScanLine)) = Colour;
			}
		}
		fontPtr++;
	}

}

void BasicRenderer::PutChar(char chr) {
	PutChar(chr, CursorPosition.X, CursorPosition.Y);
	CursorPosition.X += 8;
	if (CursorPosition.X + 8 > TargetFrameBuffer -> Width) {
		CursorPosition.X = 0;
		CursorPosition.Y += 18;	
	}
}

void BasicRenderer::Next() {
	CursorPosition.X = 0;
	CursorPosition.Y += 18;
}
