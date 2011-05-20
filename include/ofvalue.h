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

#if !defined(_OFVALUE_H_)
#define _OFVALUE_H_

#include <ofdatetime.h>
#include <storageblob.h>

#define DYNTYPE(a) ( a == typeBinary || a == typeString || ( a > 0 && ( a & 8 ) ) )
#define LISTTYPE(a) ( a & 8 )
#define DYNLIST(a) ( a & 8 && ( a == typeListBinary || a == typeListString ) )

/** \struct OFVALUE ocalcodestruct.h
    Provides a more efficient handling of values for internal use.

    \nosubgrouping
 */
struct OFSYS_API OFVALUE
{
    /** \name Data members
     */
    //@{

    union OFVALUE_
    {
        ofint64 valint;
        double valdbl;
        bool valbool;
        struct OFVALUEID_
        {
            ofuint64 id[2];
            char flags;
        } valid;
        OFDateTime valdate;
        struct OFVALUEMEM_
        {
            void * buffer;
            ofuint32 size;
            ofuint32 capacity;
        } valmem;
    } value;

    signed char type;

    enum
    {
        refCountSize = sizeof( ofint32 )
    };

    //@}

    /** \name Constructors and destructors
     */
    //@{

    /** Default constructor
     */
    OFVALUE();

    /** Type initialising constructor
     */
    OFVALUE(ofuint32 newType);

    /** Copy constructor

        This is a shallow reference counted copy. 
        Replacing items in copied lists will modify the originals, too 
        (if they still exist).
    */
    OFVALUE( const OFVALUE &copy );

    /** Destructor
     */
    ~OFVALUE();

    //@}

    /** \name Operators
     */
    //@{

    OFVALUE &operator=( const OFVALUE &rhs );

    void copy( const OFVALUE &copy );

    //@}

    /** \name Helper methods
     */
    //@{

    static void translateType(ofint32 type, char *typeName);

    //@}

    /** \name Reading and writing members
     */
    //@{

    /** Write the value to the supplied StorageBlob
        Writes the value and the type.
    */
    void write( StorageBlob *blob );

    /** Read in the value from a StorageBlob.
        Also returns the type of the value so it can be stored
        in the owning OCALCODEITEM.
    */
    void read( StorageBlob *blob, signed char *ocaltype );

    //@}

    /** \name Value manipulation
     */
    //@{

    /** Populates this from an OFVariant.
     */
    void populate( OFVariant *variant );

    /** Create a new OFVariant from the value.
        The OFVariant is created on the heap, and it is the callers
        responsibility to destroy it.
    */
    OFVariant *populate( );

    void set( const OFIDENTITY *id );
    void set( const char *str );
    void set( ofint64 integer );
    void set( const bool b );
    void set( const double d );
    void set( const OFDateTime dt );

    /** Set the value from the supplied binary
     */
    void set( const char *data, ofuint32 dataLength );

    /** For typeListString get the value of a name value pair (name=value)
        from an item.
        Case insensitive.
    */
    OFVALUE get_nameValuei( const char *name );

    /** For typeListString set the value of a name value pair (name=value)
        in an item.
        Appends if the key is not found.
        Case insensitive.
    */
    void set_nameValuei( const char *name, const char *value );

    OFIDENTITY get_OFIDENTITY( ) const;

    const char *get_string() const;

    const char *get_binary() const;

    ofuint32 get_binarylength() const;

    /** Convert the supplied type to another.
     */
    /*     static ofuint32 convert( OFVALUE *value, ofuint32 type ); */

    //@}

    /** \name List methods
     */
    //@{

    /** Returns non-zero if the value is a list type
     */
    const char isList() const;

    /** Allocate the specified list size
        Does not allocate the node buffers if the type is dynamic
    */
    void listAllocate( ofuint32 listSize );

    /** Add a list value.
        The reference count is incremented for the element.
    */
    void listAdd( OFVALUE *element );

    /** Add a list element at a specified index
        The reference count is incremented for the element.
    */
    void listAdd( OFVALUE *element, ofint64 index );

    /** Remove a list value (by index)
     */
    ofuint32 listRemove( ofint64 index );

    /** Replace a list value.
        The new element will have its reference count incremented, and that
        of the replaced element will be decremented.
    */
    ofuint32 listReplace( OFVALUE *element, ofint64 index );

    /** Retrieve a list element
        The returned OFVALUE will have its reference count incremented.
    */
    OFVALUE listRetrieve( ofint64 index );

    /** Retrieve a list's size
     */
    ofint64 listSize( ) const;

    /** Increase the list storage size
     */
    void listIncrease( ofuint32 inc );

    /** Search the list for the specified value
        \return The index of the value if found, or zero.
    */
    ofint64 listFind (const OFVALUE *val) const;
                
    //@}

    /** Decrement the reference count.
        Use with care.
    */
    void decrementRefCount();

    /** Increment the reference count.
     */
    void incrementRefCount();

    /** Clear any reference counting for this value
     */
    void noRefCounting();

    /** Convert to a string.
        This method will allocate the buffer, and it is the callers
        responsibility to destroy it.

        \return The length of the allocated buffer.
    */
    ofuint32 convertToString( char **buffer );

    /** Same as the normal convert to string, with quotes for lists of strings
     */
    ofuint32 convertToString2( char **buffer );

    void destroy( );

    void cp_nd_inc_( OFVALUE* dest, OFVALUE* src );

    /** Equality operator
     */
    ofint32 operator ==(const OFVALUE& rhs);

    /** Non-equality operator
     */
    ofint32 operator !=(const OFVALUE &rhs);

};

/** Destroy an OFVALUE
 */
void OFSYS_API destroyOFVALUE( OFVALUE *value, signed char type, bool doDelete = false );

/** Translate a variant type to an OFVALUE type.
    OCAL_EXT_TYPE is translate to typeIdentity.
 */
signed char OFSYS_API translateVariantType( OFVariant *variant );

/** Populate an OFVALUE from an OFIDENTITYLIST.
    Also destroys each element in the OFIDENTITYLIST
 */
void OFSYS_API OFVALUEfromOFIDENTITYLIST( OFIDENTITYLIST* src, OFVALUE* dest );

/** Populates an OFVALUE from an OFCHARSTARLIST

    Also destroy each string in the OFCHARSTARLIST
*/
void OFSYS_API OFVALUEfromOFCHARSTARLIST(OFCHARSTARLIST* src, OFVALUE* dest);

/** Create an OFIDENTITYLIST from an OFVALUE
 */
ofuint64 OFSYS_API OFIDENTITYLISTfromOFVALUE(OFVALUE* src, OFIDENTITYLIST* dest);

#endif // #if !defined(_OFVALUE_H_)
