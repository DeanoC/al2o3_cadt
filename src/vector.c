#include "al2o3_platform/platform.h"
#include "al2o3_memory/memory.h"
#include "al2o3_cmath/scalar.h"
#include "al2o3_cadt/vector.h"

typedef struct CADT_Vector {
	size_t elementSize;
	size_t capacity;
	size_t size;
	Memory_Allocator* allocator;

	uint8_t* data;
} CADT_Vector;
AL2O3_EXTERN_C void CADT_VectorDestroy(CADT_VectorHandle handle) {
	ASSERT(handle != NULL);
	CADT_Vector* vector = (CADT_Vector*)handle;
	if(vector->data != NULL) {
		MEMORY_ALLOCATOR_FREE(vector->allocator, vector->data);
	}
	MEMORY_ALLOCATOR_FREE(vector->allocator, vector);
}

AL2O3_EXTERN_C CADT_VectorHandle CADT_VectorClone(CADT_VectorHandle handle) {
	ASSERT(handle != NULL);
	CADT_Vector* ovector = (CADT_Vector*)handle;
	CADT_Vector* nvector = (CADT_Vector*)CADT_VectorCreate(ovector->elementSize);
	if (nvector == NULL) return NULL;
	CADT_VectorResize(nvector, ovector->size);
	memcpy(nvector->data, ovector->data, ovector->size * ovector->elementSize);
	return nvector;
}

AL2O3_EXTERN_C size_t CADT_VectorElementSize(CADT_VectorHandle handle) {
	ASSERT(handle != NULL);
	CADT_Vector const* vector = (CADT_Vector const*)handle;
	return vector->elementSize;
}

AL2O3_EXTERN_C size_t CADT_VectorSize(CADT_VectorHandle handle) {
	ASSERT(handle != NULL);
	CADT_Vector const* vector = (CADT_Vector const*)handle;
	return vector->size;
}

AL2O3_EXTERN_C void CADT_VectorResize(CADT_VectorHandle handle, size_t size) {
	ASSERT(handle != NULL);
	CADT_Vector * vector = (CADT_Vector *)handle;

	CADT_VectorReserve(handle, size);
	vector->size = size;
}
AL2O3_EXTERN_C void CADT_VectorResizeWithZero(CADT_VectorHandle handle, size_t size) {
	ASSERT(handle != NULL);
	CADT_Vector* vector = (CADT_Vector*)handle;

	CADT_VectorReserve(handle, size);
	if (size > vector->size) {
		size_t const dif = size - vector->size;
		memset(vector->data + (vector->size * vector->elementSize), 0, dif * vector->elementSize);
	}
	vector->size = size;
}

AL2O3_EXTERN_C void CADT_VectorResizeWithDefault(CADT_VectorHandle handle, size_t size, void const* defaultData) {
	if (!defaultData) {
		CADT_VectorResizeWithZero(handle, size);
		return;
	}

	ASSERT(handle != NULL);
	CADT_Vector* vector = (CADT_Vector*)handle;

	CADT_VectorReserve(handle, size);
	if (size > vector->size) {
		size_t const dif = size - vector->size;
		for (size_t i = 0; i < dif; ++i) {
			memcpy(vector->data + ((vector->size + i) * vector->elementSize), defaultData, vector->elementSize);
		}
	}

	vector->size = size;
}

AL2O3_EXTERN_C bool CADT_VectorIsEmpty(CADT_VectorHandle handle) {
	ASSERT(handle != NULL);
	CADT_Vector const* vector = (CADT_Vector const*)handle;
	return (vector->size == 0);
}
AL2O3_EXTERN_C size_t CADT_VectorCapacity(CADT_VectorHandle handle) {
	ASSERT(handle != NULL);
	CADT_Vector const* vector = (CADT_Vector const*)handle;
	return vector->capacity;
}


AL2O3_EXTERN_C void* CADT_VectorAt(CADT_VectorHandle handle, size_t index) {
	ASSERT(handle != NULL);
	CADT_Vector const* vector = (CADT_Vector const*)handle;
	ASSERT(index < vector->size);
	ASSERT(vector->data);
	return vector->data + (index * vector->elementSize);
}

AL2O3_EXTERN_C size_t CADT_VectorPushElement(CADT_VectorHandle handle, void const* element) {
	ASSERT(handle != NULL);
	CADT_Vector * vector = (CADT_Vector *)handle;
	size_t const index = vector->size;
	CADT_VectorResize(handle, vector->size+1);
	ASSERT(index < vector->size);

	memcpy(CADT_VectorAt(handle, index), element, vector->elementSize);
	return index;
}

AL2O3_EXTERN_C void CADT_VectorPopElement(CADT_VectorHandle handle, void* out) {
	ASSERT(handle != NULL);
	CADT_Vector * vector = (CADT_Vector *)handle;
	ASSERT(vector->size > 0);
	size_t const index = vector->size-1;
	memcpy(out, CADT_VectorAt(handle, index), vector->elementSize);
	CADT_VectorResize(handle, vector->size-1);
}

AL2O3_EXTERN_C void CADT_VectorPopAndDiscardElement(CADT_VectorHandle handle) {
	ASSERT(handle != NULL);
	CADT_Vector * vector = (CADT_Vector *)handle;
	ASSERT(vector->size > 0);
	CADT_VectorResize(handle, vector->size-1);
}

AL2O3_EXTERN_C void* CADT_VectorPeekElement(CADT_VectorHandle handle) {
	ASSERT(handle != NULL);
	CADT_Vector const* vector = (CADT_Vector const*)handle;
	ASSERT(vector->size > 0);
	size_t const index = vector->size-1;
	return CADT_VectorAt(handle, index);
}

AL2O3_EXTERN_C void CADT_VectorRemove(CADT_VectorHandle handle, size_t index) {
	ASSERT(handle != NULL);
	CADT_Vector * vector = (CADT_Vector *)handle;
	ASSERT(index < vector->size);
	if (vector->size == 1) {
		vector->size = 0;
		return;
	}
	if (vector->size-1 != index) {
		uint8_t* dst = vector->data + (index * vector->elementSize);
		uint8_t* src = dst + vector->elementSize;
		size_t copySize = (vector->size - index - 1) * vector->elementSize;

		memmove(dst, src, copySize);
	}

	vector->size = vector->size - 1;
	return;
}

AL2O3_EXTERN_C void CADT_VectorReplace(CADT_VectorHandle handle, size_t srcIndex, size_t dstIndex) {
	ASSERT(handle != NULL);
	CADT_Vector const* vector = (CADT_Vector const*)handle;
	memcpy(CADT_VectorAt(handle, dstIndex), CADT_VectorAt(handle, srcIndex), vector->elementSize);
}

AL2O3_EXTERN_C void CADT_VectorSwap(CADT_VectorHandle handle, size_t index0, size_t index1) {
	ASSERT(handle != NULL);
	CADT_Vector const* vector = (CADT_Vector const*)handle;
	void* tmp = STACK_ALLOC(vector->elementSize);
	memcpy(tmp, CADT_VectorAt(handle, index0), vector->elementSize);
	memcpy(CADT_VectorAt(handle, index0), CADT_VectorAt(handle, index1), vector->elementSize);
	memcpy(CADT_VectorAt(handle, index1), tmp, vector->elementSize);
}

AL2O3_EXTERN_C void CADT_VectorSwapRemove(CADT_VectorHandle handle, size_t index) {
	ASSERT(handle != NULL);
	CADT_Vector const* vector = (CADT_Vector const*)handle;
	CADT_VectorReplace(handle, vector->size - 1, index);
	CADT_VectorResize(handle, vector->size - 1);
}

AL2O3_EXTERN_C void* CADT_VectorData(CADT_VectorHandle handle) {
	ASSERT(handle != NULL);
	CADT_Vector const* vector = (CADT_Vector const*)handle;
	return vector->data;
}

AL2O3_EXTERN_C size_t CADT_VectorFind(CADT_VectorHandle handle, void const* data) {
	ASSERT(handle != NULL);
	CADT_Vector const* vector = (CADT_Vector const*)handle;
	for (size_t i = 0; i < vector->size; ++i) {
		if (memcmp(data, vector->data + (i * vector->elementSize), vector->elementSize) == 0) {
			return i;
		}
	}
	return (size_t)-1;
}

#if MEMORY_TRACKING_SETUP == 1
#undef CADT_VectorCreate
#undef CADT_VectorCreateWithAllocator
#undef CADT_VectorReserve
#endif

AL2O3_EXTERN_C CADT_VectorHandle CADT_VectorCreate(size_t elementSize) {
	return CADT_VectorCreateWithAllocator(elementSize, &Memory_GlobalAllocator);
}

AL2O3_EXTERN_C CADT_VectorHandle CADT_VectorCreateWithAllocator(size_t elementSize, Memory_Allocator* allocator) {
#if MEMORY_TRACKING_SETUP == 1
	// call the allocator direct, so that the line and file comes free the caller
	CADT_Vector* vector = allocator->calloc(1, sizeof(CADT_Vector));
#else
	CADT_Vector* vector = MEMORY_ALLOCATOR_CALLOC(allocator,1, sizeof(CADT_Vector));
#endif
	if(vector == NULL) return NULL;
	vector->elementSize = elementSize;
	vector->allocator = allocator;
	return vector;
}

AL2O3_EXTERN_C void CADT_VectorReserve(CADT_VectorHandle handle, size_t size) {
	ASSERT(handle != NULL);
	CADT_Vector * vector = (CADT_Vector *)handle;

	// reserve always grews unless ShrinkToFit
	if(size <= vector->capacity) return;

	void* oldData = vector->data;
	size_t const oldCapacity = (vector->capacity) ? vector->capacity : 1;
	size_t const newCapacity = Math_MaxSizeT(oldCapacity*2, size);
	vector->capacity = newCapacity;
#if MEMORY_TRACKING_SETUP == 1
	vector->data = (uint8_t*) vector->allocator->calloc(newCapacity, vector->elementSize);
#else
	vector->data = (uint8_t*) MEMORY_ALLOCATOR_CALLOC(vector->allocator, newCapacity, vector->elementSize);
#endif
	ASSERT(vector->data);
	if(oldData) {
		memcpy(vector->data, oldData, vector->size * vector->elementSize);
		MEMORY_ALLOCATOR_FREE(vector->allocator, oldData);
	}
}
