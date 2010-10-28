/************************************************************************/
/**
 * @file Test_gLiteHelper.cpp
 * @brief Unit tests of gLiteHelper
 * @author Anar Manafov A.Manafov@gsi.de
 */ /*

        version number:     $LastChangedRevision$
        created by:         Anar Manafov
                            2007-11-05
        last changed by:    $LastChangedBy$ $LastChangedDate$

        Copyright (c) 2007 GSI GridTeam. All rights reserved.
*************************************************************************/
// TODO: This is for boost v.1.34+: http://lists.boost.org/boost-users/2007/04/27361.php
// BOOST: tests
#include <boost/test/unit_test.hpp>
// STD
#include <string>
// Our
#include "gLiteHelper.h"

using boost::unit_test::test_suite;
using namespace MiscCommon;
using namespace MiscCommon::gLite;
using namespace std;

void test_();

test_suite* init_unit_test_suite( int, char* [] )
{
    test_suite * test = BOOST_TEST_SUITE( "Unit tests of gLiteHelper" );

    test->add( BOOST_TEST_CASE( &test_ ), 0 );

    return test;
}

void test_()
{
    BOOST_CHECK( true );
}
