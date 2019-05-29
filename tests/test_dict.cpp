#include "al2o3_catch2/catch2.hpp"
#include "al2o3_cadt/dict.h"

#define CDICT_TEST(postfix, type) \
TEST_CASE("CDict" #postfix " create/destroy", "[CADT CDict]") { \
	CADT_Dict##postfix##Handle handle = CADT_Dict##postfix##Create(); \
	REQUIRE(handle); \
	CADT_Dict##postfix##Destroy(handle); \
} \
TEST_CASE("CDict" #postfix " add/lookup", "[CADT CDict]") { \
	CADT_Dict##postfix##Handle handle = CADT_Dict##postfix##Create(); \
	REQUIRE(handle); \
	REQUIRE(CADT_Dict##postfix##Add(handle, 0, 50)); \
	REQUIRE(CADT_Dict##postfix##KeyExists(handle, 0)); \
	REQUIRE(CADT_Dict##postfix##Add(handle, 10, 60)); \
	REQUIRE(CADT_Dict##postfix##KeyExists(handle, 10)); \
	REQUIRE(CADT_Dict##postfix##Add(handle, 5, 70)); \
	REQUIRE(CADT_Dict##postfix##KeyExists(handle, 5)); \
	type r50 = 0, r60 = 0, r70 = 0; \
	REQUIRE(CADT_Dict##postfix##Lookup(handle, 0, &r50)); \
	REQUIRE(r50 == 50); \
	REQUIRE(CADT_Dict##postfix##Lookup(handle, 10, &r60)); \
	REQUIRE(r60 == 60); \
	REQUIRE(CADT_Dict##postfix##Lookup(handle, 5, &r70)); \
	REQUIRE(r70 == 70); \
	r50 = 0, r60 = 0, r70 = 0; \
	REQUIRE(CADT_Dict##postfix##Lookup(handle, 5, &r70)); \
	REQUIRE(r70 == 70); \
	REQUIRE(CADT_Dict##postfix##Lookup(handle, 0, &r50)); \
	REQUIRE(r50 == 50); \
	REQUIRE(CADT_Dict##postfix##Lookup(handle, 10, &r60)); \
	REQUIRE(r60 == 60); \
	CADT_Dict##postfix##Destroy(handle); \
} \
TEST_CASE("CDict" #postfix " remove", "[CADT CDict]") { \
	CADT_Dict##postfix##Handle handle = CADT_Dict##postfix##Create(); \
	REQUIRE(handle); \
	REQUIRE(CADT_Dict##postfix##Add(handle, 0, 50)); \
	REQUIRE(CADT_Dict##postfix##Add(handle, 10, 60)); \
	REQUIRE(CADT_Dict##postfix##Add(handle, 5, 70)); \
	CADT_Dict##postfix##Remove(handle, 10); \
	REQUIRE(!CADT_Dict##postfix##KeyExists(handle, 10)); \
	REQUIRE(CADT_Dict##postfix##KeyExists(handle, 0)); \
	REQUIRE(CADT_Dict##postfix##KeyExists(handle, 5)); \
	type r50 = 0, r70 = 0; \
	REQUIRE(CADT_Dict##postfix##Lookup(handle, 0, &r50)); \
	REQUIRE(r50 == 50); \
	REQUIRE(CADT_Dict##postfix##Lookup(handle, 5, &r70)); \
	REQUIRE(r70 == 70); \
	CADT_Dict##postfix##Remove(handle, 0); \
	REQUIRE(!CADT_Dict##postfix##KeyExists(handle, 0)); \
	CADT_Dict##postfix##Remove(handle, 5); \
	REQUIRE(!CADT_Dict##postfix##KeyExists(handle, 5)); \
	REQUIRE(CADT_Dict##postfix##IsEmpty(handle)); \
} \
TEST_CASE("CDict" #postfix " reserve", "[CADT CDict]") { \
	CADT_Dict##postfix##Handle handle = CADT_Dict##postfix##Create(); \
	REQUIRE(handle); \
	REQUIRE(CADT_Dict##postfix##Add(handle, 0, 50)); \
	REQUIRE(CADT_Dict##postfix##KeyExists(handle, 0)); \
	REQUIRE(CADT_Dict##postfix##Add(handle, 10, 60)); \
	REQUIRE(CADT_Dict##postfix##KeyExists(handle, 10)); \
	REQUIRE(CADT_Dict##postfix##Add(handle, 5, 70)); \
	REQUIRE(CADT_Dict##postfix##KeyExists(handle, 5)); \
	CADT_Dict##postfix##Reserve(handle, 100);\
	REQUIRE(CADT_Dict##postfix##Capacity(handle) >= 100); \
	type r50 = 0, r60 = 0, r70 = 0; \
	REQUIRE(CADT_Dict##postfix##Lookup(handle, 0, &r50)); \
	REQUIRE(r50 == 50); \
	REQUIRE(CADT_Dict##postfix##Lookup(handle, 10, &r60)); \
	REQUIRE(r60 == 60); \
	REQUIRE(CADT_Dict##postfix##Lookup(handle, 5, &r70)); \
	REQUIRE(r70 == 70); \
	r50 = 0, r60 = 0, r70 = 0; \
	REQUIRE(CADT_Dict##postfix##Lookup(handle, 5, &r70)); \
	REQUIRE(r70 == 70); \
	REQUIRE(CADT_Dict##postfix##Lookup(handle, 0, &r50)); \
	REQUIRE(r50 == 50); \
	REQUIRE(CADT_Dict##postfix##Lookup(handle, 10, &r60)); \
	REQUIRE(r60 == 60); \
	CADT_Dict##postfix##Destroy(handle); \
} 


CDICT_TEST(U32, uint32_t)
CDICT_TEST(U64, uint64_t)


TEST_CASE("CDictU64 64 bit add/lookup", "[CADT CDict]") {
	CADT_DictU64Handle handle = CADT_DictU64Create();
	REQUIRE(handle);
	uint64_t r50 = 0, r60 = 0, r70 = 0;
	REQUIRE(CADT_DictU64Add(handle, 0x8888888822222220U, 0x8888888822222250U));
	REQUIRE(CADT_DictU64KeyExists(handle, 0x8888888822222220U));
	REQUIRE(CADT_DictU64Lookup(handle, 0x8888888822222220U, &r50));
	REQUIRE(r50 == 0x8888888822222250U);

	REQUIRE(CADT_DictU64Add(handle, 0x8888888822222210U, 0x8888888822222260U));
	REQUIRE(CADT_DictU64KeyExists(handle, 0x8888888822222210U));
	REQUIRE(CADT_DictU64Lookup(handle, 0x8888888822222210U, &r60));
	REQUIRE(r60 == 0x8888888822222260U);

	REQUIRE(CADT_DictU64Add(handle, 0x8888888822222225U, 0x8888888822222270U));
	REQUIRE(CADT_DictU64KeyExists(handle, 0x8888888822222225U));
	REQUIRE(CADT_DictU64Lookup(handle, 0x8888888822222225U, &r70));
	REQUIRE(r70 == 0x8888888822222270U);

	r50 = 0, r60 = 0, r70 = 0;
	REQUIRE(CADT_DictU64Lookup(handle, 0x8888888822222225U, &r70));
	REQUIRE(r70 == 0x8888888822222270U);
	REQUIRE(CADT_DictU64Lookup(handle, 0x8888888822222220U, &r50));
	REQUIRE(r50 == 0x8888888822222250U);
	REQUIRE(CADT_DictU64Lookup(handle, 0x8888888822222210U, &r60));
	REQUIRE(r60 == 0x8888888822222260U);


	r50 = 0, r60 = 0, r70 = 0;
	REQUIRE(CADT_DictU64Lookup(handle, 0x8888888822222225U, &r70));
	REQUIRE(r70 == 0x8888888822222270U);
	CADT_DictU64Remove(handle, 0x8888888822222225U);
	REQUIRE(CADT_DictU64Lookup(handle, 0x8888888822222220U, &r50));
	REQUIRE(r50 == 0x8888888822222250U);
	CADT_DictU64Remove(handle, 0x8888888822222220U);
	REQUIRE(CADT_DictU64Lookup(handle, 0x8888888822222210U, &r60));
	REQUIRE(r60 == 0x8888888822222260U);

	CADT_DictU64Destroy(handle);
}