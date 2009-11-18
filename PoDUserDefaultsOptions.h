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

    typedef struct SCommonOptions
    {
        std::string m_workDir;      //!< Working folder.
        std::string m_logFileDir;   //!< The log filename.
        bool m_logFileOverwrite;    //!< Overwrite log file each session.
        std::string m_proofCFG;     //!< A location of the proof configuration file.
        int m_shutdownIfIdleForSec; //!< Shut down agent if its idle time is higher this value. If value is 0 then the feature is off.
        unsigned short m_logLevel;   //!< A log level
        unsigned int m_xrdPortsRangeMin;
        unsigned int m_xrdPortsRangeMax;
        unsigned int m_xproofPortsRangeMin;
        unsigned int m_xproofPortsRangeMax;
        unsigned int m_agentThreads;    //!< A number of threads in thread pool.
        unsigned int m_agentNodeReadBuffer; //!< A buffer size, used by a proxy (in bytes).
    } SCommonOptions_t;

    typedef struct SServerOptions
    {
        //
        // ---= SERVER =---
        //
        SCommonOptions_t m_common;
        unsigned int m_agentLocalClientPortMin;
        unsigned int m_agentLocalClientPortMax;
        unsigned int m_agentPortsRangeMin;
        unsigned int m_agentPortsRangeMax;
    } SServerOptions_t;

    typedef struct SWorkerOptions
    {
        //
        // ---= WORKER =---
        //
        SCommonOptions_t m_common;
        std::string m_setMyROOTSYS;                 //!< Whether to use user's ROOTSYS to use on workers (values: yes/no)
        std::string m_myROOTSYS;                    //!< User's ROOTSYS to use on workers
    } SWorkerOptions_t;

    typedef struct SLSFOptions
    {
        //
        // ---= LSF =---
        //
        bool m_emailOutput;         //!< specifies whether job's output is sent to the user by mail
    } SLSFOptions_t;

    typedef struct SPoDUserDefaultOptions
    {
        SServerOptions_t m_server;
        SWorkerOptions_t m_worker;
        SLSFOptions_t m_lsf;

    } SPoDUserDefaultsOptions_t;

// TODO: we use boost 1.32. This is the only method I found to convert boost::any to string.
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

        if ( _any.type() == typeid( unsigned char ) )
            ss << boost::any_cast<unsigned char>( _any );

        if ( _any.type() == typeid( unsigned short ) )
            ss << boost::any_cast<unsigned short>( _any );

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
                // HACK: Don't make a long add_options, otherwise Eclipse 3.5's CDT indexer can't handle it
                config_file_options.add_options()
                ( "server.work_dir", boost::program_options::value<std::string>( &m_options.m_server.m_common.m_workDir )->default_value( "$POD_LOCATION/" ), "" )
                ( "server.logfile_dir", boost::program_options::value<std::string>( &m_options.m_server.m_common.m_logFileDir )->default_value( "$POD_LOCATION/log" ), "" )
                ( "server.logfile_overwrite", boost::program_options::value<bool>( &m_options.m_server.m_common.m_logFileOverwrite )->default_value( false, "no" ), "" )
                ( "server.log_level", boost::program_options::value<unsigned short>( &m_options.m_server.m_common.m_logLevel )->default_value( 1 ), "" )
                ( "server.proof_cfg_path", boost::program_options::value<std::string>( &m_options.m_server.m_common.m_proofCFG )->default_value( "$POD_LOCATION/etc/proof.conf" ), "" )
                ( "server.agent_shutdown_if_idle_for_sec", boost::program_options::value<int>( &m_options.m_server.m_common.m_shutdownIfIdleForSec )->default_value( 1800 ), "" )
                ( "server.agent_local_client_port_min", boost::program_options::value<unsigned int>( &m_options.m_server.m_agentLocalClientPortMin )->default_value( 20000 ), "" )
                ( "server.agent_local_client_port_max", boost::program_options::value<unsigned int>( &m_options.m_server.m_agentLocalClientPortMax )->default_value( 25000 ), "" )
                ( "server.xrd_ports_range_min", boost::program_options::value<unsigned int>( &m_options.m_server.m_common.m_xrdPortsRangeMin ) )
                ( "server.xrd_ports_range_max", boost::program_options::value<unsigned int>( &m_options.m_server.m_common.m_xrdPortsRangeMax ) )
                ( "server.xproof_ports_range_min", boost::program_options::value<unsigned int>( &m_options.m_server.m_common.m_xproofPortsRangeMin ) )
                ( "server.xproof_ports_range_max", boost::program_options::value<unsigned int>( &m_options.m_server.m_common.m_xproofPortsRangeMax ) )
                ( "server.agent_ports_range_min", boost::program_options::value<unsigned int>( &m_options.m_server.m_agentPortsRangeMin ) )
                ( "server.agent_ports_range_max", boost::program_options::value<unsigned int>( &m_options.m_server.m_agentPortsRangeMax ) )
                ( "server.agent_threads", boost::program_options::value<unsigned int>( &m_options.m_server.m_common.m_agentThreads )->default_value( 8 ) )
                ( "server.agent_node_readbuffer", boost::program_options::value<unsigned int>( &m_options.m_server.m_common.m_agentNodeReadBuffer )->default_value( 5000 ) )
                ;
                config_file_options.add_options()
                ( "worker.work_dir", boost::program_options::value<std::string>( &m_options.m_worker.m_common.m_workDir )->default_value( "$POD_LOCATION/" ), "" )
                ( "worker.logfile_dir", boost::program_options::value<std::string>( &m_options.m_worker.m_common.m_logFileDir )->default_value( "$POD_LOCATION/" ), "" )
                ( "worker.logfile_overwrite", boost::program_options::value<bool>( &m_options.m_worker.m_common.m_logFileOverwrite )->default_value( false, "no" ), "" )
                ( "worker.log_level", boost::program_options::value<unsigned short>( &m_options.m_worker.m_common.m_logLevel )->default_value( 1 ), "" )
                ( "worker.proof_cfg_path", boost::program_options::value<std::string>( &m_options.m_worker.m_common.m_proofCFG )->default_value( "$POD_LOCATION/proof.conf" ), "" )
                ( "worker.set_my_rootsys", boost::program_options::value<std::string>( &m_options.m_worker.m_setMyROOTSYS ), "" )
                ( "worker.my_rootsys", boost::program_options::value<std::string>( &m_options.m_worker.m_myROOTSYS ), "" )
                ( "worker.agent_shutdown_if_idle_for_sec", boost::program_options::value<int>( &m_options.m_worker.m_common.m_shutdownIfIdleForSec )->default_value( 1800 ), "" )
                ( "worker.xrd_ports_range_min", boost::program_options::value<unsigned int>( &m_options.m_worker.m_common.m_xrdPortsRangeMin ) )
                ( "worker.xrd_ports_range_max", boost::program_options::value<unsigned int>( &m_options.m_worker.m_common.m_xrdPortsRangeMax ) )
                ( "worker.xproof_ports_range_min", boost::program_options::value<unsigned int>( &m_options.m_worker.m_common.m_xproofPortsRangeMin ) )
                ( "worker.xproof_ports_range_max", boost::program_options::value<unsigned int>( &m_options.m_worker.m_common.m_xproofPortsRangeMax ) )
                ( "worker.agent_threads", boost::program_options::value<unsigned int>( &m_options.m_worker.m_common.m_agentThreads )->default_value( 3 ) )
                ( "worker.agent_node_readbuffer", boost::program_options::value<unsigned int>( &m_options.m_worker.m_common.m_agentNodeReadBuffer )->default_value( 5000 ) )
                ;
                config_file_options.add_options()
                ( "lsf_plugin.email_job_output", boost::program_options::value<bool>( &m_options.m_lsf.m_emailOutput )->default_value( false, "no" ), "" )
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
