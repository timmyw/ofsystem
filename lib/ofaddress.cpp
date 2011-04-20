/*
  Copyright (C) 1997-2011 by Suntail.com AS, Tim Whelan

  Permission is hereby granted, free of charge, to any person
  obtaining a copy of this software and associated documentation files
  (the "Software"), to deal in the Software without restriction,
  including without limitation the rights to use, copy, modify, merge,
  publish, distribute, sublicense, and/or sell copies of the Software,
  and to permit persons to whom the Software is furnished to do so,
  subject to the following conditions:

  The above copyright notice and this permission notice shall be
  included in all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
  ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

#include <ofsys.h>
#include <ofaddress.h>

OFAddress::OFAddress( ) :
    m_valid( 0 )
{
    memset( &m_addr, 0, sizeof( m_addr ) );
    m_addr.sin_family = AF_INET;
}

OFAddress::OFAddress( ofint16 port ) :
    m_valid( 0 )
{
    memset( &m_addr, 0, sizeof( m_addr ) );
    m_addr.sin_family = AF_INET;
    m_addr.sin_addr.s_addr = INADDR_ANY;
    m_addr.sin_port = htons(port);
}

OFAddress::OFAddress( ofint16 port, ofint32 ipAddress ) :
    m_valid( 1 )
{
    memset( &m_addr, 0, sizeof( m_addr ) );
    m_addr.sin_family = AF_INET;
    m_addr.sin_port = htons( port );
    m_addr.sin_addr.s_addr = htonl( ipAddress );
}

void
OFAddress::set( ofuint16 port, const char *host )
{
    memset( &m_addr, 0, sizeof( m_addr ) );
    m_addr.sin_family = AF_INET;
    m_addr.sin_port = htons(port);

#if defined(OFOPSYS_WIN32)
    if ( inet_addr( host ) != INADDR_NONE )
    {
        m_addr.sin_addr.S_un.S_addr = inet_addr( host );
        m_valid = true;
    }
#else
    struct in_addr address;
#if defined(OFOPSYS_SOLARIS)
    if ( inet_pton( AF_INET, host, &address ) )
#else
        if ( inet_aton( host, &address ) )
#endif
        {
            memcpy( &m_addr.sin_addr, &address, sizeof(address) );
            m_valid = true;
        }
#endif // #if defined(OFOPSYS_WIN32)

    if ( !m_valid )
    {
        struct hostent *he = gethostbyname( host );
        if ( he )
        {
            memcpy( &m_addr.sin_addr, he->h_addr_list[0], sizeof(m_addr.sin_addr) );
            m_valid = true;
        }
    }
}

void
OFAddress::set( ofuint16 port, sockaddr_in *addr )
{
    memset( &m_addr, 0, sizeof( m_addr ) );
    m_addr.sin_family = AF_INET;
    m_addr.sin_port = htons(port);
    m_addr.sin_addr.s_addr = addr->sin_addr.s_addr;
    m_valid = true;
}

const char *
OFAddress::hostname()
{
    struct hostent *he = gethostbyaddr( reinterpret_cast<char*>(&m_addr.sin_addr), sizeof(m_addr.sin_addr), AF_INET );
    return (he)?he->h_name:0;
}

#if defined(UNIT_TEST)

#include <UnitTest.h>
int main()
{
    UT_BATCH_START( "OFAddress" );
    UT_BATCH_END( "OFAddress" );
    return 0;
}

#endif
