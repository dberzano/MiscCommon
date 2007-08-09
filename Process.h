/************************************************************************/
/**
 * @file Process.h
 * @brief This header contains a subset of helpers for Process, Daemon and PID file operations.
 * @author Anar Manafov A.Manafov@gsi.de
 *//*

        version number:     $LastChangedRevision:769 $
        created by:         Anar Manafov
                            2007-04-12
        last changed by:    $LastChangedBy$ $LastChangedDate$

        Copyright (c) 2007 GSI GridTeam. All rights reserved.
*************************************************************************/
#ifndef PROCESS_H_
#define PROCESS_H_

// API
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <dirent.h>

// STD
#include <fstream>
#include <set>
#include <map>
#include <sstream>
#include <stdexcept>

// POSIX regexp
#include <regex.h>

// OUR
#include "def.h"
#include "ErrorCode.h"
#include "MiscUtils.h"
#include "CustomIterator.h"

namespace MiscCommon
{
    /**
     * @brief The function checks, whether the process which corresponds to the given \b _PID can be found.
     * @param _PID - [in] process ID to look for.
     * @return \b true when the process is found, otherwise return value is \b false.
     **/
    inline bool IsProcessExist(pid_t _PID)
    {
        return !(::kill( _PID, 0 ) == -1 && errno == ESRCH );
    }

    /**
     * @brief A PID-file helper
     **/
    class CPIDFile
    {
        public:
            CPIDFile( const std::string &_FileName, pid_t _PID ): m_FileName(_FileName)
            {
                if ( !_FileName.empty() && _PID > 0 )
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
                else
                    m_FileName.clear();
            }

            ~CPIDFile()
            {
                if ( !m_FileName.empty() )
                    ::unlink( m_FileName.c_str() );
            }

            static pid_t GetPIDFromFile( const std::string &_FileName )
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

    /**
     * @brief This class is used to quarry a list of currently running processes
     * @note Usage: In the example container pids will be containing pids of currently running processes
     * @code
     CProcList::ProcContainer_t pids;
     CProcList::GetProcList( &pids );
     * @endcode
     **/
    class CProcList
    {
        public:
            typedef std::set<pid_t> ProcContainer_t;

        public:
            static void GetProcList( ProcContainer_t *_Procs )
            {
                if ( !_Procs )
                    throw std::invalid_argument("CProcList::GetProcList: Input container is NULL");

                _Procs->clear();

                struct dirent **namelist;
                // scanning the "/proc" filesystem
                int n = scandir("/proc", &namelist, CheckDigit, alphasort);

                if ( -1 == n )
                    throw std::runtime_error("CProcList::GetProcList: " + errno2str() );
                if ( 0 == n )
                    return; // there were no files

                for (int i = 0; i < n; ++i)
                {
                    std::stringstream ss( namelist[i]->d_name );
                    pid_t pid;
                    ss >> pid;
                    _Procs->insert( pid );
                    free(namelist[i]);
                }

                free(namelist);
            }

        private:
            static int CheckDigit( const struct dirent* _d )
            {
                const std::string sName( _d->d_name );
                // Checking whether file name has all digits
                return ( sName.end() == std::find_if( sName.begin(), sName.end(), std::not1(IsDigit()) ) );
            }
    };

    /**
     * @brief This class helps to retrieve process's information from /proc/<pid>/status
     * @note Usage:
     * @code
     CProcStatus p;
     p.Open( 8007 );
     cout << "Name" << p.GetValue( "Name" ) << endl;
     cout << "PPid" << p.GetValue( "PPid" ) << endl;
     p.Open( 1 );
     cout << "Name" << p.GetValue( "Name" ) << endl;
     cout << "PPid" << p.GetValue( "PPid" ) << endl;
     * @endcode
     **/
    class CProcStatus
    {
            typedef std::auto_ptr<std::ifstream> ifstream_ptr;
            typedef std::map<std::string, std::string> keyvalue_t;

            struct SGetValues
            {
                SGetValues( CProcStatus *_pThis ): m_pThis(_pThis)
                {}
                bool operator()( const std::string &_sVal )
                {
                    regmatch_t PMatch[3];
                    if ( 0 != regexec( &m_pThis->m_re, _sVal.c_str(), 3, PMatch, 0) )
                        return false;
                    const std::string sKey( _sVal.c_str() + PMatch[1].rm_so, PMatch[1].rm_eo - PMatch[1].rm_so );
                    std::string sValue( _sVal.c_str() + PMatch[2].rm_so, PMatch[2].rm_eo - PMatch[2].rm_so );
                    // TODO: make lowcase _KeyName
                    trim<std::string>( &sValue, "\t" );
                    trim<std::string>( &sValue, " " );
                    m_pThis->m_values.insert( std::make_pair(sKey, sValue) );
                    return true;
                }
               private:
                CProcStatus *m_pThis;
            };

        public:
            CProcStatus()
            {
                // Preparing regular expression pattern
                regcomp( &m_re, "(.*):(.*)", REG_EXTENDED );
            }
            ~CProcStatus()
            {
                regfree( &m_re );
            }

            void Open( pid_t _PId )
            {
                m_values.clear();
                if ( m_f.get() )
                    m_f->close();

                std::stringstream ss;
                ss
                << "/proc/"
                << _PId
                << "/status";
                m_f = ifstream_ptr( new std::ifstream( ss.str().c_str() ) );
                // create reader objects

                std::vector<std::string> vec;
                std::copy(custom_istream_iterator<std::string>(*m_f),
                          custom_istream_iterator<std::string>(),
                          std::back_inserter(vec));

                SGetValues val(this);
                for_each( vec.begin(), vec.end(), val );
            }

            std::string GetValue( const std::string &_KeyName ) const
            {
                // TODO: make lowcase _KeyName
                keyvalue_t::const_iterator iter = m_values.find(_KeyName);
                return( m_values.end() == iter ? std::string() : iter->second );
            }

        private:
            ifstream_ptr m_f;
            regex_t m_re;
            keyvalue_t m_values;
    };

    static sig_atomic_t g_handled_sign = false;
    static sig_atomic_t g_child_status = 0;
    /**
     * handles the signal returned by the child of the process, sets handled_sign at true
     * @int sign, the signal
     */
    static void childSignalHandler ( int sign )
    {
        if ( sign == SIGCHLD)
        {
            g_handled_sign = true;
            ::wait( &g_child_status );
        }
    }

    inline bool is_status_ok(int status)
    {
        return WIFEXITED(status) && WEXITSTATUS(status) == 0;
    }

    //TODO: Document me!
    inline void do_execv( const std::string &_Command, const StringVector_t &_Params, size_t _Delay ) throw (std::exception)
    {
        g_handled_sign = false;
        g_child_status = 0;
        signal ( SIGCHLD, childSignalHandler);

        pid_t child_pid;
        std::vector<const char*> cargs; //careful with c_str()!!!
        cargs.push_back( _Command.c_str() );
        StringVector_t::const_iterator iter = _Params.begin();
        StringVector_t::const_iterator iter_end = _Params.end();
        for (; iter != iter_end; ++iter )
            cargs.push_back( iter->c_str() );
        cargs.push_back(0);

        switch ( child_pid = fork() )
        {
            case - 1:
                // Unable to fork
                throw std::runtime_error( "do_execv: Unable to fork process" );

            case 0:
                // child: execute the required command, on success does not return
                execv ( _Command.c_str(), const_cast<char **>(&cargs[0]) ); // TODO: duplicates the std.out and std.err of the command into two files and report content of streams in error msg.
                ::exit( 1 );
        }

        //parent
        for ( size_t i = 0; i < _Delay; ++i )
        {
            if ( !IsProcessExist(child_pid) )
            {
                if ( g_handled_sign && !is_status_ok(g_child_status) )
                {
                    std::stringstream ss;
                    ss << "do_execv: Can't execute \"" << _Command << "\" with parameters: ";
                    StringVector_t::const_iterator iter = _Params.begin();
                    StringVector_t::const_iterator iter_end = _Params.end();
                    for (; iter != iter_end; ++iter )
                        ss << "\"" << *iter << "\" ";
                    throw std::runtime_error( ss.str() );
                }
                return;
            }
            sleep( 1 ); //TODO: Needs to be fixed! Implement time-function based timeout measurements instead
        }
        throw std::runtime_error("do_execv: Timeout has been reached, command execution will be terminated now." );
        //kills the child
        kill( child_pid, SIGKILL );
    }

};

#endif /*PROCESS_H_*/
