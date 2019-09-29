#include "al2o3_catch2/catch2.hpp"
#include "al2o3_cadt/freelist.h"

TEST_CASE("CADT_FreeList create/destroy", "[CADT CADT_FreeList]") {
	CADT_FreeListHandle handle = CADT_FreeListCreate(sizeof(uintptr_t)+2, 10);
	REQUIRE(handle);
	CADT_FreeListDestroy(handle);
}

TEST_CASE("CADT_FreeList Single Alloc/Free", "[CADT CADT_FreeList]") {
	CADT_FreeListHandle handle = CADT_FreeListCreate(sizeof(uintptr_t)+2, 10);
	REQUIRE(handle);
	uintptr_t* d = (uintptr_t*) CADT_FreeListAlloc(handle);
	*d = 42;
	CADT_FreeListFree(handle, d);
	REQUIRE(*d == 1);
	CADT_FreeListDestroy(handle);
}