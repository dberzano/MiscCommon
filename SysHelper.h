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

        Copyright (c) 2007 GSI GridTeam. All rights reserved.
*************************************************************************/
#ifndef SYSHELPER_H_
#define SYSHELPER_H_

// API
#include <pwd.h>
#include <netdb.h>
#include <sys/syscall.h>

// HACK: On the SLC3 HOST_NAME_MAX is undefined
#ifndef HOST_NAME_MAX
#define HOST_NAME_MAX 64
#endif

// MiscCommon
#include "def.h"
#include "MiscUtils.h"

namespace MiscCommon
{
    /**
     * @brief The function returns current user name.
     * @param _RetVal - [out] A pinter to string buffer where user name will be stored. Must not be NULL.
     **/
    inline void get_cuser_name( std::string *_RetVal )
    {
        if ( !_RetVal )
            return ;
        
        passwd *pwd( getpwuid(geteuid()) );
        *_RetVal = pwd ? std::string(pwd->pw_name): "";
    }
    /**
     * @brief The function returns home directory path of the given user.
     * @param _uid - [in] user id the home directory of which should be returned.
     * @param _RetVal - [out] A pointer to string buffer where path will be stored. Must not be NULL.
     * @return In case of error, function returns an empty buffer.
     **/
    inline void get_homedir( uid_t _uid, std::string *_RetVal )
    {
        if ( !_RetVal )
            return ;

        passwd *pwd = getpwuid( _uid );
        *_RetVal = pwd ? std::string( pwd->pw_dir) : "";
    }
    /**
     * @brief The function returns home directory path of the current user.
     * @param _RetVal - [out] A pointer to string buffer where path will be stored. Must not be NULL.
     * @return In case of error, function returns an empty buffer.
     **/
    inline void get_cuser_homedir( std::string *_RetVal )
    {
        get_homedir( getuid(), _RetVal );
    }
    /**
     * @brief The function extends any environment variable found in the give path
     * @brief to its value.\b
     * @brief This function also extends "~/" to a real user's home directory path.
     * @brief When, for example, there is a variable $GLITE_LOCATE = /opt/glite and the given path
     * @brief is "$GLITE_LOCATION/etc/test.xml", the return value will be a path "/opt/glite/etc/test.xml"
     * @param _Path - [in, out] A pointer to a string buffer which represents a path to extend. Must not be NULL.
     **/
    template<class _T>
    inline void smart_path( _T *_Path )
    {
        // Checking for "~/"
        std::string path( *_Path );
        MiscCommon::trim_left( &path, ' ');
        if ( '~' == path[0] && '/' == path[1] )
        {
            std::string path( *_Path );
            std::string sHome;
            get_cuser_homedir( &sHome );
            smart_append( &sHome, '/');

            path.erase( path.begin(), path.begin() + 2 );
            sHome += path;
            path.swap(sHome);
            _Path->swap(path);
        }

        typename _T::size_type p_begin = _Path->find( _T("$") );
        if ( _T::npos == p_begin )
            return;

        ++p_begin; // ecluding '$' from the name

        typename _T::size_type p_end = _Path->find( _T("/"), p_begin );
        if ( _T::npos == p_end )
            p_end = _Path->size();

        const _T env_var( _Path->substr(p_begin, p_end - p_begin) );
        // TODO: needs to be fixed to wide char: getenv
        LPCTSTR szvar( getenv(env_var.c_str()) );
        if ( !szvar )
            return;
        const _T var_val( szvar );
        if ( var_val.empty() )
            return;

        replace( _Path, _T("$") + env_var, var_val );

        smart_path( _Path );
    }
    /**
     * @brief The function is used to access the host name (with FCDN) of the current processor.
     * @param _RetVal - [out] The returned buffer string. Must not be NULL.
     **/
    inline void get_hostname( std::string *_RetVal )
    {
        if ( !_RetVal )
            return ;

        // getting host name - which is without domain name
        CHARVector_t Buf( HOST_NAME_MAX );
        gethostname( &Buf[0], Buf.capacity() );

        // getting host name with FCDN
        hostent *h = gethostbyname( std::string(&Buf[0]).c_str() );
        if ( !h )
            return ;

        *_RetVal = h->h_name;
    }

    /**
     * @brief A system helper, which helps to get a Thread ID of the current thread.
     * @return Current thread ID.
     **/
    inline pid_t gettid()
    {
        return syscall(__NR_gettid);
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
                pthread_mutex_init(&m, 0);
            }

            void Lock()
            {
                pthread_mutex_lock(&m);
            }

            void Unlock()
            {
                pthread_mutex_unlock(&m);
            }

        private:
            pthread_mutex_t m;
    };
    /**
     * @brief A smart CMutex helper.
     * @brief It locks the mutex on the construction and unlocks it when destructor is called.
     **/
    class smart_mutex
    {
        public:
            smart_mutex( CMutex& _pm ): m(_pm)
            {
                m.Lock();
            }
            ~smart_mutex()
            {
                m.Unlock();
            }
        private:
            CMutex& m;
    };
    
    /**
     * @brief demangling C++ symbols.
     * @code
     * cout << demangle(typeid(*this)) << endl;
     * cout << demangle(typeid(int)) << endl;
     * 
     * @endcode
     **/
    extern "C" char *__cxa_demangle(const char *mangled, char *buf, size_t *len, int *status);
    inline std::string demangle(const std::type_info& ti)
    {
        char* s=__cxa_demangle(ti.name(),0,0,0);
        std::string ret(s);
        free(s);
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

};
#endif /*SYSHELPER_H_*/
