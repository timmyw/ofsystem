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

#if !defined( OF_VARIANT_H_)
#define OF_VARIANT_H_

#include <ofdatetime.h>
#include <storageblob.h>
#include <memory>

class OFFile;

/**
 * This class provides a wrapper around a general variable class.
 *
 * At present: strings, floats (doubles), ofuint32 integers, 
 * signed ofint32 integers, datetime (valid until 2039 AD) and boolean values
 * are supported.  More will be added in the future.
 * Doesn't handle overflow, underflow or loss of sign.
 \nosubgrouping
*/
class OFSYS_API OFVariant 
{

 public:
    /** \name Constructors and destructor.
     */
    //@{

    /** Constructor. Object is empty and has no type.*/
    OFVariant( );
    
    /** Constructor which creates a new string variant.
        This variant will contain the supplied string, and be
        initialised to OFVariant::TypeString.*/
    OFVariant( const char * str );
    
    /** Constructor which creates a new ofuint32 variant.
        This variant will contain the supplied value, and be
        initialised to OFVariant::TypeWord.*/
    OFVariant( const ofint64 dwValue );
    
    /** Constructor which creates a new boolean variant.
        This variant will contain the supplied value, and be
        initialised to OFVariant::TypeBoolean.*/    
    OFVariant( const bool bValue );
    
    /** Constructor which creates a new double variant.
        This variant will contain the supplied value, and be
        initialised to OFVariant::TypeFloat.*/    
    OFVariant( const double dblValue );
    
    /** Constructor which creates a new OFDateTime variant.
        This variant will contain the supplied value, and be
        initialised to OFVariant::TypeLong.the number of seconds 
        that have passed since 00:00:00 on January 1, 1970 */    
    OFVariant( const OFDateTime oftValue );        
    
    /** Constructor which creates a new Identity.
        This variant will contain the supplied value, and be
        initialised to OFVariant::TypeIdentity .*/    
    explicit OFVariant( const OFIDENTITY &id );        

    /** Constructor which creates a new binary variant.
     */
    OFVariant( const void *data, ofuint32 length );

    /** Constructor which creates a copy of an existing variant.
        The new copy will be an exact match of the orginal.
        New buffers are created, the copy does not poofint32 to the original. */
    OFVariant( const OFVariant &copy );

    /** Constructor which creates a copy of an existing variant on 
        the stack as an instantiated object. */
    explicit OFVariant( const OFVariant* const pointer ) ;
    
    /** Destructor.
        All buffers are destroyed. */
    ~OFVariant( );
    //@}

    /** \name Operators
     */
    //@{

    /** operator =
     */
    const OFVariant& operator = (const OFVariant& rhs);
    
    /** Unary operator ' - ' for the restricted type set [ double, ofint32 ]
     */
    OFVariant operator -() const ;
    
    /** Boolean operator ' ! ' for the restricted type set [ ofuint32, bool, ofint32 ]
     */
    bool operator !() const ;
    //@}

    // used as sentinel values for undefined values
    
    static bool emptyBoolean;
    static const char * emptyString;
    static double emptyFloat;
    static ofint32 emptyInteger;
    static ofint32 emptyOFDateTime;
    static OFIDENTITY emptyIdentity;
    static const char * emptyList;

    static const char *blankBinary;

    /** \name List operations.
     */
    //@{

    /** Is the variant a list.
     */
    bool isList( ) const;

    /** Is the variant a scalar value.
     */
    bool isScalar( ) const;

    /** Turn the variant into a list.
        \param clearList A boolean flag to specify whether the new list is empty or not.
        If the parameter is false, any existing value in the variant will be used as the
        first element in the list.
    */
    void makeList( bool clearList = false );

    /** Turn the variant into a scalar value.
     */
    void makeScalar( );

    // List operations

    /** Retrieve the nth element of a list variant.
        This method will only operate on a list variant. If the variant is scalar,
        OFNULL will be returned.
        \param element An ofuint32 containing the index of the element to retrieve.
        ***NB/ index starts at 1 not 0***
        \return A pointer to the element if it exists, or OFNULL if not.
    */
    OFVariant *listRetrieve( ofuint32 element );

    /** Remove the nth element from a list variant.
        This method will only work on a list variant.
        \param element An ofuint32 containing the index of the element to remove.
    */
    void listRemove( ofuint32 element );

    /** Add a variant to the list.
        If the variant is not a list, it is converted to a list value, and the new value
        added to the end of the list.
        \param newVnt A pointer to the OFVariant to be added. A deep-copy is performed
        on this vaariant.
        \param position The position to add the new variant. If this parameter is 0 the new
        variant will be added to the end of the list. A value of 1 will force the new variant to the
        head of the list.
        If the supplied position is greater than the length of the list, the new variant will be added
        to the end of the list.
    */
    void listAdd( OFVariant *newVnt, ofuint32 position = 0 );

    /** Replace a specified element of a list.
        The old variant is deleted, and the new variant added.
        This method does nothing if the position supplied is invalid or the OFVariant is a scalar.
        \param newVnt A pointer to the new variant value.
        \param position An ofuint32 which specifies which element should be replaced.
    */
    void listReplace( OFVariant *newVnt, ofuint32 position );

    /** Retrieve the list size.
        \return An ofuint32 containing the list size. This value is 0 if the variant is not
        a list.
    */
    ofuint32 listSize( ) const;

    //@}

    /** \name Data member access methods.
     */
    //@{

    /** Tests to see if the variant has had any value assigned to it.
        Returns true if the variant is empty, in which case any
        further operations on it are at present undefined.
    */
    bool isNull( ) const;

    /** Tests if valid
        Checks the null flag and the buffer, and the type (non-zero)
    */
    bool isValid( ) const;

    /** Retrieve the internal buffer size.
     */
    ofuint32 bufferSize( ) const;

    /** Retrieve a pointer to the internal buffer.
     */
    const char *buffer( ) const;

    /** Retrieve the variant type
     */
    const ofint32 type() const;
    
    /** Retrieve the sub-type.
        Sub-types are provided to store user defined information.*/
    const ofint32 subType() const;
    
    /** Set the sub type.
        Sub-types are provided to store user defined information.*/
    void subType( ofint32 lSubType );

    //@}

    /** \name Assignment and conversion.
     */
    //@{

    /** Assigns the supplied string value to the variant.
        Any existing value is lost and replaced.
    */
    void assign( const char * str );
    
    /** Assigns the supplied ofuint64 value to the variant.
        Any existing value is lost and replaced.
    */
    void assign( const ofint64 dwValue );
    
    /** Assigns the supplied boolean value to the variant.
        Any existing value is lost and replaced.
    */
    void assign( const bool bValue );
    
    /** Assigns the supplied double value to the variant.
        Any existing value is lost and replaced.
    */
    void assign( const double dblValue );
    
    /** Assigns the supplied OFDateTime value to the variant.
        Any existing value is lost and replaced.
    */
    void assign( const OFDateTime oftValue );
    
    /** Assigns the supplied Identity value to the variant.
        Any existing value is lost and replaced.
    */
    void assign( const OFIDENTITY &id );

    /** Assigns the value of the supplied variant (copy) to this variant.
        Any existing value is lost and replaced.
    */            
    void assign( const OFVariant &theCopy );
    
    /** Assigns the binary value supplied to the variant.
     */
    void assign( const void *data, ofuint32 length );

    /** Testing method for copying one variant to another.
     */
    void copy( const OFVariant &theCopy );
    
    /** Convert the type of the variant.
        The function will attempt the most intelligent conversion,
        although some don't make much sense. For example, converting
        a ofint32 (ofuint32) variant to a boolean variant will result in 
        a loss of most of the original value.
    */
    void convert( ofint32 lType );

    void convertAll( ofint32 lType );

    /** Convert to string
        A special case which converts a binary type to a string type.
        Nothing is done for variants which are not of binary type.
    */
    void convertToString( );

    // Access methods
    
    /** Access to the value as an ofint64.
        Converts a non-numeric string to 0.
        This method was originally a cast operator, but
        has been changed to avoid confusion.
    */
    ofint64 cv_ofint64() const;
    
    /** Access to the value as a bool.
        converts a non-numeric string to false ( e.g. OFVariant("abc").cv_bool() returns false)
        This method was originally a cast operator, but
        has been changed to avoid confusion.
    */
    bool cv_bool() const;
    
    /** Access to the value as a const char.
        This method was originally a cast operator, but
        has been changed to avoid confusion.
    */
    const char * cv_pcstr() const;
    
    /** Access to the value as a double
        converts a non-numeric string to 0
        This method was originally a cast operator, but
        has been changed to avoid confusion.
    */
    double cv_double() const;
    
    /** Access to the value as a Identity
        converts a non-numeric string to OFOpenFabric::NullID;
    */        
    OFIDENTITY cv_identity() const ;
    
    /** Access to the value as an OFDateTime.
        Within OFVariant the data of OFDateTime 
        is stored as a signed ofint32.
        Treats a numeric value as seconds since 1970.
        converts a non-numeric string to 0 (e.g. can't currently 
        handle conversion of string "26-Apr-00" to OFDateTime).
    */                
    OFDateTime cv_OFDateTime() const ;        

    //@}

    /** \name Dump and undumps.
        Dumping to and from buffers, StorageBlob objects and other related bits.
    */
    //@{

    /** Dump the variant to an OFFile.
     */
    void dumpToFile( OFFile *file );
    
    /** Undump from an OFFile.
     */
    void undumpFromFile( OFFile *file );
    
    /** the number of bytes required to dump this class */
    ofuint32 dumpBufferSize() const;
    
    /** Dump the variant to a buffer.
        This is almost the equivalent of the operator <<.  The buffer is allocated
        by the method, and the length returned in bufferLength.
        The caller is responsible for deleting the buffer (use delete[]).
    */
    void dumpToBuffer2( char * & buffer, ofuint32 & bufferLength );

    /** Dump the variant to a buffer.
        Assumes the buffer already exists.
    */
    void dumpToBuffer( char ** buffer );
    
    /** dumps variant to buffer already allocated by user that is bufferLength bytes.
        Returns 0 for success and >0 if unsuccessful for extra space required to dump buffer */
    ofuint32 dumpToExistingBuffer2( char * buffer, ofuint32 bufferLength ); 
    
    /** Create the variant from the supplied buffer.
        \param buffer A pointer to a char * which will be updated to point just beyond the
        variant's data.
    */
    char *readFromBuffer( const char *buffer );

    void undumpFromBuffer( const char **buffer );
    

    /** Write the variant to the blob
     */
    void write( StorageBlob *blob ) const;

    /** Read the variant from blob.
     */
    void readFromBlob( StorageBlob *blob, int version );

    //@}

    /** \name Active reference methods.
     */
    //@{

    /** Retrieve the active reference flag.
     */
    ofuint32 activeReference( ) const;

    /** Set the active reference flag.
     */
    void activeReference( ofuint32 reference );
    //@}

    /** Determines whether the supplied types are valid for an arithmetic operation 
        totally invalid is OFDateTime */
    friend OFSYS_API bool ValidArithmeticOpTypes( const OFVariant &lhs, const OFVariant &rhs );
    friend OFSYS_API ofint32 arithOpResultType( ofint32 type1, ofint32 type2 );

    friend OFSYS_API OFVariant operator +( const OFVariant &lhs, const OFVariant &rhs);
    friend OFSYS_API OFVariant operator -( const OFVariant &lhs, const OFVariant &rhs);
    friend OFSYS_API OFVariant operator *( const OFVariant &lhs, const OFVariant &rhs);
    friend OFSYS_API OFVariant operator /( const OFVariant &lhs, const OFVariant &rhs);
    
    /** Determines whether the supplied types are valid for an equality operation
     */
    friend OFSYS_API bool ValidEqualityOpTypes( const OFVariant &lhs, const OFVariant &rhs ) ;
    
    /** Determines whether the supplied types are valid for a relational operation 
     */
    friend OFSYS_API bool ValidRelationalOpTypes( const OFVariant &lhs, const OFVariant &rhs ) ;
    
    friend OFSYS_API ofint64 operator ==( const OFVariant &lhs, const OFVariant &rhs);
    friend OFSYS_API ofint64 operator !=( const OFVariant &lhs, const OFVariant &rhs);
    
    friend OFSYS_API ofint64 operator <( const OFVariant &lhs, const OFVariant &rhs);
    friend OFSYS_API ofint64 operator <=( const OFVariant &lhs, const OFVariant &rhs);
    friend OFSYS_API ofint64 operator >( const OFVariant &lhs, const OFVariant &rhs);
    friend OFSYS_API ofint64 operator >=( const OFVariant &lhs, const OFVariant &rhs);
    
    /** Boolean binary operators && ; works with the m_lType subset [ ofuint32, bool, ofint32 ] 
     */        
    friend OFSYS_API bool operator &&( const OFVariant &lhs, const OFVariant &rhs);
    friend OFSYS_API bool operator ||( const OFVariant &lhs, const OFVariant &rhs);
    
    /** Maximum size of the display buffer.
     */
    static const ofint32 MAX_DISPLAY_BUFFER_LENGTH ;
    static const ofint32 INITIAL_DISPLAY_BUFFER_LENGTH ;

    static void dumpBinary( OFVariant *bin );

 protected:
        
    /** true if it has an empty (unassigned) value
     */
    bool m_null;
        
    /** a buffer for containing the value (as double, ofuint32, char* etc)
     */
    char * m_buffer;
        
    /** the number of elements in m_pszBuffer
     */
    ofuint32 m_bufLen;
        
    /** Buffer used to hold converted values for display.
     */
    char *m_display;

    /** Size of the allocated display buffer.
     */
    ofuint32 m_dispLen;
        
    /** the type of the variant
     */
    ofint32 m_type;

    /** Variant sub-type.
        Non-zero implies a list of OFVariant's rather than a scalar value.
    */
    ofint32 m_subType;

    /** List of OFVariants.
     */
    vector<OFVariant *> m_list;

    /** Active reference flag.
        This flag is not stored in any way, and is only
        copied by the assign( const OFVariant &) method.
    */
    ofuint32 m_activeReference;

    /** initialise the object
     */
    void create_( );
        
    /** reset the contents of the object to empty.
        Destroys the list of variants if present.
    */
    void destroy_( );
        
    /** Test if m_type is valid for a boolean operation - true for [ ofuint32, bool, ofint32 ]
     */
    bool validBooleanOpType_() const;

 private:

    enum
        {
            /** Variant is scalar.
             */
            VARIANT_SCALAR   = 0x00,

            /** Variant is a list.
             */
            VARIANT_LIST = 0x01
        };
};

#include <ofvariant.i>
    
/** A list of OFVariant instances. */
typedef std::vector<OFVariant*> OFVariantList;
    
/** An iterator for OFVariantList. */
typedef OFVariantList::iterator OFVariantIterator;

/** auto_ptr OFVariant guard
 */
typedef auto_ptr<OFVariant> OFVariantGuard;

OFSYS_API ofint32 arithOpResultType( ofint32 type1, ofint32 type2 );

#endif // #if !defined( OF_VARIANT_H_)
