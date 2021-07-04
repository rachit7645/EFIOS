#include "memory.h"

size_t GetMemorySize(EFI_MEMORY_DESCRIPTOR* mMap, uint64_t mMapEntries, uint64_t mMapDescSize) {

	static size_t memorySizeBytes = 0;
	if (memorySizeBytes > 0) return memorySizeBytes;

	for (size_t i = 0; i < mMapEntries; i++) {
		EFI_MEMORY_DESCRIPTOR* desc = (EFI_MEMORY_DESCRIPTOR*) ((uint64_t) mMap + (i * mMapDescSize));
		memorySizeBytes += desc -> numPages * 4096;
	}

	return memorySizeBytes;

}

void memset(void* start, uint8_t value, uint64_t num) {
	for (uint64_t i = 0; i < num; i++) {
		*(uint8_t*)((uint64_t)start + i) = value;
	}
}