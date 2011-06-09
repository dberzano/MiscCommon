//
//  PoDSysFiles.cpp
//  PoD
//
//  Created by Anar Manafov on 09.06.11.
//  Copyright 2011 GSI. All rights reserved.
//
//=============================================================================
#include "PoDSysFiles.h"
// BOOST
//#include <boost/program_options/options_description.hpp>
//#include <boost/program_options/parsers.hpp>
//#include <boost/program_options/variables_map.hpp>
// STD
#include <sstream>
#include <stdexcept>
#include <fstream>
// MiscCommon
#include "MiscUtils.h"
#include "SysHelper.h"
//=============================================================================
using namespace std;
using namespace MiscCommon;
//namespace bpo = boost::program_options;
//=============================================================================
CEnvironment::CEnvironment():
    m_srvPort( 0 ),
    m_ud( NULL )
{

}
//=============================================================================
CEnvironment::~CEnvironment()
{
    delete m_ud;
}
//=============================================================================
void CEnvironment::init()
{
    char *pod_location;
    pod_location = getenv( "POD_LOCATION" );
    if( NULL == pod_location )
        throw runtime_error( "POD_LOCATION is not defined. Please, initialize PoD environment." );

    m_PoDPath = pod_location;
    smart_path( &m_PoDPath );
    smart_append( &m_PoDPath, '/' );

    // Read PoD User defaults
    string pathUD( PoD::showCurrentPUDFile() );
    smart_path( &pathUD );
    PoD::CPoDUserDefaults user_defaults;
    user_defaults.init( pathUD );
    m_ud = new PoD::SPoDUserDefaultsOptions_t( user_defaults.getOptions() );

    if( !m_ud )
        throw runtime_error( "PoD user defaults is not found." );

    m_wrkDir = m_ud->m_server.m_common.m_workDir;
    smart_path( &m_wrkDir );
    smart_append( &m_wrkDir, '/' );

    _localVersion();
}
//=============================================================================
void CEnvironment::_localVersion()
{
    string version_file_name( m_PoDPath );
    version_file_name += "etc/version";
    ifstream f( version_file_name.c_str() );
    if( !f.is_open() )
        throw runtime_error( "Can't open PoD version file."
                             " Probably PoD installation is broken."
                             " You may want to reinstall PoD to repair the installation." );
    f >> m_localVer;
}
