#include "al2o3_platform/platform.h"
#include "al2o3_memory/memory.h"
#include "al2o3_cmath/scalar.h"
#include "al2o3_cadt/vector.h"

typedef struct CADT_Vector {
	size_t elementSize;
	size_t capacity;
	size_t size;

	uint8_t* data;
} CADT_Vector;

AL2O3_EXTERN_C CADT_VectorHandle CADT_VectorCreate(size_t elementSize) {
	CADT_Vector* vector = MEMORY_CALLOC(1, sizeof(CADT_Vector));
	if(vector == NULL) return NULL;
	vector->elementSize = elementSize;
	return vector;
}

AL2O3_EXTERN_C void CADT_VectorDestroy(CADT_VectorHandle handle) {
	ASSERT(handle != NULL);
	CADT_Vector* vector = (CADT_Vector*)handle;
	if(vector->data != NULL) {
		MEMORY_FREE(vector->data);
	}
	MEMORY_FREE(vector);
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

AL2O3_EXTERN_C void CADT_VectorReserve(CADT_VectorHandle handle, size_t size) {
	ASSERT(handle != NULL);
	CADT_Vector * vector = (CADT_Vector *)handle;

	// reserve always grews unless ShrinkToFit
	if(size <= vector->capacity) return;

	void* oldData = vector->data;
	size_t const oldCapacity = (vector->capacity) ? vector->capacity : 1;
	size_t const newCapacity = Math_MaxSizeT(oldCapacity*2, size);
	vector->capacity = newCapacity;
	vector->data = (uint8_t*) MEMORY_CALLOC(newCapacity, vector->elementSize);
	ASSERT(vector->data);
	if(oldData) {
		memcpy(vector->data, oldData, vector->size * vector->elementSize);
		MEMORY_FREE(oldData);
	}
}

AL2O3_EXTERN_C void* CADT_VectorElement(CADT_VectorHandle handle, size_t index) {
	ASSERT(handle != NULL);
	CADT_Vector const* vector = (CADT_Vector const*)handle;
	ASSERT(index < vector->size);
	ASSERT(vector->data);
	return vector->data + (index * vector->elementSize);
}

AL2O3_EXTERN_C size_t CADT_VectorPushElement(CADT_VectorHandle handle, void* element) {
	ASSERT(handle != NULL);
	CADT_Vector * vector = (CADT_Vector *)handle;
	size_t const index = vector->size;
	CADT_VectorResize(handle, vector->size+1);
	ASSERT(index < vector->size);

	memcpy(CADT_VectorElement(handle, index), element, vector->elementSize);
	return index;
}

AL2O3_EXTERN_C void CADT_VectorPopElementFrom(CADT_VectorHandle handle, void* out) {
	ASSERT(handle != NULL);
	CADT_Vector * vector = (CADT_Vector *)handle;
	ASSERT(vector->size > 0);
	size_t const index = vector->size-1;
	memcpy(out, CADT_VectorElement(handle, index), vector->elementSize);
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
	return CADT_VectorElement(handle, index);
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

AL2O3_EXTERN_C void* CADT_VectorData(CADT_VectorHandle handle) {
	ASSERT(handle != NULL);
	CADT_Vector const* vector = (CADT_Vector const*)handle;
	return vector->data;
}
