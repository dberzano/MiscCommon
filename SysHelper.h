/************************************************************************/
/**
 * @file SysHelper.h
 * @brief This file contains a number of helpers and wrappers of system calls.
 * @author Anar Manafov A.Manafov@gsi.de
 */ /*
 
        version number:   $LastChangedRevision$
        created by:          Anar Manafov
                                  2007-04-01
        last changed by:   $LastChangedBy$ $LastChangedDate$
 
        Copyright (c) 2007 GSI GridTeam. All rights reserved.
*************************************************************************/
#ifndef SYSHELPER_H_
#define SYSHELPER_H_

// API
#include <unistd.h>
#include <pwd.h>
#include <netdb.h>
#include <sys/syscall.h>

// HACK: On the SLC3 HOST_NAME_MAX is undefined
#ifndef HOST_NAME_MAX
#define HOST_NAME_MAX 64
#endif

// OUR
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

        CHARVector_t Buf(L_cuserid);

        cuserid( &Buf[0] );

        *_RetVal = std::string( &Buf[0] );
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
     * @brief The function extends \b ~/ and \b $HOME/ to
     * @brief a real user's home directory path.
     * @param _Path - [in, out] A pointer to string buffer which represents a path to extend. Must not be NULL.
     **/
    inline void smart_homedir_append( std::string *_Path )
    {
        if ( !_Path )
            return ;

        std::string sHome;
        get_cuser_homedir( &sHome );
        smart_append( &sHome, '/');

        replace<std::string>( _Path, "~/", sHome );
        replace<std::string>( _Path, "$HOME/", sHome );
    }
    /**
     * @brief The function is used to access the host name (with FCDN) of the current processor.
     * @param _RetVal - [out] The return buffer string. Must not be NULL.
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

};
#endif /*SYSHELPER_H_*/
