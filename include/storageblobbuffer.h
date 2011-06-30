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

#if !defined(_OFSYS_H_INCLUDED_)
#error "Please do not include this file directly.  Include <ofsys.h>"
#endif

#if !defined(_STORAGEBLOBBUFFER_H)
#define _STORAGEBLOBBUFFER_H

/** \class StorageBlobBuffer storageblob.h

    Helper class for turning instances into blobs for writing to
    network and restoring the objects from the blobs.

    Everything is stored in little end format regardless of the
    endian-ness of the hardware.

    There are no separate sizing functions as the write functions
    serve as sizing functions if the buffer has overlowed. This allows
    an data to be written to a fixed buffer (of say 10,000 bytes) and
    only reprocessed if this is not big enough.

    If the buffer is flagged it will be freed on destruction.
*/

class OFSYS_API StorageBlobBuffer : public StorageBlob
{
 private:
    unsigned char* m_blob; // base of blob being read/written
    int m_left; // number of bytes left in blob
    unsigned char* m_ptr;  // pointer to current position in blob
    // (may be beyond end of blob if sizing)
    bool m_dyn;  // blob has been dynamically allocated and
    // must be freed

 public:

    /**
     * ctor to create bufferless blob for sizing
     */
    StorageBlobBuffer();

    /**
     * ctor to create blob with a buffer either for reading or for writing
     * If reading then the buffer size is ignored and StorageBlob will happily
     * read random memory beyond the end of the buffer.
     */
    StorageBlobBuffer(unsigned char *aBlob, int aSize, bool dynFlag);

    /**
     * dtor frees buffer if flagged as dynamic
     */
    virtual ~StorageBlobBuffer();

    void makeEmpty();

    void bufferTaken();

    /**
     * initialise blob to hold copy of given memory
     */
    void setupBlob( unsigned char *aBlob, int aSize );

    /**
     * returns true if the buffer has overflowed during writing.
     */
    bool hasOverflowed();

    /**
     * allocates a dynamic buffer just large enough for the data that was sized
     * by writing.
     */
    void allocate ();

    /**
     * returns the address of the meory buffer
     */
    unsigned char *getBlob();

    /**
     * returns the size of the memory buffer
     */
    int getSize();

    char *tempReadString( );
    /**
     * write the boolean to the buffer. Reduced to byte giving true/false.
     */
    virtual void writeBool(bool aBool);

    /**
     * write a 8-bit integer
     */
    virtual void writeInt08(ofint32 anInt);

    /**
     * write a 16-bit integer
     */
    virtual void writeInt16(ofint32 anInt);

    /**
     * write a 32-bit integer
     */
    virtual void writeInt32(ofint32 anInt);

    /**
     * write a 64-bit integer
     */
    virtual void writeInt64(ofuint64 anInt);

    /**
     * write a 'C' string
     */
    virtual void writeString(const char *aString);

    /**
     * write an OFIDENTITY
     */
    virtual void writeIdentity(const OFIDENTITY *anId);

    /** Write a SRVIDENTITY
     */
    virtual void writeServerIdentity( const SRVIDENTITY *anId );
    
    /** Write a list of identities.
     */
    virtual void writeIdentityList( vector<OFIDENTITY *> *list );

    /**
     * write a double floating point number
     */
    virtual void writeDouble (double aDouble);

    /** Write a binary value.
     */
    virtual void writeBinary( void *data, ofuint32 size );

    /**
     * Reads a boolean from the buffer
     */
    virtual bool readBool();

    /**
     * Reads a 8-bit integer from the buffer
     */
    virtual ofint32 readInt08();

    /**
     * Reads a 16-bit integer from the buffer
     */
    virtual ofint32 readInt16();

    /**
     * Reads a 32-bit integer from the buffer
     */
    virtual ofint32 readInt32();

    /**
     * Reads a 64-bit integer from the buffer
     */
    virtual ofuint64 readInt64();

    /**
     * Reads a C string and returns it in a 'new char[]'
     */
    virtual char *readString();

    /**
     * Reads a C string into the buffer provided. It assumes that the buffer
     * is large enough for the string
     */
    virtual void readString(char *aString);

    /**
     * Reads on OFIDENTITY from the buffer
     */
    virtual void readIdentity(OFIDENTITY *anId);

    /** Read a SRVIDENTITY
     */
    virtual void readServerIdentity( SRVIDENTITY *anId );
    
    /** Read a list of OFIDENTITY.
     */
    virtual void readIdentityList( vector<OFIDENTITY*> *list );

    /**
     * Reads a double floating point number from the buffer
     */
    virtual double readDouble();

    /** Read a binary value.
     */
    virtual void readBinary( void *data, ofuint32 size );

    /** Set next read position
     */
    virtual void setReadPosition( ofuint32 pos );

    /** Set next write position
     */
    void setWritePosition( ofuint32 pos );

    /** Get current position
     */
    ofuint32 getCurrentPosition();
};

template<typename T>
void
write_blob( T *t, StorageBlobBuffer *b )
{
    t->write( b );
    if ( b->hasOverflowed() )
    {
        b->allocate( );
        t->write( b );
    }
}

#endif // _STORAGEBLOBBUFFER_H
