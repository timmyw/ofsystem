/*
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
