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
        cbl.push_back( 3 );
        cbl.push_back( 4 );
        cbl.push_back( 5 );
        REQUIRE( cbl.length() == 5 );
        REQUIRE( cbl.item_at(1) == 2 );
        cbl.clear();
    }

    SECTION( "Adding elements part 2" ) {
        cbl.push_back( 2 );
        cbl.insert(1,1);
        cbl.insert(2,2);
        cbl.insert(3,4);
        REQUIRE( cbl.length() == 4 );
        REQUIRE( cbl.item_at(1) == 1 );
    }

  }
}
