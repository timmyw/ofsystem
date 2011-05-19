/*
 * $OFPREPARED$
 *
 * $Revision: 2.3 $
 *
 * $Id: ofcmdarg.h,v 2.3 2007-07-25 12:27:45 timmy Exp $
 */

#if !defined(_OF_CMD_ARG_H)
#define _OF_CMD_ARG_H

/** \file ofcmdarg.h
    Contains the declarations of the OFCmdArg and OFCommandArgList classes.
*/

/** \class OFCmdArg ofcmdarg.h
    Provides the information pertaining to a single command line argument.
*/
class OFCOMBO_API OFCmdArg
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

class OFCOMBO_API OFCommandArgList 
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
