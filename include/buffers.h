/*
 */

#if !defined(_BUFFERS_H_)
#define _BUFFERS_H_

class OFSYS_API VirtualChunk
{
 public:
    virtual ~VirtualChunk();
    virtual void deref() = 0;
    virtual ofuint64 size() = 0;
    virtual void ref() = 0;
    virtual void append( const char *, ofuint32 ) = 0;
    virtual bool startWith( const char * ) = 0;
    virtual ofint64 find ( ofuint64, ofuint64, const char *, ofuint32 *) = 0;
    virtual void copyToBuffer( char *, ofuint64, ofuint64 ) = 0;
    virtual bool nextBit( ofuint64, char **, ofuint64 * ) = 0;
    virtual void overWrite( ofuint64, const char *, ofuint32) = 0;

 protected:
    VirtualChunk();
};

struct MemoryUse
{
    char *data;
    ofuint32 len;
    ofuint32 offset;
    ofuint32 max;
};

class OFSYS_API Chunk : public VirtualChunk
{
public:

     static VirtualChunk *allocateChunk();
private:
     Chunk();
     virtual ~Chunk();

     int refCount;
     vector<MemoryUse> data;
     int lastFound;


     static ofuint32 defaultSize;

     virtual void deref();
     virtual ofuint64 size();
     virtual void ref();
     virtual void append( const char *, ofuint32 );
     virtual bool startWith( const char * );
     virtual ofint64 find ( ofuint64, ofuint64, const char *, ofuint32 *);
     virtual void copyToBuffer( char *, ofuint64, ofuint64 );
     virtual bool nextBit( ofuint64, char **, ofuint64 * );
     virtual void overWrite( ofuint64, const char *, ofuint32);

     bool dataAt( ofuint32, char **, ofuint32 * );
};

struct ChunkUse
{
     VirtualChunk *chunk;
     ofuint64 firstByte;
     ofuint64 lastByte;
     ofuint64 totSize;
};

class BufferPointer;

class OFSYS_API Buffer
{

     friend class BufferPointer;

private:
     Buffer();
     ~Buffer();

     int refCount;
     vector<ChunkUse> chunks;
     VirtualChunk *myChunk;
     ofuint64 totSize;
     ofuint64 mark;

     void deref();
     void ref();
     void append( const char *, ofuint32 );
     void append( Buffer * );
     ofint64 find( const char * ) const;
     BufferPointer extractBuffer( ofuint64, ofint64 ) const;
     char *extractString( ofuint64, ofint64 ) const;
     bool nextBit( ofuint64, char **, ofuint64 * ) const;
     ofuint64 size() const;
     void stripBytes( ofuint32 );
     void markHere();
     void overwriteMark( const char *, ofuint32 );
     ofuint32 readBytes( ofuint64, char *, ofuint32 );

     static VirtualChunk * (*allocator)();
};

class OFSYS_API BufferPointer
{

     friend class Buffer;

public:

     static void setChunks( VirtualChunk *(*)() );

     BufferPointer();
     ~BufferPointer();

     BufferPointer( const BufferPointer & );
     BufferPointer & operator= ( const BufferPointer & );

     void append( const char *, ofuint32 );
     void append( const char * );
     void append( BufferPointer & );
     ofint64 find( const char * ) const;
     BufferPointer extractBuffer( ofuint64, ofint64 ) const;
     char *extractString( ofuint64, ofint64 ) const;
     bool nextBit( ofuint64, char **, ofuint64 * ) const ;
     ofuint64 size() const;
     void empty();
     void stripBytes( ofuint32 );
     void markHere();
     void overwriteMark( const char *, ofuint32 );
     ofuint32 readBytes( ofuint64, char *, ofuint32 );

private:
     Buffer *buffer;

     BufferPointer( Buffer * );
};

#endif
