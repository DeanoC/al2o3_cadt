#include "al2o3_platform/platform.h"
#include "al2o3_cmath/scalar.h"
#include "al2o3_stb/stb_dict.h"
#include "al2o3_cadt/dict.h"

#define CDICT_IMPL(postfix, type) \
AL2O3_EXTERN_C CADT_Dict##postfix##Handle CADT_Dict##postfix##Create() { \
	return (CADT_Dict##postfix##Handle) stb_dict##postfix##_create(); \
} \
AL2O3_EXTERN_C void CADT_Dict##postfix##Destroy(CADT_Dict##postfix##Handle handle) { \
	ASSERT(handle); \
	stb_dict##postfix * dict = (stb_dict##postfix *) handle; \
	stb_dict##postfix##_destroy(dict); \
} \
AL2O3_EXTERN_C CADT_Dict##postfix##Handle CADT_Dict##postfix##Clone(CADT_Dict##postfix##Handle handle) {\
	ASSERT(handle); \
	stb_dict##postfix * dict = (stb_dict##postfix *) handle; \
	return (CADT_Dict##postfix##Handle)stb_dict##postfix##_copy(dict); \
} \
AL2O3_EXTERN_C size_t CADT_Dict##postfix##Size(CADT_Dict##postfix##Handle handle) { \
	ASSERT(handle); \
	stb_dict##postfix const * dict = (stb_dict##postfix const *) handle; \
	return dict->count; \
} \
AL2O3_EXTERN_C bool CADT_Dict##postfix##IsEmpty(CADT_Dict##postfix##Handle handle) { \
	return CADT_Dict##postfix##Size(handle) == 0; \
} \
AL2O3_EXTERN_C size_t CADT_Dict##postfix##Capacity(CADT_Dict##postfix##Handle handle) {\
	ASSERT(handle); \
	stb_dict##postfix const * dict = (stb_dict##postfix const *) handle; \
	return dict->limit; \
} \
AL2O3_EXTERN_C type CADT_Dict##postfix##GetByIndex(CADT_Dict##postfix##Handle handle, size_t index) {\
	ASSERT(handle); \
	stb_dict##postfix const * dict = (stb_dict##postfix const *) handle; \
	size_t count = 0; \
	for (size_t i = 0; i < dict->limit; ++i) { \
		if (dict->table[i].k != -1l) { \
			if (count == index) \
				return dict->table[i].v; \
			else \
				count++; \
		}	\
	} \
	return 0; \
} \
AL2O3_EXTERN_C void CADT_Dict##postfix##Reserve(CADT_Dict##postfix##Handle handle, size_t const size) { \
	ASSERT(handle); \
	stb_dict##postfix * dict = (stb_dict##postfix *) handle; \
	stb_dict##postfix##_reserve(dict, (int)size); \
} \
AL2O3_EXTERN_C bool CADT_Dict##postfix##KeyExists(CADT_Dict##postfix##Handle handle, type const key) { \
	ASSERT(handle); \
	stb_dict##postfix const * dict = (stb_dict##postfix const *) handle; \
	type dummy; \
	return stb_dict##postfix##_get_flag(dict, key, &dummy); \
} \
AL2O3_EXTERN_C bool CADT_Dict##postfix##Lookup(CADT_Dict##postfix##Handle handle, type const key, type* out) { \
	ASSERT(handle); \
	stb_dict##postfix const * dict = (stb_dict##postfix const *) handle; \
	return stb_dict##postfix##_get_flag(dict, key, out); \
} \
AL2O3_EXTERN_C type CADT_Dict##postfix##Get(CADT_Dict##postfix##Handle handle, type const key) { \
	ASSERT(handle); \
	stb_dict##postfix const * dict = (stb_dict##postfix const *) handle; \
	type out = 0; \
	stb_dict##postfix##_get_flag(dict, key, &out); \
	return out; \
} \
AL2O3_EXTERN_C bool CADT_Dict##postfix##Add(CADT_Dict##postfix##Handle handle, type const key, type const in) { \
	ASSERT(handle); \
	stb_dict##postfix * dict = (stb_dict##postfix *) handle; \
	return stb_dict##postfix##_add(dict, key, in);  \
}\
AL2O3_EXTERN_C void CADT_Dict##postfix##Remove(CADT_Dict##postfix##Handle handle, type const key) { \
	ASSERT(handle); \
	stb_dict##postfix * dict = (stb_dict##postfix *) handle; \
	type dummy; \
	stb_dict##postfix##_remove(dict, key, &dummy); \
}

CDICT_IMPL(U32, uint32_t)
CDICT_IMPL(U64, uint64_t)
