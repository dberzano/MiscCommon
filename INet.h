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

namespace MiscCommon
{
    namespace INet
    {
        
        typedef int Socket_t;

        
        class smart_socket
        {
            public:
                smart_socket() : m_Socket( 0 )
                {}
                smart_socket( int _Socket ) : m_Socket( _Socket )
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
                operator int()
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

        
    };
};

#endif
