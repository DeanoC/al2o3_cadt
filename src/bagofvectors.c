#include "al2o3_platform/platform.h"
#include "al2o3_memory/memory.h"
#include "al2o3_cadt/vector.h"
#include "al2o3_cadt/dict.h"
#include "al2o3_cadt/bagofvectors.h"

typedef struct CADT_BagOfVectors {
	CADT_DictU64Handle tagDict;
	CADT_VectorHandle keys;
} CADT_BagOfVectors;


AL2O3_EXTERN_C CADT_BagOfVectorsHandle CADT_BagOfVectorsCreate() {
	ASSERT(sizeof(CADT_VectorHandle) <= sizeof(uint64_t));

	CADT_BagOfVectors* tvec = (CADT_BagOfVectors*)MEMORY_CALLOC(1, sizeof(CADT_BagOfVectors));
	if (tvec == NULL) return NULL;
	tvec->tagDict = CADT_DictU64Create();
	if (tvec->tagDict == NULL) {
		MEMORY_FREE(tvec);
		return NULL;
	}
	tvec->keys = CADT_VectorCreate(sizeof(uint64_t));
	if (tvec->keys == NULL) {
		CADT_DictU64Destroy(tvec->tagDict);
		MEMORY_FREE(tvec);
		return NULL;
	}

	return tvec;
}

AL2O3_EXTERN_C void CADT_BagOfVectorsDestroy(CADT_BagOfVectorsHandle handle) {
	ASSERT(handle);
	CADT_BagOfVectors* tvec = (CADT_BagOfVectors*)handle;

	for (size_t i = 0; i < CADT_VectorSize(tvec->keys); i++) {
		uint64_t key = *(uint64_t*)CADT_VectorAt(tvec->keys, i);
		CADT_VectorHandle vh = (CADT_VectorHandle)CADT_DictU64Get(tvec->tagDict, key);
		CADT_VectorDestroy(vh);
	}

	CADT_VectorDestroy(tvec->keys);
	CADT_DictU64Destroy(tvec->tagDict);
	MEMORY_FREE(tvec);
}
AL2O3_EXTERN_C CADT_BagOfVectorsHandle CADT_BagOfVectorsClone(CADT_BagOfVectorsHandle handle) {
	ASSERT(handle);
	CADT_BagOfVectors* ovec = (CADT_BagOfVectors*)handle;
	CADT_BagOfVectors* nvec = (CADT_BagOfVectors*)MEMORY_CALLOC(1, sizeof(CADT_BagOfVectors));
	if (nvec == NULL) return NULL;

	nvec->keys = CADT_VectorCreate(sizeof(uint64_t));
	nvec->tagDict = CADT_DictU64Create();

	for (size_t i = 0; i < CADT_VectorSize(ovec->keys); i++) {
		uint64_t key = *(uint64_t*)CADT_VectorAt(ovec->keys, i);
		CADT_VectorHandle ovh = (CADT_VectorHandle)CADT_DictU64Get(ovec->tagDict, key);

		CADT_VectorHandle nvh = CADT_VectorClone(ovh);
		CADT_BagOfVectorsOwnVector(nvec, key, nvh);
	}
	return nvec;
}
AL2O3_EXTERN_C CADT_VectorHandle CADT_BagOfVectorsAdd(CADT_BagOfVectorsHandle handle, uint64_t key, size_t elementSize) {
	if (CADT_BagOfVectorsKeyExists(handle, key) == true) {
		return NULL;
	}

	CADT_BagOfVectors* tvec = (CADT_BagOfVectors*)handle;
	ASSERT(tvec->tagDict);
	ASSERT(tvec->keys)

	CADT_VectorHandle vh = CADT_VectorCreate(elementSize);
	bool okay = CADT_DictU64Add(tvec->tagDict, key, (uint64_t)vh);
	if (okay) {
		CADT_VectorPushElement(tvec->keys, &key);
		return vh;
	}	else {
		CADT_VectorDestroy(vh);
		return NULL;
	}

}

AL2O3_EXTERN_C void CADT_BagOfVectorsOwnVector(CADT_BagOfVectorsHandle handle, uint64_t key, CADT_VectorHandle vector) {
	ASSERT(CADT_BagOfVectorsKeyExists(handle, key) == false);

	CADT_BagOfVectors* tvec = (CADT_BagOfVectors*)handle;
	bool okay = CADT_DictU64Add(tvec->tagDict, key, (uint64_t)vector);
	if (okay) {
		CADT_VectorPushElement(tvec->keys, &key);
	}
}

AL2O3_EXTERN_C void CADT_BagOfVectorsRemove(CADT_BagOfVectorsHandle handle, uint64_t key) {
	ASSERT(CADT_BagOfVectorsKeyExists(handle, key) == true);
	CADT_BagOfVectors* tvec = (CADT_BagOfVectors*)handle;
	CADT_VectorHandle vh = CADT_BagOfVectorsLookup(handle, key);
	for (size_t i = 0; i < CADT_VectorSize(tvec->keys); ++i) {
		uint64_t k = *(uint64_t*)CADT_VectorAt(tvec->keys, i);
		if (k == key) {
			CADT_VectorRemove(tvec->keys, i);
			break;
		}
	}

	CADT_DictU64Remove(tvec->tagDict, key);
	CADT_VectorDestroy(vh);
}

AL2O3_EXTERN_C bool CADT_BagOfVectorsKeyExists(CADT_BagOfVectorsHandle handle, uint64_t key) {
	ASSERT(handle);
	CADT_BagOfVectors* tvec = (CADT_BagOfVectors*) handle;
	return CADT_DictU64KeyExists(tvec->tagDict, key);
}

AL2O3_EXTERN_C size_t CADT_BagOfVectorsSize(CADT_BagOfVectorsHandle handle) {
	ASSERT(handle);
	CADT_BagOfVectors* tvec = (CADT_BagOfVectors*) handle;
	return CADT_VectorSize(tvec->keys);
}

AL2O3_EXTERN_C CADT_VectorHandle CADT_BagOfVectorsAt(CADT_BagOfVectorsHandle handle, size_t index) {
	ASSERT(handle);
	CADT_BagOfVectors* tvec = (CADT_BagOfVectors*)handle;
	uint64_t key = *(uint64_t*) CADT_VectorAt(tvec->keys, index);
	return (CADT_VectorHandle)CADT_DictU64Get(tvec->tagDict, key);
}


AL2O3_EXTERN_C uint64_t CADT_BagOfVectorsGetKey(CADT_BagOfVectorsHandle handle, size_t index) {
	ASSERT(handle);
	CADT_BagOfVectors* tvec = (CADT_BagOfVectors*) handle;
	return *(uint64_t*)CADT_VectorAt(tvec->keys, index);
}

AL2O3_EXTERN_C CADT_VectorHandle CADT_BagOfVectorsLookup(CADT_BagOfVectorsHandle handle, uint64_t key) {
	ASSERT(CADT_BagOfVectorsKeyExists(handle, key) == true);
	CADT_BagOfVectors* tvec = (CADT_BagOfVectors*) handle;
	return (CADT_VectorHandle)CADT_DictU64Get(tvec->tagDict, key);
}
