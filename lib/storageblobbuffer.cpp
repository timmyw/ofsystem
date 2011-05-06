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
#include <storageblobbuffer.h>

#if defined(OFOPSYS_SOLARIS)
#include <strings.h>
#endif

StorageBlobBuffer::StorageBlobBuffer(  ):
m_blob( NULL ), m_left( -1 ), m_ptr( NULL ), m_dyn( false )
{
}


StorageBlobBuffer::StorageBlobBuffer( unsigned char *aBlob,
                                      int aSize,
                                      bool dynFlag ):
m_blob( aBlob ),
m_left( aSize ),
m_ptr( aBlob ),
m_dyn( dynFlag )
{
}


StorageBlobBuffer::~StorageBlobBuffer(  )
{
    if ( m_dyn )
        delete [] m_blob;
}

void
StorageBlobBuffer::makeEmpty()
{
    if ( m_dyn )
        delete [] m_blob;
    m_blob = m_ptr = NULL;
    m_left = -1;
    m_dyn = false;
}

void
StorageBlobBuffer::bufferTaken( )
{
    m_dyn = false;
}

void
StorageBlobBuffer::setupBlob( unsigned char *aBlob, int aSize )
{
     m_ptr = m_blob = new unsigned char [ aSize ];
     m_left = aSize;
     m_dyn = true;
     bcopy( aBlob, m_blob, aSize );
}

bool
StorageBlobBuffer::hasOverflowed(  )
{
    return m_left < 0;
}

void
StorageBlobBuffer::allocate(  )
{
    m_left = m_ptr - m_blob;
    if ( m_dyn )
        delete[] ( m_blob );
    m_ptr = m_blob = new unsigned char[m_left];
    m_dyn = true;
}

unsigned char *
StorageBlobBuffer::getBlob(  )
{
    return m_blob;
}

int
StorageBlobBuffer::getSize(  )
{
    return m_ptr - m_blob;
}

void
StorageBlobBuffer::writeBool( bool aBool )
{
    if ( --m_left < 0 )
    {
        m_ptr++;
        return;
    }
    *m_ptr++ = aBool ? 1 : 0;
}


void
StorageBlobBuffer::writeInt08( ofint32 anInt )
{
    if ( ( m_left -- ) < 0 )
    {
        m_ptr ++;
        return;
    }
    *m_ptr++ = static_cast < char >( anInt );
}

void
StorageBlobBuffer::writeInt16( ofint32 anInt )
{
    if ( ( m_left -= 2 ) < 0 )
    {
        m_ptr += 2;
        return;
    }
    *m_ptr++ = static_cast < char >( anInt >> 8 );
    *m_ptr++ = static_cast < char >( anInt );
}

void
StorageBlobBuffer::writeInt32( ofint32 anInt )
{
    if ( ( m_left -= 4 ) < 0 )
    {
        m_ptr += 4;
        return;
    }
    *m_ptr++ = static_cast < char >( anInt >> 24 );
    *m_ptr++ = static_cast < char >( anInt >> 16 );
    *m_ptr++ = static_cast < char >( anInt >> 8 );
    *m_ptr++ = static_cast < char >( anInt );
}

void
StorageBlobBuffer::writeInt64( ofuint64 anInt )
{
    if ( ( m_left -= 8 ) < 0 )
    {
        m_ptr += 8;
        return;
    }
    *m_ptr++ = static_cast < char >( anInt >> 56 );
    *m_ptr++ = static_cast < char >( anInt >> 48 );
    *m_ptr++ = static_cast < char >( anInt >> 40 );
    *m_ptr++ = static_cast < char >( anInt >> 32 );
    *m_ptr++ = static_cast < char >( anInt >> 24 );
    *m_ptr++ = static_cast < char >( anInt >> 16 );
    *m_ptr++ = static_cast < char >( anInt >> 8 );
    *m_ptr++ = static_cast < char >( anInt );
}

void
StorageBlobBuffer::writeString( const char *aString )
{
    int i = strlen( aString ) + 1;
    if ( ( m_left -= i ) >= 0 )
        OFOS::strcpy( ( char * ) m_ptr, aString );
    m_ptr += i;
}

void
StorageBlobBuffer::writeIdentity( const OFIDENTITY * anId )
{
    writeInt32( anId->m_flags );
    writeInt64( anId->m_id0 );
    writeInt64( anId->m_id1 );
}

void
StorageBlobBuffer::readIdentity( OFIDENTITY * anId )
{
    anId->m_flags = readInt32(  );
    anId->m_id0 = readInt64(  );
    anId->m_id1 = readInt64(  );
}

void
StorageBlobBuffer::writeServerIdentity( const SRVIDENTITY * anId )
{
    assert( anId );
    writeInt64( anId->m_id );
}

void
StorageBlobBuffer::readServerIdentity( SRVIDENTITY * anId )
{
    anId->m_id = readInt64(  );
}

void
StorageBlobBuffer::writeIdentityList( vector < OFIDENTITY * >*list )
{
    writeInt32( list->size(  ) );
    for ( vector < OFIDENTITY * >::iterator i = list->begin(  ); i != list->end(  ); i++ )
        writeIdentity( *i );
}

void
StorageBlobBuffer::writeDouble( double aDouble )
{
    if ( ( m_left -= 8 ) < 0 )
    {
        m_ptr += 8;
        return;
    }
    ofint64 anInt = *( ofint64 * ) ( &aDouble );
    *m_ptr++ = static_cast < char >( anInt >> 56 );
    *m_ptr++ = static_cast < char >( anInt >> 48 );
    *m_ptr++ = static_cast < char >( anInt >> 40 );
    *m_ptr++ = static_cast < char >( anInt >> 32 );
    *m_ptr++ = static_cast < char >( anInt >> 24 );
    *m_ptr++ = static_cast < char >( anInt >> 16 );
    *m_ptr++ = static_cast < char >( anInt >> 8 );
    *m_ptr++ = static_cast < char >( anInt );
}

void
StorageBlobBuffer::writeBinary( void *data,
                                ofuint32 size )
{
    if ( ( m_left -= size ) < 0 )
    {
        m_ptr += size;
        return;
    }
    memcpy( m_ptr, data, size );
    m_ptr += size;
}

bool
StorageBlobBuffer::readBool(  )
{
    return ( *m_ptr++ != 0 );
}

ofint32 StorageBlobBuffer::readInt08(  )
{
    ofuint32 anInt;
    anInt = *m_ptr++;
    return anInt;
}

ofint32 StorageBlobBuffer::readInt16(  )
{
    ofuint32 anInt;
    anInt = *m_ptr++ << 8;
    anInt |= *m_ptr++;
    return anInt;
}

ofint32 StorageBlobBuffer::readInt32(  )
{
    ofint32 anInt;
    anInt = *m_ptr++ << 24;
    anInt |= *m_ptr++ << 16;
    anInt |= *m_ptr++ << 8;
    anInt |= *m_ptr++;
    return anInt;
}

ofuint64 StorageBlobBuffer::readInt64(  )
{
    ofuint64 anInt;
    anInt = ( ( ofuint64 ) * m_ptr++ ) << 56;
    anInt |= ( ( ofuint64 ) * m_ptr++ ) << 48;
    anInt |= ( ( ofuint64 ) * m_ptr++ ) << 40;
    anInt |= ( ( ofuint64 ) * m_ptr++ ) << 32;
    anInt |= ( ( ofuint64 ) * m_ptr++ ) << 24;
    anInt |= *m_ptr++ << 16;
    anInt |= *m_ptr++ << 8;
    anInt |= *m_ptr++;
    return anInt;
}

char *
StorageBlobBuffer::readString(  )
{
    ofint32 len = strlen( ( char * ) m_ptr ) + 1;
    char *aString = new char[len];
    OFOS::strcpy( aString, ( const char * ) m_ptr );
    m_ptr += len;
    return aString;
}

char *
StorageBlobBuffer::tempReadString( )
{
    ofint32 len = strlen( ( char * ) m_ptr ) + 1;
    char *aString = ( char * ) m_ptr;
    m_ptr += len;
    return aString;
}

void
StorageBlobBuffer::readString( char *aString )
{
    OFOS::strcpy( aString, ( const char * ) m_ptr );
    m_ptr += strlen( ( char * ) m_ptr ) + 1;
}

void
StorageBlobBuffer::readIdentityList( vector < OFIDENTITY * >*list )
{
    ofuint32 count = readInt32(  );
    for ( ; count; count-- )
    {
        OFIDENTITY *id = new OFIDENTITY;
        readIdentity( id );
        list->push_back( id );
    }
}

double
StorageBlobBuffer::readDouble(  )
{
    ofint64 anInt;
    anInt = ( ( ofuint64 ) * m_ptr++ ) << 56;
    anInt |= ( ( ofuint64 ) * m_ptr++ ) << 48;
    anInt |= ( ( ofuint64 ) * m_ptr++ ) << 40;
    anInt |= ( ( ofuint64 ) * m_ptr++ ) << 32;
    anInt |= ( ( ofuint64 ) * m_ptr++ ) << 24;
    anInt |= *m_ptr++ << 16;
    anInt |= *m_ptr++ << 8;
    anInt |= *m_ptr++;
    return *( ( double * ) &anInt );
}

void
StorageBlobBuffer::readBinary( void *data,
                               ofuint32 size )
{
    memcpy( data, m_ptr, size );
    m_ptr += size;
}

void
StorageBlobBuffer::setReadPosition( ofuint32 pos )
{
     m_ptr = m_blob + pos;
}

void
StorageBlobBuffer::setWritePosition( ofuint32 pos )
{
     m_ptr = m_blob + pos;
     m_left = 0x7fffffff;
}

ofuint32
StorageBlobBuffer::getCurrentPosition()
{
        return m_ptr-m_blob;
}
