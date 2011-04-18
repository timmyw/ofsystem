/* $Revision: 2.1 $

$Id: ofutility.h,v 2.1 2006-10-03 12:55:41 timmy Exp $
*/

#if !defined(_OF_UTILITY_H_)
#define _OF_UTILITY_H_

/** \file ofutility.h
    Contains the declaration of the OFUtility class.
*/

class OFFile;
class OFConfigurator;

/** \class OFUtility ofutility.h
    %Utility functions that are not platform related. Most of these methods
    are implemented as static methods.
    \nosubgrouping
    \ingroup ofsyscls
*/
class OFSYS_API OFUtility
{
 public:
    /** \name String handling methods.
     */
    //@{

    /** Left trim the supplied string.
        \param instr A null-terminated string containing the string to be trimmed.
        The result is left in this buffer.
    */
    static void leftTrimString( char *instr );

    /** Right trim the supplied string.
        \param instr A null-terminated string containing the string to be trimmed.
        The result is left in this buffer.
    */
    static void rightTrimString( char *instr );

    /** Split the supplied string into a list

        Uses std::string
    */
    static vector<string> split_string_to_list(string& l, string delim);

    /** Splits a string on the first occurence of a delimiter.
     */
    static void split_string_on_delim(const string& l, const string delim, string &left, string &right);

    /** Splits a string on the last occurence of a delimiter.
     */
    static void split_string_on_last_delim(const string& l, const string delim, string &left, string &right);

    /** Re-entrant version of strtok(). 
        This operates by storing the current position (the end of the current token)
        in the supplied ofint32 * (start) which should be initialised to 0 when first called.
        The string (const char * str) being searched is not modified in any way.
    
        \param str Const pointer to the string being searched.
        \param delim Null-terminated delimeter string.  This consists of an array of possible delimeter characters.
        \param start A pointer to the current position in the search string.  When the method is first called, this
        should be initialised to 0. On each exit, it'll contain the current position.
        \param token A buffer to contain the token as it is found.
        \param tokenLen The length (in bytes) of the supplied token buffer.  If this buffer is too small to contain
        the found token, nothing will be copied into %token.
        \return True if succeeded. Fails if *start points to beyond the end of the search string, or the buffer supplied
        is too small to contain the token found.
    */
    static bool str_token( const char * str, const char *delim, ofuint32 * start, char *token, ofuint32 tokenLen );

    /** A version of the standard OFUtility::str_token that accepts and works with escaped delimeters.

        \param str Const pointer to the string being searched.
        \param delim Null-terminated delimeter string.  This consists of an array of possible delimeter characters.
        \param escape Null-terminated string container the escape characters. These characters should be a pair, even
        if the start escape is the sames as the end escape character.
        \param start A pointer to the current position in the search string.  When the method is first called, this
        should be initialised to 0. On each exit, it'll contain the current position.
        \param token A buffer to contain the token as it is found.
        \param tokenLen The length (in bytes) of the supplied token buffer.  If this buffer is too small to contain
        the found token, nothing will be copied into \c token.
        \param trimWS Flags whether or not whitespace should be trimmed from the token before it is returned. Defaults to off.

        \return True if succeeded. Fails if *start points to beyond the end of the search string, or the buffer supplied
        is too small to contain the token found.
    */
    static bool str_token2( const char * str, const char *delim, const char *escape, ofuint32 * start, char *token, ofuint32 tokenLen, char trimWS = 0 );

    /** A version of the standard OFUtility::str_token that accepts and works with escaped delimeters,
        and dynamically allocates the buffer required to store the retrieved token.

        \param str Const pointer to the string being searched.
        \param delim Null-terminated delimeter string.  This consists of an array of possible delimeter characters.
        \param escape Null-terminated string container the escape characters. These characters should be a pair, even
        if the start escape is the sames as the end escape character.
        \param start A pointer to the current position in the search string.  When the method is first called, this
        should be initialised to 0. On each exit, it'll contain the current position.
        \param token A buffer which will be allocated by str_token3, and which will contain the token.
        This must be deallocated by the calling function.
        \param tokenLen A pointer to the length of the retrieved token. This value is initialised by the method.
        \param trimWS Flags whether or not whitespace should be trimmed from the token before it is returned. Defaults to off.

        \return True if succeeded. Fails if *start points to beyond the end of the search string, or the buffer supplied
        is too small to contain the token found.
    */
    static bool str_token3( const char * str, const char *delim, const char *escape, ofuint32 *start, char **token, ofuint32 *tokenLen, char trimWS = 0 );

    /** Converts a comma separated list into an OFCHARSTARLIST.

        Scans through the supplied string \c csList and inserts an
        entry into the list for each comma separated token.  Each
        token is left and right trimmed before it is inserted in the
        list.  This method does not deal with embedded commas in the
        list elements at present.

        \param csList A null-terminated string containing the comma
        separated list.

        \param list A pointer to an OFCHARSTARLIST which will be
        populated by the method. This list is not emptied before any
        elements are added.

        \return The number of elements added to the list.
    */
    static ofuint32 convertCommaToCharStar( const char *csList, OFCHARSTARLIST *list );

    //@}

    /** \name Input methods
     */
    //@{

    /** Read a single line from a file
        \warning This does NO checking of the length of the supplied buffer
        so you are much better off using OFUtility::readLine2()
     */
    static bool readLine( OFFile *file, char *line );

    /** Dynamic implementation of readLine.
        Reads the next line (delimeter by CR) from the specified file,
        but dynamically allocates the memory required for the line.
        \warning It is the caller's responsibility to destroy the buffer. It
        is allocated with new[].
    */
    static bool readLine2( OFFile *file, char **line );

    /** Read the specified line from the supplied file name.
     */
    static ofuint32 readLineNo( const char *name, char *line, ofuint32 lineno );

    /** Read the specified line from the supplied file name.
        Uses the dynamic version of read line, OFUtility::readLine2.
        \warning It is the caller's responsibility to destroy the buffer. It
        is allocated with new[].
     */
    static ofuint32 readLineNo2( const char *name, char **line, ofuint32 lineno );

    /** \type READLINESB
        Callback function type, used by readLines family of methods.
     */
    typedef ofuint32(*READLINESCB)(OFCHARSTARLIST*, ofuint32, void*);

    /** Read in the lines from a CSV file

        \param file A pointer to the file to be read.  The first read
        will be from the file's current position.

        \param cb The call back function pointer

        \param userData A pointer to the user data that will be
        supplied in each callback.

        \return The number of rows processed, or -1 on error.

        Reads in each line of the supplied file, processes it as a
        list of comma separated fields, and calling the supplied
        function callback for each line.

        If the fields in the comma separated record are wrapped in
        quotes, they will be removed before the callback happens.

        The memory allocated for the strings passed to the callback
        will be destroyed by the calling function.  The callback
        should not modify the string data.
     */
    static ofint64 readLinesCSV(OFFile *file,
                                 READLINESCB cb,
                                 void* userData);

    /** Scan the supplied line as a list of comma separated values.

    \param line A null-terminated string containing the list to scan.

    \param elements A pointer to an OFCHARSTARLIST which will contain
    the elements.

    Scans the buffer supplied in \c line, and splits it up into comma
    separated values.  Commas that appear inside double quotes will be
    treated as part of the value, not a delimiter.
     */
    static void scanCommaSepLine(const char *line, OFCHARSTARLIST *elements);

    /** Implementation of the getpass() libc function.
        On Unix-type platforms this will use the terminal IO functions to shut
        off echo and read a password in.

        The password string will be right-trimmed using OFUtility::rightTrimString()
        before being returned - to remove whitespace and the carriage return.

        \return Either the length of password entered, or -1 on error.
     */
    static ofint32 readPassword(char* password, ofuint32 password_length);

    //@}


    /** \name Conversion methods.
        Miscellaneous conversion methods that should probably be in OFConvert if they are already there.
    */
    //@{

    /** converts the character to the hex equivalent which is returned in outstring

        (which must be allocated by the caller) */
    static void decToHex( unsigned char one_char, char *outstring );
    
    /** converts length character from the string to the hex
        equivalent which is returned in outstring (which is allocated
        by the function) */
    static void decToHex( const char *binary, ofint32 length, char * &outstring );

    /** Converts the supplied binary data into hex.
        \param binary The binary data to convert.
        \param length The length of the binary data.
        \param outbuf A pointer to a pre-allocated chunk of memory, which will hold the hex data.
        \param outlen The size of the supplied output buffer.
    */
    static void decToHex( const char *binary, ofuint32 length, char *outbuf, ofuint32 outlen );

    /** Converts the supplied hex data into binary.
        \param input A pointer (char *) to the hex data.
        \param inputLength The length of the hex data.
        \param output A pointer to a pre-allocated buffer which will hold the binary data.
        \param outputLength The length of the supplied binary buffer.
    */
    static ofuint32 hexToBin( const char *input, ofuint32 inputLength, unsigned char *output, ofuint32 outputLength );

    /** Converts the supplied binary data into hex.
        \param binary The binary data to convert.
        \param length The length of the binary data.
        \param outbuf A pointer to a pre-allocated chunk of memory, which will hold the hex data.
        \param outlen The size of the supplied output buffer.
    */
    static ofuint32 binToHex( const unsigned char *input, ofuint32 inputLength, char *output, ofuint32 outputLength );

    //@}

    /** \name String conversions
    */
    //@{

    /** Convert the supplied string to a long
    */
    static ofuint32 strToULong( const char* str );

    //@}

    /** \name Miscellaneous methods.
        These simply couldn't end up anywhere else.
    */
    //@{

    /** This method finds an occurence of a byte sequence in a block of memory
        This method has been modified for use by parseMPFEFields, and probably doesn't
        work the way you think it will. Use memfind2() instead.
        \param buffer Const pointer to memory to search
        \param buflen The length (in bytes) of buffer.
        \param find Const pointer to the byte pattern we are looking for. 
        \param findlen The length (in bytes) of the byte pattern we are looking for.
    */
    static void *memfind( const void * buffer, ofuint32 buflen, const void * find, ofuint32 findlen ) ;

    /** This method finds an occurence of a byte sequence in a block of memory
        \param buffer Const pointer to memory to search
        \param buflen The length (in bytes) of buffer.
        \param find Const pointer to the byte pattern we are looking for. 
        \param findlen The length (in bytes) of the byte pattern we are looking for.
    */
    static void *memfind2( const void * buffer, ofuint32 buflen, const void * find, ofuint32 findlen ) ;

    /** Expand any environment variable found in the string
        Environment variables are delimited by $( and ).
        \param data A null-terminated string containing the data to expand.
        \param buffer A buffer to hold the output (no checking is done on this at the moment.)
    */
    static void expandEnvVars( const char *data, char *buffer );

    /** Destroys each element in an OFCHARSTARLIST.
        Each string is deleted (with []) and the list is emptied.
        \param list A pointer to the list to be destroyed.
    */
    static void destroyCharStarList( OFCHARSTARLIST *list );

    /** Checks if the supplied address is in the RFC1918 range
     */
    static char addressIn1918( struct sockaddr_in* addr );

    //@}

    /** \name Error translation
     */
    //@{

    /** Translate a system error
    */
    static ofuint32 translateSystemError( ofuint32 sysErr );

    /** Translate a system error.
        The error is taken from errno (for Uni*) or GetLastError (Win32)
    */
    static ofuint32 translateLastSystemError( );

#if defined(OFOPSYS_WIN32)
    /** Translate a Win32 error
    */
    static ofuint32 translateWin32Error( ofuint32 wErr );

#else
    /** Translate a Unix error
     */
    static ofuint32 translateUnixError( ofint32 uErr );
#endif

    /** Translate an SSL error
    */
    static ofuint32 translateSSLError( ofint32 e );

#if defined(OFOPSYS_WIN32)
    /** Get a line of input with no echo
        \return The number characters in the line.
    */
    static ofuint32 getLine(char* line, ofuint32 line_length);

#endif

    /** Retrieve all the items in a config group

    This method will also break down all the comma separated values in
    each line, and add them to the list.
     */
    static void retrieveConfigValues(OFConfigurator* config,
                                     const char* groupName,
                                     OFCHARSTARLIST* list);

    //@}

    /** \name Base64 methods
     */
    //@{

    /** Calculates the size of the buffer required to represent the data in base64.
        \param data A pointer to the data cast to const char *. If the \c buflen
        parameter is non-zero, this parameter can be OFNULL.
        \param buflen The length of the data. If zero, the \c data parameter is assumed
        to point to a null-terminated string, and the length calculated from that.
        \return An ofuint32 containing the size of the required buffer.
    */
    static ofuint32 calculateBase64Size( const char *data, ofuint32 buflen = 0 );

    /** Convert the supplied data to base64.
        \param data A pointer to the data to be converted. This is cast to a const char *.
        \param buflen An ofuint32 containing the length of the supplied data.
        \param output A pointer to an already allocated buffer.  This buffer must be at the size
        of the value returned by calculateBase64Size().
    */
    static void convertToBase64( const char *data, ofuint32 buflen, char *output );

    /** Convert back from base64.
        Returns the length of the converted data.
     */
    static ofuint32 convertFromBase64( const char *data, ofuint32 buflen, char *output );

    /** Convert back from base64 and returns the length of the decoded data.
        \param data A pointer to the data to be converted. This is cast to a const char *.
        \param buflen An ofuint32 containing the length of the supplied data.
        \param output A pointer to an already allocated buffer.  This buffer must be set at the size
        which is at least 3/4 of the size of the supplied data (rounded up to the nearest multiple
        of 3)
        Note. This method checks the supplied buflen to prevent memory overflow and ignores extraneous
        line feeds etc in the base64 data.
     */
    static ofuint32 convertFromBase64_2( const char *data, ofuint32 buflen, char *output );

    //@}

};

OFSYS_API const char* getSourceBranch();

#endif // #if !defined(_OF_UTILITY_H_)
