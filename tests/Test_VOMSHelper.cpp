/************************************************************************/
/**
 * @file Test_VOMSHelper.cpp
 * @brief Unit tests of VOMSHelper
 * @author Anar Manafov A.Manafov@gsi.de
 */ /*

        version number:     $LastChangedRevision$
        created by:         Anar Manafov
                            2007-11-06
        last changed by:    $LastChangedBy$ $LastChangedDate$

        Copyright (c) 2007 GSI GridTeam. All rights reserved.
*************************************************************************/
// BOOST: tests
#include <boost/test/unit_test.hpp>
// STD
#include <string>
#include <iostream>
// MiscCommon
#include "def.h"
// Our
#include "VOMSHelper.h"
#include "SysHelper.h"
#include "JDLHelper.h"

using boost::unit_test::test_suite;
using namespace MiscCommon;
using namespace MiscCommon::gLite;
using namespace std;

void test_get_voname();
void test_get_voname_from_jdl();


test_suite* init_unit_test_suite( int, char* [] )
{
    test_suite * test = BOOST_TEST_SUITE( "Unit tests of VOMSHelper" );

    test->add( BOOST_TEST_CASE( &test_get_voname ), 0 );
    test->add( BOOST_TEST_CASE( &test_get_voname_from_jdl ), 0 );

    return test;
}



void test_get_voname()
{
    StringVector_t VONames;
    get_voname( &VONames );
    cout << ">>> VONames: ";
    copy( VONames.begin(), VONames.end(), ostream_iterator<string>(cout, " ") );
    cout << " <<<" << endl;
    BOOST_CHECK( !VONames.empty() );
}

void test_get_voname_from_jdl()
{
    string voname;
    string jdl_path( "$GAW_LOCATION/etc/simple_dech.jdl" );
    smart_path( &jdl_path );
    get_ad_attr( &voname, jdl_path, glite::jdl::JDL::VIRTUAL_ORGANISATION );
    cout << ">>> VOName from [" << jdl_path << "]: " << voname << endl;
    BOOST_CHECK( !voname.empty() );
}
