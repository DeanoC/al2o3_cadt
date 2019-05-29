#pragma once
#ifndef GFX_CADT_DICT_H
#define GFX_CADT_DICT_H

#include "al2o3_platform/platform.h"

#define CADT_DECLARE_DICT(postfix, keytype, valuetype) \
typedef struct CADT_Dict##postfix *CADT_Dict##postfix##Handle; \
AL2O3_EXTERN_C CADT_Dict##postfix##Handle CADT_Dict##postfix##Create(); \
AL2O3_EXTERN_C void CADT_Dict##postfix##Destroy(CADT_Dict##postfix##Handle handle); \
AL2O3_EXTERN_C size_t CADT_Dict##postfix##Size(CADT_Dict##postfix##Handle handle); \
AL2O3_EXTERN_C valuetype CADT_Dict##postfix##GetByIndex(CADT_Dict##postfix##Handle handle, size_t index); \
AL2O3_EXTERN_C bool CADT_Dict##postfix##IsEmpty(CADT_Dict##postfix##Handle handle); \
AL2O3_EXTERN_C size_t CADT_Dict##postfix##Capacity(CADT_Dict##postfix##Handle handle); \
AL2O3_EXTERN_C void CADT_Dict##postfix##Reserve(CADT_Dict##postfix##Handle handle, size_t const size); \
AL2O3_EXTERN_C bool CADT_Dict##postfix##KeyExists(CADT_Dict##postfix##Handle handle, keytype const key); \
AL2O3_EXTERN_C valuetype CADT_Dict##postfix##Get(CADT_Dict##postfix##Handle handle, keytype const key); \
AL2O3_EXTERN_C bool CADT_Dict##postfix##Lookup(CADT_Dict##postfix##Handle handle, keytype const key, valuetype* out); \
AL2O3_EXTERN_C bool CADT_Dict##postfix##Add(CADT_Dict##postfix##Handle handle, keytype const key, valuetype const in); \
AL2O3_EXTERN_C void CADT_Dict##postfix##Remove(CADT_Dict##postfix##Handle handle, keytype const key);

CADT_DECLARE_DICT(U32, uint32_t, uint32_t)
CADT_DECLARE_DICT(U64, uint64_t, uint64_t)

#undef CADT_DECLARE_DICT

#endif // GFX_CADT_MESH_H