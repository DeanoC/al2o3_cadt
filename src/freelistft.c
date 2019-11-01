// License Summary: MIT see LICENSE file
#include "al2o3_platform/platform.h"
#include "al2o3_memory/memory.h"
#include "al2o3_cmath/scalar.h"
#include "al2o3_cadt/freelistft.h"

#define END_OF_LIST_SENTINEL (~0UL)
static bool AllocNewBlock(CADT_FreeListFT* fl) {

	// first thing we need to do is claim our new index range
	uint64_t baseIndex =
			Thread_AtomicFetchAdd64Relaxed(&fl->totalElementsAllocated, (fl->elementsPerBlockMask + 1));

	if (baseIndex >= (fl->elementsPerBlockMask + 1) * fl->maxBlocks) {
		LOGWARNING("Trying to allocate more than %i blocks! Increase block size or max blocks", fl->maxBlocks);
		Thread_AtomicFetchAdd64Relaxed(&fl->totalElementsAllocated, -(int64_t) (fl->elementsPerBlockMask + 1));
		return false;
	}

	ASSERT((baseIndex >> fl->elementsPerBlockShift) < fl->maxBlocks);

	size_t const blockSize = (fl->elementsPerBlockMask + 1) * fl->elementSize;

	uint8_t *base = (uint8_t *) MEMORY_CALLOC(1, blockSize);
	if (!base) {
		LOGWARNING("Out of memory!");
		return false;
	}

	Thread_AtomicStorePtrRelaxed(fl->blocks + (baseIndex >> fl->elementsPerBlockShift), base);

	// init free list for new block
	uint8_t * workPtr = base;
	for (uint32_t i = 0u; i < fl->elementsPerBlockMask; ++i) {
		*((uintptr_t *)workPtr) = (uintptr_t) (workPtr + fl->elementSize);
		workPtr += fl->elementSize;
	}


	// link the new block into the free list and attach existing free list to the
	// end of this block
Redo:;
	uint64_t const head = Thread_AtomicLoad64Relaxed(&fl->freeListHead);
	*((uintptr_t *)workPtr) = head;

	if (Thread_AtomicCompareExchange64Relaxed(&fl->freeListHead, head, (uint64_t)base) != head) {
		goto Redo; // something changed reverse the transaction
	}
	return true;
}

AL2O3_EXTERN_C CADT_FreeListFTHandle CADT_FreeListFTCreate(size_t elementSize, size_t blockCount, size_t maxBlocks) {
	ASSERT(elementSize >= sizeof(uintptr_t));

	if (!Math_IsPowerOf2SizeT(blockCount)) {
		LOGWARNING("blockCount (%u) should be a power of 2, using %u", blockCount, Math_NextPowerOfTwoSizeT(blockCount));
		blockCount = Math_NextPowerOfTwoSizeT(blockCount);
	}

	// each block has space for the data
	size_t const blockSize = (blockCount * elementSize);

	// first block is attached directly to the header
	size_t const allocSize = sizeof(CADT_FreeListFT)
			+ blockSize +
			8 + // padding to ensure atomics are at least 8 byte aligned
			(maxBlocks * sizeof(Thread_AtomicPtr_t));

	CADT_FreeListFT *fl = (CADT_FreeListFT *) MEMORY_CALLOC(1, allocSize);
	if (!fl) {
		return NULL;
	}
	fl->elementSize = elementSize;
	fl->elementsPerBlockMask = blockCount - 1;
	fl->elementsPerBlockShift = Math_Log2SizeT(blockCount);
	fl->maxBlocks = maxBlocks;

	uint8_t *base = (uint8_t *) (fl + 1);

	// get to blocks space with 8 byte alignment guarenteed
	fl->blocks = (Thread_AtomicPtr_t *) (((uintptr_t) base + blockSize + 0x8ull) & ~0x7ull);
	Thread_AtomicStorePtrRelaxed(fl->blocks + 0, base);
	Thread_AtomicStore64Relaxed(&fl->totalElementsAllocated, blockCount);

	// init free list for new block
	uint8_t * workPtr = base;
	for (uint32_t i = 0u; i < (blockCount-1); ++i) {
		*((uintptr_t *)workPtr) = (uintptr_t) (workPtr + fl->elementSize);
		workPtr += fl->elementSize;
	}

	// fix last index to point to the invalid marker
	*((uintptr_t *)workPtr) = END_OF_LIST_SENTINEL;

	// point head to start of the free list
	Thread_AtomicStore64Relaxed(&fl->freeListHead, (uintptr_t)base);

	return fl;
}

AL2O3_EXTERN_C void CADT_FreeListFTDestroy(CADT_FreeListFTHandle fl) {
	if (!fl) {
		return;
	}

	// 0th block is embedded
	for (uint32_t i = 1u; i < fl->maxBlocks; ++i) {
		void *ptr = Thread_AtomicLoadPtrRelaxed(&fl->blocks[i]);
		if (ptr) {
			MEMORY_FREE(ptr);
		}
	}

	MEMORY_FREE(fl);
}

AL2O3_EXTERN_C size_t CADT_FreeListFTElementSize(CADT_FreeListFTHandle freelist) {
	ASSERT(freelist != NULL);
	return freelist->elementSize;
}

AL2O3_EXTERN_C void *CADT_FreeListFTAlloc(CADT_FreeListFTHandle fl) {
	ASSERT(fl != NULL);

Redo:;
	uint64_t const head = Thread_AtomicLoad64Relaxed(&fl->freeListHead);
	// check to see if the free list is empty
	if (head == END_OF_LIST_SENTINEL) {
		// we only allow N thread to allocate at once, the other spin wait
		// this stops the potentially fatal incorrect failure if N > maxBlocks
		uint64_t isAllocating = Thread_AtomicFetchAdd64Relaxed(&fl->currentAllocating, 1);
		if(isAllocating <= 2) {
			// we now allocate a new block in a lock free way
			bool retry = AllocNewBlock(fl);
			if (retry == false) {
				LOGWARNING("freelist has run out of space");
				return NULL;
			}
		}
		Thread_AtomicFetchAdd64Relaxed(&fl->currentAllocating, -1);
		goto Redo;
	}

	uintptr_t next = *((uintptr_t*)head);

	if (Thread_AtomicCompareExchange64Relaxed(&fl->freeListHead, head, next) != head) {
		goto Redo; // something changed reverse the transaction
	}

	// the item is now ours to abuse
	// clear it out ready for its new life
	memset((void*)head, 0x0, fl->elementSize);

	return (void*)head;

}

AL2O3_EXTERN_C void CADT_FreeListFTRelease(CADT_FreeListFTHandle fl, void *vptr) {
	ASSERT(fl != NULL);
	ASSERT(vptr != NULL);

Redo:;
	uint64_t const head = Thread_AtomicLoad64Relaxed(&fl->freeListHead);
	uint64_t ptr = (uint64_t)vptr;
	*((uintptr_t*)ptr) = head;

	if (Thread_AtomicCompareExchange64Relaxed(&fl->freeListHead, head, ptr) != head) {
		goto Redo; // something changed reverse the transaction
	}
}


