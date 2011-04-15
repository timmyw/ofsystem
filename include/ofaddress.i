/*
 * $OFPREPARED$
 *
 * $Revision: 2.0 $
 *
 * $Id: ofaddress.i,v 2.0 2006-01-01 20:08:32 timmy Exp $
 */

inline
OFAddress::OFAddress( ofint16 port, const char *host ) :
    m_valid( 0 )
{
    set( port, host );
}

inline
OFAddress::OFAddress( const OFAddress &copy )
{
    memcpy( &m_addr, &copy.m_addr, sizeof(m_addr) );
    m_valid = copy.m_valid;
}

inline
OFAddress::~OFAddress( )
{
}

inline
void
OFAddress::ipaddress( char *buffer )
{
    OFOS::strcpy( buffer, inet_ntoa( m_addr.sin_addr ) );
}

inline
ofuint16
OFAddress::port( ) const
{
    return ntohs( m_addr.sin_port );
}

inline
const sockaddr_in *
OFAddress::address()
{
    return &m_addr;
}

inline
const bool
OFAddress::isValid() const
{
    return m_valid;
}
