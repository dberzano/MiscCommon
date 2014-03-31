/************************************************************************/
/**
 * @file SysHelper.h
 * @brief This file contains a number of helpers and wrappers of system calls.
 * @author Anar Manafov A.Manafov@gsi.de
 */ /*

        version number:     $LastChangedRevision$
        created by:         Anar Manafov
                            2007-04-01
        last changed by:    $LastChangedBy$ $LastChangedDate$

        Copyright (c) 2007-2011 GSI GridTeam. All rights reserved.
*************************************************************************/
#ifndef SYSHELPER_H_
#define SYSHELPER_H_

// API
#include <pwd.h>
#include <netdb.h>
#include <sys/syscall.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <limits.h>
#include <unistd.h>
#ifdef __APPLE__
#include <sys/sysctl.h>
#endif

// STD
#include <typeinfo>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>

// HACK: On the SLC3 HOST_NAME_MAX is undefined
#ifndef HOST_NAME_MAX
#define HOST_NAME_MAX 64
#endif

// MiscCommon
#include "def.h"
#include "MiscUtils.h"
#include "ErrorCode.h"

namespace MiscCommon
{
    /**
     * @brief The function returns current user name.
     * @param[out] _RetVal - A pinter to string buffer where user name will be stored. Must not be NULL.
     **/
    inline void get_cuser_name( std::string *_RetVal )
    {
        if( !_RetVal )
            return ;

        passwd *pwd( getpwuid( geteuid() ) );
        *_RetVal = pwd ? std::string( pwd->pw_name ) : "";
    }
    /**
     * @brief The function returns home directory path of the given user.
     * @param[in] _uid - user id the home directory of which should be returned.
     * @param[out] _RetVal - A pointer to string buffer where path will be stored. Must not be NULL.
     * @return In case of error, function returns an empty buffer.
     **/
    inline void get_homedir( uid_t _uid, std::string *_RetVal )
    {
        if( !_RetVal )
            return ;

        passwd *pwd = getpwuid( _uid );
        *_RetVal = pwd ? std::string( pwd->pw_dir ) : "";
    }
    /**
     * @brief The function returns home directory path of the given user.
     * @param[in] _UName - a name of the user the home directory of which should be returned.
     * @param[out] _RetVal - A pointer to string buffer where path will be stored. Must not be NULL.
     * @return In case of error, function returns an empty buffer.
     **/
    inline void get_homedir( const char *_UName, std::string *_RetVal )
    {
        if( !_RetVal )
            return ;

        passwd *pwd = getpwnam( _UName );
        *_RetVal = pwd ? std::string( pwd->pw_dir ) : "";
    }
    /**
     * @brief The function returns home directory path of the current user.
     * @param[out] _RetVal - A pointer to string buffer where path will be stored. Must not be NULL.
     * @return In case of error, function returns an empty buffer.
     **/
    inline void get_cuser_homedir( std::string *_RetVal )
    {
        get_homedir( getuid(), _RetVal );
    }
    /**
     * @brief The function extends any environment variable found in the give path to its value.
     * @brief This function also extends "~/" or "~user_name/" to a real user's home directory path.
     * @brief When, for example, there is a variable $GLITE_LOCATE = /opt/glite and the given path
     * @brief is "$GLITE_LOCATION/etc/test.xml", the return value will be a path "/opt/glite/etc/test.xml"
     * @param[in,out] _Path - A pointer to a string buffer which represents a path to extend. Must not be NULL.
     **/
    template<class _T>
    inline void smart_path( _T *_Path )
    {
        // Checking for "~/"
        std::string path( *_Path );
        MiscCommon::trim_left( &path, ' ' );
        if( '~' == path[0] )
        {
            std::string path( *_Path );
            // ~/.../.../
            if( '/' == path[1] )
            {
                std::string sHome;
                get_cuser_homedir( &sHome );
                smart_append( &sHome, '/' );

                path.erase( path.begin(), path.begin() + 2 );
                sHome += path;
                path.swap( sHome );
                _Path->swap( path );
            }
            else // ~user/.../.../
            {
                typename _T::size_type p = path.find( _T( "/" ) );
                if( _T::npos != p )
                {
                    const std::string uname = path.substr( 1, p - 1 );
                    std::string home_dir;
                    get_homedir( uname.c_str(), &home_dir );
                    path.erase( path.begin(), path.begin() + p );
                    path = home_dir + path;
                    _Path->swap( path );
                }
            }
        }

        typename _T::size_type p_begin = _Path->find( _T( "$" ) );
        if( _T::npos == p_begin )
        {
            //make the path to be the canonicalized absolute pathname
            char resolved_path[PATH_MAX];
            char *res = realpath( _Path->c_str(), resolved_path );
            if( NULL != res )
            {
                // add trailing slash if needed, since realpath removes it
                std::string::iterator it = _Path->end() - 1;
                bool trailing_slash = ( *it == '/' );
                *_Path = resolved_path;
                if( trailing_slash )
                    smart_append( _Path, '/' );
            };

            return;
        }

        ++p_begin; // Excluding '$' from the name

        typename _T::size_type p_end = _Path->find( _T( "/" ), p_begin );
        if( _T::npos == p_end )
            p_end = _Path->size();

        const _T env_var( _Path->substr( p_begin, p_end - p_begin ) );
        // TODO: needs to be fixed to wide char: getenv
        LPCTSTR szvar( getenv( env_var.c_str() ) );
        if( !szvar )
            return;
        const _T var_val( szvar );
        if( var_val.empty() )
            return;

        replace( _Path, _T( "$" ) + env_var, var_val );

        smart_path( _Path );
    }
    /**
     * @brief The function is used to access the host name (with FCDN) of the current processor.
     * @param[out] _RetVal - The returned buffer string. Must not be NULL.
     **/
    inline void get_hostname( std::string *_RetVal )
    {
        if( !_RetVal )
            return ;

        // getting host name - which is without domain name
        CHARVector_t Buf( HOST_NAME_MAX );
        gethostname( &Buf[0], Buf.capacity() );

        // getting host name with FCDN
        hostent *h = gethostbyname( std::string( &Buf[0] ).c_str() );
        if( !h )
            return ;

        *_RetVal = h->h_name;
    }

    /**
     * @brief Gets the IP address used for outbound connections.
     * @param[out] _RetVal - The returned buffer string. Must not be NULL.
     **/
    inline int get_outipv4( std::string *_RetVal )
    {
        if (!_RetVal)
            return -1;

        int fd;
        struct sockaddr_in bind_addr, out_addr, det_addr;
        socklen_t det_addr_len;

        memset( &bind_addr, 0, sizeof(bind_addr) );
        memset( &out_addr,  0, sizeof(out_addr)  );
        memset( &det_addr,  0, sizeof(det_addr)  );

        if ( (fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 )
            return -1;

        bind_addr.sin_family = AF_INET;
        bind_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        bind_addr.sin_port = htons(0);
        if ( bind(fd, (struct sockaddr *)&bind_addr, sizeof(bind_addr)) < 0 ) {
            close(fd);
            return -1;
        }

        out_addr.sin_family = AF_INET;
        out_addr.sin_addr.s_addr = inet_addr("1.2.3.4");
        out_addr.sin_port = htons(1);
        if ( connect(fd, (struct sockaddr *)&out_addr, sizeof(out_addr) ) < 0) {
            close(fd);
            return -1;
        }

        det_addr_len = sizeof(det_addr);
        if ( getsockname(fd, (struct sockaddr *)&det_addr, &det_addr_len) < 0 ) {
            close(fd);
            return -1;
        }

        *_RetVal = inet_ntoa(det_addr.sin_addr);
        close(fd);

        return 0;
    }

    /**
     * @brief A system helper, which helps to get a Thread ID of the current thread.
     * @return Current thread ID.
     **/
    inline unsigned long gettid()
    {
#ifdef __APPLE__
        union
        {
            pthread_t th;
            unsigned long int i;
        } v = { };
        v.th = pthread_self();
        return v.i;
#elif __linux
        return syscall( __NR_gettid );
#else
        return 0;
#endif
    }

    /**
     * @brief A Mutex wrapper. Based on \b pthread calls.
     * @note Using this implementation one can create a thread -safe singleton:
     * @code
     * class Singleton
     * {
     *      public:
     *          static Singleton& Instance();
     *          int example_data;
     *          ~Singleton() { }
     *      protected:
     *          Singleton(): example_data(42) { }
     *      private:
     *          static std::auto_ptr<Singleton> theSingleInstance;
     *          static Mutex m;
     * };
     *
     * Singleton& Singleton::Instance()
     * {
     *      MutexLocker obtain_lock(m);
     *      if (theSingleInstance.get() == 0)
     *          theSingleInstance.reset(new Singleton);
     *      return *theSingleInstance;
     * }
     *
     * std::auto_ptr<Singleton> Singleton::theSingleInstance;
     * Mutex Singleton::m;
     *
     * #include <cstdio>
     * int main()
     * {
     *      printf("%d\n", Singleton::Instance().example_data);
     *      return 0;
     * }
     * @endcode
     **/
    class CMutex
    {
        public:
            CMutex()
            {
                pthread_mutex_init( &m, 0 );
            }

            void Lock()
            {
                pthread_mutex_lock( &m );
            }

            void Unlock()
            {
                pthread_mutex_unlock( &m );
            }

        private:
            pthread_mutex_t m;
    };
    /**
     * @brief A smart CMutex helper.
     * @brief It locks the mutex on the construction and unlocks it when destructor is called.
     **/
    class smart_mutex: public NONCopyable
    {
        public:
            smart_mutex( CMutex &_mutex ): m( _mutex )
            {
                m.Lock();
            }
            ~smart_mutex()
            {
                m.Unlock();
            }
        private:
            CMutex &m;
    };

    /**
     * @brief demangling C++ symbols.
     * @code
     * cout << demangle(typeid(*this)) << endl;
     * cout << demangle(typeid(int)) << endl;
     *
     * @endcode
     **/
    extern "C" char *__cxa_demangle( const char *mangled, char *buf, size_t *len, int *status );
    inline std::string demangle( const std::type_info& ti )
    {
        char* s = __cxa_demangle( ti.name(), 0, 0, 0 );
        std::string ret( s );
        free( s );
        return ret;
    }

    inline void get_env( const std::string &_EnvVarName, std::string *_RetVal )
    {
        if( !_RetVal )
            return;

        char *szBuf( getenv( _EnvVarName.c_str() ) );
        if( szBuf )
            _RetVal->assign( szBuf );
    }
    /**
     *
     * @brief The function file_size() retrieves file size of a given file.
     * @param[in] _FileName - full file name.
     * @exception system_error - thrown if error occurs.
     *
     */
    inline off_t file_size( const std::string &_FileName )
    {
        const int fd( ::open( _FileName.c_str(), O_RDONLY ) );
        if( -1 == fd )
            throw system_error( "Can't get file size of \"" + _FileName + "\"" );

        struct stat fs;
        const int ret( ::fstat( fd, &fs ) );
        close( fd );

        if( -1 == ret )
            throw system_error( "Can't get file size of \"" + _FileName + "\"" );

        return fs.st_size;
    }
    /**
     *
     * @brief
     *
     */
    inline bool file_exists( const std::string &_FileName )
    {
        try
        {
            file_size( _FileName );
            return true;
        }
        catch( ... )
        {
            return false;
        }
    }
    /**
     *
     * @brief the function returns a number of available CPU cores
     *
     */
    inline size_t getNCores()
    {
        size_t numCPU( 1 );
#ifdef __APPLE__ //FreeBSD, MacOS X, NetBSD, OpenBSD, etc.
        int mib[4];
        size_t len = sizeof( numCPU );

        /* set the mib for hw.ncpu */
        mib[0] = CTL_HW;
        mib[1] = HW_AVAILCPU;  // alternatively, try HW_NCPU;

        /* get the number of CPUs from the system */
        sysctl( mib, 2, &numCPU, &len, NULL, 0 );

        if( numCPU < 1 )
        {
            mib[1] = HW_NCPU;
            sysctl( mib, 2, &numCPU, &len, NULL, 0 );

            if( numCPU < 1 )
            {
                numCPU = 1;
            }
        }
#elif __linux // Linux, Solaris, & AIX (per comments)
        numCPU = sysconf( _SC_NPROCESSORS_ONLN );
#endif
        return numCPU;
    }
};
#endif /*SYSHELPER_H_*/
