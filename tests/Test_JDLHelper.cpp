/************************************************************************/
/**
 * @file Test_JDLHelper.cpp
 * @brief Unit tests of JDLHelper
 * @author Anar Manafov A.Manafov@gsi.de
 */ /*

        version number:     $LastChangedRevision$
        created by:         Anar Manafov
                            2007-12-06
        last changed by:    $LastChangedBy$ $LastChangedDate$

        Copyright (c) 2007 GSI GridTeam. All rights reserved.
*************************************************************************/
// TODO: This is for boost v.1.34+: http://lists.boost.org/boost-users/2007/04/27361.php
// BOOST: tests
#include <boost/test/unit_test.hpp>
// STD
#include <string>
// Our
#include "SysHelper.h"
#include "JDLHelper.h"

using boost::unit_test::test_suite;
using namespace MiscCommon;
using namespace MiscCommon::gLite;
using namespace std;

void test_get_attr0();
void test_get_attr1();
void test_set_attr0();

test_suite* init_unit_test_suite( int, char* [] )
{
    test_suite * test = BOOST_TEST_SUITE( "Unit tests of JDLHelper" );

    test->add( BOOST_TEST_CASE( &test_get_attr0 ), 0 );
    test->add( BOOST_TEST_CASE( &test_get_attr1 ), 0 );
    test->add( BOOST_TEST_CASE( &test_set_attr0 ), 0 );

    return test;
}

void test_get_attr0()
{
  // DECH VO
  string jdl("$GAW_LOCATION/etc/simple_dech.jdl");
  smart_path( &jdl );
  string Val;
  get_ad_attr( &Val, jdl, glite::jdl::JDL::VIRTUAL_ORGANISATION );
  BOOST_CHECK( Val == "dech" );
}

void test_get_attr1()
{
  // PARAMETERS
  string jdl("$GAW_LOCATION/etc/simple_parametric.jdl");
  smart_path( &jdl );
  int Val;
  get_ad_attr( &Val, jdl, JDL_PARAMETERS );
  BOOST_CHECK( Val == 2 );
}

void test_set_attr0()
{
  // PARAMETERS
  string jdl("$GAW_LOCATION/etc/simple_parametric.jdl");
  smart_path( &jdl );
  const int ValNew(4);
  set_ad_attr( ValNew, jdl, JDL_PARAMETERS );
  
  int Val;
  get_ad_attr( &Val, jdl, JDL_PARAMETERS );
    
  BOOST_CHECK( Val == ValNew );
  
  set_ad_attr( 2, jdl, JDL_PARAMETERS );
}
