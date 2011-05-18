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

#include <ofvariant.h>
#include <float.h>
#include <offile.h>
#include <offlags.h>
//#include <ofgen/genutil.h>
#include <ofutility.h>
#include <limits>

const ofint32 OFVariant::MAX_DISPLAY_BUFFER_LENGTH = 2*OF_MAX_ID_LEN ;
const ofint32 OFVariant::INITIAL_DISPLAY_BUFFER_LENGTH = 34 ; // Changed to handle 64bit integers

const char * OFVariant::emptyString = "(Empty)";
double OFVariant::emptyFloat = -FLT_MAX;
bool OFVariant::emptyBoolean = false;
ofint32 OFVariant::emptyInteger = numeric_limits<int>::min();
OFIDENTITY OFVariant::emptyIdentity = OFIDENTITY::NullID;

#include "cas.h"

const char *OFVariant::blankBinary = "(binary content)";

void
OFVariant::create_( )
{
    if ( m_bufLen )
    {
        m_buffer = new char[m_bufLen];
        m_dispLen = INITIAL_DISPLAY_BUFFER_LENGTH;
        m_display = new char[ m_dispLen + 1 ];
        assert( m_buffer != NULL ) ;
        assert( m_display != NULL ) ;
        assert( m_display != (void*)this );

#if !defined(NDEBUG)
//         cout << "buffer:" << hex << (void*)m_buffer << dec << ":" << m_bufLen << endl;
//         cout << "displ :" << hex << (void*)m_display << dec << ":" << m_dispLen << endl;
        memset( m_buffer, (char)0xcc, m_bufLen );
        memset( m_display, (char)0xdd, m_dispLen );
#endif

    }
}

OFVariant::OFVariant() : 
    m_null( true ), 
    m_buffer( NULL ), 
    m_bufLen( 0 ), 
    m_display( 0 ), 
    m_dispLen( 0 ),
    m_type( 0 ), 
    m_subType( 0 ),
    m_activeReference( 0 )
{
}

OFVariant::OFVariant( const char * str ) : 
    m_null( true ), 
    m_buffer( NULL ), 
    m_bufLen( 0 ), 
    m_display( 0 ), 
    m_dispLen( 0 ),
    m_type( 0 ), 
    m_subType( 0 ),
    m_activeReference( 0 )
{
    assert( str );
    assign( str );
}

void
OFVariant::assign( const char * str )
{
    assert( str );
    destroy_( );
    m_bufLen = OFOS::strlen( str )+1;
    create_( );
    OFOS::strcpy( m_buffer, str );
    m_type = typeString;
    m_null = false;
}

OFVariant::OFVariant( const ofint64 dwValue ) : 
    m_null( true ), 
    m_buffer( NULL ), 
    m_bufLen( 0 ), 
    m_display( 0 ), 
    m_dispLen( 0 ),
    m_type( 0 ), 
    m_subType( 0 ),
    m_activeReference( 0 )
{
    assign( dwValue );
}

void
OFVariant::assign( const ofint64 dwValue )
{
    destroy_( );
    m_bufLen = sizeof(ofint64);
    create_( );
    memcpy( m_buffer, &dwValue, sizeof(ofint64) );
    m_type = typeInteger;
    m_null = false;
}

OFVariant::OFVariant( const bool bValue ) :
    m_null( true ), 
    m_buffer( NULL ), 
    m_bufLen( 0 ), 
    m_display( 0 ), 
    m_dispLen( 0 ),
    m_type( 0 ), 
    m_subType( 0 ),
    m_activeReference( 0 )
{
    assign( bValue );
}

void
OFVariant::assign( const bool bValue )
{
    destroy_( );
    m_bufLen = sizeof(bool);
    create_( );
    memcpy( m_buffer, &bValue, sizeof(bool) );
    m_type = typeBoolean;
    m_null = false;
}

OFVariant::OFVariant( const double dblValue ) :
    m_null( true ), 
    m_buffer( NULL ), 
    m_bufLen( 0 ), 
    m_display( 0 ), 
    m_dispLen( 0 ),
    m_type( 0 ), 
    m_subType( 0 ),
    m_activeReference( 0 )
{
    assign( dblValue );
}

void
OFVariant::assign( const double dblValue )
{
    destroy_( );
    m_bufLen = sizeof(double);
    create_( );
    memcpy( m_buffer, &dblValue, m_bufLen );
    m_type = typeFloat;
    m_null = false;
}

OFVariant::OFVariant( const OFDateTime oftValue ) :
    m_null( true ), 
    m_buffer( NULL ), 
    m_bufLen( 0 ), 
    m_display( 0 ), 
    m_dispLen( 0 ),
    m_type( 0 ), 
    m_subType( 0 ),
    m_activeReference( 0 )
{
    assign( oftValue );
}

void
OFVariant::assign( const OFDateTime oftValue  )
{    
    ofint32 temp = oftValue.getLong() ;
    
    destroy_( );
    m_bufLen = sizeof(ofint32);
    create_( );
    memcpy( m_buffer, &temp, sizeof(ofint32) );
    m_type = typeOFDateTime;
    m_null = false;
}

OFVariant::OFVariant( const OFIDENTITY &ofidValue ) :
    m_null( true ), 
    m_buffer( NULL ), 
    m_bufLen( 0 ), 
    m_display( 0 ), 
    m_dispLen( 0 ),
    m_type( 0 ), 
    m_subType( 0 ),
    m_activeReference( 0 )
{
    assign( ofidValue );
}

void
OFVariant::assign( const OFIDENTITY &ofidValue ) // OFIDENTITY is a basic_string<char>
{
//     cout << "OFVariant::assign( const OFIDENTITY & )" << endl;
//     cout << "id:" << ofidValue.id << endl;
//    assert( !ofidValue.id[OF_MAX_ID_LEN] );

    destroy_( );
    m_bufLen = sizeof( OFIDENTITY );
    create_( );
    memcpy( m_buffer, &ofidValue, sizeof( OFIDENTITY ) );
    m_type = typeIdentity;
    m_null = false;
}

OFVariant::OFVariant( const void *data, ofuint32 length ) :
    m_null( true ), 
    m_buffer( NULL ), 
    m_bufLen( 0 ), 
    m_display( 0 ), 
    m_dispLen( 0 ),
    m_type( 0 ), 
    m_subType( 0 ),
    m_activeReference( 0 )
{
    assign( data, length );
}

void
OFVariant::assign( const void *data, ofuint32 length )
{
    destroy_( );
    m_bufLen = length;
    if ( !length )
        m_bufLen = 1;
    create_( );
    if ( length )
        memcpy( m_buffer, data, length );
    else
        *(char *)m_buffer = 0;

    m_type = typeBinary;
    m_subType = VARIANT_SCALAR;
    m_null = false;
}

OFVariant::OFVariant( const OFVariant &theCopy ) :
    m_null( true ), 
    m_buffer( NULL ), 
    m_bufLen( 0 ), 
    m_display( 0 ), 
    m_dispLen( 0 ),
    m_type( 0 ), 
    m_subType( 0 ),
    m_activeReference( 0 )
{
    assign( theCopy );
}


OFVariant::OFVariant( const OFVariant* const pointer ) :
    m_null( true ), 
    m_buffer( NULL ), 
    m_bufLen( 0 ), 
    m_display( 0 ), 
    m_dispLen( 0 ),
    m_type( 0 ), 
    m_subType( 0 )
{
    assert( 0 != pointer );
    assign( *pointer );
}

void
OFVariant::assign( const OFVariant &theCopy ) // Could be using a reference count
{
    destroy_( );
    m_subType = theCopy.m_subType;
    m_bufLen = theCopy.m_bufLen;
    m_null = theCopy.m_null;
    m_type = theCopy.m_type;
    m_activeReference = theCopy.m_activeReference;
    if ( m_subType == VARIANT_SCALAR )
    {
        create_( );
        if ( m_bufLen )
            memcpy( m_buffer, theCopy.m_buffer, m_bufLen );
    }
    else
    {
        for ( OFVariantList::const_iterator i = theCopy.m_list.begin(); i != theCopy.m_list.end(); i++ )
        {
            OFVariant *vnt = new OFVariant( *(*i) );
            m_list.push_back( vnt );
        }
    }
}

void
OFVariant::copy( const OFVariant &theCopy )
{
    assign( theCopy );
}



OFVariant::~OFVariant( )
{
    destroy_( );
}

bool
OFVariant::isNull( ) const
{
    return m_null;
}

void
OFVariant::makeList( bool clearList /* = false */ )
{
    OFVariant *vnt = OFNULL;
    if ( !m_null && !clearList && m_subType != VARIANT_LIST )
        vnt = new OFVariant( *this );

    destroy_( );

    if ( m_subType == VARIANT_LIST )
        return;

    if ( !clearList && vnt )
    {
        m_list.push_back( vnt );
        m_null = false;
    }
    m_subType = VARIANT_LIST;
}

void
OFVariant::makeScalar( )
{
    if ( m_subType == VARIANT_SCALAR )
        return;
    if ( m_list.size() )
    {
        OFVariant *vnt = new OFVariant( *m_list[0] );
        assign( *vnt );
        m_subType = VARIANT_SCALAR;
    }
    else
        destroy_( );
}

ofuint32
OFVariant::bufferSize( ) const
{
    return m_bufLen;
}

const char *
OFVariant::buffer( ) const
{
    return m_buffer;
}

void
OFVariant::convert( ofint32 lType )
{
    char *temp;
    if ( m_subType == VARIANT_SCALAR )  
        switch ( lType )
        {
        case typeInteger:                 
            assign( cv_ofint64() ) ;    
            break;
        case typeString:
            temp = new char[OFOS::strlen( cv_pcstr() )+1];
            OFOS::strcpy( temp, cv_pcstr() ) ;
            assign( temp ) ;
            delete [] temp;
            break;
        case typeFloat:
            assign( cv_double() ) ;
            break;
        case typeBoolean:
            assign( cv_bool() ) ;
            break;
        case typeOFDateTime:
            assign( cv_OFDateTime() ) ;
            break;
        case typeBinary:
            m_type = typeBinary;
            break;
        case typeIdentity:
            assign( cv_identity() );
            break;
        default:
            assert(SHOULD_NEVER_GET_HERE);
            break;
        }
    else
    { // Cycle through and change each one.
        m_type = lType;
        if ( m_list.size() )
            for ( OFVariantIterator i = m_list.begin(); i != m_list.end(); i++ )
                (*i)->convert( lType );
    }
}

void
OFVariant::convertToString( )
{
    if ( m_subType == VARIANT_SCALAR )
    {
        if ( m_type == typeBinary )
        {
            char *temp = new char[m_bufLen+1];
            memcpy( temp, m_buffer, m_bufLen );
            temp[m_bufLen] = 0;
            assign( temp );
            delete [] temp;
        }
    }
    else
    { // Cycle through and change each one.
        if ( m_list.size() )
            for ( OFVariantIterator i = m_list.begin(); i != m_list.end(); i++ )
                (*i)->convertToString( );
    }
}

void
OFVariant::destroy_( )
{
    // Check that we are either null or we contain a list.
    assert( (m_null && m_list.size() == 0) || !m_null);

//     if ( !m_null )
//     {
        delete [] m_buffer;
        m_buffer = OFNULL;
        delete [] m_display;
        m_display = OFNULL;

        if ( m_subType == VARIANT_LIST )
        {
            for ( OFVariantList::iterator i = m_list.begin(); i != m_list.end(); i++ )
                delete (*i);
            m_list.clear( );
        }
        m_bufLen = 0;
        m_dispLen = 0;
        m_null = true;
        m_type = 0;
//     }
    m_subType = VARIANT_SCALAR;
}

ofint64
OFVariant::cv_ofint64() const
{
    if ( m_null || isList() )
        return emptyInteger;
    
    ofint64 * pszRet = (ofint64 *)m_buffer;
    switch ( m_type )
    {
    case typeInteger:
        // do nothing
        break;
    case typeBoolean:
        *(ofint64 *)m_display = (ofint64)*(bool*)m_buffer;
        pszRet = (ofint64 *)m_display;
        break;
    case typeString:
        *(ofint64 *)m_display = OFOS::atoll(m_buffer);
        pszRet = (ofint64 *)m_display;
        break;
    case typeFloat:
        *(ofint64 *)m_display = (ofint64)*(double*)m_buffer;
        pszRet = (ofint64 *)m_display;
        break;
    case typeOFDateTime:        
        *(ofint64 *)m_display = (ofint64)*(ofint32*)m_buffer;
        pszRet = (ofint64 *)m_display;
        break;
    case typeBinary:
        *(ofint64*)m_display = emptyInteger;
        break;
    default:
        assert(SHOULD_NEVER_GET_HERE);
        break;
    }

    return *pszRet;
}


bool
OFVariant::cv_bool() const
{
    if ( m_null || isList( ) )
        return emptyBoolean;

    bool* ret = (bool*)m_buffer;
    
    switch ( m_type )
    {
    case typeInteger:
        *(bool*)m_display = *(ofint64*)m_buffer ? true : false;
        ret = (bool*)m_display;
        break;
    case typeBoolean:
        // do nothing
        break;
    case typeString:
        if ( !OFOS::stricmp( m_buffer, "true" ) || !OFOS::stricmp( m_buffer, "1" ) )
            *(bool*)m_display = true;
        else
            *(bool*)m_display = false;
        ret = (bool*)m_display;
        break;
    case typeFloat:
        *(bool*)m_display = *(double*)m_buffer ? true : false;
        ret = (bool*)m_display;
        break;
    case typeOFDateTime:        
        *(bool*)m_display = *(ofint64*)m_buffer ? true : false;
        ret = (bool*)m_display;
        break;
    case typeBinary:
        *(bool*)m_display = emptyBoolean;
        break;
    default:
        assert(SHOULD_NEVER_GET_HERE);
        break;
    }
    return *ret;
}



const char *
OFVariant::cv_pcstr() const
{
    if ( m_null && isScalar() )
        return emptyString;

    const char * pszRet = m_buffer;
    ofint32 size = 0 ;

    if ( isScalar( ) )
    {
        switch ( m_type )
        {
        case typeInteger:
            sprintf(m_display, LLFORMAT "d", *(ofint64 *)m_buffer );
            pszRet = m_display;
            break;
        case typeBoolean:
            sprintf( m_display, "%c", (bool)*(bool*)m_buffer? '1' : '0' );
            pszRet = m_display;
            break;
        case typeString:
            // do nothing
            break;
        case typeFloat:
            sprintf( m_display, "%g", *(double*)m_buffer );
            pszRet = m_display;
            break;
        case typeOFDateTime:
            size = strlen(cv_OFDateTime().toString().c_str()) ;
            if( m_dispLen < (ofuint32)size )
            {
                delete [] m_display ;
                *(ofuint32*)(&m_dispLen) = size+1;
                const_cast<OFVariant*>(this)->m_display = new char[m_dispLen + 1] ;
            }
            strcpy( m_display, (cv_OFDateTime().toString().c_str()) ) ;
            pszRet = m_display;
            break;
        case typeIdentity:
            if ( m_dispLen < OF_MAX_ID_LEN + 1 )
            {
                delete [] m_display ;
                *(ofuint32*)(&m_dispLen) = OF_MAX_ID_LEN + 1;
                const_cast<OFVariant*>(this)->m_display = new char[m_dispLen + 1] ;
            }
            ((OFIDENTITY*)m_buffer)->cstr( m_display );
//             sprintf( m_display, "%s", m_buffer );
            pszRet = m_display;
            break;
        case typeBinary:
            if ( m_dispLen < OFOS::strlen( blankBinary ) + 1 )
            {
                delete [] m_display;
                *(ofuint32*)(&m_dispLen) = OFOS::strlen( blankBinary ) + 1;
                const_cast<OFVariant*>(this)->m_display = new char[m_dispLen + 1] ;
            }
            sprintf( m_display, "%s", blankBinary );
            pszRet = m_display;
            break;
        default:
            assert(SHOULD_NEVER_GET_HERE);
            break;
        }
    }
    else
    {   // Create a comma separated list  ?
        // This is a very non-efficient way of doing this.
        if ( !m_list.size( ) )
        {
            // List is empty.
            delete [] m_display;
            *(ofuint32*)&m_dispLen = INITIAL_DISPLAY_BUFFER_LENGTH;
            *(char **)&m_display = new char[m_dispLen+1];
            OFOS::strcpy( m_display, "<< >>" );
        }
        else
        {
            // Workout the size required.
            *(ofuint32*)(&m_dispLen) = sizeof( "<<  >>" );
            OFVariantList::const_iterator i;
            for ( i = m_list.begin(); i != m_list.end(); i++ )
            {
                *(ofuint32*)(&m_dispLen) += OFOS::strlen( (*i)->cv_pcstr() ) + 3;
            }
            delete [] m_display;
            *(char **)&m_display = new char[m_dispLen+1];
            OFOS::strcpy( m_display, "<< " );
            for ( i = m_list.begin(); i != m_list.end(); i++ )
            {
                if ( i != m_list.begin( ) )
                    OFOS::strcat( m_display, ", " );
                OFOS::strcat( m_display, (*i)->cv_pcstr() );
            }
            OFOS::strcat( m_display, " >>" );
        }
        pszRet = m_display;
    }
    return pszRet;
}

double
OFVariant::cv_double() const
{
    if ( m_null || isList() )
        return emptyFloat;
    
    double* pszRet = (double*)m_buffer;
    
    switch ( m_type )
    {
    case typeInteger:
        *(double*)m_display = (double)*(ofint64 *)m_buffer;
        pszRet = (double*)m_display;
        break;
    case typeBoolean:
        *(double*)m_display = (double)*(bool*)m_buffer;
        pszRet = (double*)m_display;
        break;
    case typeString:
        *(double*)m_display = atof(m_buffer);
        pszRet = (double*)m_display;
        break;
    case typeFloat:
        // do nothing
        break;
    case typeOFDateTime:        
        *(double*)m_display = (double)*(ofint32*)m_buffer;
        pszRet = (double*)m_display;
        break;
    case typeBinary:
        *(double*)m_display = emptyFloat;
        break;
    default:
        assert(SHOULD_NEVER_GET_HERE);
        break;
    }
    return *pszRet;
}

OFDateTime
OFVariant::cv_OFDateTime() const
{
    OFDateTime oftTemp ;
    
    if ( m_null || isList( ) )
    {
        oftTemp.setTime_t(0);
        return oftTemp;
    }
    
    switch ( m_type )
    {
    case typeString:
        oftTemp.assign( cv_pcstr() );
        break;
    case typeInteger:
    case typeBoolean:
    case typeFloat:
    case typeOFDateTime:
    case typeBinary:
        oftTemp.setTime_t( cv_ofint64() );
        break;            
    default:
        assert(SHOULD_NEVER_GET_HERE);
        break;
    }
    return oftTemp;
}

OFIDENTITY
OFVariant::cv_identity() const
{
    OFIDENTITY ofidTemp ;
    
    if ( m_null || isList() )
        return emptyIdentity;
    
    switch ( m_type )
    {
    case typeIdentity :
        ofidTemp = *(OFIDENTITY*)m_buffer;
        break ;
    case typeString:
        ofidTemp = m_buffer;
        break;
    case typeOFDateTime:
        ofidTemp = OFIDENTITY::NullID ;
        break;
    case typeInteger:
    case typeBoolean:
    case typeFloat:
    case typeBinary:
        ofidTemp = OFIDENTITY::NullID;
        break;
    default:
        assert(SHOULD_NEVER_GET_HERE);
        break;
    }   

    return ofidTemp;
}

// List operations
OFVariant *
OFVariant::listRetrieve( ofuint32 element )
{
    OFVariant *vnt = OFNULL;

    if ( m_subType == VARIANT_LIST && element > 0 && element <= m_list.size() )
    {
        element--;
        OFVariantIterator i;
        for ( i = m_list.begin( ); element; i++, element-- )
            ;
        vnt = *i;
    }
    return vnt;
}

void
OFVariant::convertAll( ofint32 lType )
{
    if ( m_subType == VARIANT_LIST )
    {
        OFVariantIterator i;
        for ( i = m_list.begin( ); i != m_list.end( ); i++ )
            (*i)->convert( lType );
    }
}

void
OFVariant::listRemove( ofuint32 element )
{
    if ( m_subType == VARIANT_LIST && element <= m_list.size( ) )
    {
        element--;
        OFVariantIterator i;
        for ( i = m_list.begin(); element && i != m_list.end(); i++, element-- )
            ;
        if ( i != m_list.end() )
        {
            delete (*i);
            m_list.erase( i );
        }
    }
}

void
OFVariant::listAdd( OFVariant *newVnt, ofuint32 position /* = 0 */ )
{
    if ( m_subType != VARIANT_LIST )
        makeList( true );
    OFVariant *vnt = new OFVariant( *newVnt );
    if ( !position )
        m_list.push_back( vnt );
    else
    {
        OFVariantIterator pos;
        for ( OFVariantIterator i = m_list.begin(); position && i != m_list.end(); position-- )
        {
            pos = i;
            i++;
        }
        m_list.insert( pos, vnt );
    }
    m_null = false;
}

void
OFVariant::listReplace( OFVariant *newVnt, ofuint32 position )
{
    if ( isList() )
    {
        OFVariantIterator pos;
        for ( OFVariantIterator i = m_list.begin( ); position && i != m_list.end(); position--, i++ )
            pos = i;
        if ( pos != m_list.end() )
        { // The pos is still valid. Delete the old value.
            delete (*pos);
            *pos = newVnt;
        }
    }
}

ofuint32
OFVariant::listSize( ) const
{
    return ( m_subType == VARIANT_LIST )?m_list.size():0;
}

bool
OFVariant::validBooleanOpType_() const
{
    bool bOk = false;
    
    if ( type() == typeInteger || 
         type() == typeBoolean )            
        bOk = true;
    
    return bOk;
}    

bool
OFVariant::operator !() const
{
    if ( m_null )
        return true;
    
    bool ret=false;
    
    ret = !( cv_bool( ) );
    return ret;
}

OFVariant OFVariant::operator -() const
{
    OFVariant ret;
    
    if ( m_null )
        ret.assign( MAKELL( 0 ) );
    else
    {
        switch ( m_type )
        {
        case typeFloat:
            ret.assign( -cv_double() );  
            break ;
        case typeInteger:
            ret.assign( -cv_ofint64() );         
            break ;
        default:         
            ret.assign( MAKELL( 0 ) );
            break;
        }
    }
    return ret;
}


const OFVariant &
OFVariant::operator = (const OFVariant& rhs)
{
    if (&rhs != this)
        assign( rhs );
    return *this;
}

const ofint32
OFVariant::type() const
{
    return m_type;
}

const ofint32
OFVariant::subType() const
{
    return m_subType;
}

void
OFVariant::subType( ofint32 lSubType )
{
    m_subType = lSubType;
}


void
OFVariant::dumpToBuffer2( char * & buffer, ofuint32 & bufferLength )
{
    if ( m_null )
    {
        buffer = NULL;
        bufferLength = 0;
        return;
    }
    bufferLength = dumpBufferSize();
    buffer = new char[bufferLength];
    *(ofuint32 *)buffer = m_type;
    *(ofuint32 *)(buffer+sizeof(ofint32)) = m_subType;
    *(ofuint32 *)(buffer+(sizeof(ofint32)*2)) = m_bufLen;
    memcpy( (buffer+sizeof(ofint32)*3), m_buffer, m_bufLen );
}

ofuint32
OFVariant::dumpToExistingBuffer2( char * buffer, ofuint32 bufferLength )
{
    ofint32 bytesLeft = bufferLength - dumpBufferSize();
    if ( bytesLeft >= 0 )
    {
        if ( !m_null )
        {
            *(ofuint32 *)buffer = m_type;
            *(ofuint32 *)(buffer+sizeof(ofint32)) = m_subType;
            *(ofuint32 *)(buffer+(sizeof(ofint32)*2)) = m_bufLen;
            memcpy( (buffer+sizeof(ofint32)*3), m_buffer, m_bufLen );
        }
        return 0;
    }
    else
    {
        return -bytesLeft;
    }
}

void 
OFVariant::dumpToFile( OFFile *file )
{
    OFUtility::dumpint32( file, m_type );
    Utility::dumpint32( file, m_subType );
    if ( m_subType == VARIANT_SCALAR )
    {
        Utility::dumpuint32( file, m_bufLen );
        if ( m_bufLen )
            file->write( m_buffer, m_bufLen );
    }
    else
    {
        Utility::dumpuint32( file, m_list.size() );
        if ( m_list.size() )
        {
            for ( OFVariantIterator i = m_list.begin(); i != m_list.end(); i++ )
            {
                (*i)->dumpToFile( file );
            }
        }       
    }
}

void 
OFVariant::undumpFromFile( OFFile *file )
{
    destroy_( );
    Utility::undumpint32( file, &m_type );
    Utility::undumpint32( file, &m_subType );
    if ( m_subType == VARIANT_SCALAR )
    {
        Utility::undumpuint32( file, &m_bufLen );
        m_buffer = new char[m_bufLen+1];
        if ( m_bufLen )
        {
            file->read( m_buffer, m_bufLen );
            m_buffer[m_bufLen] = 0;
        }
        m_null = m_bufLen ? false : true;
    }
    else
    {
        ofuint32 l;
        Utility::undumpuint32( file, &l );
        m_null = l ? false : true;
        while ( l )
        {
            OFVariant *vnt = new OFVariant;
            vnt->undumpFromFile( file );
            m_list.push_back( vnt );
            l--;
        }
    }
    m_dispLen = INITIAL_DISPLAY_BUFFER_LENGTH;
    m_display = new char[m_dispLen+1];      
}

ofuint32
OFVariant::dumpBufferSize() const
{
    if ( m_null )
        return 0;
    else
    {
        ofuint32 req = sizeof(m_type) + sizeof(m_subType);
        if ( m_subType == VARIANT_SCALAR )
            req += sizeof(m_bufLen) + m_bufLen;
        else
        {
            req += sizeof(ofuint32);
            for ( OFVariantList::const_iterator i = m_list.begin(); i != m_list.end(); i++ )
                req += (*i)->dumpBufferSize();
        }
        return req;
    }
}

void
OFVariant::write( StorageBlob *blob ) const
{ 
    blob->writeInt32(m_subType);
    if (m_subType == VARIANT_SCALAR) 
    {
        blob->writeInt32(m_type);
        blob->writeInt32(m_bufLen);
        if (m_bufLen == 0) 
            return;
        switch (m_type) 
        {
        case typeInteger:
            blob->writeInt64( (ofint64)*(ofint64 *)m_buffer);
            break;
        case typeBoolean:
            blob->writeBool(*(bool *)m_buffer);
            break;
        case typeString:
            blob->writeString(m_buffer);
            break;
        case typeFloat:
            blob->writeDouble(*(double *)m_buffer);
            break;
        case typeOFDateTime:
            blob->writeInt32(*(ofint32 *)m_buffer);
            break;
        case typeIdentity:
            blob->writeIdentity((OFIDENTITY *)m_buffer);
            break;
        case typeBinary:
            blob->writeBinary( m_buffer, m_bufLen );
            break;
        }
        return;
    }
    blob->writeInt32( m_list.size( ) );
    for ( vector<OFVariant *>::const_iterator i = m_list.begin(); i != m_list.end(); i++ )
    {
        (*i)->write (blob);
    }
}

void
OFVariant::readFromBlob( StorageBlob *blob, int version )
{
    destroy_();
    m_subType = blob->readInt32();

    if ( m_subType == VARIANT_SCALAR )
    {
        m_type = blob->readInt32();
        m_bufLen = blob->readInt32();
        if (m_bufLen == 0) 
        {
            m_null = true;
            return;
        }
        m_null = false;
        m_buffer = new char[m_bufLen];
        switch (m_type) 
        {
        case typeInteger:
            *(ofint64 *)m_buffer = blob->readInt64();
            break;
        case typeBoolean:
            *(bool *)m_buffer = blob->readBool();
            break;
        case typeString:
            blob->readString(&m_buffer[0]);
            break;
        case typeFloat:
            *(double *)m_buffer = blob->readDouble();
            break;
        case typeOFDateTime:
            *(ofint32 *)m_buffer = blob->readInt32();
            break;
        case typeIdentity:
            blob->readIdentity((OFIDENTITY *)m_buffer);
            break;
        case typeBinary:
            blob->readBinary( m_buffer, m_bufLen );
            break;
        }
        m_display = new char[MAX_DISPLAY_BUFFER_LENGTH+1];
        return;
    }
    
    long x = blob->readInt32( );
    if ( x )
    {
        makeList( );
        m_list.reserve( x );
        for (; x > 0; x--) 
        {
            m_null = false;
            OFVariant *v = new OFVariant;
            v->readFromBlob( blob, version );
            m_list.push_back( v );
        }
    }
}

void
OFVariant::dumpToBuffer( char **buffer )
{
    Utility::dumpint32( buffer, m_type );
    Utility::dumpint32( buffer, m_subType );
    if ( m_subType == VARIANT_SCALAR )
    {
        Utility::dumpuint32( buffer, m_bufLen );
        if ( m_bufLen )
            memcpy( *buffer, m_buffer, m_bufLen );
        (*buffer) += m_bufLen;
    }
    else
    {
        Utility::dumpuint32( buffer, m_list.size() );
        if ( m_list.size() )
        {
            for ( OFVariantIterator i = m_list.begin(); i != m_list.end(); i++ )
                (*i)->dumpToBuffer( buffer );
        }
    }
}

void
OFVariant::undumpFromBuffer( const char **buffer )
{
    // First destroy the current contents, if any
    destroy_( );
    Utility::undumpint32( buffer, &m_type );
    Utility::undumpint32( buffer, &m_subType );
    if ( m_subType == VARIANT_SCALAR )
    {
        Utility::undumpuint32( buffer, &m_bufLen );
        m_buffer = new char[m_bufLen+1];
        if ( m_bufLen )
        {
            memcpy( m_buffer, *buffer, m_bufLen ); 
            m_buffer[m_bufLen] = 0;
        }
        *buffer += m_bufLen;
        m_null = m_bufLen ? false : true;
    }
    else
    {
        ofuint32 l;
        Utility::undumpuint32( buffer, &l );
        m_null = ((l) ? false : true);
        while ( l )
        {
            OFVariant *vnt = new OFVariant;
            vnt->undumpFromBuffer( buffer );
            m_list.push_back( vnt );
            l--;
        }
    }
    m_dispLen = INITIAL_DISPLAY_BUFFER_LENGTH;
    m_display = new char[m_dispLen+1];
}

// Friend functions

// Arithmetic operations


OFVariant
operator +( const OFVariant &lhs, const OFVariant &rhs)
{
    OFVariant ret( MAKELL( 0 ) );

    if ( lhs.m_null || rhs.m_null )
        return ret;

    char * pszTemp;
    OFVariant lhs2( lhs );
    OFVariant rhs2( rhs );

    if ( !ValidArithmeticOpTypes( lhs, rhs ) )
    {
        ofint32 n = arithOpResultType( lhs2.type(), rhs2.type() );
        lhs2.convert( n );
        rhs2.convert( n );
    }

    if ( lhs2.m_type != rhs2.m_type )
    {
        if ( lhs2.m_type == typeOFDateTime)
            // Allow addition of ofint64 to times
        {
             OFDateTime x;
             x.setTime_t( lhs2.cv_ofint64() + rhs2.cv_ofint64() );
             ret.assign( x );
        }
        else if ( rhs2.m_type == typeOFDateTime)
        {
             OFDateTime x;
             x.setTime_t( lhs2.cv_ofint64() + rhs2.cv_ofint64() );
             ret.assign( x );
        }
        else
            ret.assign( lhs2.cv_double() + rhs2.cv_double() );                        
    }
    else
    {    
        ofuint32 t1,t2;
        switch ( lhs2.m_type )
        {
        case typeInteger:
            ret.assign( lhs2.cv_ofint64() + rhs2.cv_ofint64() );                                                
            break;
        case typeString:
            t1 = OFOS::strlen( lhs2.cv_pcstr() );
            t2 = OFOS::strlen( rhs2.cv_pcstr() );
            pszTemp = new char[t1 + t2 + 1];
            memcpy( pszTemp, lhs2.cv_pcstr(), t1 );
            memcpy( pszTemp+t1, rhs2.cv_pcstr(), t2 );
            pszTemp[t1+t2] = 0;
            ret.assign( pszTemp );
            delete [] pszTemp;
            break;
        case typeFloat:
            ret.assign( lhs2.cv_double() + rhs2.cv_double() );
            break;
        default:
            break;
        }
    }
    return ret;
}


OFVariant
operator -( const OFVariant &lhs, const OFVariant &rhs)
{
    OFVariant ret( MAKELL( 0 ) );
    if ( lhs.m_null || rhs.m_null )
        return ret;
    
    OFVariant lhs2(lhs);
    OFVariant rhs2(rhs);

    if ( !ValidArithmeticOpTypes( lhs2, rhs2 ) )
    {
        ofint32 n = arithOpResultType( lhs2.m_type, rhs2.m_type );
        lhs2.convert( n );
        rhs2.convert( n );
    }
    
    if( lhs2.m_type != rhs2.m_type )
    {
        if ( lhs2.m_type == typeOFDateTime )
            // Allow subtraction of ofint32 from times
        {
             OFDateTime x;
             x.setTime_t( lhs2.cv_ofint64() - rhs2.cv_ofint64() );
             ret.assign( x );
        }
        else if (rhs2.type() == typeOFDateTime)
        {
             OFDateTime x;
             x.setTime_t( lhs2.cv_ofint64() - rhs2.cv_ofint64() );
             ret.assign( x );
        }
        else
            ret.assign( lhs2.cv_double() - rhs2.cv_double() );                        
    }
    else
    {    
        switch ( lhs2.m_type )
        {
        case typeInteger:
            ret.assign( lhs2.cv_ofint64() - rhs2.cv_ofint64() );                                                
            break;
        case typeFloat:
            ret.assign( lhs2.cv_double() - rhs2.cv_double() );
            break;
        default:
            break;
        }
    }
    return ret;
}



OFVariant
operator *( const OFVariant &lhs, const OFVariant &rhs)
{
    OFVariant ret( MAKELL( 0 ) );
    if ( lhs.m_null || rhs.m_null )
        return ret;
    
    OFVariant lhs2(lhs);
    OFVariant rhs2(rhs);

    if ( !ValidArithmeticOpTypes( lhs2, rhs2 ) )
    {
        ofint32 n = arithOpResultType( lhs2.m_type, rhs2.m_type );
        lhs2.convert( n );
        rhs2.convert( n );
    }
    if( lhs2.m_type == typeOFDateTime || rhs2.m_type == typeOFDateTime)
        return ret;
        
    if( lhs2.m_type != rhs2.m_type )
    {
        ret.assign( lhs2.cv_double() * rhs2.cv_double() );                        
    }
    else
    {    
        switch ( lhs2.m_type )
        {
        case typeInteger:
            ret.assign( lhs2.cv_ofint64() * rhs2.cv_ofint64() );                                                
            break;
        case typeFloat:
            ret.assign( lhs2.cv_double() * rhs2.cv_double() );
            break;
        default:
            break;
        }
    }
    return ret;
}

OFVariant
operator /( const OFVariant &lhs, const OFVariant &rhs)
{
    OFVariant ret( MAKELL( 0 ) );
    if ( lhs.m_null || rhs.m_null )
        return ret;
    
    OFVariant lhs2(lhs);
    OFVariant rhs2(rhs);

    if ( !ValidArithmeticOpTypes( lhs2, rhs2 ) )
    {
        ofint32 n = arithOpResultType( lhs2.m_type, rhs2.m_type );
        lhs2.convert( n );
        rhs2.convert( n );
    }
    
    if ( lhs2.m_type == typeOFDateTime || rhs2.m_type == typeOFDateTime)
        return ret;

    if ( lhs2.m_type != rhs2.m_type )
    {
        if( rhs2.cv_double() == 0 )
            return ret;
        ret.assign( lhs2.cv_double() / rhs2.cv_double() );                        
    }
    else
    {
        if ( rhs.cv_double() != 0 )
        {
            switch ( lhs2.m_type )
            {
            case typeInteger:                   
                ret.assign( lhs2.cv_ofint64() / rhs2.cv_ofint64() );                                                
                break;
            case typeFloat:
                ret.assign( lhs2.cv_double() / rhs2.cv_double() );
                break;
            default:
                break;
            }
        }
    }
    return ret;
}

// Equality operators

ofint64
operator ==( const OFVariant &lhs, const OFVariant &rhs)
{
    ofuint32 ret = 0;

    if ( lhs.m_null || rhs.m_null )
        return ret;
    
    if ( ValidEqualityOpTypes( lhs, rhs ) )
    {
        if( lhs.m_type != rhs.m_type )
        {
            ret = static_cast<ofuint32>(lhs.cv_double() == rhs.cv_double());
        }
        else
        {
            OFIDENTITY lhsTemp;
            OFIDENTITY rhsTemp;
            switch ( lhs.type() )
            {
            case typeInteger:
                ret= static_cast<ofuint32>(lhs.cv_ofint64() == rhs.cv_ofint64());
                break;
                    
            case typeBoolean:
                assert(rhs.type() == typeBoolean);
                ret = static_cast<ofuint32>(lhs.cv_bool() == rhs.cv_bool() ) ;
                break;
            case typeString:
                assert(rhs.type() == typeString);
                ret = static_cast<ofuint32>(!strcmp( lhs.cv_pcstr(), rhs.cv_pcstr()));
                break;
            case typeFloat:
                ret = static_cast<ofuint32>(lhs.cv_double() == rhs.cv_double());                    
                break;
            case typeOFDateTime:
                ret = static_cast<ofuint32>( lhs.cv_OFDateTime() == rhs.cv_OFDateTime() ) ;
                break;                     
            case typeIdentity:
                lhsTemp = lhs.cv_identity() ;
                rhsTemp = rhs.cv_identity() ;
                ret = static_cast<ofuint32>( (( lhsTemp == rhsTemp )==0) ? 0 : 1) ;
                break;                     
            default:
                break;
            }
        }
    }

    return ret;
}

ofint64
operator !=( const OFVariant &lhs, const OFVariant &rhs)
{
    return !(lhs == rhs);
}

// Relational Operators

ofint64
operator <( const OFVariant &lhs, const OFVariant &rhs)
{
    ofuint32 ret=0;

    if ( lhs.m_null || rhs.m_null )
        return ret;
    
    if ( ValidRelationalOpTypes( lhs, rhs ) )
    {
        if( lhs.m_type != rhs.m_type )
        {
            ret = static_cast<ofuint32>( lhs.cv_double() < rhs.cv_double() );
        }
        else
        {
            switch ( lhs.m_type )
            {
            case typeInteger:                    
                ret= static_cast<ofuint32>(lhs.cv_ofint64() < rhs.cv_ofint64());
                break;
            case typeString:
                ret = static_cast<ofuint32>(strcmp( lhs.cv_pcstr(), rhs.cv_pcstr()) < 0 );                
                break;
            case typeFloat:
                ret = static_cast<ofuint32>(lhs.cv_double() < rhs.cv_double());
                break;                           
            case typeOFDateTime:
                ret = static_cast<ofuint32>(lhs.cv_OFDateTime() < rhs.cv_OFDateTime());
                break;                    
            default:
                break;
            }
        }
    }

    return ret;
}

ofint64
operator <=( const OFVariant &lhs, const OFVariant &rhs)
{
    return (lhs < rhs) || (lhs == rhs);
}

ofint64
operator >( const OFVariant &lhs, const OFVariant &rhs)
{
    return !(lhs <= rhs);
}

ofint64
operator >=( const OFVariant &lhs, const OFVariant &rhs)
{
    return !(lhs < rhs);
}

ofint32  
arithOpResultType( ofint32 type1, ofint32 type2 )
{
    // Remember string comparision operators (GE, EQUALS etc) will result in
    // a signed integer value.
    ofint32 res = typeInteger;

    if ( type1 == typeFloat || type2 == typeFloat )
        res = typeFloat;

    return res;
}

bool
ValidArithmeticOpTypes( const OFVariant &lhs, const OFVariant &rhs )
{
    ofint32 lType1 = lhs.m_type;
    ofint32 lType2 = rhs.m_type;    

    assert( typeInteger <= lType1 && lType1 <= typeIdentity ) ;
    assert( typeInteger <= lType2 && lType2 <= typeIdentity ) ;    
    
    if ( lType1 == typeString && lType2 != typeString )
    {
        return false;
    }
    if ( lType2 == typeString && lType1 != typeString )
    {
        return false;
    }
    if ( lType1 == typeBoolean && lType2 != typeBoolean )
    {
        return false;
    }
    if ( lType2 == typeBoolean && lType1 != typeBoolean )
    {
        return false;
    }
    // Modified to allow addition of values to times
    if ( lType1 == typeOFDateTime && lType2 != typeInteger )
    {
        return false;
    }
    else if ( lType2 == typeOFDateTime && lType1 != typeInteger )
    {
        return false;
    }
    else if ( lType1 == typeIdentity || lType2 == typeIdentity )
    {
        return false;
    }
    return true;
}

bool
ValidEqualityOpTypes( const OFVariant &lhs, const OFVariant &rhs )
{
    ofint32 lType1 = lhs.m_type ;
    ofint32 lType2 = rhs.m_type ;    

    assert( typeInteger <= lType1 && lType1 <= typeIdentity ) ;
    assert( typeInteger <= lType2 && lType2 <= typeIdentity ) ;    
    
    bool bOk = true;

    if ( lType1 == typeString && lType2 != typeString )
    {
        bOk = false;
    }
    else if ( lType2 == typeString && lType1 != typeString )
    {
        bOk = false;
    }
    else if ( lType1 == typeBoolean && ( lType2 != typeBoolean && lType2 != typeInteger ) )
    {
        bOk = false;
    }
    else if ( lType2 == typeBoolean && ( lType1 != typeBoolean && lType1 != typeInteger ) )
    {
        bOk = false;
    }
    else if ( lType1 == typeOFDateTime && lType2 != typeOFDateTime )
    {
        bOk = false;
    }
    else if ( lType1 != typeOFDateTime && lType2 == typeOFDateTime )
    {
        bOk = false;
    }
    else if ( lType1 == typeIdentity && lType2 != typeIdentity )
    {
        bOk = false;
    }
    else if ( lType1 != typeIdentity && lType2 == typeIdentity )
    {
        bOk = false;
    }
    
    return bOk;
}
bool
ValidRelationalOpTypes( const OFVariant &lhs, const OFVariant &rhs )
{
    ofint32 lType1 = lhs.type() ;
    ofint32 lType2 = rhs.type() ;    

    assert( typeInteger <= lType1 && lType1 <= typeIdentity ) ;
    assert( typeInteger <= lType2 && lType2 <= typeIdentity ) ;    
    
    bool bOk = true;
    
    if ( lType1 == typeString && lType2 != typeString )
    {
        bOk = false;
    }
    else if ( lType2 == typeString && lType1 != typeString )
    {
        bOk = false;
    }
    else if ( lType1 == typeBoolean && lType2 != typeBoolean )
    {
        bOk = false;
    }
    else if ( lType2 == typeBoolean && lType1 != typeBoolean )
    {
        bOk = false;
    }
    else if ( lType1 == typeOFDateTime && lType2 != typeOFDateTime )
    {
        bOk = false;
    }
    else if ( lType1 != typeOFDateTime && lType2 == typeOFDateTime )
    {
        bOk = false;
    }
    else if (lType1 == typeIdentity || lType2 == typeIdentity )
    {
        bOk = false;
    }
    
    return bOk;
}
bool
operator &&( const OFVariant &lhs, const OFVariant &rhs)
{
    bool ret = false;
    
    if ( lhs.m_null || rhs.m_null )
        return ret;
    
    if ( lhs.validBooleanOpType_() && rhs.validBooleanOpType_() ) 
        ret = lhs.cv_bool() && rhs.cv_bool() ;                

    return ret;
}

bool
operator ||( const OFVariant &lhs, const OFVariant &rhs)
{
    bool ret = false;
    
    if ( lhs.m_null || rhs.m_null )
        return ret;
    
    if ( lhs.validBooleanOpType_() && rhs.validBooleanOpType_() ) 
        ret = lhs.cv_bool() || rhs.cv_bool() ;                

    return ret;
}

void
OFVariant::dumpBinary( OFVariant *bin )
{
    cout << "Binary dump: " << bin->bufferSize() << " bytes" << endl << "[";
    ofuint32 len = bin->bufferSize() * 2 + 1;
    char *hex = new char[len];
    OFUtility::binToHex( (ofchar *)bin->buffer(), bin->bufferSize(), hex, len );
    cout << hex << "]" << endl;
    delete [] hex;
}

#if defined(UNIT_TEST)
#endif
