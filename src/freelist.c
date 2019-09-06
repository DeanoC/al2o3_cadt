#include "al2o3_platform/platform.h"
#include "al2o3_memory/memory.h"
#include "al2o3_cmath/scalar.h"
#include "al2o3_cadt/vector.h"
#include "al2o3_cadt/dict.h"
#include "al2o3_cadt/freelist.h"

typedef struct CADT_FreeList {
	size_t elementSize;
	size_t capacity;
	size_t size;

	uintptr_t headIndex;

} CADT_FreeList;

#define END_OF_LIST_SENTINEL (~0UL)

AL2O3_EXTERN_C CADT_FreeListHandle CADT_FreeListCreate(size_t elementSize, size_t capacity) {
	ASSERT(elementSize >= sizeof(uintptr_t));
	ASSERT(capacity > 0);

	size_t const size = ((sizeof(CADT_FreeList) + sizeof(elementSize) * capacity) + sizeof(uint64_t))/ sizeof(uint64_t);
	CADT_FreeList *freelist = MEMORY_CALLOC(1, size);
	if (freelist == NULL)
		return NULL;
	freelist->elementSize = elementSize;
	freelist->headIndex = 0;
	freelist->capacity = capacity;

	uint8_t *data = (uint8_t *) (freelist + 1);
	for (size_t i = 0; i < capacity - 1; ++i) {
		*(uintptr_t *) data = i + 1;
		data += freelist->elementSize;
	}
	*(uintptr_t *) data = END_OF_LIST_SENTINEL;

	return freelist;
}

AL2O3_EXTERN_C void CADT_FreeListDestroy(CADT_FreeListHandle handle) {
	ASSERT(handle != NULL);
	CADT_FreeList *freelist = (CADT_FreeList *) handle;
	MEMORY_FREE(freelist);
}

AL2O3_EXTERN_C CADT_FreeListHandle CADT_FreeListClone(CADT_FreeListHandle handle) {
	ASSERT(handle != NULL);
	CADT_FreeList const *ofreelist = (CADT_FreeList const *) handle;
	size_t const size = sizeof(CADT_FreeList) + sizeof(ofreelist->elementSize) * ofreelist->capacity;

	CADT_FreeList *freelist = MEMORY_MALLOC(size);
	if (freelist == NULL)
		return NULL;

	memcpy(freelist, ofreelist, size);

	return freelist;
}

AL2O3_EXTERN_C void *CADT_FreeListAlloc(CADT_FreeListHandle handle) {
	ASSERT(handle != NULL);
	CADT_FreeList *freelist = (CADT_FreeList *) handle;

	if (freelist->headIndex == END_OF_LIST_SENTINEL) {
		return NULL;
	}
	uint8_t *data = (uint8_t *) (freelist + 1);
	data = data + (freelist->headIndex * freelist->elementSize);
	freelist->headIndex = *(uintptr_t *) data;

	return data;
}

AL2O3_EXTERN_C void CADT_FreeListFree(CADT_FreeListHandle handle, void *ptr) {
	ASSERT(handle != NULL);
	ASSERT(ptr != NULL);
	CADT_FreeList *freelist = (CADT_FreeList *) handle;

	uint8_t *data = (uint8_t *) (freelist + 1);
	ASSERT((uint8_t *) ptr >= data);
	ASSERT((uint8_t *) ptr < data + freelist->elementSize * freelist->capacity);

	*((uintptr_t *) ptr) = freelist->headIndex;
	freelist->headIndex = ((uint8_t *) ptr - data) / freelist->elementSize;
}
