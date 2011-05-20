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

#if !defined(_OF_UTILITY_H_)
#define _OF_UTILITY_H_

#include <ofidentity.h>

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

    /** Dynamic implementation of readLine.

        Reads the next line (delimeter by CR) from the specified file
        handle, but dynamically allocates the memory required for the
        line.  \warning It is the caller's responsibility to destroy
        the buffer. It is allocated with new[].
    */
    static bool readLine2(OFOS::of_handle_t fd, char** line);

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

    /** \name Conversion functions
     */
    //@{

    /** Returns the hex char equivalent of the value stored in i from 0..15, e.g. returns 'A' for i=10 
        \param i The integer to be converted.
    */
    static char intToHexChar(ofint32 i);
    
    /** Returns the hex equivalent 2 character string, e.g. if
        data = 128 returns "80" \param data The char value to be
        converted.
    */
    static std::string dataToHexString( unsigned char data);
    
    /** returns the hex equivalent string of the first length chars from data 
        (useful for dumping data buffers) */
    static std::string dataToHexString( unsigned const char* data, ofint32 length);
    
    /** returns the ofint32 equivalent of the hex stored in hexChar, e.g. returns 10 for c='A' */
    static ofint32 hexCharToInt(char c);
    
    /** returns the unsigned char* equivalent of the hex stored in hexString in 'data' and its length in 'length'
        (useful for retrieving data buffers), user must delete[] the returned value.
        NB/ 'data' is not NULL terminated */
    static void hexStringToData(const std::string& hexString, unsigned char*& data, ofuint32* length);
    
    /** convert an ofint16 to a string */
    static std::string toString(ofint16 theValue);
    
    /** convert a ofint32 to a string */
    static std::string toString(ofint32 theValue);
    
    /** convert a ofuint32 to a string */
    static std::string toString(ofuint32 theValue);
    
    /** convert a double to a string */
    static std::string toString(double theValue);
    
    /** convert string to an int, returns 0 if failed */
    static ofint32 toInt(const std::string& theValue);
    
    /** convert string to a ofint32, returns 0 if failed */
    static ofint32 toOfint32(const std::string& theValue);
    
    /** convert string to a ofuint32, returns 0 if failed */
    static ofuint32 toofuint32(const std::string& theValue);
    
    /** convert string to a double, returns 0.0 if failed */
    static double toDouble(const std::string& theValue);
    
    /** convert from URL format to plain ascii. 

        The result parameter must be long enough to accomodate the
        new string.

        \param value A string containing the ASCII representation.
        \param result A buffer which will contain the URL result.
        \param result_length The length of the supplied buffer.
    */
    static void toASCIIfromURL(const char *value,
                               char *result,
                               ofuint32 result_length,
                               const char* spaceEnd = "+");

    /** Convert to URL format from plain ascii. 

        The result parameter must be long enough to accomodate the
        new string (which will probably be longer!).  Converts ' '
        to + (rather than %20).
    */
    static void toURLfromASCII(const char *value,
                               char *result,
                               const char* spaceEnc = "+");

    /** Convert the two absolute paths into a relative path, from fromPath to toPath.

        Caller must delete [] the returned buffer, *relPath is set
        to it as a reminder.

        e.g.
        "abc/def/ghijkl/",
        "abc/def/ghi",
        "../ghi"

        "abc/def/ghi",
        "abc/def/ghi",
        "ghi"

        Only intended for use on unencoded paths, not guaranteed
        to work on URLs with different encodings.

        My+Shared+Spaces, My Shared Spaces and
        My%20Shared%20Spaces won't be matched.
    */
    static char *toRelativePath( const char *fromPath, const char *toPath, char **relPath );
    
    /** URL encode an unencoded path. 

        '/' is left as is.
        Converts ' ' to + (rather than %20).
        \param *result should be sized as:
        OFOS::strlen( path ) * 3 + 1 to safely handle the worst case.
    */
    static char *toURLfromPath( const char *path, char *result );

    /** URL encode an unencoded path, for safe use in an XML attribute.

        Apostrophe is encoded.
        Converts ' ' to + (rather than %20).

        \param path A string containing the path to be encoded.
        \param *result should be sized as:
        OFOS::strlen( path ) * 3 + 1 to safely handle the worst case.
    */
    static char *toXURLfromASCII( const char *path, char *result );

    /** URL encode an unencoded path, for safe use in an XML attribute. 
        Apostrophe is encoded.
        '/' is left as is.
        Converts ' ' to + (rather than %20).
        \param *result should be sized as:
        OFOS::strlen( path ) * 3 + 1 to safely handle the worst case.
    */
    static char *toXURLfromPath( const char *path, char *result );

    /** \fn static char toUpper( const char ch );
        Converts the char to upper case.
    */
    static char toUpper( const char ch );

    //@}

    /** \name Dumping and undump methods
     */
    //@{

    /** Dumps the supplied OFIDENTITY to file.
        Dumps the length first, and then the data.
    */
    static void dumpId( OFFile *file, const OFIDENTITY *id );

    /** Dump the supplied OFIDENTITY to the existing buffer.
     */
    static void dumpId( char **data, const OFIDENTITY *id );

    /** Undumps the OFIDENTITY from file.
     */
    static void undumpId( OFFile *file, OFIDENTITY *id );

    /** Undumps the identity (OFIDENTITY) from the buffer.
        The buffer pointer is updated to point to just after the identity data.
        \param data A pointer to the buffer pointer.
        \param id A pointer to the identity.
    */
    static void undumpId( const char **data, OFIDENTITY *id );

    /** Dumps the null-terminated string to file.
        Writes out the string length first and then the data.
    */
    static void dumpStr( OFFile *file, const char *str );

    /** Dump the supplied null-terminated string to the supplied buffer.
     */
    static void dumpStr( char **data, const char *str );

    /** Undumps the null-terminated string from file.
     */
    static void undumpStr( OFFile *file, char *str );

    /** Undump the string from the data buffer.
        The data buffer is updated to point to just after the string.
        \param data A pointer to the data buffer pointer.
        \param str A pointer to the string buffer. No checking is done on the
        size of this buffer.
    */
    static void undumpStr( const char **data, char *str );

    /** Undump a new string from the data buffer.
        The data buffer is updated to point to just after the string.
        The length of the string is read in, and the string is allocated by
        this method.
        \param data A pointer to the data buffer pointer.
        \param str A pointer to the string buffer. No checking is done on the
        size of this buffer.
    */
    static void undumpNewStr( const char **data, char **str );

    /** Dump a short to file in network order.
        \param file A pointer to an already open file.
        \param ho An ofuint16 in network order.
    */
    static void dumpuint16( OFFile *file, ofuint16 ho );

    /** Dump a short to the supplied buffer.
        The short data is converted to host order before it's written.
        \param buffer A pointer to the char * buffer. Updated to point to just
        after the short's data.
        \param ho An unsigned short containing the short value.
    */
    static void dumpuint16( char **data, ofuint16 ho );

    /** Dump a long to file in network order.
        \param file A pointer to an already open file.
        \param ho An ofuint32 in network order.
    */
    static void dumpuint32( OFFile *file, ofuint32 ho );

    /** Dump a long to the supplied buffer.
        The long data is converted to host order before it's written.
        \param buffer A pointer to the char * buffer. Updated to point to just
        after the long's data.
        \param ho An unsigned long containing the long value.
    */
    static void dumpuint32( char **data, ofuint32 ho );

    /** Dump a long long to file in network order.
        \param file A pointer to an already open file.
        \param ho An ofuint64 in network order.
    */
    static void dumpuint64( OFFile *file, ofuint64 ho );

    /** Dump a long long to the supplied buffer.
        The long data is converted to host order before it's written.
        \param buffer A pointer to the char * buffer. Updated to point to just
        after the long's data.
        \param ho An unsigned long long containing the long value.
    */
    static void dumpuint64( char **data, ofuint64 ho );

    /** Undump a long from file.
        The long value read in from the file is converted to host order.
        \param file A pointer to an already open file.
        \param ho A pointer to an ofuint32 which will contain the long value.
    */
    static void undumpuint32( OFFile *file, ofuint32 *ho );

    /** Undump a long from the supplied buffer.
        Performs conversion to host order.
    */
    static void undumpuint32( const char **data, ofuint32 *ho );

    /** Dump a long to file in network order.
        \param file A pointer to an already open file.
        \param ho An ofuint32 in network order.
    */
    static void dumpint32( OFFile *file, ofint32 ho );

    /** Dump a long to the supplied buffer.
        The long data is converted to host order before it's written.
        \param buffer A pointer to the char * buffer. Updated to point to just
        after the long's data.
        \param ho An unsigned long containing the long value.
    */
    static void dumpint32( char **data, ofint32 ho );

    /** Undump a long from file.
        The long value read in from the file is converted to host order.
        \param file A pointer to an already open file.
        \param ho A pointer to an ofuint32 which will contain the long value.
    */
    static void undumpint32( OFFile *file, ofint32 *ho );

    /** Undump a long from the supplied buffer.
        Performs conversion to host order.
    */
    static void undumpint32( const char **data, ofint32 *ho );

    //@}

    /** \name HexBinary methods
     */
    //@{

    /** Calculates the size of the buffer required to represent the data in hexBinary.
        \param data A pointer to the data cast to const char *. If the \c buflen
        parameter is non-zero, this parameter can be OFNULL.
        \param buflen The length of the data. If zero, the \c data parameter is assumed
        to point to a null-terminated string, and the length calculated from that.
        \return An ofuint32 containing the size of the required buffer.
    */
    static ofuint32 calculateHexBinarySize( const char *data, ofuint32 buflen = 0 );
        
    /** convert to hex binary
        \param data A pointer to the data to be converted. This is cast to a const char *.
        \param buflen An ofuint32 containing the length of the supplied data.
        \param output A pointer to an already allocated buffer.  This buffer must be twice the 
        size of the supplied data
    */
    static void convertToHexBinary( const char *data, ofuint32 buflen, char *output );
    
    /** convert back from hex binary
        \param data A pointer to the data to be converted. This is cast to a const char *.
        \param buflen An ofuint32 containing the length of the supplied data.
        \param output A pointer to an already allocated buffer.  This buffer must be at least 1/2
        the length of the supplied data (rounded up to the nearest single byte)
        \return length of decoded data returned in output
    */
    static ofuint32 convertFromHexBinary( const char *data, ofuint32 buflen, char *output );
    //@}

};

OFSYS_API const char* getSourceBranch();

#endif // #if !defined(_OF_UTILITY_H_)
