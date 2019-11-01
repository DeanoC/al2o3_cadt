#include "al2o3_catch2/catch2.hpp"
#include "al2o3_memory/memory.h"
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
	CADT_FreeListRelease(handle, d);
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

	CADT_FreeListRelease(handle, d2);
	CADT_FreeListRelease(handle, d1);
	CADT_FreeListRelease(handle, d0);

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
	CADT_FreeListRelease(handle, ptrs[0]);
	uintptr_t* p0 = (uintptr_t*) CADT_FreeListAlloc(handle);
	REQUIRE(p0 != nullptr);
	REQUIRE(p0 == ptrs[0]);

	CADT_FreeListRelease(handle, ptrs[5]);
	auto cloneHandle = CADT_FreeListClone(handle);

	uintptr_t* p1 = (uintptr_t*) CADT_FreeListAlloc(handle);
	REQUIRE(p1 != nullptr);
	REQUIRE(p1 == ptrs[5]);
	for(int i = 0;i < NUM_ENTRIES;++i) {
		CADT_FreeListRelease(handle, ptrs[i]);
	}

	uintptr_t* p2 = (uintptr_t*) CADT_FreeListAlloc(cloneHandle);
	REQUIRE(p2 != nullptr);
	REQUIRE(CADT_FreeListAlloc(cloneHandle) == nullptr);

	CADT_FreeListDestroy(cloneHandle);
	CADT_FreeListDestroy(handle);

}

#include "al2o3_cadt/freelist.hpp"

TEST_CASE("Cadt::FreeList create/destroy", "[CADT CADT_FreeList]") {
	auto fl = Cadt::FreeList<uintptr_t>::Create(10);
	REQUIRE(fl);
	Cadt::FreeList<uintptr_t>::Destroy(fl);
}

TEST_CASE("Cadt::FreeList Single Alloc/Free", "[CADT CADT_FreeList]") {
	auto fl = Cadt::FreeList<uintptr_t>::Create(10);
	REQUIRE(fl);
	uintptr_t* d = fl->alloc();
	*d = 42;
	fl->release(d);
	REQUIRE(*d == 1);
	fl->destroy();
}

TEST_CASE("Cadt::FreeList Multiple Alloc/Free", "[CADT CADT_FreeList]") {
	auto fl = Cadt::FreeList<uintptr_t>::Create(10);
	REQUIRE(fl);
	uintptr_t* d0 = fl->alloc();
	*d0 = 42;
	uintptr_t* d1 = fl->alloc();
	*d1 = 3;
	uintptr_t* d2 = fl->alloc();
	*d2 = 13;

	fl->release(d0);
	fl->release(d1);
	fl->release(d2);

	fl->destroy();
}

TEST_CASE("Cadt::FreeList adhoc Alloc/Free", "[CADT CADT_FreeList]") {
	static const int NUM_ENTRIES = 10;
	auto fl = Cadt::FreeList<uintptr_t>::Create(NUM_ENTRIES);
	REQUIRE(fl);
	uintptr_t* ptrs[NUM_ENTRIES];
	for(int i = 0;i < NUM_ENTRIES;++i) {
		uintptr_t* d0 = fl->alloc();
		*d0 = 42;
		ptrs[i] = d0;
	}
	REQUIRE(fl->alloc() == nullptr);
	fl->release(ptrs[0]);
	uintptr_t* p0 = fl->alloc();
	REQUIRE(p0 != nullptr);
	REQUIRE(p0 == ptrs[0]);

	fl->release(ptrs[5]);
	auto cloneHandle = fl->clone();

	uintptr_t* p1 = fl->alloc();
	REQUIRE(p1 != nullptr);
	REQUIRE(p1 == ptrs[5]);
	for(int i = 0;i < NUM_ENTRIES;++i) {
		fl->release(ptrs[i]);
	}

	uintptr_t* p2 = cloneHandle->alloc();
	REQUIRE(p2 != nullptr);
	REQUIRE(cloneHandle->alloc() == nullptr);

	cloneHandle->destroy();
	fl->destroy();
}

struct NotPod{
	NotPod() {
		ptr = MEMORY_MALLOC(10);
	}
	~NotPod() {
		MEMORY_FREE(ptr);
	}
	void* ptr;
};

/*
TEST_CASE("Cadt::FreeList not POD", "[CADT CADT_FreeList]") {
	auto fl = Cadt::FreeList<NotPod>::Create(10);
	REQUIRE(fl);

}
*/

#include "al2o3_cadt/freelistft.h"
TEST_CASE("CADT_FreeListFT create/destroy", "[CADT CADT_FreeList]") {
	CADT_FreeListFTHandle handle = CADT_FreeListFTCreate(sizeof(uintptr_t)+2, 16, 4);
	REQUIRE(handle);
	REQUIRE(CADT_FreeListFTElementSize(handle) == sizeof(uintptr_t)+2);
	CADT_FreeListFTDestroy(handle);
}

TEST_CASE("CADT_FreeListFT Single Alloc/Free", "[CADT CADT_FreeList]") {
	CADT_FreeListFTHandle handle = CADT_FreeListFTCreate(sizeof(uintptr_t)+2, 16, 4);
	REQUIRE(handle);
	uintptr_t* d = (uintptr_t*) CADT_FreeListFTAlloc(handle);
	*d = 42;
	CADT_FreeListFTRelease(handle, d);
	REQUIRE(*d != 42);
	CADT_FreeListFTDestroy(handle);
}

TEST_CASE("CADT_FreeListFT Multiple Alloc/Free", "[CADT CADT_FreeList]") {
	CADT_FreeListFTHandle handle = CADT_FreeListFTCreate(sizeof(uintptr_t)+2, 16, 4);
	REQUIRE(handle);
	uintptr_t* d0 = (uintptr_t*) CADT_FreeListFTAlloc(handle);
	*d0 = 42;
	uintptr_t* d1 = (uintptr_t*) CADT_FreeListFTAlloc(handle);
	*d1 = 3;
	uintptr_t* d2 = (uintptr_t*) CADT_FreeListFTAlloc(handle);
	*d2 = 13;

	CADT_FreeListFTRelease(handle, d2);
	CADT_FreeListFTRelease(handle, d1);
	CADT_FreeListFTRelease(handle, d0);

	CADT_FreeListFTDestroy(handle);
}

TEST_CASE("CADT_FreeListFT adhoc Alloc/Free", "[CADT CADT_FreeList]") {
	static const int NUM_ENTRIES = 16;
	CADT_FreeListFTHandle handle = CADT_FreeListFTCreate(sizeof(uintptr_t)+2, NUM_ENTRIES, 1);
	REQUIRE(handle);
	uintptr_t* ptrs[NUM_ENTRIES];
	for(int i = 0;i < NUM_ENTRIES;++i) {
		uintptr_t* d0 = (uintptr_t*) CADT_FreeListFTAlloc(handle);
		*d0 = 42;
		ptrs[i] = d0;
	}

	LOGINFO("The next 2 WARN are expected as we are testing overflow");
	REQUIRE(CADT_FreeListFTAlloc(handle) == nullptr);
	CADT_FreeListFTRelease(handle, ptrs[0]);
	uintptr_t* p0 = (uintptr_t*) CADT_FreeListFTAlloc(handle);
	REQUIRE(p0 != nullptr);
	REQUIRE(p0 == ptrs[0]);

	CADT_FreeListFTRelease(handle, ptrs[5]);

	uintptr_t* p1 = (uintptr_t*) CADT_FreeListFTAlloc(handle);
	REQUIRE(p1 != nullptr);
	REQUIRE(p1 == ptrs[5]);
	for(int i = 0;i < NUM_ENTRIES;++i) {
		CADT_FreeListFTRelease(handle, ptrs[i]);
	}

	CADT_FreeListFTDestroy(handle);

}

TEST_CASE("CADT_FreeListFT adhoc Alloc/Free 2", "[CADT CADT_FreeList]") {
	static const int NUM_ENTRIES = 16 * 2;
	CADT_FreeListFTHandle handle = CADT_FreeListFTCreate(sizeof(uintptr_t)+2, NUM_ENTRIES/2, 2);
	REQUIRE(handle);
	uintptr_t* ptrs[NUM_ENTRIES];
	for(int i = 0;i < NUM_ENTRIES;++i) {
		uintptr_t* d0 = (uintptr_t*) CADT_FreeListFTAlloc(handle);
		*d0 = 42;
		ptrs[i] = d0;
	}

	LOGINFO("The next 2 WARN are expected as we are testing overflow");
	REQUIRE(CADT_FreeListFTAlloc(handle) == nullptr);
	CADT_FreeListFTRelease(handle, ptrs[0]);
	uintptr_t* p0 = (uintptr_t*) CADT_FreeListFTAlloc(handle);
	REQUIRE(p0 != nullptr);
	REQUIRE(p0 == ptrs[0]);

	CADT_FreeListFTRelease(handle, ptrs[5]);

	uintptr_t* p1 = (uintptr_t*) CADT_FreeListFTAlloc(handle);
	REQUIRE(p1 != nullptr);
	REQUIRE(p1 == ptrs[5]);
	for(int i = 0;i < NUM_ENTRIES;++i) {
		CADT_FreeListFTRelease(handle, ptrs[i]);
	}

	CADT_FreeListFTDestroy(handle);

}

TEST_CASE("CADT_FreeListFT reset", "[CADT CADT_FreeList]") {
	static const int NUM_ENTRIES = 16 * 2;
	CADT_FreeListFTHandle handle = CADT_FreeListFTCreate(sizeof(uintptr_t) + 2, NUM_ENTRIES / 2, 2);
	REQUIRE(handle);
	uintptr_t *ptrs[NUM_ENTRIES];
	for (int i = 0; i < NUM_ENTRIES; ++i) {
		uintptr_t *d0 = (uintptr_t *) CADT_FreeListFTAlloc(handle);
		*d0 = 42;
		ptrs[i] = d0;
	}
	LOGINFO("The next 2 WARN are expected as we are testing overflow");
	REQUIRE(CADT_FreeListFTAlloc(handle) == nullptr);
	CADT_FreeListFTReset(handle, false);
	for (int i = 0; i < NUM_ENTRIES; ++i) {
		REQUIRE(CADT_FreeListFTAlloc(handle) != nullptr);
	}
	LOGINFO("The next 2 WARN are expected as we are testing overflow");
	REQUIRE(CADT_FreeListFTAlloc(handle) == nullptr);
	CADT_FreeListFTReset(handle, true);
	for (int i = 0; i < NUM_ENTRIES; ++i) {
		REQUIRE(CADT_FreeListFTAlloc(handle) != nullptr);
	}
	LOGINFO("The next 2 WARN are expected as we are testing overflow");
	REQUIRE(CADT_FreeListFTAlloc(handle) == nullptr);

}