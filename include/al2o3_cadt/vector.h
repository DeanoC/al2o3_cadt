#pragma once
#ifndef AL2O3_CADT_VECTOR_H
#define AL2O3_CADT_VECTOR_H

#include "al2o3_platform/platform.h"

typedef struct CADT_Vector *CADT_VectorHandle;

AL2O3_EXTERN_C CADT_VectorHandle CADT_VectorCreate(size_t elementSize);
AL2O3_EXTERN_C void CADT_VectorDestroy(CADT_VectorHandle handle);
AL2O3_EXTERN_C CADT_VectorHandle CADT_VectorClone(CADT_VectorHandle handle);

AL2O3_EXTERN_C size_t CADT_VectorElementSize(CADT_VectorHandle handle);

AL2O3_EXTERN_C size_t CADT_VectorSize(CADT_VectorHandle handle);
AL2O3_EXTERN_C void CADT_VectorResize(CADT_VectorHandle handle, size_t size);
AL2O3_EXTERN_C void CADT_VectorResizeWithZero(CADT_VectorHandle handle, size_t size);
AL2O3_EXTERN_C void CADT_VectorResizeWithDefault(CADT_VectorHandle handle, size_t size, void const* defaultData);

AL2O3_EXTERN_C bool CADT_VectorIsEmpty(CADT_VectorHandle handle);

AL2O3_EXTERN_C size_t CADT_VectorCapacity(CADT_VectorHandle handle);
AL2O3_EXTERN_C void CADT_VectorReserve(CADT_VectorHandle handle, size_t size);

AL2O3_EXTERN_C void* CADT_VectorAt(CADT_VectorHandle handle, size_t index);

AL2O3_EXTERN_C size_t CADT_VectorPushElement(CADT_VectorHandle handle, void* element);
AL2O3_EXTERN_C size_t CADT_VectorPushElementCloneToEnd(CADT_VectorHandle handle, size_t srcIndex);
AL2O3_EXTERN_C void CADT_VectorReplace(CADT_VectorHandle handle, size_t srcIndex,	size_t dstIndex);
AL2O3_EXTERN_C void CADT_VectorSwap(CADT_VectorHandle handle, size_t index0, size_t index1);

AL2O3_EXTERN_C void CADT_VectorPopElementFrom(CADT_VectorHandle handle, void* out);
AL2O3_EXTERN_C void CADT_VectorPopAndDiscardElement(CADT_VectorHandle handle);
AL2O3_EXTERN_C void* CADT_VectorPeekElement(CADT_VectorHandle handle);

AL2O3_EXTERN_C void* CADT_VectorData(CADT_VectorHandle handle);
AL2O3_EXTERN_C void CADT_VectorRemove(CADT_VectorHandle handle, size_t index);
// swaps the last element into index and resizes, faster than remove
AL2O3_EXTERN_C void CADT_VectorSwapRemove(CADT_VectorHandle handle, size_t index);

#endif // AL2O3_CADT_MESH_H