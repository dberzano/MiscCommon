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

        Copyright (c) 2007-2008 GSI GridTeam. All rights reserved.
*************************************************************************/
// BOOST: tests
#include <boost/test/unit_test.hpp>
using boost::unit_test::test_suite;

// MiscCommon
#include "Process.h"

using namespace MiscCommon;
using namespace std;

void test_smart_append();
void test_CProcStatus();
void test_do_execv0();
void test_do_execv1();
void test_do_execv2();

test_suite* init_unit_test_suite( int, char* [] )
{
    test_suite * test = BOOST_TEST_SUITE( "Unit tests of Process.h" );

    test->add( BOOST_TEST_CASE( &test_CProcStatus ), 0 );
    test->add( BOOST_TEST_CASE( &test_do_execv0 ), 0 );
    test->add( BOOST_TEST_CASE( &test_do_execv1 ), 0 );
    test->add( BOOST_TEST_CASE( &test_do_execv2 ), 0 );

    return test;
}



void test_CProcStatus()
{
    CProcStatus p;
    pid_t pid( ::getpid() );
    p.Open( pid );
    BOOST_CHECK( p.GetValue("Name") == "UT_Process" );

    BOOST_CHECK( p.GetValue("NAME") == "UT_Process" );

    stringstream ss_pid;
    ss_pid << pid;
    BOOST_CHECK( p.GetValue("Pid") == ss_pid.str() );
}

void test_do_execv0()
{
    // bash -c "echo test > test.test"
    const string sFile( "/tmp/test.test" );

    StringVector_t params;
    params.push_back("-c");
    params.push_back("` echo test > /tmp/test.test; sleep 5; echo test2 > /tmp/test.test`" );

    const string cmd( "/bin/bash" );

    do_execv( cmd, params, 10, NULL );

    ifstream test_file( sFile.c_str() );
    BOOST_CHECK( test_file.is_open() );

    string test_string;
    test_file >> test_string;
    BOOST_CHECK_EQUAL( test_string, "test2" );

    if ( test_file.is_open() )
    {
        test_file.close();
        ::unlink( sFile.c_str() );
    }

    StringVector_t p;
    p.push_back("-f");
    string output;
    do_execv( "/bin/hostname", p, 10, &output);
    BOOST_CHECK( output.size() > 0 );
}

void test_do_execv1()
{
    StringVector_t params;
    params.push_back("5");
    const string cmd( "/bin/sleep" );
    BOOST_CHECK_THROW( do_execv( cmd, params, 3, NULL ), runtime_error );
}

void test_do_execv2()
{
    StringVector_t params;
    params.push_back("eee");
    const string cmd( "XXXXX" );
    BOOST_CHECK_THROW( do_execv( cmd, params, 3, NULL ), runtime_error );
}
