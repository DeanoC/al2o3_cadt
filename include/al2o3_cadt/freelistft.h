// License Summary: MIT see LICENSE file
#pragma once

#include "al2o3_platform/platform.h"
#include "al2o3_thread/atomic.h"
typedef struct CADT_FreeListFT {
	uint64_t elementSize;
	uint64_t maxBlocks;
	uint32_t elementsPerBlockMask;
	uint32_t elementsPerBlockShift;

	Thread_Atomic64_t freeListHead;
	Thread_AtomicPtr_t *blocks;
	Thread_Atomic64_t totalElementsAllocated;
	Thread_Atomic64_t currentAllocating;
} CADT_FreeListFT;

typedef struct CADT_FreeListFT *CADT_FreeListFTHandle;

// free threaded (any thread can call at any time)
AL2O3_EXTERN_C size_t CADT_FreeListFTElementSize(CADT_FreeListFTHandle handle);
AL2O3_EXTERN_C void* CADT_FreeListFTAlloc(CADT_FreeListFTHandle fl);
AL2O3_EXTERN_C void CADT_FreeListFTRelease(CADT_FreeListFTHandle handle, void* ptr);

// not thread safe!
AL2O3_EXTERN_C CADT_FreeListFTHandle CADT_FreeListFTCreate(size_t elementSize, size_t blockCount, size_t maxBlocks);
AL2O3_EXTERN_C void CADT_FreeListFTDestroy(CADT_FreeListFTHandle handle);
AL2O3_EXTERN_C void CADT_FreeListFTReset(CADT_FreeListFTHandle handle, bool freeAllocatedMemory);

