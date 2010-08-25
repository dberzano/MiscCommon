/************************************************************************/
/**
 * @file Test_Process.cpp
 * @brief Unit tests of Process.h
 * @author Anar Manafov A.Manafov@gsi.de
 */ /*

        version number:     $LastChangedRevision$
        created by:         Anar Manafov
                            2007-10-31
        last changed by:    $LastChangedBy$ $LastChangedDate$

        Copyright (c) 2007-2010 GSI GridTeam. All rights reserved.
*************************************************************************/
// BOOST: tests
// Defines test_main function to link with actual unit test code.
#define BOOST_TEST_DYN_LINK
#define BOOST_AUTO_TEST_MAIN    // Boost 1.33
#define BOOST_TEST_MAIN
#define BOOST_TEST_IGNORE_SIGCHLD // we don't need a dead child reported as an error
#include <boost/test/auto_unit_test.hpp>

// MiscCommon
#include "Process.h"
//=============================================================================
using namespace MiscCommon;
using namespace std;
using boost::unit_test::test_suite;
//=============================================================================
BOOST_AUTO_TEST_SUITE( pod_agent_ProtocolCommands );
//=============================================================================
BOOST_AUTO_TEST_CASE( test_MiscCommon_CProcStatus )
{
    CProcStatus p;
    pid_t pid( ::getpid() );
    p.Open( pid );
// TODO: need a new algorithms for a longer app names retrieval
    BOOST_CHECK( p.GetValue( "Name" ) == "MiscCommon_test" );

    BOOST_CHECK( p.GetValue( "NAME" ) == "MiscCommon_test" );

    stringstream ss_pid;
    ss_pid << pid;
    BOOST_CHECK( p.GetValue( "Pid" ) == ss_pid.str() );
}
//=============================================================================
BOOST_AUTO_TEST_CASE( test_MiscCommon_do_execv0 )
{
    // bash -c "echo test > test.test"
    const string sFile( "/tmp/test.test" );

    StringVector_t params;
    params.push_back( "-c" );
    params.push_back( "` echo test > /tmp/test.test; sleep 5; echo test2 > /tmp/test.test`" );

    const string cmd( "/bin/bash" );

    do_execv( cmd, params, 10, NULL );

    ifstream test_file( sFile.c_str() );
    BOOST_CHECK( test_file.is_open() );

    string test_string;
    test_file >> test_string;
    BOOST_CHECK_EQUAL( test_string, "test2" );

    if( test_file.is_open() )
    {
        test_file.close();
        ::unlink( sFile.c_str() );
    }

    StringVector_t p;
    p.push_back( "-f" );
    string output;
    do_execv( "/bin/hostname", p, 10, &output );
    BOOST_CHECK( output.size() > 0 );
}
//=============================================================================
BOOST_AUTO_TEST_CASE( test_MiscCommon_do_execv1 )
{
    StringVector_t params;
    params.push_back( "5" );
    const string cmd( "/bin/sleep" );
    BOOST_CHECK_THROW( do_execv( cmd, params, 3, NULL ), runtime_error );
}
//=============================================================================
BOOST_AUTO_TEST_CASE( test_MiscCommon_do_execv2 )
{
    StringVector_t params;
    params.push_back( "eee" );
    const string cmd( "XXXXX" );
    BOOST_CHECK_THROW( do_execv( cmd, params, 3, NULL ), runtime_error );
}
//=============================================================================
BOOST_AUTO_TEST_SUITE_END();
