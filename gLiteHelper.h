/************************************************************************/
/**
 * @file gLiteHelper.h
 * @brief
 * @author Anar Manafov A.Manafov@gsi.de
 */ /*

        version number:   $LastChangedRevision:769 $
        created by:          Anar Manafov
                                  2007-08-06
        last changed by:   $LastChangedBy$ $LastChangedDate$

        Copyright (c) 2007 GSI GridTeam. All rights reserved.
*************************************************************************/
#ifndef GLITEHELPER_H_
#define GLITEHELPER_H_

// gLite
#include "glite/lb/Job.h"
#include "glite/wmsutils/jobid/JobId.h"
// MiscCommon
#include "def.h"

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

        void GetChildren( MiscCommon::StringVector_t *_Container ) throw (std::exception)
        {
            if ( !_Container )
                throw std::invalid_argument( "CJobStatusObj:GetChildren: Argument is NULL." );
                
            *_Container = m_status.getValStringList( glite::lb::JobStatus::CHILDREN );
        }

    private:
        glite::lb::JobStatus m_status;
};

#endif /*GLITEHELPER_H_*/
