/************************************************************************/
/**
 * @file JDLHelper.h
 * @brief The JDLHelper.h header holds a number of helper for JDL operations.
 * @author Anar Manafov A.Manafov@gsi.de
 */ /*

        version number:     $LastChangedRevision$
        created by:         Anar Manafov
                            2007-12-06
        last changed by:    $LastChangedBy$ $LastChangedDate$

        Copyright (c) 2007 GSI GridTeam. All rights reserved.
*************************************************************************/
#ifndef JDLHELPER_H_
#define JDLHELPER_H_

// STD
#include <stdexcept>
//gLite
#include "glite/wmsutils/jobid/JobId.h"
// gLite jdl
#include "glite/jdl/JobAd.h"
#include "glite/jdl/jdl_attributes.h"
#include "glite/jdl/JDLAttributes.h"

namespace MiscCommon
{
    namespace gLite
    {

        const std::string JDL_PARAMETERS("PARAMETERS");


        template<class _T>
        void _get_attr_value( _T *_RetVal, const std::string &_AttrName, glite::jdl::JobAd *_ad )
        {
        }
        template<>
        void _get_attr_value( std::string *_RetVal, const std::string &_AttrName, glite::jdl::JobAd *_ad )
        {
            *_RetVal = _ad->getString( _AttrName );
        }
        template<>
        void _get_attr_value( int *_RetVal, const std::string &_AttrName, glite::jdl::JobAd *_ad )
        {
            *_RetVal = _ad->getInt( _AttrName );
        }

        template<class _T>
        void get_ad_attr( _T *_RetVal, const std::string &_JDL, const std::string &_AttrName ) throw(std::exception)
        {
            if ( !_RetVal )
                throw std::invalid_argument("Return parameter must not be a NULL value.");

            glite::jdl::JobAd ad;
            ad.fromFile( _JDL );

            if ( !ad.hasAttribute(_AttrName) )
                return;
            _get_attr_value( _RetVal, _AttrName, &ad );
        }

        template<class _T>
        void set_ad_attr( const _T &_Value, const std::string &_JDL, const std::string &_attr )
        {
            glite::jdl::JobAd ad;
            ad.fromFile( _JDL );

            if ( !ad.hasAttribute(_attr) )
                return;

            ad.delAttribute( _attr );
            ad.addAttribute( _attr, _Value );
            ad.toFile( _JDL );
        }

    };
};
#endif /*JDLHELPER_H_*/
