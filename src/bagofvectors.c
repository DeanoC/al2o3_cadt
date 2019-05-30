#include "al2o3_platform/platform.h"
#include "al2o3_memory/memory.h"
#include "al2o3_cadt/vector.h"
#include "al2o3_cadt/dict.h"
#include "al2o3_cadt/bagofvectors.h"

typedef struct CADT_BagOfVectors {
	CADT_DictU64Handle tagDictHandle;
	CADT_VectorHandle vectorOfVectors;
} CADT_BagOfVectors;


AL2O3_EXTERN_C CADT_BagOfVectorsHandle CADT_BagOfVectorsCreate() {
	ASSERT(sizeof(CADT_VectorHandle) <= sizeof(uint64_t));

	CADT_BagOfVectors* tvec = (CADT_BagOfVectors*)MEMORY_CALLOC(1, sizeof(CADT_BagOfVectors));
	if (tvec == NULL) return NULL;
	tvec->tagDictHandle = CADT_DictU64Create();
	if (tvec->tagDictHandle == NULL) {
		MEMORY_FREE(tvec);
		return NULL;
	}
	tvec->vectorOfVectors = CADT_VectorCreate(sizeof(CADT_VectorHandle));
	if (tvec->vectorOfVectors == NULL) {
		CADT_DictU64Destroy(tvec->tagDictHandle);
		MEMORY_FREE(tvec);
		return NULL;
	}

	return tvec;
}

AL2O3_EXTERN_C void CADT_BagOfVectorsDestroy(CADT_BagOfVectorsHandle handle) {
	ASSERT(handle);
	CADT_BagOfVectors* tvec = (CADT_BagOfVectors*)handle;

	for (size_t i = 0; i < CADT_VectorSize(tvec->vectorOfVectors); i++) {
		CADT_VectorHandle vh = *(CADT_VectorHandle*)CADT_VectorElement(tvec->vectorOfVectors, i);
		CADT_VectorDestroy(vh);
	}

	CADT_VectorDestroy(tvec->vectorOfVectors);
	CADT_DictU64Destroy(tvec->tagDictHandle);
	MEMORY_FREE(tvec);
}

AL2O3_EXTERN_C CADT_VectorHandle CADT_BagOfVectorsAdd(CADT_BagOfVectorsHandle handle, uint64_t key, size_t elementSize) {
	ASSERT(CADT_BagOfVectorsKeyExists(handle, key) == false);

	CADT_BagOfVectors* tvec = (CADT_BagOfVectors*)handle;
	ASSERT(tvec->tagDictHandle);
	CADT_VectorHandle vh = CADT_VectorCreate(elementSize);
	bool okay = CADT_DictU64Add(tvec->tagDictHandle, key, (uint64_t)vh);
	if (okay) {
		CADT_VectorPushElement(tvec->vectorOfVectors, &vh);
		return vh;
	}
	else {
		CADT_VectorDestroy(vh);
		return NULL;
	}

}

AL2O3_EXTERN_C void CADT_BagOfVectorsOwnVector(CADT_BagOfVectorsHandle handle, uint64_t key, CADT_VectorHandle vector) {
	ASSERT(CADT_BagOfVectorsKeyExists(handle, key) == false);

	CADT_BagOfVectors* tvec = (CADT_BagOfVectors*)handle;
	bool okay = CADT_DictU64Add(tvec->tagDictHandle, key, (uint64_t)vector);
	if (okay) {
		CADT_VectorPushElement(tvec->vectorOfVectors, &vector);
	}
}

AL2O3_EXTERN_C void CADT_BagOfVectorsRemove(CADT_BagOfVectorsHandle handle, uint64_t key) {
	ASSERT(CADT_BagOfVectorsKeyExists(handle, key) == true);
	CADT_BagOfVectors* tvec = (CADT_BagOfVectors*)handle;
	CADT_VectorHandle vh = CADT_BagOfVectorsGet(handle, key);
	for (size_t i = 0; i < CADT_VectorSize(tvec->vectorOfVectors); ++i) {
		CADT_VectorHandle vvh = *((CADT_VectorHandle*)CADT_VectorElement(tvec->vectorOfVectors, i));
		if (vh == vvh) {
			CADT_VectorRemove(tvec->vectorOfVectors, i);
			break;
		}
	}

	CADT_DictU64Remove(tvec->tagDictHandle, key);
	CADT_VectorDestroy(vh);
}

AL2O3_EXTERN_C bool CADT_BagOfVectorsKeyExists(CADT_BagOfVectorsHandle handle, uint64_t key) {
	ASSERT(handle);
	CADT_BagOfVectors* tvec = (CADT_BagOfVectors*) handle;
	return CADT_DictU64KeyExists(tvec->tagDictHandle, key);
}

AL2O3_EXTERN_C size_t CADT_BagOfVectorsKeyCount(CADT_BagOfVectorsHandle handle) {
	ASSERT(handle);
	CADT_BagOfVectors* tvec = (CADT_BagOfVectors*) handle;
	return CADT_DictU64Size(tvec->tagDictHandle);
}

AL2O3_EXTERN_C uint64_t CADT_BagOfVectorsGetKey(CADT_BagOfVectorsHandle handle, size_t index) {
	ASSERT(handle);
	CADT_BagOfVectors* tvec = (CADT_BagOfVectors*) handle;
	return CADT_DictU64GetByIndex(tvec->tagDictHandle, index);
}

AL2O3_EXTERN_C CADT_VectorHandle CADT_BagOfVectorsGet(CADT_BagOfVectorsHandle handle, uint64_t key) {
	ASSERT(CADT_BagOfVectorsKeyExists(handle, key) == true);
	CADT_BagOfVectors* tvec = (CADT_BagOfVectors*) handle;
	return (CADT_VectorHandle)CADT_DictU64Get(tvec->tagDictHandle, key);
}
