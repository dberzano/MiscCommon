/************************************************************************/
/**
 * @file Test_MiscUtils.cpp
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
#include <fstream>
// Our
#include "MiscUtils.h"
#include "SysHelper.h"

using boost::unit_test::test_suite;
using namespace MiscCommon;
using namespace std;

void test_smart_path0();
void test_smart_path1();
void test_smart_path2();
void test_smart_path3();
void test_smart_path4();
void test_smart_path5();
void test_smart_path6();
void test_smart_path7();
void test_smart_path8();
void test_file_size0();
void test_file_size1();

test_suite* init_unit_test_suite( int, char* [] )
{
    test_suite * test = BOOST_TEST_SUITE( "Unit tests of MiscCommon" );

    test->add( BOOST_TEST_CASE( &test_smart_path0 ) );
    test->add( BOOST_TEST_CASE( &test_smart_path1 ) );
    test->add( BOOST_TEST_CASE( &test_smart_path2 ) );
    test->add( BOOST_TEST_CASE( &test_smart_path3 ) );
    test->add( BOOST_TEST_CASE( &test_smart_path4 ) );
    test->add( BOOST_TEST_CASE( &test_smart_path5 ) );
    test->add( BOOST_TEST_CASE( &test_smart_path6 ) );
    test->add( BOOST_TEST_CASE( &test_smart_path7 ) );
    test->add( BOOST_TEST_CASE( &test_smart_path8 ) );
    test->add( BOOST_TEST_CASE( &test_file_size0 ) );
    test->add( BOOST_TEST_CASE( &test_file_size1 ) );

    return test;
}



void test_smart_path0()
{
    string path("$TEST1/opt/test.xml");
    const string path_res("/test1/demo/opt/test.xml");

    auto_setenv env("TEST1", "/test1/demo");
    smart_path( &path );
    BOOST_CHECK( path == path_res );
}

void test_smart_path1()
{
    string path("$TEST1/opt/$TEST2");
    const string path_res("/test1/demo/opt//test2/demo2");

    auto_setenv env1("TEST1", "/test1/demo");
    auto_setenv env2("TEST2", "/test2/demo2");
    smart_path( &path );
    BOOST_CHECK( path == path_res );
}

void test_smart_path2()
{
    string path("$TEST1/opt/$TEST2");
    const string path_res("$TEST1/opt/$TEST2");
    smart_path( &path );
    BOOST_CHECK( path == path_res );
}

void test_smart_path3()
{
    string path("$TEST1/opt");
    const string path_res("/test1//test2/demo2/demo/opt");

    auto_setenv env1("TEST1", "/test1/$TEST2/demo");
    auto_setenv env2("TEST2", "/test2/demo2");
    smart_path( &path );
    BOOST_CHECK( path == path_res );
}

void test_smart_path4()
{
    string path("$TEST1/opt");
    const string path_res("/test2/demo2/opt");

    auto_setenv env1("TEST1", "$TEST2");
    auto_setenv env2("TEST2", "/test2/demo2");
    smart_path( &path );
    BOOST_CHECK( path == path_res );
}

void test_smart_path5()
{
    string path("~/test/test");
    const string sTempl("/home/anar/test/test");
    smart_path( &path );
// TODO: rewrite test, so that there will be no hard-codded "/home/anar"   
// BOOST_CHECK( sTempl == path );
}

void test_smart_path6()
{
    string path("/~test/test");
    const string sTempl("/~test/test");
    smart_path( &path );
    BOOST_CHECK( sTempl == path );
}

void test_smart_path7()
{
    string path("/test/test~/");
    const string sTempl("/test/test~/");
    smart_path( &path );
    BOOST_CHECK( sTempl == path );
}

void test_smart_path8()
{
    string path("~anar/test/test");
    const string sTempl("/home/anar/test/test");
    smart_path( &path );
// TODO: rewrite test, so that there will be no hard-codded "/home/anar"  
//    BOOST_CHECK( sTempl == path );
}

void test_file_size0()
{
    // Creating test file
    const string filename( "test_file_size.txt" );
    ofstream f( filename.c_str() );
    const off_t size = 1000;
    const string buf( size, 'A' );
    f << buf;
    f.close();
    BOOST_CHECK_EQUAL( size, file_size( filename ) );
    ::unlink( filename.c_str() );
}

void test_file_size1()
{
    const string filename( "missing_file.txt" );
    BOOST_CHECK_THROW( file_size( filename ), system_error );
}
