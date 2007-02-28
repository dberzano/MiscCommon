/************************************************************************/
/**
* @file XMLHelper.h
* @brief XML Helper header
* @author Anar Manafov A.Manafov@gsi.de
*/ /*
 
        version number:    $LastChangedRevision$
        created by:        Anar Manafov
                           2006-07-15
        last changed by:   $LastChangedBy$ $LastChangedDate$
 
        Copyright (c) 2006 GSI GridTeam. All rights reserved.
************************************************************************/
#ifndef XMLHELPER_H
#define XMLHELPER_H

// Xerces-C++ headers
#include <xercesc/util/XMLString.hpp>
#include<xercesc/dom/DOMElement.hpp> 
// STD headers
#include <string>
#include <sstream>

namespace glite_api_wrapper
{
    /** @namespace glite_api_wrapper::XMLHelper
     *  @brief Helpers for Xerces XML parser.
     */
    namespace XMLHelper
    {

        /** @brief smart-wrapper around XMLCh class*/
        class smart_XMLCh
        {
            public:
                XMLCh *m_xmlString;

                smart_XMLCh() : m_xmlString( NULL )
                {}

                smart_XMLCh( const char *_Str )
                {
                    m_xmlString = xercesc::XMLString::transcode( _Str );
                }

                smart_XMLCh( const XMLCh * const _XMLCh )
                {
                    m_xmlString = xercesc::XMLString::replicate( _XMLCh );
                }

                ~smart_XMLCh()
                {
                    Release();
                }

                operator XMLCh*() const
                {
                    return m_xmlString;
                }

                std::string ToString()
                {
                    char * szTmp = xercesc::XMLString::transcode( m_xmlString );
                    std::string strRetVal( szTmp );
                    xercesc::XMLString::release( &szTmp );
                    return strRetVal;
                }

                XMLCh* operator&()
                {
                    return m_xmlString;
                }

                void Release()
                {
                    if ( NULL != m_xmlString )
                        xercesc::XMLString::release( &m_xmlString );
                }
        };

        /** @brief template functions, which helps to retrieve different types of attributes from an XML file*/
        template <typename _T>
        void get_attr_value( const xercesc::DOMElement *_element, const char *_attr, _T *_data )
    {}
        /** @brief instantiation of get_attr_value with the std::string type*/
        template <>
        inline void get_attr_value<std::string>( const xercesc::DOMElement *_element, const char *_attr, std::string *_data )
        {
            smart_XMLCh attr_name( _attr );
            smart_XMLCh xmlTmpStr( _element->getAttribute( attr_name ) );
            *_data = xmlTmpStr.ToString();
        }
        /** @brief instantiation of get_attr_value with the size_t type*/
        template <>
        inline void get_attr_value<size_t>( const xercesc::DOMElement *_element, const char *_attr, size_t *_data )
        {
            smart_XMLCh attr_name( _attr );
            smart_XMLCh xmlTmpStr( _element->getAttribute( attr_name ) );
            std::stringstream str( xmlTmpStr.ToString() );
            str >> *_data;
        }

    };
};

#endif
