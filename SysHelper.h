#ifndef SYSHELPER_H_
#define SYSHELPER_H_

// API
#include <unistd.h>

// OUR
#include "def.h"

inline void get_cur_user( std::string *_RetVal )
{
    if ( !_RetVal )
        return ;

    CHARVector_t Buf(L_cuserid);

    cuserid( &Buf[0] );

    *_RetVal = std::string( &Buf[0] );
}

#endif /*SYSHELPER_H_*/
