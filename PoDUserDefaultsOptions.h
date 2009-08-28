/*
 * PoDUserDefaultsOptions.h
 *
 *  Created on: Jun 30, 2009
 *      Author: Anar Manafov
 */

#ifndef PODUSERDEFAULTSOPTIONS_H_
#define PODUSERDEFAULTSOPTIONS_H_
// STD
#include <fstream>
// BOOST
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>

namespace PoD
{

    typedef struct SPoDUserDefaultOptions
    {
        //
        // ---= GENERAL
        //
        std::string m_workDir;         //!< Working folder.
        std::string m_logFileDir;      //!< The log filename.
        bool m_logFileOverwrite;       //!< Overwrite log file each session.
        unsigned int m_logLevel;       //!< TODO: Implement me!
        unsigned int m_agentTimeout;   //!< PoD Agent will not be allowed to work longer, than this value. Default is 0 - no timeout.
        std::string m_PROOFCfg;        //!< A location of the proof configuration file.
        std::string m_lastExecCmd;     //!< PROOFAgent will execute this command at the end of the session.
        //
        // ---= SERVER =---
        //
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
        std::string m_setMyROOTSYS;                //!< Whether to use user's ROOTSYS to use on workers (values: yes/no)
        std::string m_myROOTSYS;                   //!< User's ROOTSYS to use on workers
        unsigned int m_workerLocalXPROOFPort;      //!< PROOF's local port (on worker nodes)
        int m_shutdownIfIdleForSec;                //!< Shut down a worker if its idle time is higher this value. If value is 0 then the feature is off.
        unsigned int m_workerXrdPortsRangeMin;
        unsigned int m_workerXrdPortsRangeMax;
        unsigned int m_workerXproofPortsRangeMin;
        unsigned int m_workerXproofPortsRangeMax;
    }SPoDUserDefaultsOptions_t;

    // TODO: we use boost 1.32. This is the only method I found to conver boost::any to string.
    // In the next version of boost its solved.
    inline std::string convertAnyToString( const boost::any &_any )
    {
        if ( _any.type() == typeid( std::string ) )
            return boost::any_cast<std::string>( _any );

        std::ostringstream ss;
        if ( _any.type() == typeid( int ) )
            ss << boost::any_cast<int>( _any );

        if ( _any.type() == typeid( unsigned int ) )
            ss << boost::any_cast<unsigned int>( _any );

        if ( _any.type() == typeid( bool ) )
            ss << boost::any_cast<bool>( _any );

        return ss.str();
    }

    class CPoDUserDefaults
    {
        public:
            void init( const std::string &_PoDCfgFileName )
            {
                m_keys.clear();
                boost::program_options::options_description config_file_options( "PoD user defaults options" );
                // HACK: Don't make a long add_options, otherwise Eclipse 3.5's CDT idexer can't handle it
                config_file_options.add_options()
                ( "general.work_dir", boost::program_options::value<std::string>( &m_options.m_workDir )->default_value( "$POD_LOCATION/" ), "" )
                ( "general.logfile_dir", boost::program_options::value<std::string>( &m_options.m_logFileDir )->default_value( "$POD_LOCATION/log" ), "" )
                ( "general.logfile_overwrite", boost::program_options::value<bool>( &m_options.m_logFileOverwrite )->default_value( false, "no" ), "" )
                ( "general.log_level", boost::program_options::value<unsigned int>( &m_options.m_logLevel )->default_value( 0 ), "" )
                ( "general.agent_timeout", boost::program_options::value<unsigned int>( &m_options.m_agentTimeout )->default_value( 0 ), "" )
                ( "general.proof_cfg_path", boost::program_options::value<std::string>( &m_options.m_PROOFCfg )->default_value( "~/proof.conf" ), "" )
                ( "general.last_execute_cmd", boost::program_options::value<std::string>( &m_options.m_lastExecCmd ), "" )
                ;
                config_file_options.add_options()
                ( "server.agent_server_local_client_port_min", boost::program_options::value<unsigned int>( &m_options.m_agentServerLocalClientPortMin )->default_value( 20000 ), "" )
                ( "server.agent_server_local_client_port_max", boost::program_options::value<unsigned int>( &m_options.m_agentServerLocalClientPortMax )->default_value( 25000 ), "" )
                ( "server.xrd_ports_range_min", boost::program_options::value<unsigned int>( &m_options.m_serverXrdPortsRangeMin ) )
                ( "server.xrd_ports_range_max", boost::program_options::value<unsigned int>( &m_options.m_serverXrdPortsRangeMax ) )
                ( "server.xproof_ports_range_min", boost::program_options::value<unsigned int>( &m_options.m_serverXproofPortsRangeMin ) )
                ( "server.xproof_ports_range_max", boost::program_options::value<unsigned int>( &m_options.m_serverXproofPortsRangeMax ) )
                ( "server.agent_server_ports_range_min", boost::program_options::value<unsigned int>( &m_options.m_agentServerPortsRangeMin ) )
                ( "server.agent_server_ports_range_max", boost::program_options::value<unsigned int>( &m_options.m_agentServerPortsRangeMax ) )
                ;
                config_file_options.add_options()
                ( "worker.set_my_rootsys", boost::program_options::value<std::string>( &m_options.m_setMyROOTSYS ), "" )
                ( "worker.my_rootsys", boost::program_options::value<std::string>( &m_options.m_myROOTSYS ), "" )
                ( "worker.local_xproof_port", boost::program_options::value<unsigned int>( &m_options.m_workerLocalXPROOFPort )->default_value( 111 ), "" )
                ( "worker.agent_shutdown_if_idle_for_sec", boost::program_options::value<int>( &m_options.m_shutdownIfIdleForSec )->default_value( 1800 ), "" )
                ( "worker.xrd_ports_range_min", boost::program_options::value<unsigned int>( &m_options.m_workerXrdPortsRangeMin ) )
                ( "worker.xrd_ports_range_max", boost::program_options::value<unsigned int>( &m_options.m_workerXrdPortsRangeMax ) )
                ( "worker.xproof_ports_range_min", boost::program_options::value<unsigned int>( &m_options.m_workerXproofPortsRangeMin ) )
                ( "worker.xproof_ports_range_max", boost::program_options::value<unsigned int>( &m_options.m_workerXproofPortsRangeMax ) )
                ;

                std::ifstream ifs( _PoDCfgFileName.c_str() );
                if ( !ifs.good() )
                {
                    std::string msg( "Could not open a PoD configuration file: " );
                    msg += _PoDCfgFileName;
                    throw std::runtime_error( msg );
                }
                // Parse the config file
                boost::program_options::store( boost::program_options::parse_config_file( ifs, config_file_options ), m_keys );
                boost::program_options::notify( m_keys );
            }

            std::string getValueForKey( const std::string &_Key )
            {
                return convertAnyToString( m_keys[_Key].value() );
            }

            SPoDUserDefaultsOptions_t getOptions()
            {
            	return m_options;
            }

        private:
            boost::program_options::variables_map m_keys;
            SPoDUserDefaultsOptions_t m_options;
    };
}

#endif /* PODUSERDEFAULTSOPTIONS_H_ */
