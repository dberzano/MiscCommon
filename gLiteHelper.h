/************************************************************************/
/**
 * @file gLiteHelper.h
 * @brief The gLiteHelper.h header holds a number of helper for gLite API.
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

// gLite
#include "glite/lb/Job.h"
// MiscCommon
#include "FindCfgFile.h"

namespace MiscCommon
{
    namespace gLite
    {
        /**
         *
         * @brief The CJobStatusObj class helps to work with gLite job statuses - Status object.
         *
         */
        class CJobStatusObj
        {
            public:
                /**
                 *
                 * @brief A constructor of the CJobStatusObj class.
                 * @param[in] _gLiteJobID - an ID of job, which status information you want to get.
                 * @param[in] _Flag - flag (see: glite/lb/Job.h) [default: STAT_CLASSADS | STAT_CHILDSTAT | STAT_CHILDREN].
                 *
                 */
                CJobStatusObj( const std::string &_gLiteJobID,
                               int _Flag = glite::lb::Job::STAT_CLASSADS | glite::lb::Job::STAT_CHILDSTAT | glite::lb::Job::STAT_CHILDREN )
                {
                    glite::wmsutils::jobid::JobId jobid( _gLiteJobID );
                    glite::lb::Job job( jobid );
                    m_status = job.status( _Flag );
                }
                /**
                 *
                 * @brief getting the status object.
                 * @return a reference to the status object.
                 *
                 */
                glite::lb::JobStatus &Get()
                {
                    return m_status;
                }
                /**
                 *
                 * @brief The GetChildren() method gets values string list of children.
                 * @param[in,out] _Container - the given container to fill with children values.
                 * @exception std::invalid_argument - thrown if _Container is a NULL value
                 * @return no return value
                 *
                 */
                void GetChildren( MiscCommon::StringVector_t *_Container ) const throw (std::exception)
                {
                    if ( !_Container )
                        throw std::invalid_argument( "CJobStatusObj:GetChildren: Argument is NULL." );

                    *_Container = m_status.getValStringList( glite::lb::JobStatus::CHILDREN );
                }

            private:
                glite::lb::JobStatus m_status;
        };
        /**
         *
         * @brief The get_globus_url_copy_exe() function gets full path of globus-url-copy executable.
         * @exception std::runtime_error - thrown if algorithm is unable to find globus-url-copy executable.
         * @return of full path to globus-url-copy executable.
         * @note Defining the path to the globus-url-copy executable in the following order
         *  - 1. $GLOBUS_LOCAT/bin/globus-url-copy
         *  - 2. /opt/globus/bin/globus-url-copy
         *
         */
        inline std::string get_globus_url_copy_exe() throw(std::exception)
        {
            CFindCfgFile<std::string> globurlcp;

            globurlcp.SetOrder
            ("$GLOBUS_LOCATION/bin/globus-url-copy")
            ("/opt/globus/bin/globus-url-copy");

            std::string val;
            globurlcp.GetCfg( &val );
            if ( val.empty() )
                throw std::runtime_error("Unable to find globus-url-copy executable");
            return val;
        }

    };
};


#endif /*GLITEHELPER_H_*/
