/*
 * $OFPREPARED$
 *
 * $Revision: 2.0 $
 *
 * $Id: buffers.cpp,v 2.0 2006-01-01 19:26:29 timmy Exp $
 */

#include <ofsys.h>
#include <buffers.h>
#include <ofos.h>

VirtualChunk::VirtualChunk()
{
}

VirtualChunk::~VirtualChunk()
{
}

VirtualChunk *
Chunk::allocateChunk()
{
    Chunk* c = new Chunk;
    //cerr << "Allocating chunk:" << hex << (void*)c << dec << endl;
    return c;
    //return new Chunk;
}

ofuint32
Chunk::defaultSize = 8192;

Chunk::Chunk()
    : refCount( 1 ),
      lastFound( 0 )
{
}

Chunk::~Chunk()
{
     vector<MemoryUse>::iterator i;
     for ( i = data.begin(); i != data.end(); i++ ) delete [] i->data;
}

void
Chunk::deref()
{
     if ( --refCount <= 0 ) delete this;
}

void
Chunk::ref()
{
     refCount++;
}

void
Chunk::append( const char *datai, ofuint32 len )
{
    //cerr << "Chunk::append()" << endl;
     if ( data.size() == 0 )
     {
         //cerr << "Chunk::append() first" << endl;
	  ofuint32 curSize = len;
	  if ( curSize < defaultSize) curSize = defaultSize;
	  MemoryUse all;
	  all.data = new char [ curSize ];
	  memcpy( all.data, datai, len );
          //cerr << "After memcpy" << endl;
	  all.len = len;
	  all.offset = 0;
	  all.max = curSize;
	  data.push_back( all );
          //cerr << "About to return" << endl;
	  return;
     }
     vector<MemoryUse>::reverse_iterator i;
     i = data.rbegin();
     if ( i->max > i->len )
     {
	  ofuint32 done = i->max - i->len;
	  if ( done > len ) done = len;
	  memcpy( i->data + i->len, datai, done );
	  i->len += done;
	  len -= done;
	  if ( len == 0 ) return;
	  datai += done;
     }
     MemoryUse xtra;
     ofuint32 curSize = len;
     if ( curSize < defaultSize) curSize = defaultSize;
     xtra.data = new char [ curSize ];
     xtra.len = len;
     xtra.offset = i->offset + i->len;
     xtra.max = curSize;
     memcpy( xtra.data, datai, len );
     data.push_back( xtra );
}

ofuint64
Chunk::size()
{
     vector<MemoryUse>::reverse_iterator i;
     if ( data.size() == 0 ) return 0;
     i = data.rbegin();
     return i->offset + i->len;
}

bool
Chunk::dataAt( ofuint32 offset, char **start, ofuint32 *len )
{
     int j = 0;
     int k = data.size();
     MemoryUse *i;
     if ( lastFound != 0 )
     {
         i = &data[lastFound];
         if ( i->offset <= offset ) j = lastFound;
     }
     for ( ; j < k; j++ )
     {
         i = &data[j];
         if ( i->offset + i->len <= offset ) continue;
         offset -= i->offset;
         *start = i->data + offset;
         *len = i->len - offset;
         lastFound = j;
         return true;
     }
     return false;
}

bool
Chunk::nextBit( ofuint64 start, char ** buf, ofuint64 * len )
{
     ofuint32 sLen;
     if ( dataAt( start, buf, &sLen ) )
     {
	  *len = sLen;
	  return true;
     }
     return false;
}

bool
Chunk::startWith( const char * match )
{
     const char *p = match;
     ofuint32 len;
     char *pos;
     ofuint32 at = 0;
     if ( *p == 0 ) return true;
     while ( dataAt( at, &pos, &len ) )
     {
	  for ( ; len > 0; pos++, len--, at++ )
	  {
	       if ( *p != *pos ) return false;
	       if ( *(++p) == 0 ) return true;
	  }
     }
     return false;
}

ofint64
Chunk::find ( ofuint64 first, ofuint64 last,
	      const char *hunt , ofuint32 *tail )
{
     char *pos;
     ofuint32 len;
     *tail = 0;
     while ( dataAt( first, &pos, &len ) )
     {
	  for ( ; len > 0; pos++, len--, first++ )
	  {
	       if ( first > last ) return -1;
	       if ( *hunt != *pos ) continue;
	       if ( hunt[1] == 0 ) return first;
	       const char *huntA = hunt + 1;
	       ofuint64 firstA = first + 1;
	       char *posA;
	       ofuint32 lenA;
	       while ( dataAt ( firstA, &posA, &lenA ) )
	       {
		    for ( ; lenA > 0; posA++, lenA--, firstA++, huntA++ )
		    {
			 if ( firstA > last ) goto returnTail;
			 if ( *huntA != *posA ) goto continueSearch;
			 if ( huntA[1] == 0 ) return first;
		    }
	       }
	  returnTail:
	       *tail = huntA - hunt;
	       return -1;
	  continueSearch:
	       first++;
	       break;
	  }
     }
     return -1;
}

void
Chunk::copyToBuffer( char *pos, ofuint64 start, ofuint64 end )
{
     char *frm;
     ofuint32 len;
     end = end - start + 1;
     while ( dataAt( start, &frm, &len ) )
     {
	  for ( ; len > 0 && end > 0; frm++, pos++, start++, len--, end-- )
	       *pos = *frm;
	  if ( end == 0 ) break;
     }
}

void
Chunk::overWrite( ofuint64 start, const char * buf, ofuint32 blen)
{
     char *to;
     ofuint32 len;
     while ( dataAt( start, &to, &len ) )
     {
	  for ( ; len > 0 && blen > 0; to++, buf++, start++, len--, blen-- )
	  {
	       assert ( *to == 'X' );
	       *to = *buf;
	  }
	  if ( blen == 0 ) break;
     }
}

VirtualChunk * (*Buffer::allocator) () = Chunk::allocateChunk;

void
BufferPointer::setChunks( VirtualChunk *(*newAlloc)() )
{
     Buffer::allocator = newAlloc;
}

Buffer::Buffer()
     : refCount( 1 ),
       totSize( 0 ),
       mark( 0 )
{
     myChunk = allocator();
     //cerr << "Buffer(): " << hex << (void*)this << ":" << (void*)myChunk << dec << endl;
}

Buffer::~Buffer()
{
     myChunk->deref();
     vector<ChunkUse>::iterator i;
     for ( i = chunks.begin(); i != chunks.end(); i++ ) i->chunk->deref();
}

void
Buffer::deref()
{
     if ( --refCount <= 0 ) delete this;
}

void
Buffer::ref()
{
     refCount++;
}

void
Buffer::append( const char *data, ofuint32 len )
{
     totSize += len;
     if ( chunks.size() == 0 )
     {
         //cerr << "Buffer::append() : " << hex << (void*)this <<":" << (void*)myChunk << dec << " data:" << len << endl;

         //cerr << "chunk size:" << myChunk->size() << endl;
	  myChunk->append( data, len );
          //cerr << "done mychunk->append()" << endl;
	  ChunkUse all;
	  all.chunk = myChunk;
	  myChunk->ref();
	  all.firstByte = 0;
	  all.lastByte = len - 1;
	  all.totSize = len;
	  chunks.push_back( all );
	  return;
     }
     ofuint64 start = myChunk->size();
     myChunk->append( data, len );
     vector<ChunkUse>::reverse_iterator i;
     i = chunks.rbegin();
     if ( i->chunk == myChunk && i->lastByte == start - 1 )
     {
	  i->lastByte += len;
	  i->totSize = totSize;
	  return;
     }
     ChunkUse xtra;
     xtra.chunk = myChunk;
     myChunk->ref();
     xtra.firstByte = start;
     xtra.lastByte = start + len - 1;
     xtra.totSize = totSize;
     chunks.push_back( xtra );
}

void
Buffer::append( Buffer * b )
{
     vector<ChunkUse>::iterator i;
     ofuint64 lastSize = totSize;
     for ( i = b->chunks.begin(); i != b->chunks.end(); i++ )
     {
	  ChunkUse k = *i;
	  k.chunk->ref();
	  k.totSize += totSize;
	  lastSize = k.totSize;
	  chunks.push_back( k );
     }
     totSize = lastSize;
}

ofint64
Buffer::find( const char *hunt ) const
{
     vector<ChunkUse>::const_iterator i;
     ofuint32 tailMatch = 0;
     ofint64 here;
     for ( i = chunks.begin(); i != chunks.end(); i++ )
     {
	  if ( tailMatch > 0 )
	  {
	       if ( i->chunk->startWith( hunt + tailMatch ) )
	       {
		    return i->totSize - i->lastByte + i->firstByte
			 - 1 - tailMatch;
	       }
	  }
	  here = i->chunk->find( i->firstByte, i->lastByte, hunt, &tailMatch );
	  if ( here > 0 )
	  {
	       return i->totSize - i->lastByte + i->firstByte
			 - 1 + here;
	  }
     }
     return -1;
}

BufferPointer
Buffer::extractBuffer( ofuint64 start, ofint64 end ) const
{
     Buffer *b = new Buffer;
     ofuint64 term;
     if ( end < 0 ) term = totSize - 1;
     else term = end;
     if ( term >= totSize ) term = totSize - 1;
     if ( term < start ) return b;
     vector<ChunkUse>::const_iterator i;
     ofuint64 skipped = 0;
     for ( i = chunks.begin(); i != chunks.end(); i++ )
     {
	  if ( i->totSize <= start )
	  {
	       skipped = i->totSize;
	       continue;
	  }
	  ChunkUse k = *i;
	  i->chunk->ref();
	  k.firstByte += start - skipped;
	  b->totSize = 0;
	  while ( i->totSize < term + 1 )
	  {
	       k.totSize = k.lastByte - k.firstByte + 1 + b->totSize;
	       b->totSize = k.totSize;
	       b->chunks.push_back( k );
	       i++;
	       k = *i;
	       i->chunk->ref();
	  }
	  k.lastByte -= i->totSize - term - 1;
	  k.totSize = k.lastByte - k.firstByte + 1 + b->totSize;
	  b->totSize = k.totSize;
	  b->chunks.push_back( k );
	  break;
     }
     BufferPointer a( b );
     return a;
}

char *
Buffer::extractString( ofuint64 start, ofint64 end ) const
{
     char *str;
     char *pos;
     ofuint64 term;
     if ( end < 0 ) term = totSize - 1;
     else term = end;
     if ( term >= totSize ) term = totSize - 1;
     if ( term < start )
     {
	  str = new char[1];
	  str[0] = 0;
	  return str;
     }
     str = new char [ term - start + 2 ];
     pos = str;
     vector<ChunkUse>::const_iterator i;
     ofuint64 skipped = 0;
     for ( i = chunks.begin(); i != chunks.end(); i++ )
     {
	  if ( i->totSize <= start )
	  {
	       skipped = i->totSize;
	       continue;
	  }
	  if ( i->totSize > term )
	  {
	       start -= skipped;
	       start += i->firstByte;
	       term -= skipped;
	       term += i->firstByte;
	       i->chunk->copyToBuffer( pos, start, term );
	       pos += term - start + 1;
	       break;
	  }
	  start -= skipped;
	  start += i->firstByte;
	  skipped = i->totSize;
	  i->chunk->copyToBuffer( pos, start, i->lastByte );
	  pos += i->lastByte - start + 1;
	  i++;
	  if ( i == chunks.end() ) break;
	  while ( i->totSize < term + 1 )
	  {
	       i->chunk->copyToBuffer( pos, i->firstByte, i->lastByte );
	       pos += i->lastByte - i->firstByte + 1;
	       skipped = i->totSize;
	       i++;
	       if ( i == chunks.end() ) break;
	  }
	  if ( i == chunks.end() ) break;
	  term -= skipped;
	  term += i->firstByte;
	  i->chunk->copyToBuffer( pos, i->firstByte, term );
	  pos += term - i->firstByte + 1;
	  break;
     }
     *pos = 0;
     return str;
}

bool
Buffer::nextBit( ofuint64 start, char ** buf, ofuint64 * len ) const
{
     vector<ChunkUse>::const_iterator i;
     ofuint64 skipped = 0;
     for ( i = chunks.begin(); i != chunks.end(); i++ )
     {
	  if ( i->totSize <= start )
	  {
	       skipped = i->totSize;
	       continue;
	  }
	  if ( i->chunk->nextBit( start - skipped + i->firstByte, buf, len ) )
	  {
	       if ( *len > i->lastByte - start + skipped - i->firstByte + 1 )
	       {
		    *len = i->lastByte - start + skipped - i->firstByte + 1;
	       }
	       return true;
	  }
	  break;
     }
     return false;
}

ofuint64
Buffer::size() const
{
     return totSize;
}

void
Buffer::stripBytes( ofuint32 cnt )
{
     assert ( cnt < totSize );
     totSize -= cnt;
     vector<ChunkUse>::reverse_iterator i;
     i = chunks.rbegin();
     while ( i->lastByte - i->firstByte + 1 < cnt )
     {
	  cnt -= i->lastByte - i->firstByte + 1;
	  i->chunk->deref();
	  chunks.pop_back();
	  i = chunks.rbegin();
     }
     i->lastByte -= cnt;
     i->totSize -= cnt;
}

void
Buffer::markHere()
{
     mark = totSize;
}

void
Buffer::overwriteMark( const char *buffer, ofuint32 len )
{
     vector<ChunkUse>::const_iterator i;
     ofuint64 skipped = 0;
     for ( i = chunks.begin(); i != chunks.end(); i++ )
     {
	  if ( i->totSize <= mark )
	  {
	       skipped = i->totSize;
	       continue;
	  }
	  assert ( i->chunk == myChunk );
	  assert ( i->firstByte + mark - skipped + len < i->lastByte + 1 );
	  i->chunk->overWrite( i->firstByte + mark - skipped, buffer, len );
	  break;
     }
}

ofuint32
Buffer::readBytes( ofuint64 offset, char * buff, ofuint32 len )
{
     vector<ChunkUse>::const_iterator i;
     ofuint64 skipped = 0;
     ofuint64 curBit;
     ofuint64 curStart;
     ofuint32 bytesRead = 0;
     for ( i = chunks.begin(); i != chunks.end(); i++ )
     {
	  if ( i->totSize <= offset )
	  {
	       skipped = i->totSize;
	       continue;
	  }
	  curStart = offset - skipped;
	  while ( len > 0 )
	  {
	       if ( i == chunks.end() ) break;
	       curBit = i->lastByte - i->firstByte + 1;
	       curBit -= curStart;
	       if ( curBit > len ) curBit = len;
	       i->chunk->copyToBuffer( buff, i->firstByte + curStart,
				       i->firstByte + curStart + curBit - 1 );
	       i++;
	       curStart = 0;
	       len -= curBit;
	       buff += curBit;
	       bytesRead += curBit;
	  }
	  break;
     }
     return bytesRead;
}

BufferPointer::BufferPointer()
{
     buffer = new Buffer();
}

BufferPointer::BufferPointer( Buffer * b )
     : buffer ( b )
{
}

BufferPointer::~BufferPointer()
{
     buffer->deref();
}

BufferPointer::BufferPointer( const BufferPointer & bp )
{
     buffer = bp.buffer;
     buffer->ref();
}

BufferPointer &
BufferPointer::operator= ( const BufferPointer & bp )
{
     if ( this != &bp )
     {
	  buffer->deref();
	  buffer = bp.buffer;
	  buffer->ref();
     }
     return *this;
}

void
BufferPointer::append( const char *data, ofuint32 len )
{
     buffer->append( data, len );
}

void
BufferPointer::append( const char *string )
{
     buffer->append( string, OFOS::strlen( string ) );
}

void
BufferPointer::append( BufferPointer & bp )
{
     buffer->append( bp.buffer );
}

ofint64
BufferPointer::find( const char * hunt ) const
{
     return buffer->find( hunt );
}

BufferPointer
BufferPointer::extractBuffer( ofuint64 start, ofint64 end ) const
{
     return buffer->extractBuffer( start, end );
}

char *
BufferPointer::extractString( ofuint64 start, ofint64 end ) const
{
     return buffer->extractString( start, end );
}

bool
BufferPointer::nextBit( ofuint64 start, char ** buf, ofuint64 * len ) const
{
     return buffer->nextBit( start, buf, len );
}

ofuint64
BufferPointer::size() const
{
     return buffer->size();
}

void
BufferPointer::empty()
{
     buffer->deref();
     buffer = new Buffer;
}

void
BufferPointer::stripBytes( ofuint32 cnt )
{
     buffer->stripBytes( cnt );
}

void
BufferPointer::markHere()
{
     buffer->markHere();
}

void
BufferPointer::overwriteMark( const char *buff, ofuint32 len )
{
     buffer->overwriteMark( buff, len );
}

ofuint32
BufferPointer::readBytes( ofuint64 offset, char * buff, ofuint32 len )
{
     return buffer->readBytes( offset, buff, len );
}

