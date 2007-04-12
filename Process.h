/************************************************************************/
/**
 * @file Process.h
 * @brief $$File comment$$
 * @author Anar Manafov A.Manafov@gsi.de
 */ /*
 
        version number:     $LastChangedRevision$
        created by:            Anar Manafov
                                    2007-04-12
        last changed by:   $LastChangedBy$ $LastChangedDate$
 
        Copyright (c) 2007 GSI GridTeam. All rights reserved.
*************************************************************************/
#ifndef PROCESS_H_
#define PROCESS_H_

// API
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

// STD
#include <fstream>

// OUR
#include "def.h"

namespace MiscCommon
{
    /// Whether process can be found corresponding to _PID
    inline bool IsProcessExist( pid_t _PID )
    {
        return !( ::kill( _PID, 0 ) == -1 && errno == ESRCH );
    }

    /// pidfile helper
    class CPIDFile
    {
        public:
            CPIDFile( const std::string &_FileName, pid_t _PID ): m_FileName(_FileName)
            {
                // Preventing to start a second "instance" if the pidfile references to the running process
                const pid_t pid = GetPIDFromFile(m_FileName);
                if ( pid > 0 && IsProcessExist( pid ) )
                {
                    // We don't want to unlink this file
                    m_FileName.clear();
                    throw std::runtime_error("Error creating pidfile. The process corresponding to pidfile \"" + _FileName + "\" is still running");
                }

                // Wrtiting new pidfile
                std::ofstream f( m_FileName.c_str() );
                if ( !f. is_open() )
                    throw std::runtime_error( "can't create PID file: " + m_FileName );

                f << _PID;
            }

            ~CPIDFile()
            {
                ::unlink( m_FileName.c_str() );
            }

            pid_t GetPIDFromFile( const std::string &_FileName ) const
            {
                std::ifstream f( _FileName.c_str() );
                if ( !f. is_open() )
                    return 0;

                pid_t pid( 0 );
                f >> pid;

                return pid;
            }

        private:
            std::string m_FileName;
    };

};

#endif /*PROCESS_H_*/
