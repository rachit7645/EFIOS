#pragma once

#include "Paging.h"
#include "PageMapIndexer.h"
#include "PageFrameAllocater.h"

class PageTableManager {

	public:
		PageTableManager(PageTable* PML4);
		PageTable* PML4;
		void MapMemory(void* virtualMemory, void* physicalMemory);

};