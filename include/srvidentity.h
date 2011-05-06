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

#if !defined(_SRVIDENTITY_H_)
#define _SRVIDENTITY_H_

/** \file srvidentity.h
    Contains the declaration of the SRVIDENTITY struct.
*/

#include <ofidentity.h>

/** \struct SRVIDENTITY srvidentity.h
    Provides storage and management for a server identity.

    \nosubgrouping
    \ingroup danglecls
 */
struct OFSYS_API SRVIDENTITY
{

    /** \name Constructors.
     */
    //@{

    /** Default constructor.
        Initialises the identity to the null server identity.
    */
    SRVIDENTITY( );

    /** Initialising constructor.
        Copies at most OF_MAX_SRV_ID_LEN characters from the supplied @c value
        and uses them to form the server identity.

        \param value A null-terminated string containing the server identity.
     */
    SRVIDENTITY( const char *value );

    /** Construct a SRVIDENTITY from an OFIDENTITY.
     */
    SRVIDENTITY( const OFIDENTITY &id );

    /** Copy constructor.
        \param copy A reference to a SRVIDENTITY to copy the server identity from.
     */
    SRVIDENTITY( const SRVIDENTITY &copy );

    //@}

    /** \name Operators
     */
    //@{

    /** Assignment operator.
        Copies the supplied SRVIDENTITY.
     */
    SRVIDENTITY &operator = (const SRVIDENTITY &copy );

    /** Assign from an OFIDENTITY.
     */
    SRVIDENTITY &operator = ( const OFIDENTITY &id );

    /** Assignment operator.
        Uses the supplied null-terminated string to form a server identity.
    */
    SRVIDENTITY &operator = ( const char *copy );

    /** Equality operator.
     */
    ofuint32 operator ==( const SRVIDENTITY &rhs ) const;

    /** Non-equality operator.
     */
    ofuint32 operator !=( const SRVIDENTITY &rhs ) const;

    /** Less than operator.
        Provided for use by container templates.
    */
    bool operator <( const SRVIDENTITY &rhs ) const;

    /** stdout operator.
     */
    friend OFSYS_API ostream &operator << ( ostream &s, const SRVIDENTITY &id );

    //@}

    /** return true if id is NullSrvID
     */
    bool isNull() const;

    /** The actual server identity.
     */
    ofuint64 m_id;

    /** Convert to string representation.
     */
    char *cstr( char *strId ) const;

    /** Constant value which holds a null server identity.
     */
    static const SRVIDENTITY NullSrvID;
};

#include <srvidentity.i>

typedef vector<SRVIDENTITY*> SRVIDENTITYLIST;
typedef SRVIDENTITYLIST::iterator SRVIDENTITYIterator;
typedef SRVIDENTITYLIST::const_iterator SRVIDENTITYIteratorC;

#endif // #if !defined(_SRVIDENTITY_H_)
