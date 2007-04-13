/************************************************************************/
/**
 * @file INet.h
 * @brief $$File comment$$
 * @author Anar Manafov A.Manafov@gsi.de
 */ /*
 
        version number:    $LastChangedRevision$
        created by:        Anar Manafov
                                    2007-03-01
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
#include <fcntl.h>

// STD
#include <unistd.h>
#include <stdexcept>
#include <def.h>
#include <sstream>

// OUR
#include "ErrorCode.h"
#include "MiscUtils.h"

namespace MiscCommon
{
    namespace INet
    {

        typedef int Socket_t;

        // Forward declaration
        inline std::string socket_error_string( Socket_t _socket, char *_strMsg = NULL );

        // TODO: Implement reference count
        class smart_socket: public NONCopyable
        {
            public:
                smart_socket() :
                        m_Socket( -1 )
                {}
                smart_socket( int _Socket ) :
                        m_Socket( _Socket )
                {}
                smart_socket( int _domain, int _type, int _protocol, bool _Block = false )
                {
                    m_Socket = ::socket( _domain, _type, _protocol );
                    // Blocking or Non-blocking socket
                    if ( _Block )
                        set_nonblock();
                }
                ~smart_socket()
                {
                    close();
                }
                //                 Socket_t* operator&()
                //                 {
                //                     return & m_Socket;
                //                 }
                operator int() const
                {
                    return static_cast<int>( m_Socket );
                }
                int operator =( const int &_Val )
                {
                    close();
                    return m_Socket = _Val;
                }
                Socket_t detach()
                {
                    Socket_t Socket( m_Socket );
                    m_Socket = -1;
                    return Socket;
                }
                Socket_t get()
                {
                    return m_Socket;
                }
                int set_nonblock( bool _val = true )
                {
                    int opts = fcntl ( m_Socket, F_GETFL );

                    if ( opts < 0 )
                        return -1;

                    opts = _val ? ( opts | O_NONBLOCK ) : ( opts & ~O_NONBLOCK );

                    return fcntl ( m_Socket, F_SETFL, opts );
                }
                void close()
                {
                    if ( m_Socket > 0 )
                        ::close( m_Socket );
                }
                int shutdown( int _How = SHUT_RDWR )
                {
                    return ::shutdown( m_Socket, m_Socket );
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

        //TODO: sendall - Make this code safer!!!
        inline int sendall( int s, char *buf, int len, int flags )
        {
            int total = 0;
            int n;

            while ( total < len )
            {
                n = send( s, buf + total, len - total, flags );
                if ( n == -1 )
                    break;
                total += n;
            }

            return ( n == -1 ? -1 : total );
        }

        class CSocketServer
        {
            public:
                CSocketServer() : m_ServerSocket( AF_INET, SOCK_STREAM, 0 )
                {}
                void Bind( unsigned short _nPort, const std::string *_Addr = NULL ) throw ( std::exception )
                {
                    if ( m_ServerSocket < 0 )
                        throw std::runtime_error( socket_error_string( m_ServerSocket, "NULL socket has been given to Bind" ) );

                    sockaddr_in addr;
                    addr.sin_family = AF_INET;
                    addr.sin_port = htons( _nPort );
                    if ( !_Addr )
                        addr.sin_addr.s_addr = htonl( INADDR_ANY );
                    else
                        inet_aton( _Addr->c_str(), &addr.sin_addr );

                    if ( bind( m_ServerSocket, ( struct sockaddr * ) & addr, sizeof( addr ) ) < 0 )
                        throw std::runtime_error( socket_error_string( m_ServerSocket, "Soket bind error..." ) );
                }

                void Listen( int _Backlog ) throw ( std::exception )
                {
                    if ( ::listen( m_ServerSocket, _Backlog ) < 0 )
                        throw std::runtime_error( socket_error_string( m_ServerSocket, "can't call listen on socket server" ) );
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
                        throw std::runtime_error( socket_error_string( m_ClientSocket, "there was NULL socket given as a client socket to Connect" ) );

                    sockaddr_in addr;
                    addr.sin_family = AF_INET;
                    addr.sin_port = htons( _nPort );
                    inet_aton( _Addr.c_str(), &addr.sin_addr );

                    if ( ::connect( m_ClientSocket, ( struct sockaddr * ) & addr, sizeof( addr ) ) < 0 )
                        throw std::runtime_error( socket_error_string( m_ClientSocket, "Can't connect to the server" ) );
                }

                smart_socket& GetSocket()
                {
                    return m_ClientSocket;
                }

            private:
                smart_socket m_ClientSocket;
        };


        struct SSocket2String_Trait
        {
            bool operator() ( Socket_t _socket, sockaddr_in *_addr ) const
            {
                size_t size = sizeof( sockaddr );
                return ( getsockname( _socket, reinterpret_cast<sockaddr *>( _addr ), &size ) == -1 ) ? false : true;
            }
        };

        struct SSocketPeer2String_Trait
        {
            bool operator() ( Socket_t _socket, sockaddr_in *_addr ) const
            {
                size_t size = sizeof( sockaddr );
                return ( getpeername( _socket, reinterpret_cast<sockaddr *>( _addr ), &size ) == -1 ) ? false : true;
            }
        };

        template <class _Type>
        struct _socket2string
        {
            _socket2string ( Socket_t _Socket, std::string *_Str )
            {
                if ( !_Str )
                    return ;

                sockaddr_in addr;
                if ( !_Type() ( _Socket, &addr ) )
                    return ;

                std::stringstream ss;
                ss
                << inet_ntoa( addr.sin_addr )
                << ":"
                << ntohs( addr.sin_port );
                *_Str = ss.str();
            }
        };

        typedef _socket2string<SSocket2String_Trait> socket2string;
        typedef _socket2string<SSocketPeer2String_Trait> peer2string;

        inline std::string socket_error_string( Socket_t _socket, char *_strMsg )
        {
            std::string strSocket;
            socket2string( _socket, &strSocket );
            std::string strSocketPeer;
            peer2string( _socket, &strSocketPeer );
            std::string sErr;
            MiscCommon::errno2str( &sErr );

            std::stringstream ss;
            if ( _strMsg )
            {
                ss << _strMsg << "\n";
            }
            ss
            << "Error on Socket<"
            << strSocket << ">";

            if ( !strSocketPeer.empty() )
            {
                ss << "and peer <" << strSocketPeer << ">";
            }
            ss << ": " << sErr;

            return ss.str();
        }
    };
};

#endif
