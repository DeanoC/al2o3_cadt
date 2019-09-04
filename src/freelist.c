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

	uint8_t* data;
	uintptr_t headIndex;

} CADT_FreeList;

#define END_OF_LIST_SENTINEL (~0UL)

AL2O3_EXTERN_C CADT_FreeListHandle CADT_FreeListCreate(size_t elementSize, size_t capacity) {
	ASSERT(elementSize >= sizeof(uintptr_t));
	ASSERT(capacity > 0);

	CADT_FreeList* freelist = MEMORY_CALLOC(1, sizeof(CADT_FreeList));
	if(freelist == NULL) return NULL;
	freelist->elementSize = elementSize;

	freelist->data = (uint8_t*)MEMORY_MALLOC(sizeof(elementSize) * capacity);
	if(freelist->data == NULL) {
		MEMORY_FREE(freelist);
		return NULL;
	}

	for (size_t i = 0; i < capacity-1;++i) {
		uintptr_t* data = (uintptr_t*)(freelist->data + (i * freelist->elementSize));
		*data = i+1;
	}
	uintptr_t* data = (uintptr_t*)(freelist->data + ((capacity-1) * freelist->elementSize));
	*data = END_OF_LIST_SENTINEL;

	freelist->headIndex = 0;
	freelist->capacity = capacity;

	return freelist;
}

AL2O3_EXTERN_C void CADT_FreeListDestroy(CADT_FreeListHandle handle) {
	ASSERT(handle != NULL);
	CADT_FreeList* freelist = (CADT_FreeList*)handle;
	if(freelist->data != NULL) {
		MEMORY_FREE(freelist->data);
	}
	MEMORY_FREE(freelist);
}

AL2O3_EXTERN_C CADT_FreeListHandle CADT_FreeListClone(CADT_FreeListHandle handle) {
	ASSERT(handle != NULL);
	CADT_FreeList* ofreelist = (CADT_FreeList*)handle;
	CADT_FreeList* nfreelist = (CADT_FreeList*)CADT_FreeListCreate(ofreelist->elementSize, ofreelist->capacity);
	if (nfreelist == NULL) return NULL;
	memcpy(nfreelist->data, ofreelist->data, ofreelist->size * ofreelist->elementSize);
	nfreelist->headIndex = ofreelist->headIndex;

	return nfreelist;
}

AL2O3_EXTERN_C void* CADT_FreeListAlloc(CADT_FreeListHandle handle) {
	ASSERT(handle != NULL);
	CADT_FreeList* freelist = (CADT_FreeList*)handle;

	if (freelist->headIndex == END_OF_LIST_SENTINEL) {
		return NULL;
	}

	void* data = freelist->data + (freelist->headIndex * freelist->elementSize);
	freelist->headIndex = *(uintptr_t*)data;

	return data;
}

AL2O3_EXTERN_C void CADT_FreeListFree(CADT_FreeListHandle handle, void* ptr) {
	ASSERT(handle != NULL);
	ASSERT(ptr != NULL);
	CADT_FreeList* freelist = (CADT_FreeList*)handle;

	ASSERT((uint8_t*)ptr >= freelist->data);
	ASSERT((uint8_t*)ptr < freelist->data + freelist->elementSize * freelist->capacity);

	*((uintptr_t*)ptr) = freelist->headIndex;
	freelist->headIndex = ((uint8_t*)ptr - freelist->data) / freelist->elementSize;
}
