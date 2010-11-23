/************************************************************************/
/**
 * @file PoDUserDefaultsOptions.h
 * @brief
 * @author Anar Manafov A.Manafov@gsi.de
 */ /*

        version number:     $LastChangedRevision$
        created by:         Anar Manafov
                            2009-06-30
        last changed by:    $LastChangedBy$ $LastChangedDate$

        Copyright (c) 2009-2010 GSI GridTeam. All rights reserved.
*************************************************************************/
#ifndef PODUSERDEFAULTSOPTIONS_H_
#define PODUSERDEFAULTSOPTIONS_H_
// STD
#include <fstream>
// BOOST
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
// MiscCommon
#include "FindCfgFile.h"

namespace PoD
{
    class CPoDUserDefaults;

    inline std::string showCurrentPUDFile()
    {
        MiscCommon::CFindCfgFile<std::string> cfg;
        cfg.SetOrder
        ( "$HOME/.PoD/PoD.cfg" )
        ( "$POD_LOCATION/etc/PoD.cfg" )
        ( "$POD_LOCATION/PoD.cfg" );
        std::string val;
        cfg.GetCfg( &val );

        return val;
    }

    typedef struct SCommonOptions
    {
        std::string m_workDir;      //!< Working folder.
        std::string m_logFileDir;   //!< The log filename.
        bool m_logFileOverwrite;    //!< Overwrite log file each session.
        std::string m_proofCFG;     //!< A location of the proof configuration file.
        int m_shutdownIfIdleForSec; //!< Shut down agent if its idle time is higher this value. If value is 0 then the feature is off.
        unsigned short m_logLevel;  //!< A log level
        unsigned int m_xproofPortsRangeMin;
        unsigned int m_xproofPortsRangeMax;
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
        std::string m_packetForwarding;
        unsigned int m_agentThreads;    //!< A number of threads in thread pool.
    } SServerOptions_t;

    typedef struct SWorkerOptions
    {
        //
        // ---= WORKER =---
        //
        SCommonOptions_t m_common;
        bool m_setMyROOTSYS;                 //!< Whether to use user's ROOTSYS to use on workers (values: yes/no)
        std::string m_myROOTSYS;                    //!< User's ROOTSYS to use on workers
    } SWorkerOptions_t;

    typedef struct SLSFOptions
    {
        //
        // ---= LSF =---
        //
        bool m_emailOutput;         //!< specifies whether job's output is sent to the user by mail
        bool m_uploadJobLog;        //!< specifies whether to upload jobs log files from workers when PoD jobs are completed.
    } SLSFOptions_t;

    typedef struct SPBSOptions
    {
        //
        // ---= PBS =---
        //
        bool m_uploadJobLog;        //!< specifies whether to upload jobs log files from workers when PoD jobs are completed.
    } SPBSOptions_t;

    typedef struct SOGEOptions
    {
        //
        // ---= OGE/SGE =---
        //
        bool m_uploadJobLog;        //!< specifies whether to upload jobs log files from workers when PoD jobs are completed.
    } SOGEOptions_t;

    typedef struct SPoDUserDefaultOptions
    {
        SServerOptions_t m_server;
        SWorkerOptions_t m_worker;
        SLSFOptions_t m_lsf;
        SPBSOptions_t m_pbs;
        SOGEOptions_t m_oge;
    } SPoDUserDefaultsOptions_t;

// TODO: we use boost 1.32. This is the only method I found to convert boost::any to string.
// In the next version of boost its solved.
    inline std::string convertAnyToString( const boost::any &_any )
    {
        if( _any.type() == typeid( std::string ) )
            return boost::any_cast<std::string>( _any );

        std::ostringstream ss;
        if( _any.type() == typeid( int ) )
            ss << boost::any_cast<int>( _any );

        if( _any.type() == typeid( unsigned int ) )
            ss << boost::any_cast<unsigned int>( _any );

        if( _any.type() == typeid( unsigned char ) )
            ss << boost::any_cast<unsigned char>( _any );

        if( _any.type() == typeid( unsigned short ) )
            ss << boost::any_cast<unsigned short>( _any );

        if( _any.type() == typeid( bool ) )
            ss << boost::any_cast<bool>( _any );

        return ss.str();
    }

    class CPoDUserDefaults
    {
        public:
            void init( const std::string &_PoDCfgFileName, bool _get_default = false )
            {
                m_keys.clear();
                boost::program_options::options_description config_file_options( "PoD user defaults options" );
                // HACK: Don't make a long add_options, otherwise Eclipse 3.5's CDT indexer can't handle it
                config_file_options.add_options()
                ( "server.work_dir", boost::program_options::value<std::string>( &m_options.m_server.m_common.m_workDir )->default_value( "$HOME/.PoD" ), "" )
                ( "server.logfile_dir", boost::program_options::value<std::string>( &m_options.m_server.m_common.m_logFileDir )->default_value( "$HOME/.PoD/log" ), "" )
                ( "server.logfile_overwrite", boost::program_options::value<bool>( &m_options.m_server.m_common.m_logFileOverwrite )->default_value( true, "yes" ), "" )
                ( "server.log_level", boost::program_options::value<unsigned short>( &m_options.m_server.m_common.m_logLevel )->default_value( 1 ), "" )
                ( "server.proof_cfg_path", boost::program_options::value<std::string>( &m_options.m_server.m_common.m_proofCFG )->default_value( "$HOME/.PoD/etc/proof.conf" ), "" )
                ( "server.agent_shutdown_if_idle_for_sec", boost::program_options::value<int>( &m_options.m_server.m_common.m_shutdownIfIdleForSec )->default_value( 1800 ), "" )
                ( "server.agent_local_client_port_min", boost::program_options::value<unsigned int>( &m_options.m_server.m_agentLocalClientPortMin )->default_value( 20000 ), "" )
                ( "server.agent_local_client_port_max", boost::program_options::value<unsigned int>( &m_options.m_server.m_agentLocalClientPortMax )->default_value( 25000 ), "" )
                ( "server.xproof_ports_range_min", boost::program_options::value<unsigned int>( &m_options.m_server.m_common.m_xproofPortsRangeMin )->default_value( 21001 ) )
                ( "server.xproof_ports_range_max", boost::program_options::value<unsigned int>( &m_options.m_server.m_common.m_xproofPortsRangeMax )->default_value( 22000 ) )
                ( "server.agent_ports_range_min", boost::program_options::value<unsigned int>( &m_options.m_server.m_agentPortsRangeMin )->default_value( 22001 ) )
                ( "server.agent_ports_range_max", boost::program_options::value<unsigned int>( &m_options.m_server.m_agentPortsRangeMax )->default_value( 23000 ) )
                ( "server.agent_threads", boost::program_options::value<unsigned int>( &m_options.m_server.m_agentThreads )->default_value( 5 ) )
                ( "server.agent_node_readbuffer", boost::program_options::value<unsigned int>( &m_options.m_server.m_common.m_agentNodeReadBuffer )->default_value( 5000 ) )
                ( "server.packet_forwarding", boost::program_options::value<std::string>( &m_options.m_server.m_packetForwarding )->default_value( "auto" ), "" )
                ;
                config_file_options.add_options()
                ( "worker.work_dir", boost::program_options::value<std::string>( &m_options.m_worker.m_common.m_workDir )->default_value( "$POD_LOCATION/" ), "" )
                ( "worker.logfile_dir", boost::program_options::value<std::string>( &m_options.m_worker.m_common.m_logFileDir )->default_value( "$POD_LOCATION/" ), "" )
                ( "worker.logfile_overwrite", boost::program_options::value<bool>( &m_options.m_worker.m_common.m_logFileOverwrite )->default_value( true, "yes" ), "" )
                ( "worker.log_level", boost::program_options::value<unsigned short>( &m_options.m_worker.m_common.m_logLevel )->default_value( 1 ), "" )
                ( "worker.proof_cfg_path", boost::program_options::value<std::string>( &m_options.m_worker.m_common.m_proofCFG )->default_value( "$POD_LOCATION/proof.conf" ), "" )
                ( "worker.set_my_rootsys", boost::program_options::value<bool>( &m_options.m_worker.m_setMyROOTSYS )->default_value( true, "yes" ), "" )
                ( "worker.my_rootsys", boost::program_options::value<std::string>( &m_options.m_worker.m_myROOTSYS )->default_value( "$ROOTSYS" ), "" )
                ( "worker.agent_shutdown_if_idle_for_sec", boost::program_options::value<int>( &m_options.m_worker.m_common.m_shutdownIfIdleForSec )->default_value( 1800 ), "" )
                ( "worker.xproof_ports_range_min", boost::program_options::value<unsigned int>( &m_options.m_worker.m_common.m_xproofPortsRangeMin )->default_value( 21001 ) )
                ( "worker.xproof_ports_range_max", boost::program_options::value<unsigned int>( &m_options.m_worker.m_common.m_xproofPortsRangeMax )->default_value( 22000 ) )
                ( "worker.agent_node_readbuffer", boost::program_options::value<unsigned int>( &m_options.m_worker.m_common.m_agentNodeReadBuffer )->default_value( 5000 ) )
                ;
                config_file_options.add_options()
                ( "lsf_plugin.email_job_output", boost::program_options::value<bool>( &m_options.m_lsf.m_emailOutput )->default_value( false, "no" ), "" )
                ( "lsf_plugin.upload_job_log", boost::program_options::value<bool>( &m_options.m_lsf.m_uploadJobLog )->default_value( false, "no" ), "" )
                ;
                config_file_options.add_options()
                ( "pbs_plugin.upload_job_log", boost::program_options::value<bool>( &m_options.m_pbs.m_uploadJobLog )->default_value( false, "no" ), "" )
                ;
                config_file_options.add_options()
                ( "oge_plugin.upload_job_log", boost::program_options::value<bool>( &m_options.m_oge.m_uploadJobLog )->default_value( false, "no" ), "" )
                ;

                if( !_get_default )
                {
                    std::ifstream ifs( _PoDCfgFileName.c_str() );
                    if( !ifs.good() )
                    {
                        std::string msg( "Could not open a PoD configuration file: " );
                        msg += _PoDCfgFileName;
                        throw std::runtime_error( msg );
                    }
                    // Parse the config file
                    // TODO: use allow_unregistered when switched to boost 1.35
                    boost::program_options::store( boost::program_options::parse_config_file( ifs, config_file_options ), m_keys );
                }
                else
                {
                    // we fake reading of arguments, just to get a default values of all keys
                    char *arg[1];
                    arg[0] = new char[1];
                    arg[0][0] = '\0';
                    boost::program_options::store(
                        boost::program_options::basic_command_line_parser<char>( 1, arg ).options( config_file_options ).run(),
                        m_keys );
                    delete[] arg[0];
                }

                boost::program_options::notify( m_keys );
            }

            std::string getValueForKey( const std::string &_Key ) const
            {
                return convertAnyToString( m_keys[_Key].value() );
            }

            /// Returns strings "yes" or "no". Returns an empty string (if key is not of type bool)
            std::string getUnifiedBoolValueForBoolKey( const std::string &_Key ) const
            {
                if( m_keys[_Key].value().type() != typeid( bool ) )
                    return ( "" );

                return ( m_keys[_Key].as<bool>() ? "yes" : "no" );
            }


            const SPoDUserDefaultsOptions_t getOptions() const
            {
                return m_options;
            }

            static void printDefaults( std::ostream &_stream )
            {
                CPoDUserDefaults ud;
                ud.init( "", true );

                _stream
                        << "[server]\n"
                        << "work_dir=" << ud.getValueForKey( "server.work_dir" ) << "\n"
                        << "logfile_dir=" << ud.getValueForKey( "server.logfile_dir" ) << "\n"
                        << "logfile_overwrite=" << ud.getUnifiedBoolValueForBoolKey( "server.logfile_overwrite" ) << "\n"
                        << "log_level=" << ud.getValueForKey( "server.log_level" ) << "\n"
                        << "proof_cfg_path=" << ud.getValueForKey( "server.proof_cfg_path" ) << "\n"
                        << "agent_shutdown_if_idle_for_sec=" << ud.getValueForKey( "server.agent_shutdown_if_idle_for_sec" ) << "\n"
                        << "agent_local_client_port_min=" << ud.getValueForKey( "server.agent_local_client_port_min" ) << "\n"
                        << "agent_local_client_port_max=" << ud.getValueForKey( "server.agent_local_client_port_max" ) << "\n"
                        << "xproof_ports_range_min=" << ud.getValueForKey( "server.xproof_ports_range_min" ) << "\n"
                        << "xproof_ports_range_max=" << ud.getValueForKey( "server.xproof_ports_range_max" ) << "\n"
                        << "agent_ports_range_min=" << ud.getValueForKey( "server.agent_ports_range_min" ) << "\n"
                        << "agent_ports_range_max=" << ud.getValueForKey( "server.agent_ports_range_max" ) << "\n"
                        << "agent_threads=" << ud.getValueForKey( "server.agent_threads" ) << "\n"
                        << "agent_node_readbuffer=" << ud.getValueForKey( "server.agent_node_readbuffer" ) << "\n"
                        << "packet_forwarding=" << ud.getValueForKey( "server.packet_forwarding" ) << "\n";
                _stream
                        << "[worker]\n"
                        << "work_dir=" << ud.getValueForKey( "worker.work_dir" ) << "\n"
                        << "logfile_dir=" << ud.getValueForKey( "worker.logfile_dir" ) << "\n"
                        << "logfile_overwrite=" << ud.getUnifiedBoolValueForBoolKey( "worker.logfile_overwrite" ) << "\n"
                        << "log_level=" << ud.getValueForKey( "worker.log_level" ) << "\n"
                        << "proof_cfg_path=" << ud.getValueForKey( "worker.proof_cfg_path" ) << "\n"
                        << "set_my_rootsys=" << ud.getUnifiedBoolValueForBoolKey( "worker.set_my_rootsys" ) << "\n"
                        << "my_rootsys=" << ud.getValueForKey( "worker.my_rootsys" ) << "\n"
                        << "agent_shutdown_if_idle_for_sec=" << ud.getValueForKey( "worker.agent_shutdown_if_idle_for_sec" ) << "\n"
                        << "xproof_ports_range_min=" << ud.getValueForKey( "worker.xproof_ports_range_min" ) << "\n"
                        << "xproof_ports_range_max=" << ud.getValueForKey( "worker.xproof_ports_range_max" ) << "\n"
                        << "agent_node_readbuffer=" << ud.getValueForKey( "worker.agent_node_readbuffer" ) << "\n";
                _stream
                        << "[lsf_plugin]\n"
                        << "email_job_output=" << ud.getUnifiedBoolValueForBoolKey( "lsf_plugin.email_job_output" ) << "\n"
                        << "upload_job_log=" << ud.getUnifiedBoolValueForBoolKey( "lsf_plugin.upload_job_log" ) << "\n";
                _stream
                        << "[pbs_plugin]\n"
                        << "upload_job_log=" << ud.getUnifiedBoolValueForBoolKey( "pbs_plugin.upload_job_log" ) << "\n";
                _stream
                        << "[oge_plugin]\n"
                        << "upload_job_log=" << ud.getUnifiedBoolValueForBoolKey( "oge_plugin.upload_job_log" ) << "\n";
            }

        private:
            boost::program_options::variables_map m_keys;
            SPoDUserDefaultsOptions_t m_options;
    };
}

#endif /* PODUSERDEFAULTSOPTIONS_H_ */
