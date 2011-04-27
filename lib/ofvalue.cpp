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
#include <ofvalue.h>
#include <ofos.h>
#include <offlags.h>

#define REFCOUNT(a) (*(ofint32*)((a).valmem.buffer))
#define REFCOUNTADDR(a) ((volatile ofuint32*)((a).valmem.buffer))
#define REFCOUNTSIZE OFVALUE::refCountSize
#define LISTSIZE(a) ((a.valmem.buffer)?\
    (a.valmem.size?a.valmem.size-REFCOUNTSIZE:0)/sizeof(OFVALUE):\
    0)
#define LISTCAP(a) ((a.valmem.buffer)?\
   (a.valmem.capacity?a.valmem.capacity-REFCOUNTSIZE:0)/sizeof(OFVALUE):0)
#define LISTELEM(VAL,ELEM)\
    (\
        (\
            (OFVALUE*)((char *)VAL.valmem.buffer+REFCOUNTSIZE)\
        )[ELEM]\
    )

#define VALUE_BINSTR "Binary data"

OFVALUE::OFVALUE() :
    type( 0 )
{
    value.valid.id[0] = 0;
    value.valid.id[1] = 0;
    value.valid.flags = 0;
}

OFVALUE::OFVALUE(ofuint32 newType):
    type(newType)
{
    value.valid.id[0] = 0;
    value.valid.id[1] = 0;
    value.valid.flags = 0;
}

OFVALUE::OFVALUE( const OFVALUE &copy )
{
    // No need to destroy. This must be a new OFVALUE
    type = copy.type;
    value.valid.id[0] = copy.value.valid.id[0];
    value.valid.id[1] = copy.value.valid.id[1];
    value.valid.flags = copy.value.valid.flags;
    incrementRefCount();
}

void
OFVALUE::copy( const OFVALUE &copy )
{
    destroy();
    type = copy.type;
    value.valid.id[0] = copy.value.valid.id[0];
    value.valid.id[1] = copy.value.valid.id[1];
    value.valid.flags = copy.value.valid.flags;
    // The call the incrementRefCount() is purposely
    // left out here.
}

OFVALUE::~OFVALUE()
{
    destroy( );
}

OFVALUE &
OFVALUE::operator=( const OFVALUE &rhs )
{
    if ( this == &rhs )
        return *this;

    // This OFVALUE may have contained something before
    // this assignment was called.
    destroy();

    type = rhs.type;
    // Compiler can do a straight memcpy of the rhs.
    value = rhs.value;
    incrementRefCount();
    return *this;
}

#if defined(HAVE_OFVARIANT)
void
OFVALUE::populate( OFVariant *variant )
{
//     cout << hex << "THIS:" << (void*)this << dec << endl;
    destroy( );
    OFIDENTITY id;
    signed char newtype = translateVariantType( variant );
    switch ( newtype )
    {
    case 0: // special case because the variant had no type.
        if ( variant->isList() && variant->listSize() )
        {
//             cout << "Special case for type 0" << endl;
            OFVariant *elem = variant->listRetrieve( 1 );
            variant->convert( elem->type() );
            populate( variant );
            return;
        }
        else
        {
//             cout << "Its a list, but its empty" << endl;
            type = typeListInteger;
            return;
        }
        break;
    case typeInteger:
        set( variant->cv_ofint64() );
        break;
    case typeBoolean:
        set( variant->cv_bool() );
        break;
    case typeString:
        set( variant->cv_pcstr() );
        break;
    case typeFloat:
        set( variant->cv_double() );
        break;
    case typeOFDateTime:
        set( variant->cv_OFDateTime() );
        break;
    case typeBinary:
        set( variant->buffer(), variant->bufferSize() );
        break;
    case typeIdentity:
        id = variant->cv_identity();
        set( &id );
        break;
    case typeListInteger:
    case typeListBoolean:
    case typeListString:
    case typeListFloat:
    case typeListOFDateTime:
    case typeListIdentity:
    case typeListBinary:
        ofuint32 listSize = variant->listSize();
        listAllocate( listSize );
        value.valmem.size = REFCOUNTSIZE + (listSize) * sizeof( OFVALUE );
        for ( ofuint32 cur = 0; cur < listSize; cur++ )
        {
            LISTELEM( value, cur ).populate( variant->listRetrieve( cur+1 ) );
        }
        break;

    }
    type = newtype;
    if ( DYNTYPE( type ) && value.valmem.buffer )
    {
        REFCOUNT(value) = 1;
    }
}

OFVariant *
OFVALUE::populate( )
{
    assert( type );
    OFVariant *vnt = new OFVariant;
    switch ( type )
    {
    case typeInteger:
        vnt->assign( value.valint );
        break;
    case typeBoolean:
        vnt->assign( value.valbool );
        break;
    case typeString:
        vnt->assign( get_string() );
        break;
    case typeFloat:
        vnt->assign( value.valdbl );
        break;
    case typeOFDateTime:
        vnt->assign( value.valdate );
        break;
    case typeIdentity:
        vnt->assign( get_OFIDENTITY() );
        break;
    case typeBinary:
         if ( value.valmem.size != 0 )
         {
              vnt->assign( (const void *)(((char *)value.valmem.buffer)+REFCOUNTSIZE), value.valmem.size-REFCOUNTSIZE );
         }
         else
         {
              vnt->assign( NULL, 0 );
         }
        break;
    case typeListInteger:
    case typeListBoolean:
    case typeListString:
    case typeListFloat:
    case typeListOFDateTime:
    case typeListIdentity:
    case typeListBinary:
        vnt->makeList();
        for ( ofuint32 cur = 0; cur < LISTSIZE(value); cur++ )
        {
            OFVariant *elem = LISTELEM( value, cur ).populate();
            vnt->listAdd( elem );
        }
        break;
    }
    return vnt;
}
#endif // HAVE_OFVARIANT

const char
OFVALUE::isList() const
{
    return ( type & 8 );
}

void
OFVALUE::listAllocate( ofuint32 listSize )
{
    value.valmem.size = 0;
    value.valmem.capacity = REFCOUNTSIZE + listSize * sizeof(OFVALUE);
    value.valmem.buffer = new char[value.valmem.capacity];
    memset( (char *)value.valmem.buffer, 0, value.valmem.capacity );
    REFCOUNT(value) = 1;
}

void
OFVALUE::listAdd( OFVALUE *element )
{
    if ( !type )
        type = element->type | 8;

    if ( LISTSIZE( value ) == LISTCAP( value ) )
        listIncrease( 1 );

    // Copy without a destroy, and increment reference count
    cp_nd_inc_( &(LISTELEM(value,LISTSIZE(value))), element );

    value.valmem.size = REFCOUNTSIZE + (LISTSIZE(value)+1) * sizeof( OFVALUE );
}

void
OFVALUE::listAdd( OFVALUE *element, ofint64 index )
{
        index -= 1;
        ofuint32 listSize = LISTSIZE(value);
        if (index < 0)
                return;
        if (index >= (ofint64)listSize)
        {
                listAdd (element);
                return;
        }
        if (listSize == LISTCAP (value))
                listIncrease (1);
        value.valmem.size = REFCOUNTSIZE + (listSize+1) * sizeof( OFVALUE );
        char *temp = new char[(listSize-index)*sizeof(OFVALUE)];
        memcpy( temp, &( LISTELEM( value, index ) ), (listSize-index)*sizeof(OFVALUE) );
        cp_nd_inc_( &(LISTELEM(value,index)), element );
        //LISTELEM( value, index ) = *element;
        memcpy( &( LISTELEM( value, index+1 ) ), temp, (listSize-index)*sizeof(OFVALUE) );
        delete [] temp;
}

void
OFVALUE::cp_nd_inc_( OFVALUE* dest, OFVALUE* src )
{
    if ( ((void*)dest) == ((void*)src) )
        return;
    dest->type = src->type;
    dest->value.valid.id[0] = src->value.valid.id[0];
    dest->value.valid.id[1] = src->value.valid.id[1];
    dest->value.valid.flags = src->value.valid.flags;
    dest->incrementRefCount();
}

ofuint32
OFVALUE::listRemove (ofint64 index)
{
        ofuint32 listSize = LISTSIZE (value);
        if (index < 1 || index > (ofint64)listSize)
                return ERR_INVALID_INDEX;
        index -= 1;
        // Make sure we destroy the element being removed.
        LISTELEM( value, index ).destroy();
        // Copy the remaining elements up one, if there are any
        if (index < (ofint64)listSize)
        {
                char *temp = new char[(listSize-index-1)*sizeof(OFVALUE)];
                memcpy( temp, &(LISTELEM( value, index+1)), (listSize-index-1)*sizeof(OFVALUE) );
                memcpy( &(LISTELEM( value, index )), temp, (listSize-index-1)*sizeof(OFVALUE) );
        }
        listSize--;
        value.valmem.size = REFCOUNTSIZE + (listSize) * sizeof( OFVALUE );
        return ERR_SUCCESS;
}

ofuint32
OFVALUE::listReplace( OFVALUE *element, ofint64 index )
{
        if (index < 1 || index > (ofint64)LISTSIZE(value))
                return ERR_INVALID_INDEX;
        index -= 1;
        LISTELEM(value,index).destroy();
        cp_nd_inc_( &(LISTELEM(value,index)), element );
        //LISTELEM(value,index) = *element;
        return ERR_SUCCESS;
}

OFVALUE
OFVALUE::listRetrieve( ofint64 index )
{
        OFVALUE got;
        if (index >= 1 && index <= (ofint64)LISTSIZE(value))
                got = LISTELEM (value, index -1);
        return got;
}

ofint64
OFVALUE::listFind (const OFVALUE* val) const
{
        if (!(val->type & type))
                return 0;
        for (ofint64 j = 1; j <= (ofint64)LISTSIZE(value); j++)
        {
                OFVALUE elem = ((OFVALUE*)this)->listRetrieve (j);
                if (elem == *val)
                        return j;
        }
        return 0;
}

ofint64
OFVALUE::listSize() const
{
    return LISTSIZE(value);
}

void
OFVALUE::listIncrease( ofuint32 inc )
{
    if (inc<16)
        inc = 16;
//     cout << "listIncrease:in: buffer:" << hex << (void*)value.valmem.buffer << dec << endl;
    ofuint32 newSize = LISTSIZE(value) + inc;
    value.valmem.capacity = REFCOUNTSIZE + newSize * sizeof(OFVALUE);
    char *newbuf = new char[value.valmem.capacity];
    //memset(newbuf,0,value.valmem.capacity);
    char *newpos = newbuf + REFCOUNTSIZE + LISTSIZE(value)*sizeof(OFVALUE);
    memset(newpos,0, inc * sizeof(OFVALUE) );
    if ( DYNTYPE(type) && value.valmem.buffer )
    {
        if ( LISTSIZE(value) )
        {
//             cout << "Copying " << value.valmem.size << " bytes" << endl;
            memcpy( newbuf, (char *)value.valmem.buffer, value.valmem.size );
        }
        // Decrement the original reference count, if one exists
        if ( REFCOUNT(value) != -1 )
        {
            ofuint32 i;
            for (;;)
            {
                i = REFCOUNT(value);
                if (compare_and_swap_int(REFCOUNTADDR(value), i, i-1))
                    break;
            }
            if ( i == 1 )
                delete [] (char *)value.valmem.buffer;
        }
    }
//     else
//     {
//         cout << "List inc. no copy" << endl;
//     }
    value.valmem.buffer = newbuf;
    REFCOUNT(value) = 1;
//     cout << "listIncrease:out: buffer:" << hex << (void*)value.valmem.buffer << dec << endl;
}

void
OFVALUE::set( ofint64 integer )
{
    if ( DYNTYPE( type ) )
        destroy( );
    type = typeInteger;
    value.valint = integer;
}

void
OFVALUE::set( const bool b )
{
    if ( DYNTYPE( type ) )
        destroy( );
    type = typeBoolean;
    value.valbool = b;
}

void
OFVALUE::set( const double d )
{
    if ( DYNTYPE( type ) )
        destroy( );
    type = typeFloat;
    value.valdbl = d;
}

void
OFVALUE::set( const OFIDENTITY *id )
{
    if ( DYNTYPE( type ) )
        destroy( );
    type = typeIdentity;
    value.valid.id[0] = id->m_id0;
    value.valid.id[1] = id->m_id1;
    value.valid.flags = id->m_flags;
}

void
OFVALUE::set( const OFDateTime dt )
{
    if ( DYNTYPE( type ) )
        destroy( );
    type = typeOFDateTime;
    value.valdate = dt;
}

void
OFVALUE::set( const char *str )
{
    ofuint32 newlen = OFOS::strlen( str ) + 1 + REFCOUNTSIZE;
    //    if ( DYNTYPE( type ) )
    destroy( );
    type = typeString;
    value.valmem.capacity = value.valmem.size = newlen;
    value.valmem.buffer = new char[newlen];
    OFOS::strcpy( ((char *)value.valmem.buffer)+REFCOUNTSIZE, str );
    REFCOUNT(value) = 1;
//     cout << hex << (void*)(value.valmem.buffer) << dec << " set(char*): " << REFCOUNT( value ) << endl;
}

void
OFVALUE::set( const char *data, ofuint32 dataLength )
{
    ofuint32 newlen = dataLength + REFCOUNTSIZE;
    if ( DYNTYPE( type ) )
        destroy( );
    type = typeBinary;
    if ( dataLength == 0 )
    {
         value.valmem.capacity = value.valmem.size = 0;
         value.valmem.buffer = NULL;
         return;
    }
    value.valmem.capacity = value.valmem.size = newlen;
    value.valmem.buffer = new char[newlen];
    memcpy( ((char *)value.valmem.buffer)+REFCOUNTSIZE, data, dataLength );
    REFCOUNT(value) = 1;
//     cout << hex << (void*)(value.valmem.buffer) << dec << " set(char*, ofuint32): " << REFCOUNT( value ) << endl;
}

void
OFVALUE::set_nameValuei( const char *name, const char *new_value )
{
    char *buf = new char[OFOS::strlen(name) +
                         1 + 
                         OFOS::strlen(new_value) + 1];
    OFOS::strcpy(buf, name);
    OFOS::strcat(buf, "=");
    
    ofuint32 index = 0;
    for (ofint64 j = 1; !index && j <= listSize(); ++j)
    {
        OFVALUE temp = listRetrieve( j );
        if ( !OFOS::strnicmp( temp.get_string(), buf, OFOS::strlen( buf ) ) )
        {
            index = j;
            OFOS::strcat(buf, new_value);
            temp.set( buf );
            listReplace( &temp, j );
        }
    }

    if ( !index )
    {
        OFOS::strcat(buf, new_value); 
        OFVALUE temp;           
        temp.set( buf );
        listAdd( &temp );
    }

    delete [] buf;
}

OFVALUE
OFVALUE::get_nameValuei( const char *name )
{    
        assert (type == typeListString);

        OFVALUE temp;
        const char *cur_value=0;

        for (ofint64 j = 1; !cur_value && j <= listSize(); ++j)
        {
                temp = listRetrieve( j );
                assert( temp.type == typeString );

                const char *s = temp.get_string();
                const char *eq = OFOS::strchr( s, '=' );
                if ( eq && eq - s == (int)OFOS::strlen( name ) )
                {
                        if ( !OFOS::strnicmp( s, name, OFOS::strlen( name ) ) )
                        {
                                cur_value = eq + 1;
                        }
                }
        }

        OFVALUE val;

        if ( cur_value )
                val.set( cur_value );
        else
                val.set( "" );

        return val;
}


OFIDENTITY
OFVALUE::get_OFIDENTITY( ) const
{
    OFIDENTITY id( value.valid.id[0], value.valid.id[1], value.valid.flags );
    return id;
}

const char *
OFVALUE::get_string() const
{
    return ((char *)value.valmem.buffer)+REFCOUNTSIZE;
}

const char *
OFVALUE::get_binary() const
{
     if ( value.valmem.size != 0 )
          return ((char *)value.valmem.buffer)+REFCOUNTSIZE;
     return NULL;
}

ofuint32
OFVALUE::get_binarylength() const
{
     if ( value.valmem.size != 0 )
          return value.valmem.size - REFCOUNTSIZE;
     return 0;
}

void
OFVALUE::write( StorageBlob *blob )
{
//     cout << "Writing value of type:" << (ofint32)type << endl;
    blob->writeInt32( type );
    if ( DYNTYPE( type ) )
    {
        if ( !LISTTYPE( type ) )
        {
            if ( value.valmem.size > REFCOUNTSIZE )
            {
                 blob->writeInt32( value.valmem.size - REFCOUNTSIZE );
                 blob->writeBinary( (char *)value.valmem.buffer + REFCOUNTSIZE,
                                    value.valmem.size - REFCOUNTSIZE );
            }
            else
            {
                 blob->writeInt32( 0 );
            }
        }
        else
        {
            blob->writeInt32( LISTSIZE(value) );
            for ( ofuint32 cur = 0; cur < LISTSIZE(value); cur++ )
            {
                LISTELEM( value, cur ).write( blob );
            }
        }
    }
    else
    {
        OFIDENTITY x;
        switch( type )
        {
        case typeInteger:
            blob->writeInt64( value.valint );
            break;
        case typeBoolean:
            blob->writeInt08( value.valbool ? 1 : 0 );
            break;
        case typeFloat:
            blob->writeDouble( value.valdbl );
            break;
        case typeOFDateTime:
            blob->writeDateTime( &value.valdate );
            break;
        case typeIdentity:
            x.m_id0 = value.valid.id[0];
            x.m_id1 = value.valid.id[1];
            x.m_flags = value.valid.flags;
            blob->writeIdentity( &x );
            break;
        default:
            assert(0);
            break;
        }
    }
}

void
OFVALUE::read( StorageBlob *blob, signed char *ocaltype )
{
    type = blob->readInt32();
    if ( DYNTYPE( type ) )
    {
         // If it's a list of dynamic type, read in each element.
         // If not, read in the entire binary chunk
         if ( !LISTTYPE( type ) )
         {
              value.valmem.size = value.valmem.capacity = blob->readInt32();
              if ( value.valmem.size )
              {
                   value.valmem.size += REFCOUNTSIZE;
                   value.valmem.capacity += REFCOUNTSIZE;
                   value.valmem.buffer = new char[value.valmem.size];
                   blob->readBinary( (char *)value.valmem.buffer + REFCOUNTSIZE,
                                     value.valmem.size - REFCOUNTSIZE );
                   REFCOUNT( value ) = 1;
              }
         }
         else
         {
              ofuint32 i = blob->readInt32( );
              listAllocate( i );
              value.valmem.size = REFCOUNTSIZE + (i) * sizeof( OFVALUE );
              for ( ofuint32 cur = 0; cur < LISTSIZE(value); cur++ )
              {
                   LISTELEM(value,cur).read( blob, 0 );
              }
         }
    }
    else
    {
        OFIDENTITY x;
        switch( type )
        {
        case typeInteger:
            value.valint = blob->readInt64( );
            break;
        case typeBoolean:
            value.valbool = ( blob->readInt08( ) != 0 );
            break;
        case typeFloat:
            value.valdbl = blob->readDouble( );
            break;
        case typeOFDateTime:
            blob->readDateTime( &value.valdate );
            break;
        case typeIdentity:
            blob->readIdentity( &x );
            value.valid.id[0] = x.m_id0;
            value.valid.id[1] = x.m_id1;
            value.valid.flags = x.m_flags;
            break;
        default:
            assert(0);
            break;
        }
    }
    if ( ocaltype )
        *ocaltype = type;
}

void
OFVALUE::destroy( )
{
    if ( DYNTYPE( type ) && value.valmem.buffer )
    {
#if !defined(NDEBUG)
        //cout << hex << (void*)(value.valmem.buffer) << dec << " destroy ref count: 0x" << hex << REFCOUNT(value) << dec << endl;
        assert( REFCOUNT(value) != 0xdead );
        assert( REFCOUNT(value) );
        //if (REFCOUNT(value) > 3500)
        //        cout << hex << (void*)(value.valmem.buffer) << dec << " destroy ref count: 0x" << hex << REFCOUNT(value) << dec << endl;
        //assert( REFCOUNT(value) < 4000 );
#endif
        if ( REFCOUNT(value) != -1 )
        {
            // If we're a dynamic list, decrement the reference count of each 
            // element in the list
            ofuint32 i;
            for (;;)
            {
                i = REFCOUNT(value);
                if ( compare_and_swap_int( REFCOUNTADDR(value), i, i-1 ) )
                    break;
            }

            if ( i == 1 )
            {
#if !defined(NDEBUG)
                REFCOUNT(value) = 0xdead;
#endif
                delete [] (char *)value.valmem.buffer;
                value.valmem.size = value.valmem.capacity = 0;
                value.valmem.buffer = 0;
            }
        }
    }
    type = 0;
}

void
OFVALUE::decrementRefCount()
{
    if ( DYNTYPE( type ) && value.valmem.buffer && REFCOUNT(value) != -1 && REFCOUNT(value) > 0 )
    {
        ofuint32 i;
        for (;;)
        {
            i = REFCOUNT(value);
            if ( compare_and_swap_int( REFCOUNTADDR(value), i, i-1 ) )
                break;
        }
    }
}

void
OFVALUE::incrementRefCount()
{
    if ( DYNTYPE( type ) && value.valmem.buffer && REFCOUNT(value) != -1 )
    {
        ofuint32 i;
        for (;;)
        {
            i = REFCOUNT(value);
            if ( compare_and_swap_int( REFCOUNTADDR(value), i, i+1 ) )
                break;
        }

    }
}

void
OFVALUE::noRefCounting()
{
    if ( DYNTYPE( type ) && value.valmem.buffer )
    {
        ofuint32 i;
        for (;;)
        {
            i = REFCOUNT(value);
            if ( compare_and_swap_int( REFCOUNTADDR(value), i, (ofuint32)-1 ) )
                break;
        }
    }
}

ofuint32
OFVALUE::convertToString( char **buffer )
{
    char *buf = 0;
    ofuint32 buflen = 0;
    OFDateTime dt;

    if ( !type )
    {
        *buffer = new char[1];
        **buffer = 0;
        return 0;
    }
    if ( LISTTYPE( type ) )
    {
        if ( LISTSIZE(value) == 0 )
        {
            *buffer = new char[6];
            OFOS::strcpy( *buffer, "<< >>" );
            return OFOS::strlen( *buffer );
        }
        // stick in the opening angle brackets
        buflen = 4;
        char first = 1;
        buf = new char[buflen];
        OFOS::strcpy( buf, "<< " );
        for ( ofuint32 cur = 0; cur < LISTSIZE(value); cur++ )
        {
            char *tbuf = 0;
            char last = ( cur == LISTSIZE(value) - 1 );
            ofuint32 chunkLen = LISTELEM(value,cur).convertToString( &tbuf );
            buflen += chunkLen + (first?0:3) + (last?4:0);
            char *cbuf = new char[buflen];
            OFOS::strcpy( cbuf, buf );
            if ( !first )
                OFOS::strcat( cbuf, ", " );
            first = 0;
            OFOS::strcat( cbuf, tbuf );
            if ( last )
                OFOS::strcat( cbuf, " >>" );
            delete [] tbuf;
            delete [] buf;
            buf = cbuf;
        }
        *buffer = buf;
        return OFOS::strlen( buf );
    }

    // First work out the required length
    if ( !DYNTYPE( type ) ) // Maximum length (identity 38 bytes + null)
        buflen = 39;
    else
    {
        // Its a string or binary
        if ( type == typeString )
            buflen = value.valmem.size + 1;
        else
            buflen = OFOS::strlen( VALUE_BINSTR ) + 1;
    }
    buf = new char[buflen];
    // These types can only be atomic
    switch ( type )
    {
    case typeInteger:
        OFOS::snprintf( buf, buflen-1, LLFORMAT "d", value.valint );
        break;
    case typeBoolean:
        OFOS::snprintf( buf, buflen - 1, "%d", value.valbool );
        break;
    case typeString:
        OFOS::strcpy( buf, get_string() );
        break;
    case typeFloat:
        OFOS::snprintf( buf, buflen-1, "%g", value.valdbl );
        break;
    case typeOFDateTime:
        dt = value.valdate;
        OFOS::snprintf( buf, buflen-1, "%s", dt.toString().c_str() );
        break;
    case typeBinary:
        OFOS::strcpy( buf, VALUE_BINSTR );
        break;
    case typeIdentity:
        OFOS::snprintf( buf, buflen-1,
#if defined(OFOPSYS_WIN32)
                        "%04hx%016I64x%016I64x",
#else
                        "%04hx%016llx%016llx",
#endif
                        value.valid.flags, value.valid.id[0], value.valid.id[1] );
        break;

    }
    *buffer = buf;
    return OFOS::strlen( *buffer );
}

ofuint32
OFVALUE::convertToString2( char **buffer )
{
    char *buf = 0;
    ofuint32 buflen = 0;
    OFDateTime dt;

    if ( !type )
    {
        *buffer = new char[1];
        **buffer = 0;
        return 0;
    }
    if ( LISTTYPE( type ) )
    {
        if ( LISTSIZE(value) == 0 )
        {
            *buffer = new char[25];
            char typeName[129];
            OFVALUE::translateType( type - 8, typeName );
            OFOS::snprintf( *buffer, 24, "emptyList(%s)", typeName+4 );
            return OFOS::strlen( *buffer );
        }
        // stick in the opening angle brackets
        buflen = 4;
        char first = 1;
        buf = new char[buflen];
        OFOS::strcpy( buf, "<< " );
        for ( ofuint32 cur = 0; cur < LISTSIZE(value); cur++ )
        {
            char *tbuf = 0;
            char last = ( cur == LISTSIZE(value) - 1 );
            ofuint32 chunkLen = LISTELEM(value,cur).convertToString( &tbuf );
            if ( type == typeListString )
                    chunkLen += 2;

            buflen += chunkLen + (first?0:3) + (last?4:0);
            char *cbuf = new char[buflen];
            OFOS::strcpy( cbuf, buf );
            if ( !first )
                OFOS::strcat( cbuf, ", " );
            first = 0;
            if ( type == typeListString ) OFOS::strcat (cbuf, "\"");
            OFOS::strcat( cbuf, tbuf );
            if ( type == typeListString ) OFOS::strcat (cbuf, "\"");

            if ( last )
                OFOS::strcat( cbuf, " >>" );
            delete [] tbuf;
            delete [] buf;
            buf = cbuf;
        }
        *buffer = buf;
        return OFOS::strlen( buf );
    }

    // First work out the required length
    if ( !DYNTYPE( type ) ) // Maximum length (identity 38 bytes + null)
        buflen = 39;
    else
    {
        // Its a string or binary
        if ( type == typeString )
            buflen = value.valmem.size + 1;
        else
            buflen = OFOS::strlen( VALUE_BINSTR ) + 1;
    }
    buf = new char[buflen];
    // These types can only be atomic
    switch ( type )
    {
    case typeInteger:
        OFOS::snprintf( buf, buflen-1, LLFORMAT "d", value.valint );
        break;
    case typeBoolean:
        OFOS::snprintf( buf, buflen - 1, "%d", value.valbool );
        break;
    case typeString:
        OFOS::strcpy( buf, get_string() );
        break;
    case typeFloat:
        OFOS::snprintf( buf, buflen-1, "%g", value.valdbl );
        break;
    case typeOFDateTime:
        dt = value.valdate;
        OFOS::snprintf( buf, buflen-1, "%s", dt.toString().c_str() );
        break;
    case typeBinary:
        OFOS::strcpy( buf, VALUE_BINSTR );
        break;
    case typeIdentity:
        OFOS::snprintf( buf, buflen-1,
#if defined(OFOPSYS_WIN32)
                        "%04hx%016I64x%016I64x",
#else
                        "%04hx%016llx%016llx",
#endif
                        value.valid.flags, value.valid.id[0], value.valid.id[1] );
        break;

    }
    *buffer = buf;
    return OFOS::strlen( *buffer );
}

ofint32
OFVALUE::operator ==(const OFVALUE &rhs)
{
        if (type != rhs.type)
                return 1;

        ofuint32 j;
        switch (type)
        {
        case typeInteger:
                return value.valint == rhs.value.valint;
        case typeBoolean:
                return value.valbool == rhs.value.valbool;
        case typeString:
                return (OFOS::stricmp(get_string(), rhs.get_string())==0);
        case typeFloat:
                return value.valdbl == rhs.value.valdbl;
        case typeOFDateTime:
                return value.valdate == rhs.value.valdate;
        case typeIdentity:
                return get_OFIDENTITY() == rhs.get_OFIDENTITY();
        case typeBinary:
                if (get_binarylength() != rhs.get_binarylength())
                        return 0;
                return (memcmp(get_binary(),
                               rhs.get_binary(),
                               get_binarylength()) == 0);
        case typeListInteger:
        case typeListBoolean:
        case typeListString:
        case typeListFloat:
        case typeListOFDateTime:
        case typeListIdentity:
        case typeListBinary:
                if (LISTSIZE(value) != LISTSIZE(rhs.value))
                        return 0;
                for (j=1; j <= LISTSIZE(value); j++)
                {
                        OFVALUE lhs_elem = listRetrieve (j);
                        OFVALUE rhs_elem = listRetrieve (j);
                        if ( !(lhs_elem == rhs_elem))
                                return 0;
                }
        }
        return 1;
}

ofint32 OFVALUE::operator != (const OFVALUE& rhs)
{
    return !operator==(rhs);
}

void OFVALUE::translateType(ofint32 type, char *typeName)
{
    OFOS::strcpy( typeName, "void" );
    if ( type == -1 )
        OFOS::strcpy( typeName, "class" );
    else
    {
        const char *tn = translate_OFVALUE_TYPES2( type );
        if ( tn )
            OFOS::strcpy( typeName, tn );
    }
}

void OFSYS_API OFVALUEfromOFIDENTITYLIST( OFIDENTITYLIST* src, OFVALUE* dest )
{
    dest->type = typeListIdentity;
    for ( OFIDENTITYLIST::iterator i = src->begin();
          i != src->end();
          i++ )
    {
        OFVALUE elem;
        elem.set (*i);
        dest->listAdd (&elem);
        delete (*i);
    }
}

void OFSYS_API OFVALUEfromOFCHARSTARLIST(OFCHARSTARLIST* src, OFVALUE* dest)
{
    dest->type = typeListString;
    for (OFCHARSTARLIST::iterator i = src->begin();
         i != src->end(); i++)
    {
        OFVALUE elem;
        elem.set(*i);
        dest->listAdd(&elem);
        delete[] (*i);
    }
}

ofuint64 OFSYS_API OFIDENTITYLISTfromOFVALUE(OFVALUE* src, OFIDENTITYLIST* dest)
{
    assert(src);
    assert(dest);
    ofuint64 cnt = 0;
    if (!src->listSize())
        return cnt;
    for (ofint64 cur = 1; cur <= src->listSize(); cur++)
    {
        OFVALUE elem = src->listRetrieve(cur);
        OFIDENTITY *ne = new OFIDENTITY(elem.get_OFIDENTITY());
        dest->push_back(ne);
        cnt++;
    }
    return cnt;
}

/////////////////////////////////////////////////////////////
// Unit tests

#if defined(UNIT_TEST)

#include <UnitTest.h>

int
main( int argc, char *argv[] )
{

    UT_BATCH_START( "ofvalue" );

    UT_TEST_START( 1, "void OFVALUE::set(const char *)" );
    const char *hello = "Hello World.";
    OFVALUE value1;
    value1.set( hello );
    cout << value1.get_string() << endl;
    UT_EVAL_EXPR( !OFOS::stricmp( value1.get_string(), hello ) );
    UT_TEST_END;

    UT_TEST_START( 2, "void OFVALUE::populate(OFVariant *)" );
    OFVariant vnt2( hello );
    OFVALUE value2;
    value2.populate( &vnt2 );
    cout << value2.get_string() << endl;
    UT_EVAL_EXPR( ( !OFOS::stricmp( value2.get_string(), hello ) ) );
    UT_TEST_END;

    UT_TEST_START( 3, "OFVariant *OFVALUE::populate()" );
    OFVariant *vnt3 = value2.populate();
    cout << vnt3->cv_pcstr() << endl;
    UT_EVAL_EXPR( ( !OFOS::stricmp( vnt3->cv_pcstr(), hello ) ) );
    delete vnt3;
    UT_TEST_END;
    
    UT_TEST_START( 4, "void OFVALUE::populate(OFVariant*) LIST" );
    OFVariant vntList;
    vntList.makeList( );
    for ( ofuint32 cur = 0; cur < 5; cur++ )
    {
        OFVariant *elem = new OFVariant( (ofint64)cur );
        vntList.listAdd( elem );
    }
    OFVALUE value4;
    value4.populate( &vntList );
    char *str4;
    value4.convertToString( &str4 );
    cout << str4 << endl;
    UT_EVAL_EXPR( !OFOS::stricmp( str4, "<< 0, 1, 2, 3, 4 >>" ) );
    delete [] str4;
    UT_TEST_END;

    UT_TEST_START( 5, "void OFVALUE::populate(OFVariant*) LIST(char*)" );
    OFVariant vntList5;
    vntList5.makeList( );
    for ( ofuint32 cur = 0; cur < 5; cur++ )
    {
        char temp5[129];
        OFOS::snprintf( temp5, 128, "word%ld", cur );
        OFVariant *elem = new OFVariant( temp5 );
        vntList5.listAdd( elem );
    }
    OFVALUE value5;
    value5.populate( &vntList5 );
    char *str5;
    value5.convertToString( &str5 );
    UT_EVAL_EXPR( !OFOS::stricmp( str5, "<< word0, word1, word2, word3, word4 >>" ) );
    delete [] str5;
    UT_TEST_END;

    UT_TEST_START( 6, "void OFVALUE::listAdd( OFVALUE* )" );
    OFVALUE value6;
    value6.set( "Another word" );
    value5.listAdd( &value6 );
    char *str6;
    value5.convertToString( &str6 );
    UT_EVAL_EXPR( !OFOS::strcmp( str6, "<< word0, word1, word2, word3, word4, Another word >>" ) );
    delete [] str6;
    UT_TEST_END;

    UT_TEST_START( 7, "void OFVALUE::listAdd( OFVALUE *, ofint64 )" );
    OFVALUE value7;
    value7.set( "Inserted" );
    value5.listAdd( &value7, 3 );
    char *str7;
    value5.convertToString( &str7 );
    cout << "str7: [" << str7 << "]" << endl;
    UT_EVAL_EXPR( !OFOS::strcmp( str7, "<< word0, word1, Inserted, word2, word3, word4, Another word >>" ) );
    delete [] str7;
    UT_TEST_END;

    UT_TEST_START( 8, "void OFVALUE::listRemove( ofint64 )" );
    value5.listRemove( 3 );
    char *str8;
    value5.convertToString( &str8 );
    UT_EVAL_EXPR( !OFOS::strcmp( str8, "<< word0, word1, word2, word3, word4, Another word >>" ) );
    delete [] str8;
    UT_TEST_END;

    UT_TEST_START( 9, "void OFVALUE::listReplace( ofint64 )" );
    OFVALUE value9;
    value9.set( "Replaced" );
    value5.listReplace( &value9, 3 );
    char *str9;
    value5.convertToString( &str9 );
    cout << str9 << endl;
    UT_EVAL_EXPR( !OFOS::strcmp( str9, "<< word0, word1, Replaced, word3, word4, Another word >>" ) );
    delete [] str9;

    UT_TEST_START( 10, "void OFVALUE::listRetrieve( ofint64 )" );
    OFVALUE value10 = value5.listRetrieve( 3 );
    cout << value10.get_string() << endl;
    UT_EVAL_EXPR( !OFOS::strcmp( value10.get_string(), "Replaced" ) );
    UT_TEST_END;

    UT_TEST_START( 11, "ofint64 OFVALUE::listSize()" );
    ofint64 int11 = value5.listSize();
    UT_EVAL_EXPR( int11 == 6 );
    UT_TEST_END;

    UT_TEST_START( 12, "void populate(OFVariant *) OFIDENTITY" );
    //                012345678901234567890123456789012345
    //                                    0000000000000063
    OFIDENTITY id12( "0000800000000000000c0000000000000071" );
    OFVariant variant12( id12 );
    OFVALUE value12;
    value12.populate( &variant12 );
    char *str12;
    value12.convertToString( &str12 );
    cout << str12 << endl;
    UT_EVAL_EXPR( !OFOS::stricmp( str12, "0000800000000000000c0000000000000071" ) );
    delete [] str12;
    UT_TEST_END;

    UT_TEST_START( 13, "OFIDENTITY OFVALUE::get_OFIDENTITY()" );
    OFIDENTITY id13( value12.get_OFIDENTITY() );
    OFIDENTITY id13a( "0000800000000000000c0000000000000071" );
    UT_EVAL_EXPR( id13 == id13a );
    UT_TEST_END;
    
    UT_TEST_START( 14, "void OFVALUE::listAdd()" );
    OFVALUE value14;
    OFVALUE elem14a, elem14b, elem14c;
    elem14a.set( "One" );
    elem14b.set( "Two" );
    elem14c.set( "Three" );
    value14.listAdd( &elem14a );
    value14.listAdd( &elem14b );
    value14.listAdd( &elem14c );
    char *str14;
    value14.convertToString( &str14 );
    cout << str14 << endl;
    delete [] str14;

    UT_TEST_END;
    
    UT_TEST_START( 15, "OFVALUE &operator=( const OFVALUE &rhs )" );

    OFVALUE value15a;
    value15a.set( (ofint64)5 );
    value15a.value.valmem.buffer = (void *)129;
    OFVALUE value15b;
    value15b = value15a;
    UT_EVAL_EXPR( value15a.value.valint == value15b.value.valint );

    UT_TEST_END;

    UT_TEST_START( 16, "OFVALUE &operator=( const OFVALUE &rhs ) (OFIDENTITY)" );
    OFIDENTITY id16a( "0001800000000000000c0000000000000071" );
    OFVALUE value16;
    value16.set( &id16a );
    OFVALUE value16b;
    value16b = value16;
    OFIDENTITY id16b = value16b.get_OFIDENTITY();
    UT_EVAL_EXPR( id16a == id16b );
    UT_TEST_END;

    UT_TEST_START( 17, "OFVariant *OFVALUE::populate() (list of integer)" );
    OFVALUE value17, elem17;
    elem17.set( MAKELL(1) ); value17.listAdd( &elem17 );
    elem17.set( MAKELL(2) ); value17.listAdd( &elem17 );
    elem17.set( MAKELL(3) ); value17.listAdd( &elem17 );
    elem17.set( MAKELL(4) ); value17.listAdd( &elem17 );
    elem17.set( MAKELL(5) ); value17.listAdd( &elem17 );
    elem17.set( MAKELL(6) ); value17.listAdd( &elem17 );
    elem17.set( MAKELL(7) ); value17.listAdd( &elem17 );
    OFVariant *variant17 = value17.populate();
    UT_EVAL_EXPR( !OFOS::stricmp( variant17->cv_pcstr(), "<< 1, 2, 3, 4, 5, 6, 7 >>" ) );
    delete variant17;
    UT_TEST_END;

    UT_TEST_START( 18, "OFVariant *OFVALUE::populate() (list of string)" );
    OFVALUE value18, elem18;
    elem18.set( "one" ); value18.listAdd( &elem18 );
    elem18.set( "two" ); value18.listAdd( &elem18 );
    elem18.set( "three" ); value18.listAdd( &elem18 );
    elem18.set( "four" ); value18.listAdd( &elem18 );
    elem18.set( "five" ); value18.listAdd( &elem18 );
    OFVariant *variant18 = value18.populate();
    cout << "variant18: [" << variant18->cv_pcstr() << "]" << endl;
    UT_EVAL_EXPR( !OFOS::stricmp( variant18->cv_pcstr(), "<< one, two, three, four, five >>" ) );
    delete variant18;
    UT_TEST_END;

    UT_TEST_START( 19, "OFVALUE::listAdd(OFVALUE*,int)" );
    OFVALUE value19, elem19;
    elem19.set( "one" ); value19.listAdd( &elem19 );
    elem19.set( "two" ); value19.listAdd( &elem19, 1 );
    char* str19;
    value19.convertToString( &str19 );
    cout << "str19: [" << str19 << "]" << endl;
    UT_EVAL_EXPR( !OFOS::stricmp( str19, "<< two, one >>" ) );
    delete [] str19;
    UT_TEST_END;

    UT_TEST_START (20, "operator ==(const OFVALUE&)");
    OFVALUE value20_1; value20_1.set ("Hello");
    OFVALUE value20_2; value20_2.set ("Hello");
    UT_EVAL_EXPR (value20_1 == value20_2);

    value20_2.set ("Goodbye");
    UT_EVAL_EXPR (!(value20_1 == value20_2));

    OFVALUE value20_3 = value19;
    UT_EVAL_EXPR (value20_3 == value19);
    UT_EVAL_EXPR (!(value20_3 == value18 ));
    UT_TEST_END;

    UT_TEST_START (21, "ofint64 listFind (const OFVALUE &) const");

    OFVALUE value21;
    OFVALUE elem21; elem21.set ("One"); value21.listAdd (&elem21);
    elem21.set ("Two"); value21.listAdd (&elem21);
    elem21.set ("Three"); value21.listAdd (&elem21);

    OFVALUE value21a; value21a.set ("One");
    OFVALUE value21b; value21b.set ("Four");
        
    UT_EVAL_EXPR ( value21.listFind (&value21a) );
    UT_EVAL_EXPR ( !value21.listFind (&value21b) );
    UT_TEST_END;

    UT_TEST_START(22, "OFIDENTITYLISTfromOFVALUE");
    OFVALUE value22;
    OFIDENTITY id22("0000000000000000000100000000000000a0");
    OFVALUE elem22a; elem22a.set(&id22);
    id22 = "00000000000000000001000000000000009f";
    OFVALUE elem22b; elem22b.set(&id22);
    id22 = "00000000000000000001000000000000009e";
    OFVALUE elem22c; elem22c.set(&id22);
    value22.listAdd(&elem22a);
    value22.listAdd(&elem22b);
    value22.listAdd(&elem22c);

    OFIDENTITYLIST list22;

    ofuint64 cnt22 = OFIDENTITYLISTfromOFVALUE(&value22, &list22);

    UT_EVAL_EXPR(cnt22 == 3);
    UT_EVAL_EXPR(*list22[0] == OFIDENTITY("0000000000000000000100000000000000a0"));
    UT_EVAL_EXPR(*list22[2] == OFIDENTITY("00000000000000000001000000000000009e"));
    UT_TEST_END;

    UT_BATCH_END;
    return 0;
}
#endif // #if defined(UNIT_TEST)

