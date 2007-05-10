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
    /**
     *  @brief Helpers for Xerces XML parser.
     **/
    namespace XMLHelper
    {
        /**
         * @brief smart-wrapper around XMLCh class
         **/
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
        /**
         * @brief template functions, which helps to retrieve different types of attributes from an XML file.
         * @param _element - [in] XML node to process. Must not be NULL.
         * @param _attr - [in] Name of the attribute to read. Must not be NULL.
         * @param _data - [out] A buffer to keep a return value - value of the attribute. Must not be NULL.
         **/
        template <class _T>
        void get_attr_value( const xercesc::DOMElement *_element, const char *_attr, _T *_data )
        {
            smart_XMLCh attr_name( _attr );
            smart_XMLCh xmlTmpStr( _element->getAttribute( attr_name ) );
            std::istringstream str( xmlTmpStr.ToString() );
            str >> *_data;
        }
        /**
         * @brief specialization of get_attr_value with the bool type -- xml value: true or false
         * @param _element - [in] XML node to process. Must not be NULL.
         * @param _attr - [in] Name of the attribute to read. Must not be NULL.
         * @param _data - [out] A buffer to keep a return value - value of the attribute. Must not be NULL.
         **/
        template <>
        inline void get_attr_value<bool>( const xercesc::DOMElement *_element, const char *_attr, bool *_data )
        {
            smart_XMLCh attr_name( _attr );
            smart_XMLCh xmlTmpStr( _element->getAttribute( attr_name ) );
            std::string str( xmlTmpStr.ToString() );
            MiscCommon::to_lower( str );
            *_data = !( str.empty() || ( "no" == str ) );
        }
        /**
         * @brief specialization of get_attr_value with the std::string type
         * @param _element - [in] XML node to process. Must not be NULL.
         * @param _attr - [in] Name of the attribute to read. Must not be NULL.
         * @param _data - [out] A buffer to keep a return value - value of the attribute. Must not be NULL.
         **/
        template <>
        inline void get_attr_value<std::string>( const xercesc::DOMElement *_element, const char *_attr, std::string *_data )
        {
            smart_XMLCh attr_name( _attr );
            smart_XMLCh xmlTmpStr( _element->getAttribute( attr_name ) );
            *_data = xmlTmpStr.ToString();
        }
        // TODO: Simplify this template by implementing traits
        /**
         * @brief Returns a Node by the given name. (basic template without implementation)         
         **/
        template <class _T>
        inline xercesc::DOMNode* GetSingleNodeByName( const _T *_Val, const std::string &_NodeName );
        /**
         * @brief Returns a Node by the given name. A xercesc::DOMDocument specialization
         * @param ._Doc - [in] XML Document object. Must not be NULL.
         * @param _NodeName - [in] Name of the child node to find.
         * @return pointer to the found XML node or NULL in case of error 
         **/
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
        /**
         * @brief Returns a Node by the given name. A xercesc::DOMNode specialization.
         * @param ._node - [in] XML Node. Must not be NULL.
         * @param _NodeName - [in] Name of the child node to find.
         * @return pointer to the found XML node or NULL in case of error 
         **/
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
        /**
         * @brief A helper template function, which wraps GetSingleNodeByName template-functions.
         * @param ._Node - [in] Could be a pointer to either XML Document or XML Node. Must not be NULL.
         * @param _NodeName - [in] Name of the child node to find.         
         * @exception std::runtime_error "can't find XML element [_NodeName]"
         * @return pointer to the found XML node or an exception will be raised.
         **/
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
