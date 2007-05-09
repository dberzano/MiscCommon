/************************************************************************/
/**
 * @file SysHelper.h
 * @brief $$File comment$$
 * @author Anar Manafov A.Manafov@gsi.de
 */ /*
 
        version number:   $LastChangedRevision$
        created by:          Anar Manafov
                                  2007-04-01
        last changed by:   $LastChangedBy$ $LastChangedDate$
 
        Copyright (c) 2007 GSI GridTeam. All rights reserved.
*************************************************************************/
#ifndef SYSHELPER_H_
#define SYSHELPER_H_

// API
#include <unistd.h>
#include <pwd.h>
#include <netdb.h>

// OUR
#include "def.h"
namespace MiscCommon
{

    inline void get_cuser_name( std::string *_RetVal )
    {
        if ( !_RetVal )
            return ;

        CHARVector_t Buf(L_cuserid);

        cuserid( &Buf[0] );

        *_RetVal = std::string( &Buf[0] );
    }

    inline void get_homedir( uid_t _uid, std::string *_RetVal )
    {
        if ( !_RetVal )
            return ;

        passwd *pwd = getpwuid( _uid );
        *_RetVal = pwd ? std::string( pwd->pw_dir) : "";
    }

    inline void get_cuser_homedir( std::string *_RetVal )
    {
        get_homedir( getuid(), _RetVal );
    }

    inline void smart_homedir_append( std::string *_Path )
    {
        if ( !_Path )
            return ;

        std::string sHome;
        get_cuser_homedir( &sHome );
        smart_append( &sHome, '/');

        replace<std::string>( _Path, "~/", sHome );
        replace<std::string>( _Path, "$HOME/", sHome );
    }

    inline void get_hostname( std::string *_RetVal )
    {
        if ( !_RetVal )
            return ;

        // getting host name - which is without domain name
        CHARVector_t Buf( HOST_NAME_MAX );
        gethostname( &Buf[0], Buf.capacity() );

        // getting host name with FCDN
        hostent *h = gethostbyname( std::string(&Buf[0]).c_str() );
        if ( !h )
            return ;

        *_RetVal = h->h_name;
    }

};
#endif /*SYSHELPER_H_*/
