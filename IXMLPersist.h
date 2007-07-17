/************************************************************************/
/**
 * @file IXMLPersist.h
 * @brief IXMLPersist is a persistence interface.
 * @author Anar Manafov A.Manafov@gsi.de
 */ /*
 
        version number:   $LastChangedRevision$
        created by:          Anar Manafov
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

    /**
     * @brief an XML persistence interface
     **/
    class IXMLPersist
    {
        public:
            /**
             * @brief Read data from the selected xml node.
             * @param _element - [in] xml node to read
             * @return MiscCommon::erXMLNullNode if \b _element is NULL and MiscCommon::erOK on success.
             **/
            virtual ERRORCODE Read( xercesc::DOMNode* _element ) = 0;
            /**
             * @brief Write data to the selected xml node.
             * @param _element - [in] xml node to read
             * @return MiscCommon::erXMLNullNode if \b _element is NULL, MiscCommon::erError in case of error and MiscCommon::erOK on success.
             **/
            virtual ERRORCODE Write( xercesc::DOMNode* _element ) = 0;
    };

    template <class _T>
    struct IXMLPersistImpl
    { 
        void Read( xercesc::DOMNode* _element )
        {
            _T *pThis = reinterpret_cast<_T*>( this );
            pThis->ReadXmlCfg( _element );
        }
        void Write( xercesc::DOMNode* _element )
        {
            _T *pThis = reinterpret_cast<_T*>( this );
            pThis->WriteXmlCfg( _element );
        }
    };
};

#endif
