#include "al2o3_platform/platform.h"
#include "al2o3_memory/memory.h"
#include "al2o3_cmath/scalar.h"
#include "al2o3_cadt/vector.h"

typedef struct CADT_Vector_t {
	size_t elementSize;
	size_t capacity;
	size_t size;

	void* data;
} CADT_Vector_t;

AL2O3_EXTERN_C CADT_VectorHandle CADT_VectorCreate(size_t elementSize) {
	CADT_Vector_t* vector = MEMORY_CALLOC(1, sizeof(CADT_Vector_t));
	if(vector == NULL) return NULL;
	vector->elementSize = elementSize;
	return vector;
}

AL2O3_EXTERN_C void CADT_VectorDestroy(CADT_VectorHandle handle) {
	ASSERT(handle != NULL);
	CADT_Vector_t* vector = (CADT_Vector_t*)handle;
	if(vector->data != NULL) {
		MEMORY_FREE(vector->data);
	}
	MEMORY_FREE(vector);
}

AL2O3_EXTERN_C size_t CADT_VectorElementSizeFrom(CADT_VectorHandle handle) {
	ASSERT(handle != NULL);
	CADT_Vector_t const* vector = (CADT_Vector_t const*)handle;
	return vector->elementSize;
}

AL2O3_EXTERN_C size_t CADT_VectorSizeFrom(CADT_VectorHandle handle) {
	ASSERT(handle != NULL);
	CADT_Vector_t const* vector = (CADT_Vector_t const*)handle;
	return vector->size;
}

AL2O3_EXTERN_C void CADT_VectorResize(CADT_VectorHandle handle, size_t size) {
	ASSERT(handle != NULL);
	CADT_Vector_t * vector = (CADT_Vector_t *)handle;

	CADT_VectorReserve(handle, size);
	vector->size = size;
}
AL2O3_EXTERN_C bool CADT_VectorIsEmpty(CADT_VectorHandle handle) {
	ASSERT(handle != NULL);
	CADT_Vector_t const* vector = (CADT_Vector_t const*)handle;
	return (vector->size == 0);
}
AL2O3_EXTERN_C size_t CADT_VectorCapacityFrom(CADT_VectorHandle handle) {
	ASSERT(handle != NULL);
	CADT_Vector_t const* vector = (CADT_Vector_t const*)handle;
	return vector->capacity;
}

AL2O3_EXTERN_C void CADT_VectorReserve(CADT_VectorHandle handle, size_t size) {
	ASSERT(handle != NULL);
	CADT_Vector_t * vector = (CADT_Vector_t *)handle;

	// reserve always grews unless ShrinkToFit
	if(size <= vector->capacity) return;

	void* oldData = vector->data;
	size_t const oldCapacity = (vector->capacity) ? vector->capacity : 1;
	size_t const newCapacity = Math_MaxSizeT(oldCapacity*2, size);
	vector->capacity = newCapacity;
	vector->data = MEMORY_CALLOC(newCapacity, vector->elementSize);
	ASSERT(vector->data);
	if(oldData) {
		memcpy(vector->data, oldData, vector->size * vector->elementSize);
		MEMORY_FREE(oldData);
	}
}

AL2O3_EXTERN_C void* CADT_VectorElementFrom(CADT_VectorHandle handle, size_t index) {
	ASSERT(handle != NULL);
	CADT_Vector_t const* vector = (CADT_Vector_t const*)handle;
	ASSERT(index < vector->size);
	ASSERT(vector->data);
	return ((uint8_t*)vector->data) + (index * vector->elementSize);
}

AL2O3_EXTERN_C size_t CADT_VectorPushElement(CADT_VectorHandle handle, void* element) {
	ASSERT(handle != NULL);
	CADT_Vector_t * vector = (CADT_Vector_t *)handle;
	size_t const index = vector->size;
	CADT_VectorResize(handle, vector->size+1);
	ASSERT(index < vector->size);

	memcpy(CADT_VectorElementFrom(handle, index), element, vector->elementSize);
	return index;
}

AL2O3_EXTERN_C void CADT_VectorPopElementFrom(CADT_VectorHandle handle, void* out) {
	ASSERT(handle != NULL);
	CADT_Vector_t * vector = (CADT_Vector_t *)handle;
	ASSERT(vector->size > 0);
	size_t const index = vector->size-1;
	memcpy(out, CADT_VectorElementFrom(handle, index), vector->elementSize);
	CADT_VectorResize(handle, vector->size-1);
}

AL2O3_EXTERN_C void CADT_VectorPopAndDiscardElementFrom(CADT_VectorHandle handle) {
	ASSERT(handle != NULL);
	CADT_Vector_t * vector = (CADT_Vector_t *)handle;
	ASSERT(vector->size > 0);
	CADT_VectorResize(handle, vector->size-1);
}

AL2O3_EXTERN_C void* CADT_VectorPeekElementFrom(CADT_VectorHandle handle) {
	ASSERT(handle != NULL);
	CADT_Vector_t const* vector = (CADT_Vector_t const*)handle;
	ASSERT(vector->size > 0);
	size_t const index = vector->size-1;
	return CADT_VectorElementFrom(handle, index);
}

AL2O3_EXTERN_C void* CADT_VectorDataFrom(CADT_VectorHandle handle) {
	ASSERT(handle != NULL);
	CADT_Vector_t const* vector = (CADT_Vector_t const*)handle;
	return vector->data;
}
