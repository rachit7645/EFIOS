#pragma once

#include <stdint.h>

#define EFI_CONVENTIONAL_MEMORY 	7

struct EFI_MEMORY_DESCRIPTOR {

	uint32_t type;
	void* physAddr;
	void* virtAddr;
	uint64_t numPages;
	uint64_t attribs;

};

extern const char* EFI_MEMORY_TYPES_STRINGS[];