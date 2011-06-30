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

//#include <ofgen/openfabric.h>
#include <ofsys.h>
#include <ofcommandarg.h>
#include <ofutility.h>
#include <ofos.h>

OFCommandArgList::OFCommandArgList(const char * mask, ofint32 argc, char * argv[]):
    m_last(0),
    m_unrecog(0)
{
    ofuint32 start = 0;
    char token[1025];
    OFCmdArg *arg;
    ofint32 j;

    m_unrecog = new ofuint32[argc+1];
    for (j = 0; j<argc; j++)
        m_unrecog[j] = 1;
    while ( OFUtility::str_token( mask, ":", &start, token, 1024 ) )
    {
        if ( findArg_( token, &arg, argc, argv ) )
        {
            m_args.push_back( arg );
            if ( arg->id() > m_last )
                m_last = arg->id();
        }
    }

    // Clear unrecognised flags for those arguments which aren't switches
    for (j = 1; j<argc; j++)
        if (m_unrecog[j] && argv[j][0] != '-' && argv[j][0] != '/')
            m_unrecog[j] = 0;
            
}

OFCommandArgList::~OFCommandArgList( )
{
    for ( OFCmdArgIterator i = m_args.begin(); i != m_args.end(); i++ )
        delete (*i);
    delete [] m_unrecog;
}

void OFCommandArgList::dump()
{
    for (OFCmdArgIterator i = m_args.begin(); i != m_args.end(); i++)
    {
        cout << (*i)->name() << "=" << (*i)->value() << endl;
    }
}

bool
OFCommandArgList::doesArgumentExist(const char * name)
{
    bool found = false;
    if ( m_args.size() )
        for ( OFCmdArgIterator i = m_args.begin(); i != m_args.end() && !found; i++ )
        {
            if ( !OFOS::stricmp( (*i)->name(), name ) )
                found = true;
        }
    return found;
}

const char *
OFCommandArgList::getArgumentValue(const char * name)
{
    bool found = false;
    const char * value = NULL;
    if ( m_args.size() )
        for ( OFCmdArgIterator i = m_args.begin(); i != m_args.end() && !found; i++ )
        {
            if ( !OFOS::stricmp( (*i)->name(), name ) )
            {
                found = true;
                value = (*i)->value();
            }
        }
    return value;
}

void 
OFCommandArgList::addArgument( const char *name, const char *value )
{
    OFCmdArg *arg = 0;
    for ( OFCmdArgIterator i = m_args.begin(); !arg && i != m_args.end(); i++ )
        if ( !OFOS::stricmp( name, (*i)->name() ) )
            arg = *i;

    char newarg = arg?1:0;
    if ( !arg )
        arg = new OFCmdArg;
    arg->name( name );
    arg->value( value );
    if ( !newarg )
        m_args.push_back( arg );
}

ofint32
OFCommandArgList::getLast()
{
    return m_last; 
}

ofint32
OFCommandArgList::getCount() 
{
    return m_args.size();
}

ofuint32*
OFCommandArgList::unrecognised()
{
    return m_unrecog;
}

bool
OFCommandArgList::findArg_( const char * argument, OFCmdArg **arg, ofint32 argc, char * argv[] )
{
    bool found = false;
    char temp[1512];
    char name[101];
    bool valReq = false;
    
    OFOS::strcpy( temp, argument );
    if ( OFOS::strchr( temp, '&' ) )
    {
        valReq = true;
        *((char*)OFOS::strchr(temp, '&')) = 0;
    }
    char * tok = strtok( temp, "|" );
    OFOS::strcpy( name, tok );
    while ( tok && !found )
    {
        for ( ofint32 x=1; x < argc && !found; x++ )
        {
            char pref_ok = 0;
            const char* pos = argv[x];
            while (OFOS::strchr("-/", pos[0]))
            {
                pref_ok = 1;
                pos++;
            }
            if (pref_ok && !OFOS::stricmp(pos, tok))
            {
                // Found it
                *arg = new OFCmdArg;
                (*arg)->id( x );
                m_unrecog[x] = 0;
                found = true;
                (*arg)->name( name );
                (*arg)->valReq( valReq );
                if ( valReq && x < argc-1 )
                {
                    (*arg)->value( argv[x+1] );
                    (*arg)->id( x+1 );
                    m_unrecog[x+1] = 0;
                    x++;
                }
            }
        }
        tok = strtok( NULL, "|" );
    }
    return found;
}

/////////////////////////////

OFCmdArg::OFCmdArg( )
{
}

OFCmdArg::~OFCmdArg( )
{
}

void
OFCmdArg::name( const char * newName )
{
    OFOS::strcpy( m_name, newName );
}

void
OFCmdArg::value( const char * newValue )
{ 
    OFOS::strcpy( m_value, newValue ); 
}

const char *
OFCmdArg::name( ) const
{
    return m_name; 
}

const char * 
OFCmdArg::value( ) const
{
    return m_value;
}

ofint32 
OFCmdArg::id( ) const
{
    return m_id; 
}

void 
OFCmdArg::id( ofint32 newId )
{
    m_id = newId; 
}

void 
OFCmdArg::valReq( bool newValReq )
{
    m_valReq = newValReq; 
}

const bool
OFCmdArg::valReq( ) const
{
    return m_valReq; 
}


