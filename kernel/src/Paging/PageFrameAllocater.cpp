#include "PageFrameAllocater.h"

uint64_t freeMemory;
uint64_t reservedMemory;
uint64_t usedMemory;
bool Initialized = false;

PageFrameAllocater GlobalAllocator;

void PageFrameAllocater::ReadEFIMemoryMap(EFI_MEMORY_DESCRIPTOR* mMap, size_t mMapSize, size_t mMapDescSize) {

	if(Initialized) return;

	Initialized = true;
	
	uint64_t mMapEntries = mMapSize / mMapDescSize;

	void* largestFreeMemSeg = NULL;
	size_t largestFreeMemSegSize = 0;

	for (size_t i = 0; i < mMapEntries; i++) {

		EFI_MEMORY_DESCRIPTOR* desc = (EFI_MEMORY_DESCRIPTOR*) ((uint64_t)mMap + (i * mMapDescSize));
		if (desc -> type == EFI_CONVENTIONAL_MEMORY) {
			if (desc -> numPages * 4096 > largestFreeMemSegSize) {
				largestFreeMemSeg = desc -> physAddr;
				largestFreeMemSegSize = desc -> numPages * 4096;
			}
		}
	}

	uint64_t memorySize = GetMemorySize(mMap, mMapEntries, mMapDescSize);
	freeMemory = memorySize;

	uint64_t bitmapSize = memorySize / 4096 / 8 + 1;

	InitBitmap(bitmapSize, largestFreeMemSeg);

	// Lock Pages

	LockPages(&PageBitmap, PageBitmap.Size / 4096 + 1);

	// Reserve Pages of Unusable / Reserved memory

	for (uint64_t i = 0; i < mMapEntries; i++){
        EFI_MEMORY_DESCRIPTOR* desc = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)mMap + (i * mMapDescSize));
        if (desc -> type != EFI_CONVENTIONAL_MEMORY) { 
            ReservePages(desc->physAddr, desc->numPages);
        }
    }

}

void PageFrameAllocater::InitBitmap(size_t bitmapSize, void* bufferAddress) {

	this -> PageBitmap.Size = bitmapSize;
	this -> PageBitmap.Buffer = (uint8_t*)bufferAddress;

	for (size_t i = 0; i < bitmapSize; i++) {
		*(uint8_t*) (PageBitmap.Buffer + i) = 0;
	}

}
uint64_t pageBitmapIndex = 0;
void* PageFrameAllocater::RequestPage() {

	for (; pageBitmapIndex < PageBitmap.Size * 8; pageBitmapIndex++) {
		if (PageBitmap[pageBitmapIndex] == true) continue;
		LockPage((void*)(pageBitmapIndex * 4096));
		return (void*)(pageBitmapIndex * 4096);
	}

	return NULL;		// Page frame swap to file

}

void PageFrameAllocater::FreePage(void* address) {

	uint64_t index = (uint64_t) address / 4096;
	if (PageBitmap[index] == false) return;
	if (PageBitmap.Set(index, false)) {
		freeMemory += 4096;
		usedMemory -= 4096;
		if (pageBitmapIndex > index) pageBitmapIndex = index;
	}

}

void PageFrameAllocater::FreePages(void* address, uint64_t pageCount) {

	for (size_t t = 0; t < pageCount; t++) {
		FreePage((void*)((uint64_t)address + t * 4096));
	}
 
}

void PageFrameAllocater::LockPage(void* address) {

	uint64_t index = (uint64_t) address / 4096;
	if (PageBitmap[index] == true) return;
	if (PageBitmap.Set(index, true)) {
		freeMemory -= 4096;
		usedMemory += 4096;
	}

}

void PageFrameAllocater::LockPages(void* address, uint64_t pageCount) {

	for (size_t t = 0; t < pageCount; t++) {
		LockPage((void*)((uint64_t)address + t * 4096));
	}
 
}

void PageFrameAllocater::ReleasePage(void* address) {

	uint64_t index = (uint64_t) address / 4096;
	if (PageBitmap[index] == false) return;
	if (PageBitmap.Set(index, false)) {
		freeMemory += 4096;
		reservedMemory -= 4096;
	}

}

void PageFrameAllocater::ReleasePages(void* address, uint64_t pageCount) {

	for (size_t t = 0; t < pageCount; t++) {
		ReleasePage((void*)((uint64_t)address + t * 4096));
	}
 
}

void PageFrameAllocater::ReservePage(void* address) {

	uint64_t index = (uint64_t)address / 4096;
    if (PageBitmap[index] == true) return;
    if (PageBitmap.Set(index, true)) {
    	freeMemory -= 4096;
    	reservedMemory += 4096;
		if (pageBitmapIndex > index) pageBitmapIndex = index;
	}

}

void PageFrameAllocater::ReservePages(void* address, uint64_t pageCount) {

	for (size_t t = 0; t < pageCount; t++) {
		ReservePage((void*)((uint64_t)address + t * 4096));
	}
 
}

uint64_t PageFrameAllocater::getFreeRam() {
	return freeMemory;
}

uint64_t PageFrameAllocater::getUsedRam() {
	return usedMemory;
}

uint64_t PageFrameAllocater::getReservedRam() {
	return reservedMemory;
}