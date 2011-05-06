/* $Revision: 2.0 $

   $Id: storageblob.i,v 2.0 2006-01-01 20:08:30 timmy Exp $
*/

inline 
StorageBlob::StorageBlob() : 
     m_blob(NULL), m_left(-1), m_ptr(NULL), m_dyn(false)
{
}

inline 
StorageBlob::StorageBlob(unsigned char *aBlob, int aSize, bool dynFlag)
     : m_blob(aBlob), m_left(aSize), m_ptr(aBlob), m_dyn(dynFlag)
{
}

inline 
StorageBlob::~StorageBlob()
{
     if (m_dyn) free (m_blob);
}

inline
void
StorageBlob::writeBool(bool aBool)
{
     if ( --m_left < 0 )
     {
	  m_ptr++;
	  return;
     }
     *m_ptr++ = aBool ? 1 : 0;
}

inline
void
StorageBlob::writeInt(ofint32 anInt)
{
     if ( (m_left -= 4 ) < 0 )
     {
	  m_ptr += 4;
	  return;
     }
     *m_ptr++ = static_cast<char>(anInt);
     *m_ptr++ = static_cast<char>(anInt >> 8);
     *m_ptr++ = static_cast<char>(anInt >> 16);
     *m_ptr++ = static_cast<char>(anInt >> 24);
}

inline void
StorageBlob::writeUInt(ofuint32 anInt)
{
     if ((m_left -= 4) < 0) 
     {
	  m_ptr += 4;
	  return;
     }
     *m_ptr++ = static_cast<char>(anInt);
     *m_ptr++ = static_cast<char>(anInt >> 8);
     *m_ptr++ = static_cast<char>(anInt >> 16);
     *m_ptr++ = static_cast<char>(anInt >> 24);
}

inline void
StorageBlob::writeString(const char *aString)
{
     int i = strlen(aString) + 1;
     if ( (m_left -= i ) >= 0 )
	  OFOS::strcpy( (char *)m_ptr, aString );
     m_ptr += i;
}

inline void
StorageBlob::writeIdentity(const OFIDENTITY *anId)
{
    writeInt32( anId->m_flags );
    writeInt64( anId->m_id0 );
    writeInt64( anId->m_id1 );
}

inline void
StorageBlob::writeServerIdentity( const SRVIDENTITY *anId )
{
    writeInt64( anId->m_id );
}

inline void
StorageBlob::writeDouble (double aDouble)
{
     if ( (m_left -= 8 ) < 0) 
     {
	  m_ptr += 8;
	  return;
     }
     char *pos = (char *)&aDouble;
     for ( ofint32 x=0; x<8; x++ )
	  *m_ptr++ = *pos++;
}

inline
void
StorageBlob::writeBinary( void *data, ofuint32 size )
{
     if ( ( m_left -= size ) < 0 )
     {
	  m_ptr += size;
	  return;
     }
     memcpy( m_ptr, data, size );
     m_ptr += size;
}

inline bool
StorageBlob::hasOverflowed() {return m_left < 0;}

inline void
StorageBlob::allocate ()
{
     m_left = m_ptr - m_blob;
     if (m_dyn) free (m_blob);
     m_ptr = m_blob = (unsigned char *)malloc (m_left);
     m_dyn = true;
}

inline unsigned char *
StorageBlob::getBlob() {return m_blob;}

inline int
StorageBlob::getSize() {return m_ptr - m_blob;}

inline bool
StorageBlob::readBool()
{
     return (*m_ptr++ != 0);
}

inline ofint32
StorageBlob::readInt()
{
     ofint32 anInt;
     anInt = *m_ptr++;
     anInt |= *m_ptr++ << 8;
     anInt |= *m_ptr++ << 16;
     anInt |= *m_ptr++ << 24;
     return anInt;
}

inline ofuint32
StorageBlob::readUInt()
{
     ofuint32 anInt;
     anInt = *m_ptr++;
     anInt |= *m_ptr++ << 8;
     anInt |= *m_ptr++ << 16;
     anInt |= *m_ptr++ << 24;
     return anInt;
}

inline
const char *
StorageBlob::readString()
{
     const char *aString = (const char *)m_ptr;
     m_ptr += strlen((char *)m_ptr) + 1;
     return aString;
}

inline void
StorageBlob::readIdentity(OFIDENTITY *anId)
{
    anId->m_flags = readInt32(  );
    anId->m_id0 = readInt64(  );
    anId->m_id1 = readInt64(  );
}

inline void
StorageBlob::readServerIdentity( SRVIDENTITY *anId )
{
    anId->m_id = readInt64(  );
}

inline double
StorageBlob::readDouble()
{
     double d = *(double *)m_ptr;
     m_ptr += sizeof( double );

     return d;
}

inline
void
StorageBlob::readBinary( void *data, ofuint32 size )
{
     memcpy( data, m_ptr, size );
     m_ptr += size;
}
