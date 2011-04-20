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

#include <offile.h>

inline
OFConfigGroup::OFConfigGroup( const char *groupName, ofuint32 stack_level ) :
                              m_stack_level(stack_level)
{
     OFOS::strcpy( m_name, groupName );
}

inline
const char *
OFConfigGroup::name( ) const
{
     return m_name;
}

inline
void
OFConfigGroup::addValue( OFConfigItem *item )
{
     m_items.push_back( item );
}

inline
ofuint32
OFConfigGroup::getItemCount()
{
     return m_items.size( );
}

inline
const char *
OFConfigItem::value( ) const
{
    return m_value;
}

inline
bool
OFConfigItem::value( char *theValue )
{
     OFOS::strcpy( theValue, m_value );
     return true;
}

inline
bool
OFConfigItem::value( ofuint32 *pdwValue )
{
     *pdwValue = strtoul( m_value, NULL, 10);
     return true;
}

inline
bool
OFConfigItem::value( double *pdblValue )
{
     *pdblValue = atof( m_value );
     return true;
}

inline
const char *
OFConfigItem::name( ) const
{
     return m_name;
}

inline
ofuint32
OFConfigItem::itemSize() const
{
     return ofstrlen( m_value );
}

// ////////////////////////////////////////
// OFConfigComment

inline
OFConfigComment::OFConfigComment( const char * line )
{
     ofstrcpy( m_comment, line );
}

inline
OFConfigComment::~OFConfigComment( )
{
}

inline
void
OFConfigComment::dump( OFFile &file )
{
    file.write( m_comment, ofstrlen(m_comment) );
    file.write( OFPlatform::CRLF, OFOS::strlen(OFPlatform::CRLF) );
}

inline
void
OFConfigInclude::dump(OFFile &file)
{
    char t[OF_MAX_PATH_LEN+25];
    OFOS::snprintf(t, OF_MAX_PATH_LEN+24, "$include %s\n", m_filename);
    file.write(t, OFOS::strlen(t));
}
