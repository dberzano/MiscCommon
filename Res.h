/************************************************************************/
/**
 * @file Res.h
 * @brief The resources file
 * @author Anar Manafov A.Manafov@gsi.de
 */ /*
 
        version number:    $LastChangedRevision: 333 $
        created by:        Anar Manafov
                           2006-03-25
        last changed by:   $LastChangedBy: manafov $ $LastChangedDate: 2006-11-24 18:14:05 +0100 (Fri, 24 Nov 2006) $
 
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
    const LPCSTR g_cszLOG_SEVERITY_INFO( "INFO" );
    const LPCSTR g_cszLOG_SEVERITY_WARNING( "WARNING" );
    const LPCSTR g_cszLOG_SEVERITY_FAULT( "FAULT" );
    const LPCSTR g_cszLOG_SEVERITY_CRITICAL_ERROR( "CRITICAL_ERROR" );
    const LPCSTR g_cszLOG_SEVERITY_DEBUG( "DEBUG" );

    const LPCSTR g_cszMODULENAME_CORE( "CORE" );
    
};
#endif
