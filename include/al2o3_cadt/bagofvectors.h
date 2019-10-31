#pragma once

#include "al2o3_platform/platform.h"
#include "al2o3_cadt/vector.h"

typedef struct CADT_BagOfVectors *CADT_BagOfVectorsHandle;

AL2O3_EXTERN_C CADT_BagOfVectorsHandle CADT_BagOfVectorsCreate();
AL2O3_EXTERN_C void CADT_BagOfVectorsDestroy(CADT_BagOfVectorsHandle handle);
AL2O3_EXTERN_C CADT_BagOfVectorsHandle CADT_BagOfVectorsClone(CADT_BagOfVectorsHandle handle);

AL2O3_EXTERN_C CADT_VectorHandle CADT_BagOfVectorsAdd(CADT_BagOfVectorsHandle handle, uint64_t key, size_t elementSize);
AL2O3_EXTERN_C bool CADT_BagOfVectorsKeyExists(CADT_BagOfVectorsHandle handle, uint64_t key);
AL2O3_EXTERN_C void CADT_BagOfVectorsOwnVector(CADT_BagOfVectorsHandle handle, uint64_t key, CADT_VectorHandle vector);
AL2O3_EXTERN_C void CADT_BagOfVectorsRemove(CADT_BagOfVectorsHandle handle, uint64_t key);
AL2O3_EXTERN_C CADT_VectorHandle CADT_BagOfVectorsLookup(CADT_BagOfVectorsHandle handle, uint64_t key);

AL2O3_EXTERN_C size_t CADT_BagOfVectorsSize(CADT_BagOfVectorsHandle handle);
AL2O3_EXTERN_C CADT_VectorHandle CADT_BagOfVectorsAt(CADT_BagOfVectorsHandle handle, size_t index);
AL2O3_EXTERN_C uint64_t CADT_BagOfVectorsGetKey(CADT_BagOfVectorsHandle handle, size_t index);
