/************************************************************************/
/**
 * @file Test_MiscUtils.cpp
 * @brief Unit tests of the CWMPEndpoint class
 * @author Anar Manafov A.Manafov@gsi.de
 */ /*

        version number:     $LastChangedRevision$
        created by:         Anar Manafov
                            2007-11-07
        last changed by:    $LastChangedBy$ $LastChangedDate$

        Copyright (c) 2007 GSI GridTeam. All rights reserved.
*************************************************************************/
// BOOST: tests
#include <boost/test/unit_test.hpp>
// STD
#include <string>
#include <iostream>
// Our
#include "WMPEndpoint.h"
#include "SysHelper.h"

using boost::unit_test::test_suite;
using namespace glite_api_wrapper;
using namespace MiscCommon;
using namespace std;

// #1
// Preconditions for tests:
// - valid files "glite_wms.conf" must be present in
//      $HOME/.glite/dech/glite_wms.conf
//      $HOME/.glite/dgtest/glite_wms.conf
// TODO: add also test for $GLITE_LOCATION/etc/<VO>/glite_wms.conf
// - initialize voms proxy with dech and dgtest vo extensions. Use two jdl files with two vo names.
// TESTS:
// - Test using two jdl files, one with dgtest vo another with dech vo that
// ednpoints are found and returned accordingly

void test_get_endpoints();

test_suite* init_unit_test_suite( int, char* [] )
{
    test_suite * test = BOOST_TEST_SUITE( "Unit tests of MiscCommon" );

    test->add( BOOST_TEST_CASE( &test_get_endpoints ), 0 );

    return test;
}


void test_get_endpoints()
{
    // TODO: Move test strings these (below) txt file.
    // dgtest: https://dgrid-rb.fzk.de:7443/glite_wms_wmproxy_server
    // dech: https://grid25.gsi.de:7443/glite_wms_wmproxy_server
    const string endpoint_DECH( "https://grid25.gsi.de:7443/glite_wms_wmproxy_server" );
    const string endpoint_DGTEST( "https://dgrid-rb.fzk.de:7443/glite_wms_wmproxy_server" );

    StringVector_t endpoints;
    CWMPEndpoint ep;
    // DECH VO
    string dech_jdl( "$GAW_LOCATION/etc/simple_dech.jdl" );
    smart_path( &dech_jdl );
    ep.Get( &endpoints, dech_jdl );
    BOOST_CHECK( !endpoints.empty() );
    cout << ">>> DECH endpoints: ";
    copy( endpoints.begin(), endpoints.end(),
          ostream_iterator<string>( cout, " " ) );
    cout << endl;

    StringVector_t::const_iterator iter_DECH( find( endpoints.begin(), endpoints.end(), endpoint_DECH ) );
    BOOST_CHECK( endpoints.end() != iter_DECH );

    // DGTEST VO
    /*  string dgtest_jdl("$GAW_LOCATION/etc/simple_dgtest.jdl");
      smart_path( &dgtest_jdl );
      ep.Get( &endpoints, dgtest_jdl );
      BOOST_CHECK( !endpoints.empty() );
      cout << ">>> DGTEST endpoints: ";
      copy( endpoints.begin(), endpoints.end(),
            ostream_iterator<string>(cout, " "));
      cout << endl;

      StringVector_t::const_iterator iter_DGTEST( find(endpoints.begin(), endpoints.end(), endpoint_DGTEST) );
      BOOST_CHECK( endpoints.end() != iter_DGTEST );*/
}
