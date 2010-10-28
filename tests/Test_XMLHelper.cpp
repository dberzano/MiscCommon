/************************************************************************/
/**
 * @file Test_XMLHelper.cpp
 * @brief Unit tests of the XMLHelper
 * @author Anar Manafov A.Manafov@gsi.de
 */ /*

        version number:     $LastChangedRevision$
        created by:         Anar Manafov
                            2008-01-17
        last changed by:    $LastChangedBy$ $LastChangedDate$

        Copyright (c) 2008 GSI GridTeam. All rights reserved.
*************************************************************************/
// BOOST: tests
#include <boost/test/unit_test.hpp>
// STD
#include <string>
// XERCES-C
#include <xercesc/util/PlatformUtils.hpp>
// MiscCommon
#include "XMLHelper.h"

using boost::unit_test::test_suite;
using namespace MiscCommon::XMLHelper;
using namespace std;
XERCES_CPP_NAMESPACE_USE;

void test_smrat_XMLCh0();

test_suite* init_unit_test_suite( int, char* [] )
{
    test_suite * test = BOOST_TEST_SUITE( "Unit tests of MiscCommon" );

    test->add( BOOST_TEST_CASE( &test_smrat_XMLCh0 ), 0 );

    return test;
}

void test_smrat_XMLCh0()
{
    try
    {
        XMLPlatformUtils::Initialize();
    }
    catch( const XMLException& toCatch )
    {
        throw runtime_error( "Failed to call XMLPlatformUtils::Initialize()" );
    }
    smart_XMLCh xmlch( "Test" );
    BOOST_CHECK( "Test" == xmlch );
    BOOST_CHECK( xmlch == "Test" );
    BOOST_CHECK( smart_XMLCh( "Test2" ) == "Test2" );
    BOOST_CHECK( "Test2" == smart_XMLCh( "Test2" ) );

    XMLPlatformUtils::Terminate();
}
