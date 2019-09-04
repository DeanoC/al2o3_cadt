#pragma once
#ifndef AL2O3_CADT_FREELIST_H
#define AL2O3_CADT_FREELIST_H

#include "al2o3_platform/platform.h"

typedef struct CADT_FreeList *CADT_FreeListHandle;

AL2O3_EXTERN_C CADT_FreeListHandle CADT_FreeListCreate(size_t elementSize, size_t capacity);
AL2O3_EXTERN_C void CADT_FreeListDestroy(CADT_FreeListHandle handle);
AL2O3_EXTERN_C CADT_FreeListHandle CADT_FreeListClone(CADT_FreeListHandle handle);

AL2O3_EXTERN_C void* CADT_FreeListAlloc(CADT_FreeListHandle handle);
AL2O3_EXTERN_C void CADT_FreeListFree(CADT_FreeListHandle handle, void* ptr);

#endif // end