#include "al2o3_catch2/catch2.hpp"
#include "al2o3_cadt/vector.h"

TEST_CASE("CVector create/destroy", "[CADT CVector]") {
	CADT_VectorHandle handle = CADT_VectorCreate(10);
	REQUIRE(handle);
	CADT_VectorDestroy(handle);
}

TEST_CASE("CVector Initial", "[CADT CVector]") {
	CADT_VectorHandle handle = CADT_VectorCreate(10);
	REQUIRE(handle);
	REQUIRE( CADT_VectorCapacity(handle) == 0);
	REQUIRE( CADT_VectorSize(handle) >= 0);
	REQUIRE( CADT_VectorData(handle) == NULL);
	CADT_VectorDestroy(handle);
}

TEST_CASE("CVector At", "[CADT CVector]") {
	CADT_VectorHandle handle = CADT_VectorCreate(10);
	REQUIRE(handle);
	uint8_t e0[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	uint8_t e1[10] = { 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 };

	REQUIRE(CADT_VectorPushElement(handle, e0) == 0);
	REQUIRE(CADT_VectorPushElement(handle, e1) == 1);

	uint8_t* e0p = (uint8_t*)CADT_VectorAt(handle, 0);
	REQUIRE(memcmp(e0p, e0, 10) == 0);
	uint8_t* e1p = (uint8_t*)CADT_VectorAt(handle, 1);
	REQUIRE(memcmp(e1p, e1, 10) == 0);

	CADT_VectorResize(handle, 1);
	e0p = (uint8_t*)CADT_VectorAt(handle, 0);
	REQUIRE(memcmp(e0p, e0, 10) == 0);
	CADT_VectorPopAndDiscardElement(handle);
	REQUIRE(CADT_VectorSize(handle) == 0);

	CADT_VectorDestroy(handle);
}

TEST_CASE("CVector Resizes and At", "[CADT CVector]") {
	CADT_VectorHandle handle = CADT_VectorCreate(sizeof(uint32_t));
	REQUIRE(handle);
	CADT_VectorResize(handle, 10);
	REQUIRE( CADT_VectorSize(handle) == 10);
	REQUIRE( CADT_VectorCapacity(handle) >= 10);
	REQUIRE( CADT_VectorData(handle) != NULL);
	CADT_VectorResize(handle, 5);
	REQUIRE( CADT_VectorSize(handle) == 5);
	REQUIRE( CADT_VectorCapacity(handle) >= 10);
	CADT_VectorResize(handle, 15);
	REQUIRE( CADT_VectorSize(handle) == 15);
	REQUIRE( CADT_VectorCapacity(handle) >= 15);

	CADT_VectorResize(handle, 0);
	CADT_VectorResizeWithZero(handle, 5);
	REQUIRE(*((uint32_t*)CADT_VectorAt(handle, 0)) == 0);
	REQUIRE(*((uint32_t*)CADT_VectorAt(handle, 1)) == 0);
	REQUIRE(*((uint32_t*)CADT_VectorAt(handle, 2)) == 0);
	REQUIRE(*((uint32_t*)CADT_VectorAt(handle, 3)) == 0);
	REQUIRE(*((uint32_t*)CADT_VectorAt(handle, 4)) == 0);

	uint32_t defaultVal = 0xDCDCDCDC;
	CADT_VectorResizeWithZero(handle, 1);
	CADT_VectorResizeWithDefault(handle, 5, &defaultVal);
	REQUIRE(*((uint32_t*)CADT_VectorAt(handle, 0)) == 0);
	REQUIRE(*((uint32_t*)CADT_VectorAt(handle, 1)) == defaultVal);
	REQUIRE(*((uint32_t*)CADT_VectorAt(handle, 2)) == defaultVal);
	REQUIRE(*((uint32_t*)CADT_VectorAt(handle, 3)) == defaultVal);
	REQUIRE(*((uint32_t*)CADT_VectorAt(handle, 4)) == defaultVal);

	CADT_VectorDestroy(handle);
}

TEST_CASE("CVector Push/Pop/Peek", "[CADT CVector]") {
	CADT_VectorHandle handle = CADT_VectorCreate(10);
	REQUIRE(handle);
	uint8_t e[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	CADT_VectorPushElement(handle, e);
	REQUIRE( CADT_VectorSize(handle) == 1);
	REQUIRE( CADT_VectorCapacity(handle) >= 1);
	REQUIRE( memcmp(CADT_VectorPeekElement(handle), e, 10) == 0);
	uint8_t r[10];
	CADT_VectorPopElement(handle, r);
	REQUIRE( CADT_VectorSize(handle) == 0);
	REQUIRE( CADT_VectorCapacity(handle) >= 1);
	REQUIRE( memcmp(r, e, 10) == 0);

	CADT_VectorDestroy(handle);
}

TEST_CASE("CVector IsEmpty", "[CADT CVector]") {
	CADT_VectorHandle handle = CADT_VectorCreate(10);
	REQUIRE(handle);
	REQUIRE(CADT_VectorIsEmpty(handle));
	uint8_t e[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	CADT_VectorPushElement(handle, e);
	REQUIRE(!CADT_VectorIsEmpty(handle));
	CADT_VectorPushElement(handle, e);
	REQUIRE(!CADT_VectorIsEmpty(handle));
	CADT_VectorPopAndDiscardElement(handle);
	CADT_VectorPopAndDiscardElement(handle);
	REQUIRE(CADT_VectorIsEmpty(handle));

}

TEST_CASE("CVector Remove", "[CADT CVector]") {
	CADT_VectorHandle handle = CADT_VectorCreate(10);
	REQUIRE(handle);
	uint8_t e0[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	uint8_t e1[10] = { 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 };

	REQUIRE(CADT_VectorPushElement(handle, e0) == 0);
	REQUIRE(CADT_VectorPushElement(handle, e1) == 1);
	REQUIRE(CADT_VectorPushElement(handle, e0) == 2);
	REQUIRE(CADT_VectorPushElement(handle, e1) == 3);
	REQUIRE(CADT_VectorSize(handle) == 4);

	CADT_VectorRemove(handle, 3);
	REQUIRE(CADT_VectorSize(handle) == 3);
	CADT_VectorRemove(handle, 0);
	REQUIRE(CADT_VectorSize(handle) == 2);
	uint8_t* e1p = (uint8_t*)CADT_VectorAt(handle, 0);
	REQUIRE(memcmp(e1p, e1, 10) == 0);
	uint8_t* e0p = (uint8_t*)CADT_VectorAt(handle, 1);
	REQUIRE(memcmp(e0p, e0, 10) == 0);
	CADT_VectorRemove(handle, 1);
	REQUIRE(CADT_VectorSize(handle) == 1);
	CADT_VectorRemove(handle, 0);
	REQUIRE(CADT_VectorSize(handle) == 0);
}

TEST_CASE("CVector Swap/Replace/Remove", "[CADT CVector]") {
	CADT_VectorHandle handle = CADT_VectorCreate(sizeof(uint32_t));
	REQUIRE(handle);

	CADT_VectorResizeWithZero(handle, 5);
	REQUIRE(*(uint32_t*)CADT_VectorAt(handle, 0) == 0);
	*(uint32_t*)CADT_VectorAt(handle, 1) = 10;
	*(uint32_t*)CADT_VectorAt(handle, 2) = 20;
	*(uint32_t*)CADT_VectorAt(handle, 3) = 30;
	*(uint32_t*)CADT_VectorAt(handle, 4) = 40;
	REQUIRE(CADT_VectorSize(handle) == 5);

	CADT_VectorReplace(handle, 4, 1);
	REQUIRE(*(uint32_t*)CADT_VectorAt(handle, 1) == 40);
	REQUIRE(*(uint32_t*)CADT_VectorAt(handle, 4) == 40);
	CADT_VectorSwap(handle, 2, 3);
	REQUIRE(*(uint32_t*)CADT_VectorAt(handle, 2) == 30);
	REQUIRE(*(uint32_t*)CADT_VectorAt(handle, 3) == 20);
	CADT_VectorSwapRemove(handle, 0);
	REQUIRE(*(uint32_t*)CADT_VectorAt(handle, 0) == 40);
	REQUIRE(*(uint32_t*)CADT_VectorAt(handle, 1) == 40);
	REQUIRE(CADT_VectorSize(handle) == 4);
	CADT_VectorRemove(handle, 1);
	REQUIRE(*(uint32_t*)CADT_VectorAt(handle, 0) == 40);
	REQUIRE(*(uint32_t*)CADT_VectorAt(handle, 1) == 30);
	REQUIRE(CADT_VectorSize(handle) == 3);

	CADT_VectorDestroy(handle);
}