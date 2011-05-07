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

#include <ofidentity.h>
#include <ofutility.h>
#include <ofos.h>
#include <iomanip>

const OFIDENTITY OFIDENTITY::NullID( 0, 0, 0x0);
const OFIDENTITY OFIDENTITY::ThisID( MAKEULL( 0xfffffffffffffffe ), MAKEULL( 0xeeeeeeeeeeeeeeee ), 0xffff );

#if defined(OFOPSYS_WIN32)
ostream &
operator << ( ostream &s, const ofuint64 &x )
{
    ofuint32 hi =  ( x & MAKEULL( 0xffffffff00000000 ) ) >> 32;
    ofuint32 lo = (ofuint32)(x & MAKEULL(0xffffffff));
    return s << hex << setfill('0') << setw(8) << hi << setw(8) << lo << dec;
}
#endif

ostream &
operator << ( ostream &s, const OFIDENTITY &id )
{
    return s << hex << setfill('0') << setw(4) << id.m_flags << setw(16) 
         << (ofint64)id.m_id0 << setw(16)
         << (ofint64)id.m_id1 << dec;
}

bool
operator < ( const OFIDENTITY &lhs, const OFIDENTITY &rhs )
{
    if ( lhs.m_id0 < rhs.m_id0 || 
         ( lhs.m_id0 == rhs.m_id0 && lhs.m_id1 < rhs.m_id1 ) )
        return true;
    else
        return false;
}

bool
operator > ( const OFIDENTITY &lhs, const OFIDENTITY &rhs )
{
    if ( lhs.m_id0 > rhs.m_id0 || 
         ( lhs.m_id0 == rhs.m_id0 && lhs.m_id1 > rhs.m_id1 ) )
        return true;
    else
        return false;
}

bool
OFIDENTITY::isNull() const
{
    return m_id1 == NullID.m_id1 && m_id0 == NullID.m_id0;
}

#if !defined(NDEBUG)
int
OFIDENTITY::operator ==( const OFIDENTITY &rhs ) const
{
//     cout << "operator ==(): this" << hex << setfill('0') << setw(16) << m_id0 << setw(16) << m_id1 << dec << endl;
//     cout << "operator ==(): rhs " << hex << setfill('0') << setw(16) << rhs.m_id0 << setw(16) << rhs.m_id1 << dec << endl;

    return ( m_id0 == rhs.m_id0 && m_id1 == rhs.m_id1 );
}

/*int
operator == ( const OFIDENTITY &lhs, const OFIDENTITY &rhs)
{
     return lhs.m_id1 == rhs.m_id1 && lhs.m_id0 == rhs.m_id0;
}*/

int
OFIDENTITY::operator ==( const char *rhs ) const
{
    ofuint64 id0, id1;
    char temp[17];
    OFOS::strncpy2( temp, rhs+4, 16 );
    sscanf( temp, "%llx", &id0 );
    sscanf( rhs+20, "%llx", &id1 );

    cout << *this << endl;
//     cout << "operator ==(const char*):" << hex << setfill('0') << setw(16) << id0 << setw(16) << id1 << dec << endl;

    return ( m_id0 == id0 && m_id1 == id1 );
}

int
OFIDENTITY::operator !=( const OFIDENTITY &rhs ) const
{
    return ( m_id0 != rhs.m_id0 || m_id1 != rhs.m_id1 );
}
#endif

OFIDENTITYLIST 
listAnd( const OFIDENTITYLIST& list1, const OFIDENTITYLIST& list2 )
{
    OFIDENTITYLIST ret;

    OFIDENTITYLIST::const_iterator it1;
    for ( it1 = list1.begin(); it1 != list1.end(); it1++ )
    {
        OFIDENTITY* id1 = *it1;
        if ( listContains( list2, id1 ) && !listContains( ret, id1 ) )
            ret.push_back( new OFIDENTITY( *id1 ) );
    }

    assert( listUnique( ret ) );
    return ret;
}

OFIDENTITYLIST 
listOr( const OFIDENTITYLIST& list1, const OFIDENTITYLIST& list2 )
{
    OFIDENTITYLIST ret;

    OFIDENTITYLIST::const_iterator it1;
    for ( it1 = list1.begin(); it1 != list1.end(); it1++ )
    {
        OFIDENTITY* id1 = *it1;
        if ( !listContains( ret, id1 ) )
            ret.push_back( new OFIDENTITY( *id1 ) );
    }
    OFIDENTITYLIST::const_iterator it2;
    for ( it2 = list2.begin(); it2 != list2.end(); it2++ )
    {
        OFIDENTITY* id2 = *it2;
        if ( !listContains( ret, id2 ) )
            ret.push_back( new OFIDENTITY( *id2 ) );
    }

    assert( listUnique( ret ) );
    return ret;
}

bool 
listContains( const OFIDENTITYLIST& list1, const OFIDENTITY* id )
{
    assert( id );
    bool ret = false;

    OFIDENTITYLIST::const_iterator it;
    for ( it = list1.begin(); it != list1.end(); it++ )
    {
        OFIDENTITY* id2 = *it;
        if ( *id2 == *id )
        {
            ret = true;
            break;
        }
    }

    return ret;

}

void listAdd(vector<OFIDENTITY>& roles, OFIDENTITY& role)
{
    if (roles.size() == 0)
        roles.push_back(role);
    for (vector<OFIDENTITY>::const_iterator i = roles.begin();
         i != roles.end(); i++)
        if (*i == role)
            return;
    roles.push_back(role);
}

bool 
listUnique( const OFIDENTITYLIST& list1 )
{
    bool ret = true;

    OFIDENTITYLIST::const_iterator it1;
    for ( it1 = list1.begin(); it1 != list1.end(); it1++ )
    {
        OFIDENTITY* id1 = *it1;
        OFIDENTITYLIST::const_iterator it2;
        for ( it2 = list1.begin(); it2 != list1.end(); it2++ )
        {
            if ( it1 != it2 )
            {
                OFIDENTITY* id2 = *it2;
                if ( *id1 == *id2 )
                {
                    cout << "id=" << *id1; 
                    ret = false;
                    break;
                }
            }
        }
    }

    return ret;
}

ofuint32 OFIDENTITYLISTfromCSLIST(const char* cslist, OFIDENTITYLIST* idlist)
{
    OFCHARSTARLIST strlist;
    OFUtility::convertCommaToCharStar(cslist, &strlist);
    for (OFCHARSTARLIST::iterator i = strlist.begin();
         i != strlist.end(); i++)
    {
        OFIDENTITY* id = new OFIDENTITY(*i);
        idlist->push_back(id);
        delete[] (*i);
    }
    return idlist->size();
}

#if defined(UNIT_TEST)

#include <UnitTest.h>

#define OUTID(a) cout << "flags:" << hex << a.m_flags << " id0:" << setfill('*') << setw(16) << a.m_id0 << " id1:" << setw(16) << a.m_id1 << dec << endl;
int
main( int c, char *v[] )
{
    UT_BATCH_START( "OFIDENTITY" );
    char str1[OF_MAX_ID_LEN+1];

    UT_TEST_START( 1, "constructors - default" );
    OFIDENTITY id1;
    OUTID( id1 );
    id1.cstr( str1 );
    OUTID( id1 );
    UT_EVAL_EXPR( ( !OFOS::strcmp( str1, "000000000000000000000000000000000000" ) ) );
    id1.cstr( str1 );
    UT_EVAL_EXPR( ( !OFOS::strcmp( str1, "000000000000000000000000000000000000" ) ) );
    id1.cstr( str1 );
    UT_EVAL_EXPR( ( !OFOS::strcmp( str1, "000000000000000000000000000000000000" ) ) );
    UT_TEST_END( 1 );

    UT_TEST_START( 2, "constructor - nullid" );
    OFIDENTITY id2( OFIDENTITY::NullID );
    id2.cstr( str1 );
    UT_EVAL_EXPR( ( !OFOS::strcmp( str1, "ffff00000000000000000000000000000000" ) ) );
    UT_TEST_END( 2 );

    UT_TEST_START( 3, "constructor - initialise" );
    OFIDENTITY id3( MAKEULL( 0xaa ), MAKEULL( 0xaa ), 0 );
    printf( "%016I64x:%016I64x\n", id3.m_id0, id3.m_id1 );
    cout << "id3:" << id3 << endl;
    OUTID( id3 );

    id3.cstr( str1 );
    UT_EVAL_EXPR( ( !OFOS::strcmp( str1, "000000000000000000aa00000000000000aa" ) ) );
    UT_TEST_END( 3 );

    UT_TEST_START( 4, "constructor - copy" );
    OFIDENTITY id4( id3 );
    id4.cstr( str1 );
    UT_EVAL_EXPR( ( !OFOS::strcmp( str1, "000000000000000000aa00000000000000aa" ) ) );
    UT_TEST_END( 4 );

    UT_TEST_START( 5, "equality operator" );
    OFIDENTITY id5( OFIDENTITY::NullID );
    UT_EVAL_EXPR( id5 == OFIDENTITY::NullID );
    id5 = id4;
    UT_EVAL_EXPR( id5 == id4 );

    UT_TEST_END( 5 );

    UT_TEST_START( 6, "char * constructor" );
    OFIDENTITY id6( "000000000000000000aa00000000000000aa" );
    cout << "000000000000000000aa00000000000000aa" << endl;
    cout << id6 << endl;
    UT_TEST_END( 6 );


    OFIDENTITY idl1( "000000000000000000aa00000000000000aa" );
    OFIDENTITY idl2( "000000000000000000aa00000000000000ae" );
    OFIDENTITY idl3( "000000000000000000000000000000000000" );
    OFIDENTITY idl4( "000000000000000000ab00000000000000aa" );
    OFIDENTITY idl5( "000000000000000000aa00000000000000aa" );

    OFIDENTITYLIST list1;
    list1.push_back( &idl1 );
    list1.push_back( &idl2 );
    list1.push_back( &idl3 );
    list1.push_back( &idl4 );
    list1.push_back( &idl5 );

    OFIDENTITYLIST list2;
    list2.push_back( &idl1 );
    list2.push_back( &idl2 );
    list2.push_back( &idl3 );
    list2.push_back( &idl4 );
    list2.push_back( &idl2 );

    OFIDENTITYLIST list3;
    list3.push_back( &idl1 );
    list3.push_back( &idl2 );
    list3.push_back( &idl3 );
    list3.push_back( &idl4 );

    OFIDENTITYLIST list4;
    list4.push_back( &idl1 );

    OFIDENTITYLIST list5;

    UT_TEST_START( 7, "listUnique" );
    UT_EVAL_EXPR( listUnique( list1 ) == false );
    UT_EVAL_EXPR( listUnique( list2 ) == false );
    UT_EVAL_EXPR( listUnique( list3 ) == true );
    UT_EVAL_EXPR( listUnique( list4 ) == true );
    UT_EVAL_EXPR( listUnique( list5 ) == true );
    UT_TEST_END( 7 );

    UT_TEST_START( 8, "listContains" );
    UT_EVAL_EXPR( listContains( list1, &idl1 ) == true );
    UT_EVAL_EXPR( listContains( list1, &idl5 ) == true );
    UT_EVAL_EXPR( listContains( list2, &idl2 ) == true );
    UT_EVAL_EXPR( listContains( list3, &idl5 ) == true );
    UT_EVAL_EXPR( listContains( list4, &idl1 ) == true );
    UT_EVAL_EXPR( listContains( list4, &idl2 ) == false );
    UT_EVAL_EXPR( listContains( list5, &idl2 ) == false );
    UT_TEST_END( 8 );

    UT_TEST_START( 9, "listOr" );
    UT_EVAL_EXPR( listOr( list3, list3 ).size() == list3.size() );
    UT_EVAL_EXPR( listOr( list4, list5 ).size() == 1 );
    UT_EVAL_EXPR( listOr( list3, list4 ).size() == 4 );
    UT_TEST_END( 9 );

    UT_TEST_START( 10, "listOr" );
    UT_EVAL_EXPR( listAnd( list3, list3 ).size() == list3.size() );
    UT_EVAL_EXPR( listAnd( list4, list5 ).size() == 0 );
    UT_EVAL_EXPR( listAnd( list3, list4 ).size() == 1 );
    UT_TEST_END( 10 );

    UT_BATCH_END( "OFIDENTITY" );
    return 0;

}

#endif // UNIT_TEST
