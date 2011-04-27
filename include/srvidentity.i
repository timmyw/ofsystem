/*
* $OFPREPARED$
*
* $Revision: 2.0 $
*
* $Id: srvidentity.i,v 2.0 2006-01-01 20:08:29 timmy Exp $
*/

inline
SRVIDENTITY::SRVIDENTITY( ) :
    m_id( 0x0000000000000000 )
{
}

inline
SRVIDENTITY::SRVIDENTITY( const char *value )
{
    assert( value );
#if defined(OFOPSYS_WIN32)
    sscanf( value, "%I64x", &m_id );
#else
    sscanf( value, "%llx", &m_id );
#endif
}

inline
SRVIDENTITY::SRVIDENTITY( const OFIDENTITY &id ) :
    m_id( id.m_id0 )
{
}

inline
SRVIDENTITY::SRVIDENTITY( const SRVIDENTITY &copy ) :
    m_id( copy.m_id )
{
}

inline
SRVIDENTITY &
SRVIDENTITY::operator = ( const SRVIDENTITY &copy )
{
    m_id = copy.m_id;
    return *this;
}

inline
SRVIDENTITY &
SRVIDENTITY::operator = ( const OFIDENTITY &id )
{
    m_id = id.m_id0;
    return *this;
}

inline
SRVIDENTITY &
SRVIDENTITY::operator = ( const char *copy )
{
    assert( copy );
#if defined(OFOPSYS_WIN32)
    sscanf( copy, "%I64x", &m_id );
#else
    sscanf( copy, "%llx", &m_id );
#endif
    return *this;
}

inline
ofuint32
SRVIDENTITY::operator ==( const SRVIDENTITY &rhs ) const
{
    return m_id == rhs.m_id;
}

inline
ofuint32
SRVIDENTITY::operator !=( const SRVIDENTITY &rhs ) const
{
    return m_id != rhs.m_id;
}

inline
bool
SRVIDENTITY::operator <( const SRVIDENTITY &rhs ) const
{
    return m_id < rhs.m_id;
}

inline
char *
SRVIDENTITY::cstr( char *strId ) const
{
#if defined(OFOPSYS_WIN32)
    sprintf( strId, "%016I64x", m_id );
#else
    sprintf( strId, "%016llx", m_id );
#endif
    return strId;
}
