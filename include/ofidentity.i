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

#include <iomanip>

inline
OFIDENTITY::OFIDENTITY( ) :
    m_id0( MAKEULL( 0 ) ),
    m_id1( MAKEULL( 0 ) ),
    m_flags( 0 )
{
}

inline
OFIDENTITY::OFIDENTITY( ofuint64 hi, ofuint64 lo, ofuint16 flags /* = 0 */ ) :
    m_id0( hi ),
    m_id1( lo ),
    m_flags( flags )
{
}

inline
OFIDENTITY::OFIDENTITY( const char *strId ) :
    m_id0( MAKEULL( 0 ) ),
    m_id1( MAKEULL( 0 ) ),
    m_flags( 0 )
{
    if (OFOS::strlen(strId) < OF_MAX_ID_LEN)
        return;
    char temp[17];
    OFOS::strncpy2( temp, strId, 4 );
    OFOS::sscanf( temp, "%x", (int*)&m_flags );
    OFOS::strncpy2( temp, strId+4, 16 );
#if defined(OFOPSYS_WIN32)
    OFOS::sscanf( temp, "%I64x", &m_id0 );
    OFOS::sscanf( strId+20, "%I64x", &m_id1 );
#else
    OFOS::sscanf( temp, "%llx", &m_id0 );
    OFOS::sscanf( strId+20, "%llx", &m_id1 );
#endif
}

inline
OFIDENTITY::OFIDENTITY( const OFIDENTITY &rhs ) :
    m_id0( rhs.m_id0 ),
    m_id1( rhs.m_id1 ),
    m_flags( rhs.m_flags )
{
}

inline
OFIDENTITY::~OFIDENTITY( )
{
}

inline
OFIDENTITY &
OFIDENTITY::operator =( const OFIDENTITY &rhs )
{
	if ( this != &rhs )
	{
	    m_flags = rhs.m_flags;
	    m_id0 = rhs.m_id0;
		m_id1 = rhs.m_id1;
	}
    return *this;
}

inline
OFIDENTITY &
OFIDENTITY::operator =( const char *strId )
{
    if ( ofstrlen( strId ) < OF_MAX_ID_LEN )
    {
        m_id0 = MAKEULL( 0 );
        m_id1 = MAKEULL( 0 );
        m_flags = 0;
        return *this;
    }
    char temp[17];
    OFOS::strncpy2( temp, strId, 4 );
    OFOS::sscanf( temp, "%x", (int*)&m_flags );
    OFOS::strncpy2( temp, strId+4, 16 );
#if defined(OFOPSYS_WIN32)
    OFOS::sscanf( temp, "%I64x", &m_id0 );
#else
    OFOS::sscanf( temp, "%llx", &m_id0 );
#endif

    OFOS::strncpy2( temp, strId+20, 16 );
#if defined(OFOPSYS_WIN32)
    OFOS::sscanf( temp, "%I64x", &m_id1 );
#else
    OFOS::sscanf( temp, "%llx", &m_id1 );
#endif

    return *this;
}

inline
void
OFIDENTITY::assign( const char *data, ofuint32 dataLength )
{
    if ( dataLength >= OF_MAX_ID_LEN )
    {
        char temp[17];
        memcpy( temp, data, 4 ); temp[4] = 0;
        OFOS::sscanf( temp, "%x", (int*)&m_flags );
        memcpy( temp, data+4, 16 ); temp[16] = 0;
#if defined(OFOPSYS_WIN32)
        OFOS::sscanf( temp, "%I64x", &m_id0 );
#else
        OFOS::sscanf( temp, "%llx", &m_id0 );
#endif
        memcpy( temp, data+20, 16 ); temp[16] = 0;
#if defined(OFOPSYS_WIN32)
        OFOS::sscanf( temp, "%I64x", &m_id1 );
#else
        OFOS::sscanf( temp, "%llx", &m_id1 );
#endif
    }
    else
    {
        m_id0 = MAKEULL( 0 );
        m_id1 = MAKEULL( 0 );
        m_flags = 0;
    }
}

#if defined(NDEBUG)
inline
int
OFIDENTITY::operator ==( const OFIDENTITY &rhs ) const
{
    return ( m_id0 == rhs.m_id0 && m_id1 == rhs.m_id1 );
}

inline
int
OFIDENTITY::operator ==( const char *rhs ) const
{
    ofuint64 id0, id1;
    char temp[17];
    OFOS::strncpy2( temp, rhs+4, 16 );
    sscanf( temp, "%llx", &id0 );
    sscanf( rhs+20, "%llx", &id1 );

    cout << *this << endl;
    cout << "operator ==(const char*):" << hex << setfill('0') << setw(16) << id0 << setw(16) << id1 << dec << endl;

    return ( m_id0 == id0 && m_id1 == id1 );
}

/*inline
int
operator == ( const OFIDENTITY &lhs, const OFIDENTITY &rhs)
{
     return lhs.m_id1 == rhs.m_id1 && lhs.m_id0 == rhs.m_id0;
}*/

inline
int
OFIDENTITY::operator !=( const OFIDENTITY &rhs ) const
{
    return ( m_id0 != rhs.m_id0 || m_id1 != rhs.m_id1 );
}
#endif

inline
char *
OFIDENTITY::cstr( char *id ) const
{
#if defined(OFOPSYS_WIN32)
    OFOS::snprintf(id, OF_MAX_ID_LEN, "%04x%016I64x%016I64x", m_flags, m_id0, m_id1 );
#else
    sprintf( id, "%04x%016llx%016llx", m_flags, m_id0, m_id1 );
#endif
    return id;
}
