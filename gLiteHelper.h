/************************************************************************/
/**
 * @file gLiteHelper.h
 * @brief
 * @author Anar Manafov A.Manafov@gsi.de
 */ /*

        version number:     $LastChangedRevision$
        created by:         Anar Manafov
                            2007-08-06
        last changed by:    $LastChangedBy:manafov $ $LastChangedDate:2007-08-09 17:37:52 +0200 (Thu, 09 Aug 2007) $

        Copyright (c) 2007 GSI GridTeam. All rights reserved.
*************************************************************************/
#ifndef GLITEHELPER_H_
#define GLITEHELPER_H_

// STD
#include <stdexcept>

// gLite
#include "glite/lb/Job.h"
#include "glite/wmsutils/jobid/JobId.h"
// gLite jdl
#include "glite/jdl/JobAd.h"
#include "glite/jdl/jdl_attributes.h"
#include "glite/jdl/JDLAttributes.h"

// VOMS
#ifdef IOV_MAX
#undef IOV_MAX
#endif
#include "glite/security/voms/voms_api.h"

// MiscCommon
#include "def.h"
#include "BOOST_FILESYSTEM.h"

namespace MiscCommon
{
    namespace gLite
    {

        typedef std::vector<voms> VOMSVector_t;

        class CJobStatusObj
        {
            public:
                CJobStatusObj( const std::string &_gLiteJobID, int _Flag = glite::lb::Job::STAT_CLASSADS |  glite::lb::Job::STAT_CHILDSTAT | glite::lb::Job::STAT_CHILDREN )
                {
                    glite::wmsutils::jobid::JobId jobid( _gLiteJobID );
                    glite::lb::Job job( jobid );
                    m_status = job.status( _Flag );
                }

                glite::lb::JobStatus &Get()
                {
                    return m_status;
                }

                void GetChildren( MiscCommon::StringVector_t *_Container ) const throw (std::exception)
                {
                    if ( !_Container )
                        throw std::invalid_argument( "CJobStatusObj:GetChildren: Argument is NULL." );

                    *_Container = m_status.getValStringList( glite::lb::JobStatus::CHILDREN );
                }

            private:
                glite::lb::JobStatus m_status;
        };

        inline std::string get_globus_url_copy_exe() throw(std::exception)
        {
            // Defining the path to the globus-url-copy executable
            // 1. Trying $GLOBUS_LOCATION
            // 1.1. using  $GLOBUS_LOCAT/bin/globus-url-copy
            // OR
            // 2. if exists using /opt/globus/bin/globus-url-copy
            // OR
            // runtime_error("Unable to find globus-url-copy executable");
            const std::string globurlcp( "globus-url-copy" );
            if ( getenv("GLOBUS_LOCATION") )
                return ( std::string(getenv("GLOBUS_LOCATION")) + "/bin/" + globurlcp );
            else if ( is_directory ("/opt/globus/bin") )
                return ( "/opt/globus/bin/" + globurlcp );
            else
                throw std::runtime_error("Unable to find globus-url-copy executable");
        }
        
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
        
        inline void get_voname_from_jdl( std::string *_RetVal, const std::string &_JDL )
        {
          if ( !_RetVal )
              throw std::invalid_argument("Return parameter must not be a NULL value.");
          
          glite::jdl::JobAd ad;
          ad.fromFile( _JDL );
          // checking VIRTUAL_ORGANISATION
          if ( !ad.hasAttribute(glite::jdl::JDL::VIRTUAL_ORGANISATION) )
              return;
          
          std::string voname( ad.getString (glite::jdl::JDL::VIRTUAL_ORGANISATION) );
          _RetVal->swap( voname );          
        }

    };
};


#endif /*GLITEHELPER_H_*/
