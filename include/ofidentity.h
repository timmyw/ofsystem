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

#if !defined(_OF_IDENTITY_H_)
#define _OF_IDENTITY_H_

#include <ofsys.h>
#include <ofos.h>

#if defined(OFOPSYS_WIN32)
OFSYS_API ostream & operator <<(ostream&s, const ofuint64 &x);
#endif // OFOPSYS_WIN32

/** \struct OFIDENTITY ofidentity.h
    Provides the storage and management of an openFabric identity.
    This structure is a replacement for the original, and uses
    ofuint64 values to store the identity.

    The format of a string representation has also changed, removing
    the middle 'ff', simply being 4 hex chars for the flags, and 32 hex chars
    for the identity.

    \nosubgrouping
*/
struct OFSYS_API OFIDENTITY
{
    /** The identity.
     */
    ofuint64 m_id0;
    ofuint64 m_id1;

    /** The identity flags.
     */
    ofuint16 m_flags;

    /** \name Constructors and destructor.
     */
    //@{

    /** Default constructor.
     */
    OFIDENTITY( );

    /** Initialising constructor.
     */
    OFIDENTITY( ofuint64 hi, ofuint64 lo, ofuint16 flags = 0 );

    /** Initialising constructor.
     */
    OFIDENTITY( const char *strId );

    /** Copy constructor.
     */
    OFIDENTITY( const OFIDENTITY &rhs );

    /** Destructor
     */
    ~OFIDENTITY( );
    //@}

    /** \name Operators.
     */
    //@{

#if !defined(SWIG)

    /** Assignment operator.
     */
    OFIDENTITY &operator =( const OFIDENTITY &rhs );

    /** Assignment operator (const char *).
     */
    OFIDENTITY &operator =( const char *strId );

    /** Equality operator.
     */
    int operator ==( const OFIDENTITY &rhs ) const;

    /** String equality operator.
     */
    int operator ==( const char *rhs ) const;

    /** Non-equality operator.
     */
    int operator !=( const OFIDENTITY &rhs ) const;

    /** Less than operator
     */
    friend OFSYS_API bool operator < ( const OFIDENTITY &lhs, const OFIDENTITY &rhs );

    /** Greater than operator
     */
    friend OFSYS_API bool operator > ( const OFIDENTITY &lhs, const OFIDENTITY &rhs );

    /** stdout operator.
     */
    friend OFSYS_API ostream &operator << ( ostream &s, const OFIDENTITY &id );

#endif

    //@}

    /** return true if id is NullID
     */
    bool isNull() const;

    /** \name Assignment methods.
     */
    //@{

    /** Assign the identity contained within the supplied string
     */
    void assign( const char *data, ofuint32 dataLength );

    //@}

    /** \name Helper methods.
     */
    //@{

    /** Convert to string representation.
        Prints out a representation of the identity to
        the supplied string. It is the callers responsibility
        to ensure that the buffer supplied is large enough.

        \return The same pointer passed in as the data buffer.
     */
    char * cstr( char *id ) const;

    //@}

    /** \name Constants.
     */
    //@{

    /** Null identity.
     */
    static const OFIDENTITY NullID;

    /** This identity.
     */
    static const OFIDENTITY ThisID;

    //@}
    //friend OFCOMBO_API int operator ==( const OFIDENTITY &lhs, const OFIDENTITY &rhs);
};

#include <ofidentity.i>

/** \typedef OFIDENTITYLIST
 */
typedef vector<OFIDENTITY*> OFIDENTITYLIST;
typedef OFIDENTITYLIST::iterator OFIDENTITYIterator;
typedef OFIDENTITYLIST::const_iterator OFIDENTITYIteratorC;

/** \typedef OFIDENTITYLIST2
    A vector of OFIDENTITY elements
 */
typedef vector<OFIDENTITY> OFIDENTITYLIST2;

/** create a new OFIDENTITYLIST made up of ids that exist in both list1 and list2
    deep copies ids into the new list
 */
OFSYS_API OFIDENTITYLIST listAnd( const OFIDENTITYLIST& list1, const OFIDENTITYLIST& list2 );

/** create a new OFIDENTITYLIST made up of ids that exist in either list1 and list2
    deep copies ids into the new list
 */
OFSYS_API OFIDENTITYLIST listOr( const OFIDENTITYLIST& list1, const OFIDENTITYLIST& list2 );

/** return true if the list contains the id
 */
OFSYS_API bool listContains(const OFIDENTITYLIST& list1, const OFIDENTITY* id);

/** return true if all the ids in the list are different
 */
OFSYS_API bool listUnique( const OFIDENTITYLIST& list1 );

/** Add an identity to a list of identities
 */
OFSYS_API void listAdd(vector<OFIDENTITY>& roles, OFIDENTITY& role);

/** Convert a comma separated list to a list of identities
 */
OFSYS_API ofuint32 OFIDENTITYLISTfromCSLIST(const char* cslist, OFIDENTITYLIST* idlist);

class OFFile;
class StorageBlob;

/** Write the specified identity to file.
    Assumes the file has already been opened with the appropriate permissions.
*/
void OFSYS_API writeIdentityToFile( OFFile *file, const char *id );

/** Reads an identity from the specified file.
 */
void OFSYS_API readIdentityFromFile( OFFile *file, char *id );

/** Find the specified OFIDENTITY in the list
 */
char OFSYS_API findIdentity( OFIDENTITYLIST *list, OFIDENTITY *id );

/** Dump an OFIDENTITYLIST to the supplied StorageBlobBuffer.
 */
void OFSYS_API dumpToBlob( OFIDENTITYLIST *list, StorageBlob *b );

/** Read an OFIDENTITYLIST from the supplied StorageBlobBuffer.
 */
void OFSYS_API readFromBlob( OFIDENTITYLIST *list, StorageBlob *b );

/** Dump an OFIDENTITYLIST to the supplied StorageBlobBuffer.
 */
void OFSYS_API dumpToBlob( OFIDENTITYLIST *list, StorageBlob *b );

#endif // #if !defined(_OF_IDENTITY_H_)
