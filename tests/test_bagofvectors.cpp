#include "al2o3_catch2/catch2.hpp"
#include "al2o3_cadt/bagofvectors.h"

TEST_CASE("CADT_BagOfVectors create/destroy", "[CADT CADT_BagOfVectors]") {
	CADT_BagOfVectorsHandle handle = CADT_BagOfVectorsCreate();
	REQUIRE(handle);
	CADT_BagOfVectorsDestroy(handle);
}

static const uint64_t TestTag = 0xDCDCDCDCDCDCDCD0U;

TEST_CASE("CADT_BagOfVectors add/exists", "[CADT CADT_BagOfVectors]") {
	CADT_BagOfVectorsHandle handle = CADT_BagOfVectorsCreate();
	REQUIRE(handle);

	CADT_BagOfVectorsAdd(handle,
														TestTag | '0',
														10);
	REQUIRE( CADT_BagOfVectorsKeyExists(handle, TestTag | '0' ));
	CADT_BagOfVectorsAdd(handle,
														TestTag | '1',
														20);
	REQUIRE( CADT_BagOfVectorsKeyExists(handle, TestTag | '1' ));

	CADT_BagOfVectorsDestroy(handle);
}

TEST_CASE("CADT_BagOfVectors lookup", "[CADT CADT_BagOfVectors]") {
	CADT_BagOfVectorsHandle handle = CADT_BagOfVectorsCreate();
	REQUIRE(handle);

	CADT_BagOfVectorsAdd(handle,
													 TestTag | '0',
													 10);
	CADT_VectorHandle v0 = CADT_BagOfVectorsLookup(handle, TestTag | '0');
	REQUIRE(CADT_VectorElementSize(v0) == 10);
	CADT_BagOfVectorsAdd(handle,
													 TestTag | '1',
													 20);
	CADT_VectorHandle v0a = CADT_BagOfVectorsLookup(handle, TestTag | '0');
	CADT_VectorHandle v1a = CADT_BagOfVectorsLookup(handle, TestTag | '1');
	REQUIRE(CADT_VectorElementSize(v0a) == 10);
	REQUIRE(CADT_VectorElementSize(v1a) == 20);

	CADT_BagOfVectorsDestroy(handle);
}
