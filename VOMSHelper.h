/************************************************************************/
/**
 * @file VOMSHelper.h
 * @brief VOMS operations helpers
 * @author Anar Manafov A.Manafov@gsi.de
 */ /*

        version number:     $LastChangedRevision$
        created by:         Anar Manafov
                            2007-11-06
        last changed by:    $LastChangedBy$ $LastChangedDate$

        Copyright (c) 2007 GSI GridTeam. All rights reserved.
*************************************************************************/
#ifndef VOMSHELPER_H_
#define VOMSHELPER_H_

// STD
#include <stdexcept>
#include <algorithm>
// VOMS
#ifdef IOV_MAX
#undef IOV_MAX
#endif
#include "glite/security/voms/voms_api.h"
// MiscCommon
#include "def.h"

namespace MiscCommon
{
    /**
     *
     * @brief A gLite helpers workspace
     *
     */
    namespace gLite
    {
        /**
         *
         * @brief The ::VOMSVector_t type represents a container of VOMSes.
         *
         */
        typedef std::vector<voms> VOMSVector_t;
        /**
         *
         * @brief The SGetName functor helps to retrieve the voname member of the voms structure.
         *
         */
        struct SGetName
        {
            std::string operator()( const voms &_voms ) const
            {
                return _voms.voname;
            }
        };
        /**
         *
         * @brief The get_voname() function retrieves names of VOs found in user's proxy
         * @param[in,out] _RetVal - VOMSVector_t vector filled by names of found VOs
         * @param[in] _voms_dir - This is the directory where the VOMS server' certificates are kept.
         * If this value is empty (default), then the value of $X509 VOMS DIR is considered,
         * and if this is also empty than its default is /etc/grid-security/vomsdir.
         * @param[in] _cert_dir - This is the directory where the CA certificates are kept.
         * If this value is empty (default), then the value of $X509 CERT DIR is considered, and if
         * this is also empty than its default is /etc/grid-security/certificate
         * @exception std::invalid_argument - thrown when _RetVal is NULL.
         * @exception std::runtime_error - thrown when user proxy doesn't have a valid VOMS extension.
         * @return no return value
         *
         */
        inline void get_voname( MiscCommon::StringVector_t *_RetVal,
                                const std::string &_voms_dir = "", const std::string &_cert_dir = "" ) throw( std::exception )
        {
            if( !_RetVal )
                throw std::invalid_argument( "Container must not be a NULL value." );

            // Getting VOMS name
            vomsdata data( _voms_dir, _cert_dir );
            data.RetrieveFromProxy( RECURSE_CHAIN );

            if( data.data.empty() )
                throw std::runtime_error( "User's proxy doesn't have a valid VOMS extension." );

            std::transform( data.data.begin(), data.data.end(), std::back_inserter( *_RetVal ), SGetName() );
        }

    };
};


#endif /*VOMSHELPER_H_*/
