/************************************************************************/
/**
 * @file TimeoutGuard.h
 * @brief $$File comment$$
 * @author Anar Manafov A.Manafov@gsi.de
 */ /*
 
        version number:    $LastChangedRevision: 641 $
        created by:            Anar Manafov
                                    2007-04-17
        last changed by:   $LastChangedBy: manafov $ $LastChangedDate: 2007-04-16 13:57:38 +0200 (Mon, 16 Apr 2007) $
 
        Copyright (c) 2007 GSI GridTeam. All rights reserved.
*************************************************************************/
#ifndef TIMEOUTGUARD_H_
#define TIMEOUTGUARD_H_

#include "Process.h"
#include "BOOSTHelper.h"

namespace MiscCommon
{

    /// CTimeoutGuard - sends SEGTERM signal to the specified PID after timeout period is over
    class CTimeoutGuard
    {
            CTimeoutGuard():
                    m_IsInit(false),
                    m_pid(0),
                    m_secTimeOut(0)
            {}
            ~CTimeoutGuard()
            {}

        public:
            void Init( pid_t _pid, size_t _timeout )
            {
                if ( m_IsInit )
                    throw std::logic_error( "CTimeoutGuard is already initialized" );

                m_pid = _pid;
                m_secTimeOut = _timeout;
                m_IsInit = true;
                m_Thread = Thread_PTR_t( new boost::thread(
                                             boost::bind( &CTimeoutGuard::ThreadWorker, this ) ) );
            }
            static CTimeoutGuard& Instance()
            {
                static CTimeoutGuard obj;
                return obj;
            }
            void ThreadWorker() const
            {
                sleep( m_secTimeOut );
                if ( m_pid > 0 && IsProcessExist(m_pid) )
                {
                    // TODO: log me!
                    ::kill( m_pid, SIGTERM );
                }
            }

        private:
            bool m_IsInit;
            pid_t m_pid;
            size_t m_secTimeOut;
            Thread_PTR_t m_Thread;
    };

};
#endif /*TIMEOUTGUARD_H_*/