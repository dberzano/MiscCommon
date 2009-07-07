/*
 * PoDUserDefaultsOptions.h
 *
 *  Created on: Jun 30, 2009
 *      Author: Anar Manafov
 */

#ifndef PODUSERDEFAULTSOPTIONS_H_
#define PODUSERDEFAULTSOPTIONS_H_

namespace PoD
{

    typedef struct SPoDUserDefaultOptions
    {
        //
        // ---= GENERAL
        //
        std::string m_workDir;           //!< Working folder.
        std::string m_logFileDir;        //!< The log filename.
        bool m_logFileOverwrite;         //!< Overwrite log file each session.
        unsigned int m_logLevel;               //!< TODO: Implement me!
        unsigned int m_agentTimeout;           //!< PoD Agent will not be allowed to work longer, than this value. Default is 0 - no timeout.
        std::string m_PROOFCfg;         //!< A location of the proof configuration file.
        std::string m_lastExecCmd;      //!< PROOFAgent will execute this command at the end of the session.
        //
        // ---= SERVER =---
        //
        unsigned int m_agentServerListenPort;
        std::string m_agentServerHost;                     //!< PROOFAgent's server host
        unsigned int m_agentServerLocalClientPortMin;
        unsigned int m_agentServerLocalClientPortMax;
        unsigned int m_serverXrdPortsRangeMin;
        unsigned int m_serverXrdPortsRangeMax;
        unsigned int m_serverXproofPortsRangeMin;
        unsigned int m_serverXproofPortsRangeMax;
        unsigned int m_agentServerPortsRangeMin;
        unsigned int m_agentServerPortsRangeMax;
        //
        // ---= WORKER =---
        //
        unsigned int m_workerLocalXPROOFPort;                //!< PROOF's local port (on worker nodes)
        int m_shutdownIfIdleForSec;                      //!< Shut down a worker if its idle time is higher this value. If value is 0 then the feature is off.
        unsigned int m_workerXrdPortsRangeMin;
        unsigned int m_workerXrdPortsRangeMax;
        unsigned int m_workerXproofPortsRangeMin;
        unsigned int m_workerXproofPortsRangeMax;
    }SPoDUserDefaultsOptions_t;

}

#endif /* PODUSERDEFAULTSOPTIONS_H_ */
