/************************************************************************/
/**
* @file XMLHelper.h
* @brief XML Helper header
* @author Anar Manafov A.Manafov@gsi.de
*/ /*
 
        version number:   $LastChangedRevision$
        created by:          Anar Manafov
                                  2006-07-15
        last changed by:   $LastChangedBy$ $LastChangedDate$
 
        Copyright (c) 2006, 2007 GSI GridTeam. All rights reserved.
************************************************************************/
#ifndef XMLHELPER_H
#define XMLHELPER_H

// Xerces-C++ headers
#include <xercesc/util/XMLString.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/DOM.hpp>

// STD headers
#include <string>
#include <sstream>

// Our
#include "MiscUtils.h"

namespace MiscCommon
{
    /** @namespace MiscCommon::XMLHelper
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
        template <class _T>
        void get_attr_value( const xercesc::DOMElement *_element, const char *_attr, _T *_data )
        {
            smart_XMLCh attr_name( _attr );
            smart_XMLCh xmlTmpStr( _element->getAttribute( attr_name ) );
            std::istringstream str( xmlTmpStr.ToString() );
            str >> *_data;
        }
        /** @brief instantiation of get_attr_value with the bool type -- xml value: true or false*/
        template <>
        inline void get_attr_value<bool>( const xercesc::DOMElement *_element, const char *_attr, bool *_data )
        {
            smart_XMLCh attr_name( _attr );
            smart_XMLCh xmlTmpStr( _element->getAttribute( attr_name ) );
            std::string str( xmlTmpStr.ToString() );
            MiscCommon::to_lower( str );
            *_data = !( str.empty() || ( "no" == str ) );
        }
        template <>
        inline void get_attr_value<std::string>( const xercesc::DOMElement *_element, const char *_attr, std::string *_data )
        {
            smart_XMLCh attr_name( _attr );
            smart_XMLCh xmlTmpStr( _element->getAttribute( attr_name ) );
            *_data = xmlTmpStr.ToString();
        }

        // TODO: Simplify this template by implementing traits
        template <class _T>
        inline xercesc::DOMNode* GetSingleNodeByName( const _T *_Val, const std::string &_NodeName );

        template <>
        inline xercesc::DOMNode* GetSingleNodeByName( const xercesc::DOMDocument *_Doc, const std::string &_NodeName )
        {
            if ( !_Doc )
                return NULL;
            const smart_XMLCh ElementName( _NodeName.c_str() );

            const xercesc::DOMNodeList *list = _Doc->getElementsByTagName( ElementName );
            if ( !list )
                return NULL;
            return list->item( 0 );
        }

        template <>
        inline xercesc::DOMNode* GetSingleNodeByName( const xercesc::DOMNode *_node, const std::string &_NodeName )
        {
            if ( !_node )
                return NULL;
            const smart_XMLCh ElementName( _NodeName.c_str() );

            const xercesc::DOMElement* element( NULL );
            if ( xercesc::DOMNode::ELEMENT_NODE == _node->getNodeType() )
                element = dynamic_cast< const xercesc::DOMElement* >( _node ) ;
            else
                return NULL;

            const xercesc::DOMNodeList *list = element->getElementsByTagName( ElementName );
            if ( !list )
                return NULL;
            return list->item( 0 );
        }

        template < class _T>
        inline xercesc::DOMNode* GetSingleNodeByName_Ex( const _T *_Node, const std::string &_NodeName ) throw(std::exception)
        {
            xercesc::DOMNode *node = GetSingleNodeByName( _Node, _NodeName.c_str() );
            if ( !node )
                throw( std::runtime_error( "can't find XML element \"" + _NodeName + "\"" ) );
            return node;
        }

    };
};

#endif
