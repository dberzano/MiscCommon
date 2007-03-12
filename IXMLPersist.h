/************************************************************************/
/**
 * @file IXMLPersist.h
 * @brief IXMLPersist is a persistence interface
 * @author Anar Manafov A.Manafov@gsi.de
 */ /*
 
        version number:    $LastChangedRevision$
        created by:        Anar Manafov
                                    2006-07-15
        last changed by:   $LastChangedBy$ $LastChangedDate$
 
        Copyright (c) 2006 GSI GridTeam. All rights reserved.
************************************************************************/
#ifndef IXMLPERSIST_H
#define IXMLPERSIST_H

// Xerces-C++ headers
#include <xercesc/dom/DOMNode.hpp> 
// GAW
#include "ErrorCode.h"

namespace MiscCommon
{

    /** @brief an XML persistence interface*/
    class IXMLPersist
    {
        public:
            virtual ERRORCODE Read( xercesc::DOMNode* _element ) = 0;
            virtual ERRORCODE Write( xercesc::DOMNode* _element ) = 0;
    };

};

#endif
