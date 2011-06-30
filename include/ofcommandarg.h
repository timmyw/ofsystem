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

#if !defined(_OF_CMD_ARG_H)
#define _OF_CMD_ARG_H

/** \file ofcmdarg.h
    Contains the declarations of the OFCmdArg and OFCommandArgList classes.
*/

/** \class OFCmdArg ofcmdarg.h
    Provides the information pertaining to a single command line argument.
*/
class OFSYS_API OFCmdArg
{
 public:
    OFCmdArg();
    ~OFCmdArg();
    OFCmdArg( const char * argName, const char * argValue = NULL );
    
    void name( const char * newName );
    void value( const char * newValue );
    
    const char * name( ) const;
    const char * value( ) const;
    
    ofint32 id( ) const ;
    void id( ofint32 newId );
    
    void valReq( bool newValReq );
    const bool valReq( ) const;

 protected:
    char m_name[100];
    char m_value[1024];
    ofint32 m_id;
    bool m_valReq;
};

typedef vector<OFCmdArg*> OFCmdArgList;
typedef OFCmdArgList::iterator OFCmdArgIterator;

/** \class OFCommandArgList ofcmdarg.h
    Provides management of command line arguments.
*/

class OFSYS_API OFCommandArgList 
{
 public:
    /** Default constructor.
     */
    OFCommandArgList();

    /** Destructor.
     */
    ~OFCommandArgList();

    /** Initialising constructor.
        Parses the argument mask and scans through the arguments provided.
    */
    OFCommandArgList(const char * mask, ofint32 argc, char * argv[]);
    
    bool doesArgumentExist(const char * argName);
    bool exist(const char* argName) { return doesArgumentExist(argName); }

    const char* getArgumentValue(const char * argName);
    const char* get(const char* argName) { return getArgumentValue(argName); }
    
    void addArgument(const char *name, const char *value);

    ofint32 getLast();
    ofint32 getCount();

    /** Dump the supplied arguments to stdout */
    void dump();

    /** Retrieve a list of unrecognised argument indexes.
        Each unrecognised option will have a non-zero value.
     */
    ofuint32 *unrecognised();

 protected:
    
    OFCmdArgList m_args;
    ofint32 m_last;
    
    bool findArg_(const char * argument, OFCmdArg **arg, ofint32 argc, char * argv[]);

    /** Index of first unrecognised option
     */
    ofuint32 *m_unrecog;

};

#endif // #if !defined(_OF_CMD_ARG_H)
