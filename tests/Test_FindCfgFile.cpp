/************************************************************************/
/**
 * @file Test_FindCfgFile.cpp
 * @brief
 * @author Anar Manafov A.Manafov@gsi.de
 */ /*

        version number:     $LastChangedRevision$
        created by:         Anar Manafov
                            2007-11-23
        last changed by:    $LastChangedBy$ $LastChangedDate$

        Copyright (c) 2007 GSI GridTeam. All rights reserved.
************************************************************************/
// TODO: This is for boost v.1.34+: http://lists.boost.org/boost-users/2007/04/27361.php
// BOOST: tests
#include <boost/test/unit_test.hpp>
// MiscCommon
#include "FindCfgFile.h"

using boost::unit_test::test_suite;
using namespace std;
using namespace MiscCommon;

void test_order();
void test_match1();
void test_match2();

test_suite* init_unit_test_suite( int, char* [] )
{
    test_suite * test = BOOST_TEST_SUITE( "Unit tests of FindCfgFile" );

    test->add( BOOST_TEST_CASE( &test_order ), 0 );
    test->add( BOOST_TEST_CASE( &test_match1 ), 0 );
    test->add( BOOST_TEST_CASE( &test_match2 ), 0 );

    return test;
}

void test_order()
{
    const string sT1("Test1");
    const string sT2("Test2");
    const string sT3("Test3");
    const string sTempl( sT1 + sT2 + sT3 );
    CFindCfgFile<string> cfg;
    cfg.SetOrder(sT1)(sT2)(sT3);
    ostringstream ss;
    cfg.DumpOrder( &ss, "" );
    BOOST_CHECK( ss.str() == sTempl );
}

void test_match1()
{
    CFindCfgFile<string> cfg;
    const string sTempl( "/etc/bashrc" );
    cfg.SetOrder("/etc/rtt")(sTempl)("testtesttest")("$HOME/.bashrc");
    string val;
    cfg.GetCfg( &val );
// TODO: rewrite test, so that there will be a file, which always there on all systems.
// /etc/bashrc is not always present  
//    BOOST_CHECK( val == sTempl );
}

void test_match2()
{
    CFindCfgFile<string> cfg;
    const string sTempl( "$HOME/.bashrc" );
    cfg.SetOrder(sTempl)("/etc/rtt")("/etc/bashrc")("testtesttest");
    string val;
    cfg.GetCfg( &val );
    BOOST_CHECK( val == sTempl );
}
