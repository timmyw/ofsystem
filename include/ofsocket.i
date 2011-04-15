/*
* $OFPREPARED$
*
* $Revision: 2.0 $
*
* $Id: ofsocket.i,v 2.0 2006-01-01 20:08:32 timmy Exp $
*/

inline
ofint32
OFSocket::connect( )
{
     return connect( m_addr );
}

inline
bool
OFSocket::isConnected() const
{
     return m_open;
}

inline
bool
OFSocket::isValid( ) const
{
     return !(m_socket==OFOS::OF_INVALID_SOCKET);
}

inline
OFOS::of_socket_t
OFSocket::handle( ) const
{
     return m_socket;
}
