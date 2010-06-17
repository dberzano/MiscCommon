/************************************************************************/
/**
 * @file Test_JobsContainer.cpp
 * @brief Unit tests of JobsContainer
 * @author Anar Manafov A.Manafov@gsi.de
 */ /*

        version number:     $LastChangedRevision$
        created by:         Anar Manafov
                            2007-03-10
        last changed by:    $LastChangedBy$ $LastChangedDate$

        Copyright (c) 2007 GSI GridTeam. All rights reserved.
*************************************************************************/
// BOOST: tests
#include <boost/test/unit_test.hpp>
using boost::unit_test::test_suite;

// STD
#include <string>
#include <iostream>

// GAW
#include "JobsContainer.h"

using namespace std;
using namespace glite_api_wrapper;

void test_container_add();
void test_container_remove();
void test_container_clear();

test_suite* init_unit_test_suite( int, char* [] )
{
    test_suite * test = BOOST_TEST_SUITE( "Unit tests of GAW JobsContainer" );

    test->add( BOOST_TEST_CASE( &test_container_add ), 0 );
    test->add( BOOST_TEST_CASE( &test_container_remove ), 0 );
    test->add( BOOST_TEST_CASE( &test_container_clear ), 0 );

    return test;
}

void test_container_add()
{
    CJobsContainer jc;
    jc.Add( "https://test1/test1" );
    jc.Add( "https://test2/test2" );
    jc.Add( "https://test3/test3" );

    cout << jc << endl;
    BOOST_CHECK( 3 == jc.GetContainer().size() );
}

void test_container_remove()
{
    CJobsContainer jc;
    jc.Add( "https://test1/test1" );
    jc.Add( "https://test2/test2" );
    jc.Add( "https://test3/test3" );

    jc.Remove( "https://test2/test2" );

    cout << jc << endl;
    BOOST_CHECK( 2 == jc.GetContainer().size() );
}

void test_container_clear()
{
    CJobsContainer jc;
    jc.Add( "https://test1/test1" );
    jc.Add( "https://test2/test2" );
    jc.Add( "https://test3/test3" );

    jc.ClearAll();

    cout << jc << endl;
    BOOST_CHECK( jc.GetContainer().empty() );
}
