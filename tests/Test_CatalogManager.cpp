/************************************************************************/
/**
 * @file Test_CatalogManager.cpp
 * @brief
 * @author Anar Manafov A.Manafov@gsi.de
 */ /*

        version number:     $LastChangedRevision$
        created by:         Anar Manafov
                            2007-12-17
        last changed by:    $LastChangedBy$ $LastChangedDate$

        Copyright (c) 2007 GSI GridTeam. All rights reserved.
************************************************************************/
// TODO: This is for boost v.1.34+: http://lists.boost.org/boost-users/2007/04/27361.php
// BOOST: tests
#include <boost/test/unit_test.hpp>
// glite-api-wrapper
#include "gLiteAPIWrapper.h"

using namespace glite_api_wrapper;
using boost::unit_test::test_suite;
using namespace std;

void test_pwd();
void test_cwd();
void test_cwd_except();
void test_rmdir();
void test_mkdir_rmdir();
void test_register_rm();
void test_ls();
void test_ls_stream();
void test_timeout();

test_suite* init_unit_test_suite( int, char* [] )
{
    test_suite * test = BOOST_TEST_SUITE( "Unit tests of GAW Catalog Manager" );

    test->add( BOOST_TEST_CASE( &test_pwd ) );
    test->add( BOOST_TEST_CASE( &test_cwd ) );
    test->add( BOOST_TEST_CASE( &test_cwd_except ) );
    test->add( BOOST_TEST_CASE( &test_rmdir ) );
    test->add( BOOST_TEST_CASE( &test_mkdir_rmdir ) );
    test->add( BOOST_TEST_CASE( &test_register_rm ) );
    test->add( BOOST_TEST_CASE( &test_ls ) );
    test->add( BOOST_TEST_CASE( &test_ls_stream ) );
    test->add( BOOST_TEST_CASE( &test_timeout ) );

    return test;
}

void test_pwd()
{
    BOOST_CHECK( !CGLiteAPIWrapper::Instance().Init() );

    CCatalogManager * pCatalog( &CGLiteAPIWrapper::Instance().GetCatalogManager() );
    BOOST_CHECK( pCatalog ) ;

    string sRetVal;
    gaw_lfc_pwd pwd;
    pCatalog->Run( pwd, &sRetVal );
    cout << "---> Current catalog directory: " << sRetVal << endl;
    BOOST_CHECK( !sRetVal.empty() );
}

void test_cwd()
{
    BOOST_CHECK( !CGLiteAPIWrapper::Instance().Init() );

    CCatalogManager * pCatalog( &CGLiteAPIWrapper::Instance().GetCatalogManager() );
    BOOST_CHECK( pCatalog );

    gaw_lfc_cwd cwd;
    cwd.m_dir = "dech";
    cout << "---> Changing catalog directory to: " << cwd.m_dir << endl;
    pCatalog->Run( cwd );
    string sRetVal;
    gaw_lfc_pwd pwd;
    pCatalog->Run( pwd, &sRetVal );
    cout << "---> Current catalog directory: " << sRetVal << endl;
    BOOST_CHECK_EQUAL( "/grid/dech", sRetVal );
}

void test_cwd_except()
{
    BOOST_CHECK( !CGLiteAPIWrapper::Instance().Init() );

    CCatalogManager * pCatalog( &CGLiteAPIWrapper::Instance().GetCatalogManager() );
    BOOST_CHECK( pCatalog );

    gaw_lfc_cwd cwd;
    cwd.m_dir = "test_DIR_DOESNT_EXIST_test";
    cout << "---> Changing catalog directory to: " << cwd.m_dir << endl;
    BOOST_CHECK_THROW( pCatalog->Run( cwd ), runtime_error );
}

void test_rmdir()
{
    BOOST_CHECK( !CGLiteAPIWrapper::Instance().Init() );

    CCatalogManager * pCatalog( &CGLiteAPIWrapper::Instance().GetCatalogManager() );
    BOOST_CHECK( pCatalog );

    gaw_lfc_rmdir rmdir;
    rmdir.m_dir = "DDD_TEST_DDD";
    cout << "---> Removing directory: " << rmdir.m_dir << endl;
    BOOST_CHECK_THROW( pCatalog->Run( rmdir ), runtime_error );
}

void test_mkdir_rmdir()
{
    BOOST_CHECK( !CGLiteAPIWrapper::Instance().Init() );

    CCatalogManager * pCatalog( &CGLiteAPIWrapper::Instance().GetCatalogManager() );
    BOOST_CHECK( pCatalog );

    // Creating Directory
    gaw_lfc_mkdir mkdir;
    mkdir.m_dir = "GAW_TEST";
    cout << "---> Creating directory: " << mkdir.m_dir << endl;
    pCatalog->Run( mkdir );

    // Removing Directory
    gaw_lfc_rmdir rmdir;
    rmdir.m_dir = mkdir.m_dir;
    cout << "---> Removing directory: " << rmdir.m_dir << endl;
    pCatalog->Run( rmdir );
}

void test_register_rm()
{
    BOOST_CHECK( !CGLiteAPIWrapper::Instance().Init() );

    CCatalogManager * pCatalog( &CGLiteAPIWrapper::Instance().GetCatalogManager() );
    BOOST_CHECK( pCatalog );

    gaw_lfc_register reg;
    reg.m_file_name = "gaw_file_test.file";
    reg.m_guid = "929C0533-B0C3-4DB7-9A1B-1386F01DDEC3";
    reg.m_SE_server = "grid0.gsi.de";
    reg.m_sfn = "lfn://grid0.gsi.de/test/test3/" + reg.m_file_name;
    cout << "---> Creating catalog file: " << reg.m_file_name << endl;
    pCatalog->Run( reg );

    gaw_lfc_rm rm;
    rm.m_file_name = reg.m_file_name;
    cout << "---> Removing catalog file: " << rm.m_file_name << endl;
    pCatalog->Run( rm );
}

void test_ls()
{
    BOOST_CHECK( !CGLiteAPIWrapper::Instance().Init() );

    CCatalogManager * pCatalog( &CGLiteAPIWrapper::Instance().GetCatalogManager() );
    BOOST_CHECK( pCatalog );

    LFCHelper::LFCFileInfoVector_t file_list;
    gaw_lfc_ls ls;
    ls.m_dir = "/grid/dech";
    pCatalog->Run( ls, &file_list );
    BOOST_CHECK( !file_list.empty() );
}

void test_ls_stream()
{
    BOOST_CHECK( !CGLiteAPIWrapper::Instance().Init() );

    CCatalogManager * pCatalog( &CGLiteAPIWrapper::Instance().GetCatalogManager() );
    BOOST_CHECK( pCatalog );

    gaw_lfc_sls ls;
    ls.m_dir = "/grid/dech";
    cout << "---> Listing directory: " << ls.m_dir << endl;
    pCatalog->Run( ls, &cout );
}

void test_timeout()
{
    BOOST_CHECK( !CGLiteAPIWrapper::Instance().Init() );

    CCatalogManager * pCatalog( &CGLiteAPIWrapper::Instance().GetCatalogManager() );
    BOOST_CHECK( pCatalog );

    gaw_lfc_pwd pwd;
    string sDir;
    pCatalog->Run( pwd, &sDir );

    cout << "---> Timeout for 1.5 min (checking ability to reconnect)... " << endl;
    sleep( 90 ); // 90 seconds timeout
    cout << "---> done timeout" << endl;

    string sDirNew;
    pCatalog->Run( pwd, &sDirNew );

    BOOST_CHECK_EQUAL( sDir, sDirNew );
}
