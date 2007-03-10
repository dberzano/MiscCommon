/************************************************************************/
/**
 * @file $$File name$$
 * @brief $$File comment$$
 * @author Anar Manafov A.Manafov@gsi.de
 */ /*
 
        version number:    $LastChangedRevision$
        created by:        Anar Manafov
                           $$date$$
        last changed by:   $LastChangedBy$ $LastChangedDate$
 
        Copyright (c) 2006,2007 GSI GridTeam. All rights reserved.
    *************************************************************************/
#ifndef INET_H
#define INET_H

// API
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

// STD
#include <unistd.h>
#include <stdexcept>
#include <def.h>

// OUR
#include "ErrorCode.h"

namespace MiscCommon
{
    namespace INet
    {

        typedef int Socket_t;

        class smart_socket
        {
            public:
                smart_socket() :
                        m_Socket( 0 )
                {}
                smart_socket( int _Socket ) :
                        m_Socket( _Socket )
                {}
                smart_socket( int _domain, int _type, int _protocol )
                {
                    m_Socket = ::socket( _domain, _type, _protocol );
                }
                ~smart_socket()
                {
                    close();
                }
                Socket_t* operator&()
                {
                    return & m_Socket;
                }
                operator int() const
                {
                    return static_cast<int>( m_Socket );
                }
                int operator =( const int &_Val )
                {
                    close();
                    return m_Socket = _Val;
                }
                void close()
                {
                    if ( m_Socket )
                        ::close( m_Socket );
                }

            private:
                Socket_t m_Socket;
        };

        template <typename _T>
        smart_socket& operator >> ( smart_socket &_Socket, _T *_Buf ) throw ( std::exception );

        template <>
        inline smart_socket& operator >> ( smart_socket &_Socket, BYTEVector_t *_Buf ) throw ( std::exception )
        {
            if ( !_Buf )
                throw std::runtime_error( "The given buffer pointer is NULL." );

            const ssize_t bytes_read = ::recv( _Socket, &( *_Buf ) [ 0 ], _Buf->capacity(), 0 );
            if ( bytes_read > 0 )
                _Buf->resize( bytes_read );
            else
            {
                std::string sErr;
                MiscCommon::errno2str( &sErr );
                throw std::runtime_error( sErr.c_str() );
            }
            return _Socket;
        }

        template <typename _T>
        smart_socket& operator << ( smart_socket &_Socket, _T &_Buf );

        template <>
        inline smart_socket& operator << ( smart_socket &_Socket, BYTEVector_t &_Buf )
        {
            ::send( _Socket, &_Buf[ 0 ], _Buf.size(), 0 );
            return _Socket;
        }

        class CSocketServer
        {
            public:
                CSocketServer() : m_ServerSocket( AF_INET, SOCK_STREAM, 0 )
                {}
                void Bind( unsigned short _nPort, const std::string *_Addr = NULL ) throw ( std::exception )
                {
                    if ( m_ServerSocket < 0 )
                        throw std::runtime_error( "Soket error..." ); // TODO: perror( "socket" );

                    sockaddr_in addr;
                    addr.sin_family = AF_INET;
                    addr.sin_port = ::htons( _nPort );
                    if ( !_Addr )
                        addr.sin_addr.s_addr = ::htonl( INADDR_ANY );
                    else
                        ::inet_aton( _Addr->c_str(), &addr.sin_addr );

                    if ( bind( m_ServerSocket, ( struct sockaddr * ) & addr, sizeof( addr ) ) < 0 )
                        throw std::runtime_error( "Soket bind error..." ); // TODO: perror( "bind" );
                }

                void Listen( int _Backlog ) throw ( std::exception )
                {
                    // TODO: Add error handling
                    ::listen( m_ServerSocket, _Backlog );
                }

                Socket_t Accept() throw ( std::exception )
                {
                    return ::accept( m_ServerSocket, NULL, NULL ) ;
                }

                smart_socket& GetSocket()
                {
                    return m_ServerSocket;
                }

            private:
                smart_socket m_ServerSocket;
        };

        class CSocketClient
        {
            public:
                CSocketClient() : m_ClientSocket( AF_INET, SOCK_STREAM, 0 )
                {}

                void Connect( unsigned short _nPort, const std::string &_Addr )
                {
                    if ( m_ClientSocket < 0 )
                        throw std::runtime_error( "Soket error..." ); // TODO: perror( "socket" );

                    sockaddr_in addr;
                    addr.sin_family = AF_INET;
                    addr.sin_port = ::htons( _nPort );
                    ::inet_aton( _Addr.c_str(), &addr.sin_addr );

                    if ( ::connect( m_ClientSocket, ( struct sockaddr * ) & addr, sizeof( addr ) ) < 0 )
                        throw std::runtime_error( "Soket CONNECT error..." ); // TODO: perror( "connect" );
                }

                smart_socket& GetSocket()
                {
                    return m_ClientSocket;
                }

            private:
                smart_socket m_ClientSocket;
        };


    };
};

#endif
