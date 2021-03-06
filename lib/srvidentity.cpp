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
#include <srvidentity.h>

const SRVIDENTITY SRVIDENTITY::NullSrvID( "0000000000000000" );

ostream & operator << ( ostream &s, const SRVIDENTITY &id )
{
    return s 
		<< hex 
		<< setfill('0') 
		<< setw(16) 
		<< id.m_id << dec;
}

bool SRVIDENTITY::isNull() const
{
    return m_id == NullSrvID.m_id;
}

void readFromBlob( SRVIDENTITYLIST* list, StorageBlob* b )
{
    ofuint32 c = b->readInt32 ();
    list->reserve (c);
    for(; c; c--) {
        SRVIDENTITY* i = new SRVIDENTITY;
        b->readServerIdentity (i);
        list->push_back (i);
    }
}

void dumpToBlob(SRVIDENTITYLIST* list, StorageBlob* b)
{
    assert(list);
    b->writeInt32( list->size() );
    for ( SRVIDENTITYLIST::iterator i = list->begin(); i != list->end(); i++ )
        b->writeServerIdentity( *i );
}
