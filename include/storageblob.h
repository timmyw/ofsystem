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

#ifndef _STORAGEBLOB_H
#define _STORAGEBLOB_H

#include <ofsys.h>
#include <ofidentity.h>
#include <ofdatetime.h>

struct SRVIDENTITY; 

/** \class StorageBlob storageblob.h
    Virtual class which defines required functionality of writing/reading in a
    machine independent fashion either to/from a memory buffer or to/from a
    OpenFabric Object Repository data item.
*/
class OFSYS_API StorageBlob
{
 public:

    /** Empty destructor
     */
    virtual ~StorageBlob() {;}

    /**
     * write the boolean to the buffer. Reduced to byte giving true/false.
     */
    virtual void writeBool(bool aBool) = 0;

    /**
     * write a 8-bit integer
     */
    virtual void writeInt08(ofint32 anInt) = 0;

    /**
     * write a 16-bit integer
     */
    virtual void writeInt16(ofint32 anInt) = 0;

    /**
     * write a 32-bit integer
     */
    virtual void writeInt32(ofint32 anInt) = 0;

    /**
     * write a 64-bit integer
     */
    virtual void writeInt64(ofuint64 anInt) = 0;

    /**
     * write a 'C' string
     */
    virtual void writeString(const char *aString) = 0;

    /**
     * write an OFIDENTITY
     */
    virtual void writeIdentity(const OFIDENTITY *anId) = 0;

    /** Write a OFDateTime
     */
    void writeDateTime(const OFDateTime *time);

    /** Write a SRVIDENTITY
     */
    virtual void writeServerIdentity( const SRVIDENTITY *anId ) = 0;

    /**
     * write a double floating point number
     */
    virtual void writeDouble (double aDouble) = 0;

    /** Write a binary value.
     */
    virtual void writeBinary( void *data, ofuint32 size ) = 0;

    /**
     * Reads a boolean from the buffer
     */
    virtual bool readBool() = 0;

    /**
     * Reads a 8-bit integer from the buffer
     */
    virtual ofint32 readInt08() = 0;

    /**
     * Reads a 16-bit integer from the buffer
     */
    virtual ofint32 readInt16() = 0;

    /**
     * Reads a 32-bit integer from the buffer
     */
    virtual ofint32 readInt32() = 0;

    /**
     * Reads a 64-bit integer from the buffer
     */
    virtual ofuint64 readInt64() = 0;

    /**
     * Reads a C string and returns it in a 'new char[]'
     */
    virtual char *readString() = 0;

    /**
     * Reads a C string into the buffer provided. It assumes that the buffer
     * is large enough for the string
     */
    virtual void readString(char *aString) = 0;

    /**
     * Reads on OFIDENTITY from the buffer
     */
    virtual void readIdentity(OFIDENTITY *anId) = 0;

    /** Read a OFDateTime
     */
    void readDateTime(OFDateTime *time);

    /** Read a SRVIDENTITY
     */
    virtual void readServerIdentity( SRVIDENTITY *anId ) = 0;

    /**
     * Reads a double floating point number from the buffer
     */
    virtual double readDouble() = 0;

    /** Read a binary value.
     */
    virtual void readBinary( void *data, ofuint32 size ) = 0;

    /** Set next read position
     */
    virtual void setReadPosition( ofuint32 pos ) = 0;

};

#endif // _STORAGEBLOB_H
