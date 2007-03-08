/************************************************************************/
/**
 * @file ErrorCode.h
 * @brief error cods collection file
 * @author Anar Manafov A.Manafov@gsi.de
 */ /*
 
        version number:    $LastChangedRevision$
        created by:        Anar Manafov
                           2006-05-11
        last changed by:   $LastChangedBy$ $LastChangedDate$
 
        Copyright (c) 2006 GSI GridTeam. All rights reserved.
************************************************************************/
#ifndef ERRORCODE_H
#define ERRORCODE_H

// API
#include <errno.h>

// STD
#include <string>

namespace MiscCommon
{

    typedef unsigned int ERRORCODE;
    typedef const ERRORCODE ERRORCODE_C;

    ERRORCODE_C erOK( 0 );
    ERRORCODE_C erError( 1 );
    ERRORCODE_C erNotImpl( 3 );

    // General Error codes
    ERRORCODE_C BASE_FOR_GENERAL_ERR( 50 ); // <<<----- BASE counter
    ERRORCODE_C erNULLArg( BASE_FOR_GENERAL_ERR + 1 );
    ERRORCODE_C erFILE_NOT_FOUND( BASE_FOR_GENERAL_ERR + 2 );

    // XML Error codes
    ERRORCODE_C BASE_FOR_XML_ERR( 100 ); // <<<----- BASE counter
    ERRORCODE_C erXMLInit( BASE_FOR_XML_ERR + 1 );
    ERRORCODE_C erXMLReadConfig( BASE_FOR_XML_ERR + 2 );
    ERRORCODE_C erXMLNullNode( BASE_FOR_XML_ERR + 3 );

      /**
     * retrieves error string
     * @param _msg - [out] an error message will be written to this buffer, must not be NULL.
     * @return
       */
    inline ERRORCODE errno2str( std::string *_msg )
    {
        if ( !_msg )
            return erNULLArg;
        char *p = strerror( errno );
        *_msg = p;
        return erOK;
    }


};

#endif
