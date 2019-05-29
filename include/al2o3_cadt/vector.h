#pragma once
#ifndef AL2O3_CADT_VECTOR_H
#define AL2O3_CADT_VECTOR_H

#include "al2o3_platform/platform.h"

typedef void* CADT_VectorHandle;

AL2O3_EXTERN_C CADT_VectorHandle CADT_VectorCreate(size_t elementSize);
AL2O3_EXTERN_C void CADT_VectorDestroy(CADT_VectorHandle handle);

AL2O3_EXTERN_C size_t CADT_VectorElementSizeFrom(CADT_VectorHandle handle);

AL2O3_EXTERN_C size_t CADT_VectorSizeFrom(CADT_VectorHandle handle);
AL2O3_EXTERN_C void CADT_VectorResize(CADT_VectorHandle handle, size_t size);
AL2O3_EXTERN_C bool CADT_VectorIsEmpty(CADT_VectorHandle handle);

AL2O3_EXTERN_C size_t CADT_VectorCapacityFrom(CADT_VectorHandle handle);
AL2O3_EXTERN_C void CADT_VectorReserve(CADT_VectorHandle handle, size_t size);

AL2O3_EXTERN_C void* CADT_VectorElementFrom(CADT_VectorHandle handle, size_t index);

AL2O3_EXTERN_C size_t CADT_VectorPushElement(CADT_VectorHandle handle, void* element);
AL2O3_EXTERN_C size_t CADT_VectorPushElementClone(CADT_VectorHandle handle, size_t srcIndex);
AL2O3_EXTERN_C void CADT_VectorReplace(CADT_VectorHandle handle, size_t srcIndex,	size_t dstIndex);

AL2O3_EXTERN_C void CADT_VectorPopElementFrom(CADT_VectorHandle handle, void* out);
AL2O3_EXTERN_C void CADT_VectorPopAndDiscardElementFrom(CADT_VectorHandle handle);
AL2O3_EXTERN_C void* CADT_VectorPeekElementFrom(CADT_VectorHandle handle);

AL2O3_EXTERN_C void* CADT_VectorDataFrom(CADT_VectorHandle handle);

#endif // AL2O3_CADT_MESH_H