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

// MiscCommon
#include "ErrorCode.h"
#include "XMLHelper.h"

namespace MiscCommon
{

    /**
     * @brief an XML persistence interface
     **/
    class IXMLPersist
    {
        public:
            virtual ~IXMLPersist()
            {}
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


    // TODO: Document me!
#define DECLARE_XMLPERSIST_IMPL(_T) friend class MiscCommon::IXMLPersistImpl<_T>;
    
    // TODO: Document me!
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
    
    // TODO: Document me!
#define BEGIN_READ_XML_CFG(_T) \
    void ReadXmlCfg( xercesc::DOMNode* _element ) \
    {   \
        const std::string str( "An internal error has been detected. Can't read configuration of "+ std::string(#_T) + " manager, " ); \
        if ( !_element )    \
            throw std::invalid_argument( str + "DOMNode is NULL." );  \
        MiscCommon::XMLHelper::smart_XMLCh ElementName( "config" );    \
        xercesc::DOMElement *config_element( dynamic_cast<xercesc::DOMElement* >( _element ) );   \
        if ( !config_element )  \
            throw std::runtime_error( str + "element \"config\" is missing" );    \
        xercesc::DOMNodeList *list( config_element->getElementsByTagName( ElementName ) );   \
        if( !list ) \
            throw std::runtime_error( str + "element \"config\" is missing" );    \
        xercesc::DOMNode* node( list->item( 0 ) );   \
        if( !node ) \
             throw std::runtime_error( str + "element \"config\" is missing" );    \
        xercesc::DOMElement* elementConfig( NULL );  \
        if ( xercesc::DOMNode::ELEMENT_NODE == node->getNodeType() ) \
            elementConfig = dynamic_cast< xercesc::DOMElement* >( node ) ;   \
        if ( !elementConfig )   \
            throw std::runtime_error( str + "empty XML document" );
    
#define READ_ELEMENT( ELEMENT_NAME, VAR ) MiscCommon::XMLHelper::get_attr_value( elementConfig, ELEMENT_NAME, &VAR );

#define END_READ_XML_CFG }
    
#define BEGIN_WRITE_XML_CFG(_T) void WriteXmlCfg( xercesc::DOMNode* _element ) {
#define END_WRITE_XML_CFG }

    
};

#endif
