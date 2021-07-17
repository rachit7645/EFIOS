#include "Panic.h"
#include "BasicRenderer.h"

void Panic(const char* panicMessage) {

	GlobalRenderer -> ClearColor = 0x000000FF;
	GlobalRenderer -> Clear();
	GlobalRenderer -> CursorPosition = {0, 0};
	GlobalRenderer -> Colour = 0xFFFFFFFF;

	GlobalRenderer -> Print("Kernel Panic\n\n");
	GlobalRenderer -> Print(panicMessage);

}