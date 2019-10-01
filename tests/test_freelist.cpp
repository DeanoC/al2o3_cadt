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

TEST_CASE("CADT_FreeList Multiple Alloc/Free", "[CADT CADT_FreeList]") {
	CADT_FreeListHandle handle = CADT_FreeListCreate(sizeof(uintptr_t)+2, 10);
	REQUIRE(handle);
	uintptr_t* d0 = (uintptr_t*) CADT_FreeListAlloc(handle);
	*d0 = 42;
	uintptr_t* d1 = (uintptr_t*) CADT_FreeListAlloc(handle);
	*d1 = 3;
	uintptr_t* d2 = (uintptr_t*) CADT_FreeListAlloc(handle);
	*d2 = 13;

	CADT_FreeListFree(handle, d2);
	CADT_FreeListFree(handle, d1);
	CADT_FreeListFree(handle, d0);

	CADT_FreeListDestroy(handle);
}

TEST_CASE("CADT_FreeList adhoc Alloc/Free", "[CADT CADT_FreeList]") {
	static const int NUM_ENTRIES = 10;
	CADT_FreeListHandle handle = CADT_FreeListCreate(sizeof(uintptr_t)+2, NUM_ENTRIES);
	REQUIRE(handle);
	uintptr_t* ptrs[NUM_ENTRIES];
	for(int i = 0;i < NUM_ENTRIES;++i) {
		uintptr_t* d0 = (uintptr_t*) CADT_FreeListAlloc(handle);
		*d0 = 42;
		ptrs[i] = d0;
	}
	REQUIRE(CADT_FreeListAlloc(handle) == nullptr);
	CADT_FreeListFree(handle, ptrs[0]);
	uintptr_t* p0 = (uintptr_t*) CADT_FreeListAlloc(handle);
	REQUIRE(p0 != nullptr);
	REQUIRE(p0 == ptrs[0]);

	CADT_FreeListFree(handle, ptrs[5]);
	auto cloneHandle = CADT_FreeListClone(handle);

	uintptr_t* p1 = (uintptr_t*) CADT_FreeListAlloc(handle);
	REQUIRE(p1 != nullptr);
	REQUIRE(p1 == ptrs[5]);
	for(int i = 0;i < NUM_ENTRIES;++i) {
		CADT_FreeListFree(handle, ptrs[i]);
	}

	uintptr_t* p2 = (uintptr_t*) CADT_FreeListAlloc(cloneHandle);
	REQUIRE(p2 != nullptr);
	REQUIRE(CADT_FreeListAlloc(cloneHandle) == nullptr);

	CADT_FreeListDestroy(cloneHandle);
	CADT_FreeListDestroy(handle);

}

