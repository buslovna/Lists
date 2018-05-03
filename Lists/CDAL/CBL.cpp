#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "CBL.h"

SCENARIO( "Testing Catch", "[Catch]" ) {

  GIVEN( "A CBL with some items" ) {

    cop3530::CBL<int> cbl(50);
    REQUIRE( cbl.length() == 0 );

    SECTION( "Adding elements" ) {
        cbl.push_back( 2 );
        cbl.push_front( 1 );
        REQUIRE( cbl.length() == 2 );
        REQUIRE( cbl.item_at(1) == 2 );
    }

  }
}
