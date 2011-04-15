/* $Revision: 2.0 $

$Id: ofaddress.cpp,v 2.0 2006-01-01 19:26:31 timmy Exp $
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
