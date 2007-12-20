/************************************************************************/
/**
 * @file VOMSHelper.h
 * @brief
 * @author Anar Manafov A.Manafov@gsi.de
 */ /*

        version number:     $LastChangedRevision$
        created by:         Anar Manafov
                            2007-11-06
        last changed by:    $LastChangedBy:manafov $ $LastChangedDate:2007-08-09 17:37:52 +0200 (Thu, 09 Aug 2007) $

        Copyright (c) 2007 GSI GridTeam. All rights reserved.
*************************************************************************/
#ifndef VOMSHELPER_H_
#define VOMSHELPER_H_

// STD
#include <stdexcept>
// VOMS
#ifdef IOV_MAX
#undef IOV_MAX
#endif
#include "glite/security/voms/voms_api.h"

// MiscCommon
#include "def.h"

namespace MiscCommon
{
    /// A gLite helpers workspace
    namespace gLite
    {

        typedef std::vector<voms> VOMSVector_t;

        struct SGetName
        {
            std::string operator() (const voms &_voms) const
            {
                return _voms.voname;
            }
        };

        // TODO: write doxygen doc for the method
        // _voms_dir This is the directory where the VOMS server' certificates are kept.
        // If this value is empty (the default), then the value of $X509 VOMS DIR is considered,
        // and if this is also empty than its default is /etc/grid-security/vomsdir.
        // cert dir This is the directory where the CA certificates are kept. If this value
        // is empty (the default), then the value of $X509 CERT DIR is considered, and if
        // this is also empty than its default is /etc/grid-security/certificate
        inline void get_voname( MiscCommon::StringVector_t *_RetVal,
                                const std::string &_voms_dir = "", const std::string &_cert_dir = "" ) throw (std::exception)
        {
            if ( !_RetVal )
                throw std::invalid_argument("Container must not be a NULL value.");

            // Getting VOMS name
            vomsdata data( _voms_dir, _cert_dir );
            data.RetrieveFromProxy( RECURSE_CHAIN );

            if ( data.data.empty() )
                throw std::runtime_error("User proxy doesn't have a valid VOMS extension.");

            std::transform( data.data.begin(), data.data.end(), std::back_inserter(*_RetVal), SGetName() );
        }

    };
};


#endif /*VOMSHELPER_H_*/
