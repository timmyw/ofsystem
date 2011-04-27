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
#include <storageblobbuffered.h>

StorageBlobBuffered::StorageBlobBuffered(  )
     : m_readPos( 0 )
{
}


StorageBlobBuffered::StorageBlobBuffered( BufferPointer bp )
     : m_bp( bp ),
       m_readPos( 0 )
{
}


StorageBlobBuffered::~StorageBlobBuffered(  )
{
}

BufferPointer
StorageBlobBuffered::getBuffer(  )
{
    return m_bp;
}

ofuint64
StorageBlobBuffered::getSize( )
{
     return m_bp.size();
}

void
StorageBlobBuffered::setupBuffer( BufferPointer bp )
{
     m_bp = bp;
     m_readPos = 0;
}

void
StorageBlobBuffered::writeBool( bool aBool )
{
     char x[1];

     x[0] = aBool ? 1 : 0;
     m_bp.append( x, 1 );
}


void
StorageBlobBuffered::writeInt08( ofint32 anInt )
{
     char x[1];

     x[0] = anInt;
     m_bp.append( x, 1 );
}

void
StorageBlobBuffered::writeInt16( ofint32 anInt )
{
     char x[2];
     x[0] = ( anInt >> 8 );
     x[1] = ( anInt );
     m_bp.append( x, 2 );
}

void
StorageBlobBuffered::writeInt32( ofint32 anInt )
{
     char x[4];
     x[0] = ( anInt >> 24 );
     x[1] = ( anInt >> 16 );
     x[2] = ( anInt >> 8 );
     x[3] = ( anInt );
     m_bp.append( x, 4 );
}

void
StorageBlobBuffered::writeInt64( ofuint64 anInt )
{
     char x[8];
     x[0] = ( anInt >> 56 );
     x[1] = ( anInt >> 48 );
     x[2] = ( anInt >> 40 );
     x[3] = ( anInt >> 32 );
     x[4] = ( anInt >> 24 );
     x[5] = ( anInt >> 16 );
     x[6] = ( anInt >> 8 );
     x[7] = ( anInt );
     m_bp.append( x, 8 );
}

void
StorageBlobBuffered::writeString( const char *aString )
{
     int i = strlen( aString ) + 1;
     m_bp.append( aString, i );
}

void
StorageBlobBuffered::writeIdentity( const OFIDENTITY * anId )
{
    writeInt32( anId->m_flags );
    writeInt64( anId->m_id0 );
    writeInt64( anId->m_id1 );
}

void
StorageBlobBuffered::readIdentity( OFIDENTITY * anId )
{
    anId->m_flags = readInt32(  );
    anId->m_id0 = readInt64(  );
    anId->m_id1 = readInt64(  );
}

void
StorageBlobBuffered::writeServerIdentity( const SRVIDENTITY * anId )
{
    assert( anId );
    writeInt64( anId->m_id );
}

void
StorageBlobBuffered::readServerIdentity( SRVIDENTITY * anId )
{
    anId->m_id = readInt64(  );
}

void
StorageBlobBuffered::writeIdentityList( vector < OFIDENTITY * >*list )
{
    writeInt32( list->size(  ) );
    for ( vector < OFIDENTITY * >::iterator i = list->begin(  );
	  i != list->end(  ); i++ )
        writeIdentity( *i );
}

void
StorageBlobBuffered::writeDouble( double aDouble )
{
    ofint64 anInt = *( ofint64 * ) ( &aDouble );
    writeInt64( anInt );
}

void
StorageBlobBuffered::writeBinary( void *data,
                                ofuint32 size )
{
     m_bp.append( (char *)data, size );
}

bool
StorageBlobBuffered::readBool(  )
{
     char x[1];

     m_bp.readBytes( m_readPos, x, 1 );
     m_readPos++;
     return ( x[0] != 0 );
}

ofint32 StorageBlobBuffered::readInt08(  )
{
     char x[1];

     m_bp.readBytes( m_readPos, x, 1 );
     m_readPos++;
     return x[0];
}

ofint32 StorageBlobBuffered::readInt16(  )
{
    ofuint32 anInt;
    unsigned char x[2];

    m_bp.readBytes( m_readPos, ( char * )x, 2 );
    m_readPos += 2;
    anInt = ( x[0] << 8 ) | x[1];
    return anInt;
}

ofint32 StorageBlobBuffered::readInt32(  )
{
    ofuint32 anInt;
    unsigned char x[4];

    m_bp.readBytes( m_readPos, ( char * )x, 4 );
    m_readPos += 4;
    anInt = ( x[0] << 24 ) | ( x[1] << 16 ) | ( x[2] << 8 ) | x[3];
    return anInt;
}

ofuint64 StorageBlobBuffered::readInt64(  )
{
    ofuint64 anInt;
    unsigned char x[8];

    m_bp.readBytes( m_readPos, ( char * )x, 8 );
    m_readPos += 8;
    anInt = ( ((ofuint64)x[0]) << 56 ) | ( ((ofuint64)x[1]) << 48 )
	 | ( ((ofuint64)x[2]) << 40 ) | ( ((ofuint64)x[3]) << 32 )
	 | ( ((ofuint64)x[4]) << 24 ) | ( ((ofuint64)x[5]) << 16 )
	 | ( ((ofuint64)x[6]) << 8 ) | ((ofuint64)x[7]);
    return anInt;
}

char *
StorageBlobBuffered::readString(  )
{
    ofint32 len = 0;
    char x[1];
    for (;;)
    {
	 if ( m_bp.readBytes( m_readPos + len, x, 1 ) != 1 ) break;
	 if ( x[0] == 0 ) break;
	 len++;
    }
    len++;
    char *aString = new char[len];
    m_bp.readBytes( m_readPos, aString, len );
    m_readPos += len;
    return aString;
}

void
StorageBlobBuffered::readString( char *aString )
{
    ofint32 len = 0;
    char x[1];
    for (;;)
    {
	 if ( m_bp.readBytes( m_readPos + len, x, 1 ) != 1 ) break;
	 if ( x[0] == 0 ) break;
	 len++;
    }
    len++;
    m_bp.readBytes( m_readPos, aString, len );
    m_readPos += len;
    return;
}

void
StorageBlobBuffered::readIdentityList( vector < OFIDENTITY * >*list )
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
StorageBlobBuffered::readDouble(  )
{
    ofint64 anInt;
    anInt = readInt64( );
    return *( ( double * ) &anInt );
}

void
StorageBlobBuffered::readBinary( void *data,
                               ofuint32 size )
{
     m_bp.readBytes( m_readPos, (char *)data, size );
     m_readPos += size;
}

void
StorageBlobBuffered::setReadPosition( ofuint32 pos )
{
     m_readPos = pos;
}
