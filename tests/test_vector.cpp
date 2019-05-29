#include "al2o3_catch2/catch2.hpp"
#include "al2o3_cadt/vector.h"

TEST_CASE("CVector create/destroy", "[CADT CVector]") {
	CADT_VectorHandle handle = CADT_VectorCreate(10);
	REQUIRE(handle);
	CADT_VectorDestroy(handle);
}

TEST_CASE("CVector Initial Gets", "[CADT CVector]") {
	CADT_VectorHandle handle = CADT_VectorCreate(10);
	REQUIRE(handle);
	REQUIRE( CADT_VectorCapacityFrom(handle) == 0);
	REQUIRE( CADT_VectorSizeFrom(handle) >= 0);
	REQUIRE( CADT_VectorDataFrom(handle) == NULL);
	CADT_VectorDestroy(handle);
}

TEST_CASE("CVector Resizes", "[CADT CVector]") {
	CADT_VectorHandle handle = CADT_VectorCreate(10);
	REQUIRE(handle);
	CADT_VectorResize(handle, 10);
	REQUIRE( CADT_VectorSizeFrom(handle) == 10);
	REQUIRE( CADT_VectorCapacityFrom(handle) >= 10);
	REQUIRE( CADT_VectorDataFrom(handle) != NULL);
	CADT_VectorResize(handle, 5);
	REQUIRE( CADT_VectorSizeFrom(handle) == 5);
	REQUIRE( CADT_VectorCapacityFrom(handle) >= 10);
	CADT_VectorResize(handle, 15);
	REQUIRE( CADT_VectorSizeFrom(handle) == 15);
	REQUIRE( CADT_VectorCapacityFrom(handle) >= 15);

	CADT_VectorDestroy(handle);
}
TEST_CASE("CVector Push/Pop/Peek", "[CADT CVector]") {
	CADT_VectorHandle handle = CADT_VectorCreate(10);
	REQUIRE(handle);
	uint8_t e[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	CADT_VectorPushElement(handle, e);
	REQUIRE( CADT_VectorSizeFrom(handle) == 1);
	REQUIRE( CADT_VectorCapacityFrom(handle) >= 1);
	REQUIRE( memcmp(CADT_VectorPeekElementFrom(handle), e, 10) == 0);
	uint8_t r[10];
	CADT_VectorPopElementFrom(handle, r);
	REQUIRE( CADT_VectorSizeFrom(handle) == 0);
	REQUIRE( CADT_VectorCapacityFrom(handle) >= 1);
	REQUIRE( memcmp(r, e, 10) == 0);

	CADT_VectorDestroy(handle);
}

TEST_CASE("CVector ElementFrom", "[CADT CVector]") {
	CADT_VectorHandle handle = CADT_VectorCreate(10);
	REQUIRE(handle);
	uint8_t e0[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	uint8_t e1[10] = { 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 };

	REQUIRE(CADT_VectorPushElement(handle, e0) == 0);
	REQUIRE(CADT_VectorPushElement(handle, e1) == 1);

	uint8_t* e0p = (uint8_t*)CADT_VectorElementFrom(handle, 0);
	REQUIRE(memcmp(e0p, e0, 10) == 0);
	uint8_t* e1p = (uint8_t*)CADT_VectorElementFrom(handle, 1);
	REQUIRE(memcmp(e1p, e1, 10) == 0);

	CADT_VectorResize(handle, 1);
	e0p = (uint8_t*)CADT_VectorElementFrom(handle, 0);
	REQUIRE(memcmp(e0p, e0, 10) == 0);
	CADT_VectorPopAndDiscardElementFrom(handle);
	REQUIRE(CADT_VectorSizeFrom(handle) == 0);

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
	CADT_VectorPopAndDiscardElementFrom(handle);
	CADT_VectorPopAndDiscardElementFrom(handle);
	REQUIRE(CADT_VectorIsEmpty(handle));

}