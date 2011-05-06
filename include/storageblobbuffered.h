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

#ifndef _STORAGEBLOBBUFFERED_H
#define _STORAGEBLOBBUFFERED_H

#include <ofsys.h>
#include <srvidentity.h>
#include <storageblob.h>
#include <buffers.h>

/**
   \class StorageBlobBuffer storageblob.h
   Helper class for turning instances into blobs for writing to network
   and restoring the objects from the blobs.

   Everything is stored in little end format regardless of the endian-ness of the
   hardware.

   There are no separate sizing functions as the write functions serve as sizing
   functions if the buffer has overlowed. This allows an data to be written to
   a fixed buffer (of say 10,000 bytes) and only reprocessed if this is not big
   enough.

   If the buffer is flagged it will be freed on destruction.
*/

class OFSYS_API StorageBlobBuffered : public StorageBlob
{
 private:
     BufferPointer m_bp;
     ofuint64 m_readPos;

 public:

    /**
     * ctor to create blob for writing to its own Buffer
     */
    StorageBlobBuffered();

    /**
     * ctor to create blob for reading/writing from given Buffer
     */
    StorageBlobBuffered( BufferPointer bp);

    /**
     * dtor
     */
    virtual ~StorageBlobBuffered();

    /**
     * returns the Buffer
     */
    BufferPointer getBuffer();

    /**
     * returns the buffer size
     */
    ofuint64 getSize();

    /**
     * sets the Buffer
     */
    void setupBuffer( BufferPointer bp );

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

    /** Get the read position
     */
    virtual ofuint32 getReadPosition() { return m_readPos; }

};

#endif // _STORAGEBLOBBUFFERED_H
