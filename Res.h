/************************************************************************/
/**
 * @file Res.h
 * @brief The resources file
 * @author Anar Manafov A.Manafov@gsi.de
 */ /*
 
        version number:    $LastChangedRevision$
        created by:        Anar Manafov
                                    2006-03-25
        last changed by:   $LastChangedBy$ $LastChangedDate$
 
        Copyright (c) 2007 GSI GridTeam. All rights reserved.
************************************************************************/
#ifndef RES_H
#define RES_H

#include "def.h"

namespace MiscCommon
{

    //---------------------- CLog -------------------------------------------
    const LPCSTR g_cszLOG_DATETIME_FRMT( "%Y-%m-%d %H:%M:%S" );         //!< Log Date/Time format
    const size_t LOG_DATETIME_BUFF_LEN( 25 );
    const LPCSTR g_cszLOG_SEVERITY_INFO( "INF" );
    const LPCSTR g_cszLOG_SEVERITY_WARNING( "WRN" );
    const LPCSTR g_cszLOG_SEVERITY_FAULT( "ERR" );
    const LPCSTR g_cszLOG_SEVERITY_CRITICAL_ERROR( "FLT" );
    const LPCSTR g_cszLOG_SEVERITY_DEBUG( "DBG" );

    const LPCSTR g_cszMODULENAME_CORE( "CORE" );
    
};
#endif
