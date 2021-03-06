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

#include <ofsys.h>
#include <ofutility.h>
#include <ofos.h>
#include <offile.h>
//#include <openssl/ssl.h>
#include <ofconfig.h>

#if defined(OFOPSYS_WIN32)
#include <errno.h>
#include <conio.h>
#endif // defined(OFOPSYS_WIN32)

#if !defined(OFOPSYS_WIN32)
#include <termios.h>
#endif

void 
OFUtility::leftTrimString( char *instr )
{
    if ( OFOS::strlen( instr ) > 0 )
    {
        char *pos = instr;
        while ( OFOS::strchr( "\n\t\r ", *pos ) && OFOS::strlen( pos ) > 0)
            pos++;
        char *temp = new char[ OFOS::strlen(pos) + 1 ];
        OFOS::strcpy( temp, pos );
        OFOS::strcpy( instr, temp );
        delete [] temp;
    }
}

void
OFUtility::rightTrimString( char *instr )
{        
    if ( OFOS::strlen( instr ) > 0)
    {
        while ( OFOS::strchr( "\n\t\r ", *(instr+OFOS::strlen(instr)-1) ) && OFOS::strlen( instr ) > 0 )
        {
            *(instr+OFOS::strlen(instr)-1) = 0;
        }
    }
}

ofuint32
OFUtility::readLineNo( const char *name, char *line, ofuint32 lineno )
{
    ofuint32 ret = 0;
    OFFile file( name );
    if ( file.isOpen() )
    {
        bool ok = true;
        while ( ok && lineno )
        {
            ok = readLine( &file, line );
            lineno--;
        }
        file.close( );
    }
    return ret;
}

/*static*/
ofuint32
OFUtility::readLineNo2( const char *name, char **line, ofuint32 lineno )
{
    ofuint32 ret = 0;
    *line = 0;
    OFFile file( name, 0664, OFFILE_OPEN_EXISTING | OFFILE_READONLY );
    if ( file.isOpen() )
    {
        bool ok = true;
        while ( ok && lineno )
        {
            delete [] *line; *line = 0;
            ok = readLine2( &file, line );
            lineno--;
        }
        file.close( );
    }
    return ret;
}

bool
OFUtility::readLine( OFFile *file, char *line )
{
    bool ok = false;
    ofuint32 pos = file->position( );
    char templine[1025];
    ofint32 x = file->read( templine, 1024 );
    templine[x] = 0;
    if ( x )
    {
        char* s = strchr( templine, '\n' );

        if (s)
        {
            // not at end of file yet, set file position to start of next line
            ofuint32 dwOff = (s - templine);
            //printf( "dwPos:%d ", dwPos );
            pos += dwOff+1;
            templine[dwOff] = 0;
            file->position( pos );
        }

        ok = true;
        OFOS::strcpy( line, templine );
    }
    return ok;
}

/*static*/
bool
OFUtility::readLine2( OFFile *file, char **line )
{
        ofuint32 chunkSize = 1024;
        ofuint32 bufSize = chunkSize;
        ofuint32 filepos = file->position();
        char *buffer = new char[chunkSize+1];
#if !defined(NDEBUG)
        memset (buffer, 0, chunkSize);
#endif
        char *pos = buffer;
        char *eol = 0;
        bool ok = false;

        // Read in a chunk, and attempt to find a CR
        ofuint32 br;
        while (!eol && (br = file->read(pos, chunkSize)))
        {
                buffer[bufSize] = 0;
                eol = (char*)OFOS::strstr (buffer, "\n");
                if (!eol)
                {
                        bufSize += chunkSize;
                        char *newbuf = new char[bufSize+1];
                        memcpy (newbuf, buffer, bufSize - chunkSize) ;
                        delete [] buffer;
                        buffer = newbuf;
                        pos = buffer + bufSize - chunkSize;
                }
        }
        if (eol)
        {
                // set file position to start of next line
                filepos += 1 + (eol - buffer);
                file->position( filepos );

                *eol = 0;
                *line = new char[OFOS::strlen (buffer) + 1];
                OFOS::strcpy (*line, buffer);
                ok = true;
        }

        delete [] buffer;
        return ok;
}

/*static*/
bool
OFUtility::readLine2(OFOS::of_handle_t fd, char **line)
{
    ofuint32 chunkSize = 1024;
    ofuint32 bufSize = chunkSize;
    ofuint32 filepos = OFFile::get_position(fd);
    char *buffer = new char[chunkSize+1];
#if !defined(NDEBUG)
    memset (buffer, 0, chunkSize);
#endif
    char *pos = buffer;
    char *eol = 0;
    bool ok = false;

    // Read in a chunk, and attempt to find a CR
    ofuint32 br;
    while (!eol && (br = OFFile::read_file(fd, pos, chunkSize)))
    {
        buffer[bufSize] = 0;
        eol = (char*)OFOS::strstr (buffer, "\n");
        if (!eol)
        {
            bufSize += chunkSize;
            char *newbuf = new char[bufSize+1];
            memcpy (newbuf, buffer, bufSize - chunkSize) ;
            delete [] buffer;
            buffer = newbuf;
            pos = buffer + bufSize - chunkSize;
        }
    }
    if (eol)
    {
        // set file position to start of next line
        filepos += 1 + (eol - buffer);
        OFFile::set_position(fd, filepos);

        *eol = 0;
        *line = new char[OFOS::strlen(buffer) + 1];
        OFOS::strcpy(*line, buffer);
        ok = true;
    }

    delete [] buffer;
    return ok;
}

void
OFUtility::decToHex( unsigned char one_char, char *outstring )
{
    outstring[0] = one_char / 16 + 48;
    if ( outstring[0] > 57 ) outstring[0] += 7;
    outstring[1] = one_char % 16 + 48;
    if ( outstring[1] > 57 ) outstring[1] += 7;
    outstring[2] = 0;
}

void
OFUtility::decToHex( const char *binary, ofint32 length, char * &outstring )
{
    outstring = new char[length*2+1];
    ofuint32 point = 0;
    ofuint32 cur=0;
    while ( cur < (ofuint32)length )
    {
        outstring[point] = binary[cur] / 16 + 48;
        if ( outstring[point] > 57 ) outstring[point] += 7;
        point++;
        outstring[point] = binary[cur] % 16 + 48;
        if ( outstring[point] > 57 ) outstring[point] += 7;
        point++;
        cur++;
    }
    outstring[point]=0;
}

void
OFUtility::decToHex( const char *binary, ofuint32 length, char *outbuf, ofuint32 outlen )
{
    ofuint32 point = 0;
    ofuint32 cur=0;

    if ( outlen >= ( length * 2 + 1 ) )
    {
        while ( cur < length )
        {
            outbuf[point] = binary[cur] / 16 + 48;
            if ( outbuf[point] > 57 ) outbuf[point] += 7;
            point++;
            outbuf[point] = binary[cur] % 16 + 48;
            if ( outbuf[point] > 57 ) outbuf[point] += 7;
            point++;
            cur++;
        }
        outbuf[point]=0;
    }
}


ofuint32
OFUtility::hexToBin( const char *input, ofuint32 inputLength, unsigned char *output, ofuint32 outputLength )
{
    unsigned char *out = output;
    const char *pos;
    char do_first = 1;

    /* Make sure there are an even number of characters to work with.
       If not, pretend there is a zero prepended to the input
     */
    if (inputLength % 2)
    {
        do_first = 0;
        *out = 0;
    }
    if ( outputLength >= inputLength / 2 )
    {
        for ( pos = input; pos <= input+inputLength-1; pos++ )
        {
            if (do_first)
            {
                if ( *pos >= 'A' && *pos <= 'Z' )
                    *out = ((*pos)-'A'+10) << 4;
                else if ( *pos >= 'a' && *pos <= 'z' )
                    *out = ((*pos)-'a'+10) << 4;
                else if ( *pos >= '0' && *pos <= '9' )
                    *out = ((*pos)-'0') << 4;
                pos++;
            }
            else
                do_first = 1;

            if ( *pos >= 'A' && *pos <= 'Z' )
                *out = *out | ((*pos)-'A'+10);
            else if ( *pos >= 'a' && *pos <= 'z' )
                *out = *out | ((*pos)-'a'+10);
            else if ( *pos >= '0' && *pos <= '9' )
                *out = *out | ((*pos)-'0');
            
            out++;

        }
    }
    else
        return 0;
    return out - output;
}

ofuint32
OFUtility::binToHex( const unsigned char *input, ofuint32 inputLength, char *output, ofuint32 outputLength )
{
    const unsigned char *pos = input;
    char *out = output;
    if ( outputLength >= inputLength * 2 )
    {
        for( ; pos < input + inputLength; pos++ )
        {
            *out = (*pos >> 4) + 48;
            if ( (*pos >> 4) > 9 )
                *out = (*pos >> 4) + 55;
            out++;
            *out = (*pos & 15) + 48;
            if ( (*pos & 15) > 9 )
                *out = (*pos & 15) + 55;
            out++;
        }
        if ( outputLength > inputLength * 2 )
            *out = 0;
    }
    else
        return 0;
    return 1;
}

ofuint32
OFUtility::strToULong( const char* str )
{
    char temp[129];
    OFOS::strncpy2( temp, str, 128 );
    char *pos = temp;
    while ( *pos && OFOS::strchr( "0123456789+-", *pos ) )
        pos++;
    if ( *pos )
        *pos = 0;
    return strtoul( temp, 0, 10 );
}

bool 
OFUtility::str_token( const char * str, const char *delim, ofuint32 * start, char *token, ofuint32 tokenLen )
{
     bool ok = false;
     RESETSTRING( token );
     if ( *start < OFOS::strlen( str ) )
     {
	  const char * tstart = str + *start;
	  const char * pos = tstart;
	  while ( *pos && !OFOS::strchr( delim, *pos ) )
	       pos++;
	  if ( tokenLen >= (ofuint32)(pos-tstart) )
	  {
	       OFOS::strncpy( token, tstart, pos-tstart );
	       token[pos-tstart] = 0;
	       *start += (pos-tstart)+1;
	       ok = true;
	  }
     }
     return ok;
}

bool 
OFUtility::str_token2( const char * str, const char *delim, const char *escape, ofuint32 * start, char *token, ofuint32 tokenLen, char trimWS /* = 0 */ )
{
    bool ok = false;
    bool esc = false;
    RESETSTRING( token );

    if ( *start < OFOS::strlen( str ) )
    {
        const char * tstart = str + *start;

        // Deal with starting WS if necessary
        const char *pos = tstart;

        if ( trimWS )
            while ( *pos && OFOS::strchr( " \n\t", *pos ) )
                pos++;

        const char *realStart = pos;

        while ( *pos && (!OFOS::strchr( delim, *pos ) || esc ) )
        {
            if ( !esc && *pos == *escape && !(pos!=tstart && *(pos-1)=='\\') )
                esc = true;
            else if ( esc && *pos == *(escape+1) && !(pos!=tstart && *(pos-1)=='\\') )
                esc = false;
            pos++;
        }

        if ( tokenLen >= (ofuint32)(pos-realStart) )
        {
            OFOS::strncpy( token, realStart, pos-realStart );
            token[pos-realStart] = 0;
            *start += (pos-tstart)+1;

            // Deal with any ending whitespace
            if ( trimWS )
                while ( OFOS::strlen( token ) && OFOS::strchr( " \n\t", token[OFOS::strlen(token) - 1] ) )
                    token[OFOS::strlen(token) - 1] = 0;

            ok = true;
        }

        // Check for any escaped chars in the mix.
        if ( OFOS::strchr( token, *escape ) )
        {
            char *temp = new char[tokenLen+1];
            char *cur = temp;
            pos = token;
            while ( *pos )
            {
                if ( (pos != token && *(pos-1)=='\\') && OFOS::strchr( escape, *pos ) )
                {
                    cur--;
                    *cur++ = *pos;
                }
                else
                    if ( !OFOS::strchr( escape, *pos ) )
                        *cur++ = *pos;
                pos++;
            }
            *cur = 0;
            OFOS::strcpy( token, temp );
            delete [] temp;
        }
    }
    return ok;
}

bool 
OFUtility::str_token3 (const char* str,
                       const char* delim,
                       const char* escape,
                       ofuint32* start,
                       char** token,
                       ofuint32* tokenLen,
                       char trimWS /* = 0 */ )
{
        bool ok = false;
        bool esc = false;
        RESETSTRING( token );

        if (*start < OFOS::strlen (str))
        {
                const char * tstart = str + *start;

                // Deal with starting WS if necessary
                const char *pos = tstart;

                if ( trimWS )
                        while ( *pos && OFOS::strchr( " \n\t", *pos ) )
                                pos++;

                const char *realStart = pos;

                while (*pos && (!OFOS::strchr (delim, *pos) || esc))
                {
                        if (!esc && escape && *pos == *escape && !(pos!=tstart && *(pos-1)=='\\'))
                                esc = true;
                        else if (esc && escape && *pos == *(escape+1) && !(pos!=tstart && *(pos-1)=='\\'))
                                esc = false;
                        pos++;
                }

                *tokenLen = (ofuint32)(pos-realStart);
                *token = new char[*tokenLen+1];

                OFOS::strncpy( *token, realStart, *tokenLen );
                (*token)[pos-realStart] = 0;
                *start += (pos-tstart)+1;

                // Deal with any ending whitespace
                if ( trimWS )
                        while ( OFOS::strlen( *token ) && OFOS::strchr( " \n\t", (*token)[OFOS::strlen(*token) - 1] ) )
                        {
                                (*token)[OFOS::strlen(*token) - 1] = 0;
                        }

                ok = true;

                // Check for any escaped chars in the mix.
                if ( escape && OFOS::strchr( *token, *escape ) )
                {
                        char *temp = new char[*tokenLen+1];
                        char *cur = temp;
                        pos = *token;
                        while ( *pos )
                        {
                                if ( (pos != *token && *(pos-1)=='\\') && OFOS::strchr( escape, *pos ) )
                                {
                                        cur--;
                                        *cur++ = *pos;
                                }
                                else
                                        if ( !OFOS::strchr( escape, *pos ) )
                                                *cur++ = *pos;
                                pos++;
                        }
                        *cur = 0;
                        OFOS::strcpy( *token, temp );
                        delete [] temp;
                }
        }
        return ok;
}

/* static */
ofuint32
OFUtility::convertCommaToCharStar( const char *csList, OFCHARSTARLIST *list )
{
    assert( csList );
    assert( list );

    ofuint32 count = 0;
    const char *pos = csList;
    const char *cur = csList;
    while ( *cur )
    {
        if ( *cur == ',' )
        {
            char *token = new char[cur-pos+1];
            OFOS::strncpy2( token, pos, cur-pos );
            OFUtility::leftTrimString( token );
            OFUtility::rightTrimString( token );
            list->push_back( token );
            pos = cur+1;
            count++;
        }
        cur++;
    }
    return count;
}

void * 
OFUtility::memfind( const void *buffer, ofuint32 buflen, const void * find, ofuint32 findlen ) 
{
        const void* pointer = buffer;
        //bool found = false;

        while (/*!found && */pointer != NULL && !(((char*)pointer + findlen) > ((char*)buffer + buflen)))
        {
                pointer = memchr (pointer, '-', buflen);
                if (pointer)
                {
                        if (memcmp (pointer, find, (size_t) findlen) == 0)
                                return (void*)pointer;
                        else
                                pointer = ((char*)pointer + 1);
                }
        }
        return 0;
}

void * 
OFUtility::memfind2( const void * buffer, ofuint32 buflen, const void * find, ofuint32 findlen ) 
{
    void *pointer = const_cast<void *>( buffer );
    bool found = false;

    while ( !found && pointer != NULL && !( ( (char*)pointer + findlen ) > ((char*)buffer + buflen) ) )
    {
        if ( memcmp(pointer, find, (size_t) findlen ) == 0 )
            found = true;
        else
            pointer = ((char*)pointer) + 1;
    }
    return found?pointer:0;
}

/* static */
void
OFUtility::expandEnvVars( const char *data, char *buffer )
{
    RESETSTRING( buffer );
    
    char *work = new char[4096];
    char *temp = new char[4096];
    OFOS::strcpy( work, data );
    const char *startpos = OFNULL;

    while ((startpos = OFOS::strstr(work, "$(")) != 0)
    {
        const char *endpos = OFOS::strchr(startpos, ')');
        if ( endpos )
        {
            char envval[1025];
            RESETSTRING( envval );
            char envvar[257];
            if ( ( endpos - startpos ) > 2 )
            {
                OFOS::strncpy2( envvar, startpos+2, (endpos-startpos-2) );
                if ( getenv( envvar ) )
                    OFOS::strcpy( envval, getenv( envvar ) );
            }
            // Now we need to substitute the value in.
            OFOS::strcpy( temp, work );
            temp[startpos-work] = 0;
            OFOS::strcat( temp, envval );
            OFOS::strcat( temp, endpos + 1 );
            OFOS::strcpy( work, temp );
        }
    }

    OFOS::strcpy( buffer, work );
    delete [] temp;
    delete [] work;
}

// static
void
OFUtility::destroyCharStarList( OFCHARSTARLIST *list )
{
    assert( list );
    for ( OFCHARSTARIterator i = list->begin(); i != list->end(); i++ )
        delete [] (*i);
    list->clear( );
}

/*static */
ofuint32
OFUtility::translateSystemError( ofuint32 sysErr )
{
#if defined(OFOPSYS_WIN32)
    return translateWin32Error( sysErr );
#else
    return translateUnixError( sysErr );
#endif
}

/*static*/
ofuint32
OFUtility::translateLastSystemError( )
{
#if defined(OFOPSYS_WIN32)
    return translateWin32Error( GetLastError() );
#else
    return translateUnixError( errno );
#endif
}

#if defined(OFOPSYS_WIN32)
/*static */
ofuint32
OFUtility::translateWin32Error( ofuint32 wErr )
{
    ofint32 errmap[][2] = {
		{ ERR_WSACONNREFUSED, WSAECONNREFUSED },
        { ERR_WSAEISCONN, WSAEISCONN },
        { ERR_WSAETIMEDOUT_10060, WSAETIMEDOUT },

        { ERR_WSAEINTR_10004, WSAEINTR },
        { ERR_WSAEBADF_10009, WSAEBADF },
        { ERR_WSAEACCES_10013, WSAEACCES },
        { ERR_WSAEFAULT_10014, WSAEFAULT },
        { ERR_WSAEINVAL_10022, WSAEINVAL },
        { ERR_WSAEMFILE_10024, WSAEMFILE },
        { ERR_WSAEWOULDBLOCK_10035, WSAEWOULDBLOCK },
        { ERR_WSAEINPROGRESS_10036, WSAEINPROGRESS },
        { ERR_WSAEALREADY_10037, WSAEALREADY },
        { ERR_WSAENOTSOCK_10038, WSAENOTSOCK },
        { ERR_WSAEDESTADDRREQ_10039, WSAEDESTADDRREQ },
        { ERR_WSAEMSGSIZE_10040, WSAEMSGSIZE },
        { ERR_WSAEPROTOTYPE_10041, WSAEPROTOTYPE },
        { ERR_WSAENOPROTOOPT_10042, WSAENOPROTOOPT },
        { ERR_WSAEPROTONOSUPPORT_10043, WSAEPROTONOSUPPORT },
        { ERR_WSAESOCKTNOSUPPORT_10044, WSAESOCKTNOSUPPORT },
        { ERR_WSAEOPNOTSUPP_10045, WSAEOPNOTSUPP },
        { ERR_WSAEPFNOSUPPORT_10046, WSAEPFNOSUPPORT },
        { ERR_WSAEAFNOSUPPORT_10047, WSAEAFNOSUPPORT },
        { ERR_WSAEADDRINUSE_10048, WSAEADDRINUSE },
        { ERR_WSAEADDRNOTAVAIL_10049, WSAEADDRNOTAVAIL },
        { ERR_WSAENETDOWN_10050, WSAENETDOWN },
        { ERR_WSAENETUNREACH_10051, WSAENETUNREACH },
        { ERR_WSAENETRESET_10052, WSAENETRESET },
        { ERR_WSAECONNABORTED_10053, WSAECONNABORTED },
        { ERR_WSAECONNRESET_10054, WSAECONNRESET },
        { ERR_WSAENOBUFS_10055, WSAENOBUFS },
        { ERR_WSAENOTCONN_10057, WSAENOTCONN },
        { ERR_WSAESHUTDOWN_10058, WSAESHUTDOWN },
        { ERR_WSAETOOMANYREFS_10059, WSAETOOMANYREFS },
        { ERR_WSAELOOP_10062, WSAELOOP },
        { ERR_WSAENAMETOOLONG_10063, WSAENAMETOOLONG },
        { ERR_WSAEHOSTDOWN_10064, WSAEHOSTDOWN },
        { ERR_WSAEHOSTUNREACH_10065, WSAEHOSTUNREACH },
        { ERR_WSAENOTEMPTY_10066, WSAENOTEMPTY },
        { ERR_WSAEPROCLIM_10067, WSAEPROCLIM },
        { ERR_WSAEUSERS_10068, WSAEUSERS },
        { ERR_WSAEDQUOT_10069, WSAEDQUOT },
        { ERR_WSAESTALE_10070, WSAESTALE },
        { ERR_WSAEREMOTE_10071, WSAEREMOTE },
        { ERR_WSASYSNOTREADY_10091, WSASYSNOTREADY },
        { ERR_WSAVERNOTSUPPORTED_10092, WSAVERNOTSUPPORTED },
        { ERR_WSANOTINITIALISED_10093, WSANOTINITIALISED },
                { ERR_NOT_SERVICE, ERROR_SERVICE_DOES_NOT_EXIST },
                { ERR_SERVICE_NOT_ACTIVE_1062, ERROR_SERVICE_NOT_ACTIVE },
                { ERR_FAILED_SERVICE_CONTROLLER_CONNECT_1063, ERROR_FAILED_SERVICE_CONTROLLER_CONNECT },
		{ -1, -1 }
	};

    ofuint32 oferr = ERR_UNKNOWN_SYSTEM_ERROR;
    for ( ofuint32 cur = 0; oferr == ERR_UNKNOWN_SYSTEM_ERROR && errmap[cur][0] != -1; cur++ )
        if ( errmap[cur][1] == wErr )
            oferr = errmap[cur][0];

#if !defined(NDEBUG)
	if ( oferr == ERR_UNKNOWN_SYSTEM_ERROR )
		cout << "Unmapped error:" << wErr << endl;
#endif
    return oferr;
}
#endif

#if !defined(OFOPSYS_WIN32) && !defined(OFOPSYS_FREEBSD)
ofuint32
OFUtility::translateUnixError( ofint32 uErr )
{
    ofint32 errmap[][2] = {
        { ERR_EPERM,EPERM },
        { ERR_ENOENT,ENOENT },
        { ERR_ESRCH,ESRCH },
        { ERR_EINTR,EINTR },
        { ERR_EIO,EIO },
        { ERR_ENXIO,ENXIO },
        { ERR_E2BIG,E2BIG },
        { ERR_ENOEXEC,ENOEXEC },
        { ERR_EBADF,EBADF },
        { ERR_ECHILD,ECHILD },
        { ERR_EAGAIN,EAGAIN },
        { ERR_ENOMEM,ENOMEM },
        { ERR_EACCES,EACCES },
        { ERR_EFAULT,EFAULT },
        { ERR_ENOTBLK,ENOTBLK },
        { ERR_EBUSY,EBUSY },
        { ERR_EEXIST,EEXIST },
        { ERR_EXDEV,EXDEV },
        { ERR_ENODEV,ENODEV },
        { ERR_ENOTDIR,ENOTDIR },
        { ERR_EISDIR,EISDIR },
        { ERR_EINVAL,EINVAL },
        { ERR_ENFILE,ENFILE },
        { ERR_EMFILE,EMFILE },
        { ERR_ENOTTY,ENOTTY },
        { ERR_ETXTBSY,ETXTBSY },
        { ERR_EFBIG,EFBIG },
        { ERR_ENOSPC,ENOSPC },
        { ERR_ESPIPE,ESPIPE },
        { ERR_EROFS,EROFS },
        { ERR_EMLINK,EMLINK },
        { ERR_EPIPE,EPIPE },
        { ERR_EDOM,EDOM },
        { ERR_ERANGE,ERANGE },
        { ERR_EDEADLK,EDEADLK },
        { ERR_ENAMETOOLONG,ENAMETOOLONG },
        { ERR_ENOLCK,ENOLCK },
        { ERR_ENOSYS,ENOSYS },
        { ERR_ENOTEMPTY,ENOTEMPTY },
        { ERR_ELOOP,ELOOP },
        { ERR_EWOULDBLOCK,EWOULDBLOCK },
        { ERR_ENOMSG,ENOMSG },
        { ERR_EIDRM,EIDRM },
#if !defined(OFOPSYS_DARWIN)
        { ERR_ECHRNG,ECHRNG },
        { ERR_EL2NSYNC,EL2NSYNC },
        { ERR_EL3HLT,EL2HLT },
        { ERR_EL3RST,EL3RST },
        { ERR_ELNRNG,ELNRNG },
        { ERR_EUNATCH,EUNATCH },
        { ERR_ENOCSI,ENOCSI },
        { ERR_EL2HLT,EL2HLT },
        { ERR_EBADE,EBADE },
        { ERR_EBADR,EBADR },
        { ERR_EXFULL,EXFULL },
        { ERR_ENOANO,ENOANO },
        { ERR_EBADRQC,EBADRQC },
        { ERR_EBADSLT,EBADSLT },
        { ERR_EDEADLOCK,EDEADLOCK },
        { ERR_EBFONT,EBFONT },
        { ERR_ENOSTR,ENOSTR },
        { ERR_ENODATA,ENODATA },
        { ERR_ETIME,ETIME },
        { ERR_ENOSR,ENOSR },
        { ERR_ENONET,ENONET },
        { ERR_ENOPKG,ENOPKG },
#endif
        { ERR_EREMOTE,EREMOTE },
#if !defined(OFOPSYS_DARWIN)
        { ERR_ENOLINK,ENOLINK },
        { ERR_EADV,EADV },
        { ERR_ESRMNT,ESRMNT },
        { ERR_ECOMM,ECOMM },
        { ERR_EPROTO,EPROTO },
        { ERR_EMULTIHOP,EMULTIHOP },
#if !defined(OFOPSYS_SOLARIS)
        { ERR_EDOTDOT,EDOTDOT },
#endif
        { ERR_EBADMSG,EBADMSG },
#endif
        { ERR_EOVERFLOW,EOVERFLOW },
#if !defined(OFOPSYS_DARWIN)
        { ERR_ENOTUNIQ,ENOTUNIQ },
        { ERR_EBADFD,EBADFD },
        { ERR_EREMCHG,EREMCHG },
        { ERR_ELIBACC,ELIBACC },
        { ERR_ELIBBAD,ELIBBAD },
        { ERR_ELIBSCN,ELIBSCN },
        { ERR_ELIBMAX,ELIBMAX },
        { ERR_ELIBEXEC,ELIBEXEC },
#endif
        { ERR_EILSEQ,EILSEQ },
#if !defined(OFOPSYS_DARWIN)
        { ERR_ERESTART,ERESTART },
        { ERR_ESTRPIPE,ESTRPIPE },
#endif
        { ERR_EUSERS,EUSERS },
        { ERR_ENOTSOCK,ENOTSOCK },
        { ERR_EDESTADDRREQ,EDESTADDRREQ },
        { ERR_EMSGSIZE,EMSGSIZE },
        { ERR_EPROTOTYPE,EPROTOTYPE },
        { ERR_ENOPROTOOPT,ENOPROTOOPT },
        { ERR_EPROTONOSUPPORT,EPROTONOSUPPORT },
        { ERR_ESOCKTNOSUPPORT,ESOCKTNOSUPPORT },
        { ERR_EOPNOTSUPP,EOPNOTSUPP },
        { ERR_EPFNOSUPPORT,EPFNOSUPPORT },
        { ERR_EAFNOSUPPORT,EAFNOSUPPORT },
        { ERR_EADDRINUSE,EADDRINUSE },
        { ERR_EADDRNOTAVAIL,EADDRNOTAVAIL },
        { ERR_ENETDOWN,ENETDOWN },
        { ERR_ENETUNREACH,ENETUNREACH },
        { ERR_ENETRESET,ENETRESET },
        { ERR_ECONNABORTED,ECONNABORTED },
        { ERR_ECONNRESET,ECONNRESET },
        { ERR_ENOBUFS,ENOBUFS },
        { ERR_EISCONN,EISCONN },
        { ERR_ENOTCONN,ENOTCONN },
        { ERR_ESHUTDOWN,ESHUTDOWN },
        { ERR_ETOOMANYREFS,ETOOMANYREFS },
        { ERR_ETIMEDOUT,ETIMEDOUT },
        { ERR_ECONNREFUSED,ECONNREFUSED },
        { ERR_EHOSTDOWN,EHOSTDOWN },
        { ERR_EHOSTUNREACH,EHOSTUNREACH },
        { ERR_EALREADY,EALREADY },
        { ERR_EINPROGRESS,EINPROGRESS },
        { ERR_ESTALE,ESTALE },
#if !defined(OFOPSYS_SOLARIS) && !defined(OFOPSYS_DARWIN)
        { ERR_EUCLEAN,EUCLEAN },
        { ERR_ENOTNAM,ENOTNAM },
        { ERR_ENAVAIL,ENAVAIL },
        { ERR_EISNAM,EISNAM },
        { ERR_EREMOTEIO,EREMOTEIO },
#endif
        { ERR_EDQUOT,EDQUOT },
#if !defined(OFOPSYS_SOLARIS) && !defined(OFOPSYS_DARWIN)
        { ERR_ENOMEDIUM,ENOMEDIUM },
        { ERR_EMEDIUMTYPE,EMEDIUMTYPE },
#endif
        { -1, -1 }
    };

    ofuint32 oferr = ERR_UNKNOWN_SYSTEM_ERROR;
    for ( ofuint32 cur = 0; oferr == ERR_UNKNOWN_SYSTEM_ERROR && errmap[cur][0] != -1; cur++ )
        if ( errmap[cur][1] == uErr )
            oferr = errmap[cur][0];
#if !defined(NDEBUG)
	if ( oferr == ERR_UNKNOWN_SYSTEM_ERROR )
		cerr << "Could not map error: " << oferr << endl;
#endif
    return oferr;
}
#endif

char
OFUtility::addressIn1918( struct sockaddr_in* addr )
{
    if ( !addr )
        return 0;
    ofuint32 a = addr->sin_addr.s_addr;
    a = OFOS::hton32(a);
    if ( ( a & 0xff000000 ) >> 24 == 0x0a )
        return 1;
    if ( ( a & 0xfff00000 ) >> 16 == 0xac10 )
        return 2;
    if ( ( a & 0xffff0000 ) >> 16 == 0xc0a8 )
        return 3;
    return 0;
}

#if defined(OFOPSYS_WIN32)
ofuint32
OFUtility::getLine(char* line, ofuint32 buffer_length)
{
	//int cpos=0;
	ofuint32 clen=0;
	ofuint32 cont = 1;
	char c;

	while (cont)
	{
		c = getch();
		line[clen++] = c;
		if (c==0xd || clen>=buffer_length)
            cont=0;
	}
	line[clen]=0;
	return clen;
}
#endif

ofint32
OFUtility::readPassword(char* password, ofuint32 password_length)
{
#if defined(OFOPSYS_WIN32)
    OFUtility::getLine(password, password_length);
    OFUtility::rightTrimString(password);
    return OFOS::strlen(password);
#else
    struct termios old, newinfo;
    ofint32 nread;
     
    /* Turn echoing off and fail if we can't. */
    if (tcgetattr (fileno (stdin), &old) != 0)
        return -1;
    newinfo = old;
    newinfo.c_lflag &= ~ECHO;
    if (tcsetattr (fileno (stdin), TCSAFLUSH, &newinfo) != 0)
        return -1;
     
    /* Read the password. */
#if defined(__USE_GNU)
    char* temppass = 0;
    size_t templen = 0;
    nread = getline (&temppass, &templen, stdin);
    if (nread)
        OFOS::strncpy2(password, temppass, minimum<ofuint32>(password_length, templen));
    OFUtility::rightTrimString(password);
    free(temppass);
#else
    fgets(password, password_length-1, stdin);
    nread = OFOS::strlen(password);
#endif
 
    /* Restore terminal. */
    tcsetattr (fileno (stdin), TCSAFLUSH, &old);
     
    return nread;
#endif
}

void
OFUtility::scanCommaSepLine(const char *line, OFCHARSTARLIST *elements)
{
    ofuint32 max_word_len = OFOS::strlen(line);
    char* word = new char[max_word_len+1];
    char in_quotes = 0;
    const char* pos = line;
    while (*pos)
    {
        // Start a new word
        char* word_pos = word;
        while (*pos && (*pos != ',' ||  in_quotes))
        {
            if (*pos == '"')
                in_quotes = !in_quotes;
            else
                *word_pos++ = *pos;
            pos++;
        }
        *word_pos = 0;

        // Only if we haven't reached the end of the string
        if (*pos)
        {
            // Jump over comma
            pos++;
            // And white space
            while (*pos && (*pos == ' ' || *pos == '\t'))
                pos++;
        }
        elements->push_back(word);
        word = new char[max_word_len+1];
    }
    delete[] word;
}

/*static*/
ofint64
OFUtility::readLinesCSV(OFFile *file,
                        READLINESCB cb,
                        void* userData)
{
    ofint64 record_count = 0;
    if (!file->isOpen())
        return -1;
    
    char* line = 0;
    ofint64 line_number = 1;
    while (OFUtility::readLine2(file, &line))
    {
        OFCHARSTARLIST words;
        OFUtility::scanCommaSepLine(line, &words);
        delete[] line;

        cb(&words, line_number, userData);
        for (OFCHARSTARLIST::iterator i = words.begin();
             i != words.end();
             i++)
            delete[] (*i);
        words.clear();

        line_number++;
    }
    return record_count;
}

void
OFUtility::retrieveConfigValues(OFConfigurator* config,
                                const char* groupName,
                                OFCHARSTARLIST* list)
{
    OFCHARSTARLIST values;
    config->getActualValuesInGroup(groupName, &values);
    for (OFCHARSTARLIST::iterator i = values.begin();
         i != values.end(); i++)
    {
        ofuint32 start = 0;
        char* token;
        ofuint32 token_length;
        while (OFUtility::str_token3(*i, ",", "\'\'", &start, &token, &token_length, 1))
        {
            list->push_back(token);
        }
        delete[] *i;
    }
}

vector<string> OFUtility::split_string_to_list(string& l, string delim)
{
    vector<string> c;
    while (l.length())
    {
        string sub;
        string::size_type p = l.find(delim);
        if (p != string::npos)
        {
            sub = l.substr(0, p);
            l = l.substr(++p);
        }
        else
        {
            sub = l;
            l = "";
        }
        c.push_back(sub);
    }
    return c;
}

void OFUtility::split_string_on_delim(const string& l, const string delim, string &left, string &right)
{
    string::size_type p = l.find(delim);
    if (p == string::npos) {
        left = l;
        right = "";
    } else {
        left = l.substr(0, p);
        p++;
        right = l.substr(p, string::npos);
    }
}

void OFUtility::split_string_on_last_delim(const string& l
                                           , const string delim
                                           , string &left
                                           , string &right)
{
    string::size_type p = l.rfind(delim);
    if (p == string::npos) {
        right = l;
        left = "";
    } else {
        left = l.substr(0, p);
        p++;
        right = l.substr(p, string::npos);
    }
}

void OFUtility::convertToBase64( const char *data, ofuint32 buflen, char *output )
{
    char conv[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";

    const char *current = data;
    const char *finish = data + buflen;
    while ( current < finish )
    {
        char outbuf[4];
        char inbuf[3];

        ofint32 x;
        for ( x=0; x<3; x++ )
            if ( current+x <= finish )
                inbuf[x] = current[x];
            else
                inbuf[x] = 0;

        outbuf[0] = (inbuf[0] & 0xfc) >> 2;
        outbuf[1] = ((inbuf[0] & 0x3) << 4) | ((inbuf[1] & 0xf0) >> 4);        //(*(ofuint16*)current++) & 0xfc0;
        outbuf[2] = ((inbuf[1] & 0xf) << 2) | ((inbuf[2] & 0xc0) >> 6);
        outbuf[3] = (inbuf[2] & 0x3f);

        ofint32 upto = 4;
        if ( finish - current == 1 )
            upto = 2;
        if ( finish - current == 2 )
            upto = 3;
        for ( x = 0; x < 4; x++ )
            if ( x < upto )
                *output++ = conv[(ofuint32)outbuf[x]];
            else
                *output++ = '=';

        current += 3;
    }
    *output = 0;
}

ofuint32 OFUtility::convertFromBase64( const char *data, ofuint32 buflen, char *output )
{
    // SGVsbG8gQmlnIEJveQ==
    const char *current = data;
    char inbuf[4];
    char *pos = output;
    ofint32 forandy=(ofint32)buflen;
    ofuint32 count = 0;
    ofuint32 size = 0;
    while ( *current && forandy > 0 )
    {
        while ( *current && forandy > 0 && (*current=='\r'||*current=='\n'))
        {
            current++;
            forandy--;
        }
        ofint32 x;
        for ( x = 0; x < 4; x++ )
        {
            if ( current[x] == '\n' || current[x] == '\r' )
                x = x?x-1:x;
            else
            {
                unsigned char t = current[x];
                if ( t >= 'A' && t <= 'Z' )
                    t -= 'A';
                else if ( t >= 'a' && t <= 'z' )
                    t = t - 'a' + 26;
                else if ( t >= '0' && t <= '9' )
                    t = t - '0' + 52;
                else if ( t == '+' )
                    t = 62;
                else if ( t == '/' )
                    t = 63;
                else if ( t == '=' )
                {
                    if ( !size )
                        size = count;
                    t = 0;
                }
                inbuf[x] = t;
            }
        }
        *pos++ = (inbuf[0] << 2) | ((inbuf[1] & 0x30) >> 4);
        *pos++ = ((inbuf[1] & 0xf) << 4) | ((inbuf[2] & 0x3c) >> 2);
        *pos++ = ((inbuf[2] & 0x3) << 6) | (inbuf[3] & 0x3f);
        count += 3;
        current += 4;
        forandy -= 4;
    }
    *pos = 0;
    return size;
}


ofuint32 OFUtility::convertFromBase64_2( const char *data, ofuint32 buflen, char *output )
{
    // SGVsbG8gQmlnIEJveQ==
    const char *current = data;
    char inbuf[4];
    char *pos = output;
    
    // Check we are not going to write over the end of the buffer.
    while ( *current && (ofuint32)( pos - output ) < ( buflen + 4 ) )
    {
        ofint32 x;
        // Ignore line feeds/carriage returns and other garbage
        if ( current[0] > 31 )
        {    
            for ( x = 0; x < 4; x++ )
            {
                unsigned char t = current[x];
                
                if ( t >= 'A' && t <= 'Z' )
                    t -= 'A';
                else if ( t >= 'a' && t <= 'z' )
                    t = t - 'a' + 26;
                else if ( t >= '0' && t <= '9' )
                    t = t - '0' + 52;
                else if ( t == '+' )
                    t = 62;
                else if ( t == '/' )
                    t = 63;
                else if ( t == '=' )
                    t = 0;
                inbuf[x] = t;
            }
            *pos++ = (inbuf[0] << 2) | ((inbuf[1] & 0x30) >> 4);
            *pos++ = ((inbuf[1] & 0xf) << 4) | ((inbuf[2] & 0x3c) >> 2);
            *pos++ = ((inbuf[2] & 0x3) << 6) | (inbuf[3] & 0x3f);
            current += 4;
        }
        else
            current += 1;
    }
    return pos - ( output + 1 );
}

ofuint32 OFUtility::calculateBase64Size( const char *data, ofuint32 buflen /* = 0 */ )
{
	if ( !buflen )
		buflen = ofstrlen( data );
	return ( (buflen/3+1) * 4 + 1);
}

char
OFUtility::intToHexChar(ofint32 i)
{
    if (i >= 0 && i <= 9)
    {
        return '0'+i;
    }
    else if (i >= 10 && i <= 15)
    {
        return 'A'+i-10;
    }
    else
    {
        assert(SHOULD_NEVER_GET_HERE);
        return 0;
    }
}

OFINLINE std::string OFUtility::dataToHexString( unsigned char data)
{
    std::string outstring;
    char temp;
    temp = intToHexChar(data / 16);
    outstring += temp;

    temp = intToHexChar(data % 16);
    outstring += temp;

    return outstring;
}
    
std::string OFUtility::dataToHexString( unsigned const char* data, ofint32 length)
{
    std::string outstring;
    for ( ofint32 i=0; i < length; i++)
        outstring += dataToHexString(data[i]);
    return outstring;
}

OFINLINE ofint32 OFUtility::hexCharToInt(char c)
{
    if (c >= '0' && c <= '9')
    {
        return c-'0';
    }
    else if (c >= 'A' && c <= 'F')
    {
        return c-'A'+10;
    }
    else if (c >= 'a' && c <= 'f')
    {
        return c-'a'+10;
    }
    else
    {
        assert(SHOULD_NEVER_GET_HERE);
        return 0;
    }
}

void OFUtility::hexStringToData(const std::string& hexString, unsigned char*& data, ofuint32* length)
{
    assert(hexString.length()%2 == 0); // only works for even length strings (which is all dataToHexString() creates)
    *length = hexString.length()/2;
    data = new unsigned char[*length];
    for (ofuint32 i=0; i < *length; i++)
    {
        data[i] = 16*hexCharToInt(hexString[i*2]) + hexCharToInt(hexString[i*2+1]);
    }
}

ofuint32 OFUtility::calculateHexBinarySize( const char *data, ofuint32 buflen /* = 0 */ )
{
    if ( !buflen )
        buflen = ofstrlen( data );
    return ( buflen * 2 + 1);
}

OFINLINE std::string OFUtility::toString(ofint16 theValue)
{
    char buffer[32];
    OFOS::snprintf(buffer, 31, "%d",theValue);
    std::string outstring(buffer);
    return outstring;
}
        
OFINLINE std::string OFUtility::toString(ofint32 theValue)
{
    char buffer[32];
    OFOS::snprintf(buffer, 31, "%" LFORMAT "d",theValue);
    std::string outstring(buffer);
    return outstring;
}

OFINLINE std::string OFUtility::toString(ofuint32 theValue)
{
    char buffer[32];
    OFOS::snprintf(buffer,31, "%" LFORMAT "u", (ofuint32)theValue);
    std::string outstring(buffer);
    return outstring;
}

OFINLINE std::string OFUtility::toString(double theValue)
{
    char buffer[32];
    OFOS::snprintf(buffer,31, "%f", theValue);
    std::string outstring(buffer);
    return outstring;
}

OFINLINE ofint32 OFUtility::toInt(const std::string& theValue)
{
    return atoi(theValue.c_str());
}

OFINLINE ofint32 OFUtility::toOfint32(const std::string& theValue)
{
    return atol(theValue.c_str());
}

OFINLINE ofuint32 OFUtility::toofuint32(const std::string& theValue)
{
    return strtoul(theValue.c_str(),NULL,10);
}

OFINLINE double OFUtility::toDouble(const std::string& theValue)
{
    return atof(theValue.c_str());
}

void OFUtility::toASCIIfromURL(const char *value,
                               char *result,
                               ofuint32 result_length,
                               const char* spaceEnc)
{
    char *pos = (char*)value;
    char *copypos = result;
    
    while (*pos && ((ofuint32)(copypos-result) < result_length))
    {
        if (!OFOS::strcmp(pos, spaceEnc))
        {
            *copypos++ = 32;
            pos += OFOS::strlen(spaceEnc);
        }
        else 
        {
            if ( *pos != '%' && *pos != '+' )
                *copypos++ = *pos++;
            else
            {
                if (*pos == '+')
                {
                    *copypos++ = 32;
                    pos++;
                }
                else
                {
                    if (pos[1] && ((pos[1] >= '0' && pos[1] <= '9') ||
                                   (pos[1] >= 'A' && pos[1] <= 'F') ||
                                   (pos[1] >= 'a' && pos[1] <= 'f')) &&
                        pos[2] && ((pos[2] >= '0' && pos[2] <= '9') ||
                                   (pos[2] >= 'A' && pos[2] <= 'F') ||
                                   (pos[2] >= 'a' && pos[2] <= 'f')))
                    {
                        pos++;
                        char one = toUpper(*pos++);
                        char two = toUpper(*pos++);
                        ofint16 dec = (one>='0' && one <= '9')?(one-48)*16:(one-55)*16;
                        dec += (two >= '0' && two <= '9')?(two-48):(two-55);
                        *copypos = (char)dec;
                        copypos++;
                    }
                    else
                    {
                        *copypos++ = '%';
                        pos++;
                    }
                }
            }
        }
    }
    *copypos = 0;
}

void
OFUtility::toURLfromASCII( const char *value, char *result, const char* spaceEnc /* = "+" */ )
{
    assert(result);

    if (!value)
    {
        result[0] = '\0';
        return;
    }
 
    for (const char* cp = value; *cp != 0; cp++)
    {
        const ofchar c = *cp;
        if ( (c >= 'a' && c <= 'z') ||
             (c >= 'A' && c <= 'Z') ||
             (c >= '0' && c <= '9') ||
             c == '-' ||
             c == '_' ||
             c == '.' ||
             c == '*' ||
             c == '\'' ||
             c == '(' ||
             c == ')' ||
             c == ':' )
            *result++ = c;
        else if (c == ' ')
        {
            OFOS::strcpy(result, spaceEnc);
            result += OFOS::strlen(spaceEnc);
            //*result++ = '+';
        }
        else
        {
            *result++ = '%';
            *result++ = (c >> 4) + (((c >> 4) > 9) ? 'A' - 10: '0');
            *result++ = (c & 0x0F) + (((c & 0x0F) > 9) ? 'A' - 10: '0');
        }
    }
    /* terminate new string*/
    *result = 0;
}

char *
OFUtility::toURLfromPath( const char *value, char *result )
{
    char *ret = result;

    assert(result);

    if (!value)
    {
        result[0] = '\0';
        return result;
    }
 
    for (const char* cp = value; *cp != 0; cp++)
    {
        const ofchar c = *cp;
        if ( (c >= 'a' && c <= 'z') ||
             (c >= 'A' && c <= 'Z') ||
             (c >= '0' && c <= '9') ||
             c == '/' || // keep slashes unencoded
             c == '-' ||
             c == '_' ||
             c == '.' ||
             c == '*' ||
             c == '\'' ||
             c == '(' ||
             c == ')' ||
             c == ':' )
            *result++ = c;
        else if (c == ' ')
            *result++ = '+';
        else
        {
            *result++ = '%';
            *result++ = (c >> 4) + (((c >> 4) > 9) ? 'A' - 10: '0');
            *result++ = (c & 0x0F) + (((c & 0x0F) > 9) ? 'A' - 10: '0');
        }
    }
    // terminate new string
    *result = 0;

    return ret;
}

char *
OFUtility::toXURLfromPath( const char *value, char *result )
{
    char *ret = result;

    assert(result);

    if (!value)
    {
        result[0] = '\0';
        return result;
    }
 
    for (const char* cp = value; *cp != 0; cp++)
    {
        const ofchar c = *cp;
        if ( (c >= 'a' && c <= 'z') ||
             (c >= 'A' && c <= 'Z') ||
             (c >= '0' && c <= '9') ||
             c == '/' || // keep slashes unencoded
             c == '-' ||
             c == '_' ||
             c == '.' ||
             c == '*' ||
             //c == '\'' || encode apostrophe
             c == '(' ||
             c == ')' ||
             c == ':' )
            *result++ = c;
        else if (c == ' ')
            *result++ = '+';
        else
        {
            *result++ = '%';
            *result++ = (c >> 4) + (((c >> 4) > 9) ? 'A' - 10: '0');
            *result++ = (c & 0x0F) + (((c & 0x0F) > 9) ? 'A' - 10: '0');
        }
    }
    // terminate new string
    *result = 0;

    return ret;
}

char *
OFUtility::toXURLfromASCII( const char *value, char *result )
{
    char *ret = result;

    assert(result);

    if (!value)
    {
        result[0] = '\0';
        return result;
    }
 
    for (const char* cp = value; *cp != 0; cp++)
    {
        const ofchar c = *cp;
        if ( (c >= 'a' && c <= 'z') ||
             (c >= 'A' && c <= 'Z') ||
             (c >= '0' && c <= '9') ||
             c == '-' ||
             c == '_' ||
             c == '.' ||
             c == '*' ||
             //c == '\'' || encode apostrophe
             c == '(' ||
             c == ')' ||
             c == ':' )
            *result++ = c;
        else if (c == ' ')
            *result++ = '+';
        else
        {
            *result++ = '%';
            *result++ = (c >> 4) + (((c >> 4) > 9) ? 'A' - 10: '0');
            *result++ = (c & 0x0F) + (((c & 0x0F) > 9) ? 'A' - 10: '0');
        }
    }
    // terminate new string
    *result = 0;

    return ret;
}

char
OFUtility::toUpper( const char ch )
{
    return ( ch < 'a' || ch > 'z' )?ch:ch-('a'-'A');
}

char *
OFUtility::toRelativePath( const char *fromPath, const char *toPath, char **relPath )
{
    ofuint32 baseLevel = 0;
    ofuint32 refLevel = 0;
    const char *mp = fromPath;
    const char *tp = toPath;
    const char *rel = toPath;

    for ( ; mp && tp && *mp; ++mp, ++tp )
    {
        if ( *mp == *tp )
        {
            // match

            if ( *mp == *OF_OBJECT_SEPARATOR_STR )
            {
                baseLevel++;
                refLevel++;
                rel = tp+1;
            }   
            else if ( *mp == 0 /* == *tp */ )
            {
                rel = ".";
            }
            else
            {
                // ignore
            }
        }
        else
        {
            if ( *tp == 0 )
            {
                // end of target path

                if ( *mp == *OF_OBJECT_SEPARATOR_STR )
                {
                    if ( mp[1] == 0 )
                    {
                        // special case
                        // abc/def/
                        // abd/def
                        rel = ".";
                    }  
                    else
                    {
                        // abc/def/g...
                        // abd/def
                        rel = "";
                    }
                    ++mp;
                }
            }
                
            break;
        }
    }

    // count remaining levels in the base path
    for ( ; mp && *mp; ++mp )
    {
        if ( *mp == *OF_OBJECT_SEPARATOR_STR )
        {
            baseLevel++;
        }
    }

    // calculate space needed
    ofuint32 size = 1; // null terminator

    ofuint32 j;
    for ( j = baseLevel; j > refLevel; --j )
    {
        size += 2; // ..

        if (j > refLevel + 1 || (rel && *rel))
            ++size; // OF_OBJECT_SEPARATOR_STR
    }
    if ( rel && *rel )
    {
        size += OFOS::strlen( rel );
    }

    char *work = *relPath = new char [ size ];

    RESETSTRING( work );
    for ( j = baseLevel; j > refLevel; --j )
    {
        OFOS::strcat( work, ".." );

        if (j > refLevel + 1 || (rel && *rel))
            OFOS::strcat( work, OF_OBJECT_SEPARATOR_STR );
    }
    if ( rel && *rel )
    {
        OFOS::strcat( work, rel );
    }

    return *relPath;        
}

void OFUtility::to_bin (ofuint32 src, char* out)
{
        ofuint32 j = 1<<31;
        char* pos = out;
        for (ofuint32 i = 32; i; i--)
        {
                *pos++ = (src&j)?'1':'0';
                j /= 2;
        }
        *pos = 0;
}

void 
OFUtility::dumpId( OFFile *file, const OFIDENTITY *id )
{
    OFUtility::dumpuint16( file, id->m_flags );
    OFUtility::dumpuint64( file, id->m_id0 );
    OFUtility::dumpuint64( file, id->m_id1 );
}

void
OFUtility::dumpId( char **data, const OFIDENTITY *id )
{
    OFUtility::dumpuint16( data, id->m_flags );
    OFUtility::dumpuint64( data, id->m_id0 );
    OFUtility::dumpuint64( data, id->m_id1 );
}

void 
OFUtility::undumpId( OFFile *file, OFIDENTITY *id )
{
    char tempid[128];
    ofuint32 l;
    OFUtility::undumpuint32( file, &l );
    file->read( tempid, l );
    tempid[l] = 0;
    *id = tempid;
}

void
OFUtility::undumpId( const char **data, OFIDENTITY *id )
{
    ofuint32 len = 0;
    char tempid[128];

    OFUtility::undumpuint32( data, &len );

    ::memcpy( tempid, *data, len );
    tempid[len] = 0;
    *data += len;

    *id = tempid;
}

void 
OFUtility::dumpStr( OFFile *file, const char *str )
{
    ofuint32 l = ofstrlen( str );
    OFUtility::dumpuint32( file, l );
    file->write( str, l );
}

void
OFUtility::dumpStr( char **data, const char *str )
{
    ofint32 l = ofstrlen( str );
    OFUtility::dumpuint32( data, l );
    ::memcpy( *data, str, l );
    *data += l;
}

void 
OFUtility::undumpStr( OFFile *file, char *str )
{
    ofuint32 l;
    OFUtility::undumpuint32( file, &l );
    file->read( str, l );
    str[l] = 0;
}

void
OFUtility::undumpStr( const char **data, char *str )
{
    ofint32 l;
    const char *pos = *data;
    ::memcpy( &l, pos, sizeof(l) );
    l = ntohl( l );
    pos += sizeof(l);
    ::memcpy( str, pos, l );
    str[l] = 0;
    *data = pos + l;
}

/* static */
void
OFUtility::undumpNewStr( const char **data, char **str )
{
    ofint32 len;
    const char *pos = *data;
    ::memcpy( &len, pos, sizeof(len) );
    len = ntohl( len );
    *str = new char[ len + 1 ];
    pos += sizeof( len );
    ::memcpy( *str, pos, len );
    (*str)[ len ] = 0;
    *data = pos + len;
}

void OFUtility::dumpuint16( OFFile *file, ofuint16 ho )
{
    unsigned short no = htons( ho );
    file->write( &no, sizeof( no ) );
}

void OFUtility::dumpuint32( OFFile *file, ofuint32 ho )
{
	unsigned long no = htonl( ho );
	file->write( &no, sizeof( no ) );
}

void OFUtility::dumpuint64( OFFile *file, ofuint64 ho )
{
    unsigned long no = htonl( ho & 0xffffffff );
    file->write( &no, sizeof( no ) );
    no = htonl( ho >> 32 );
    file->write( &no, sizeof( no ) );
}

void OFUtility::undumpuint32( OFFile *file, ofuint32 *ho )
{
    unsigned long no;
    file->read( &no, sizeof( no ) );
    *ho = ntohl( no );
}

void OFUtility::dumpuint16( char **data, ofuint16 ho )
{
    ofuint16 no = htons( ho );
    ofuint16 *temp = (ofuint16*)*data;
    *temp = no;
    *data += sizeof(no);
}

void OFUtility::dumpuint32( char **data, ofuint32 ho )
{
    ofuint32 no = htonl( ho );
    ofuint32 *temp = (ofuint32*)*data;
    *temp = no;
    *data += sizeof(no);
}

void OFUtility::dumpuint64( char **data, ofuint64 ho )
{
    unsigned long no = htonl( ho & 0xffffffff );
    ofuint32 *temp = (ofuint32*)*data;
    *temp = no;
    *data += sizeof(no);
    no = htonl( ho >> 32 );
    temp = (ofuint32*)*data;
    *temp = no;
    *data += sizeof(no);
}

void OFUtility::undumpuint32( const char **data, ofuint32 *ho )
{
    ofuint32 no = *(ofuint32*)*data;
    *ho = ntohl( no );
    *data += sizeof( no );
}

void OFUtility::dumpint32( OFFile *file, ofint32 ho )
{
    ofint32 no = htonl( ho );
    file->write( &no, sizeof( no ) );
}

void OFUtility::undumpint32( OFFile *file, ofint32 *ho )
{
    ofint32 no;
    file->read( &no, sizeof( no ) );
    *ho = ntohl( no );
}

void OFUtility::dumpint32( char **data, ofint32 ho )
{
    ofint32 no = htonl( ho );
    ofint32 *temp = (ofint32*)*data;
    *temp = no;
    *data += sizeof(no);
}

void OFUtility::undumpint32( const char **data, ofint32 *ho )
{
    ofint32 no = *(ofint32*)*data;
    *ho = ntohl( no );
    *data += sizeof( no );
}

// HexBinary Conversion
void
OFUtility::convertToHexBinary( const char *data, ofuint32 buflen, char *output )
{
    char conv[] = "0123456789ABCDEF";

    const char *current = data;
    const char *finish = data + buflen;
    
    while ( current < finish )
    {
        // High byte
        output[0] = conv[ ( current[0] & 0xF0 ) >> 4 ];
        output++;
        // Low byte
        output[0] = conv[ ( current[0] & 0x0F ) ];
        output++;
        current++;
    }
    *output = 0;
}

ofuint32
OFUtility::convertFromHexBinary( const char *data, ofuint32 buflen, char *output )
{
    const char *current = data;
    char *pos = output;
    unsigned char t;
    
    // Check we are not going to write over the end of the buffer.
    while (*current&&(ofuint32)(pos-output)<(ofuint32)(buflen+4))
    {
        t = current[0];
        if ( t >= 'A' && t <= 'F' )
            t = t - 'A' + 10;
        else if ( t >= 'a' && t <='f' )
            t = t - 'a' + 10;
        else if ( t >= '0' && t <= '9' )
            t = t - '0';        
        pos[0] = t << 4;
        current++;
        
        t = current[0];
        if ( t >= 'A' && t <= 'F' )
            t = t - 'A' + 10;
        else if ( t >= 'a' && t <='f' )
            t = t - 'a' + 10;
        else if ( t >= '0' && t <= '9' )
            t = t - '0'; 
        pos[0] = pos[0] | t;
        current++;
        pos++;
    }
    return pos - output;
}

// //////////////////////////////////////////////////////////////////////////

#if defined(UNIT_TEST)
#include "UnitTest.h"

ofuint32 cb1(OFCHARSTARLIST* line, ofuint32 line_number, void* user)
{
    cout << "Line:" << line_number;
    for (OFCHARSTARLIST::iterator i = line->begin();
         i != line->end();
         i++)
        cout << "<" << *i << ">";
    cout << endl;
    return 0;
}

int main( ofint32 argc, char *argv[] )
{
    UT_BATCH_START( "OFUtility" );

    UT_TEST_START( 1, "decToHex()" );
    const char *instr1 = "hello world.";
    char *outbuf = new char[OFOS::strlen(instr1)*2+1];
    OFUtility::decToHex( instr1, OFOS::strlen(instr1), outbuf, OFOS::strlen(instr1)*2+1 );
    UT_EVAL_EXPR(!OFOS::stricmp( outbuf, "68656C6C6F20776F726C642E"));
    UT_TEST_END;

    UT_TEST_START( 2, "hexToBin()" );
    char *instr2 = new char[OFOS::strlen(outbuf)+1];
    OFOS::strcpy( instr2, outbuf );
    delete [] outbuf;
    char *outbuf2 = new char[OFOS::strlen(instr2)/2+1];
    ofuint32 len2 = OFUtility::hexToBin( instr2, OFOS::strlen(instr2), (unsigned char *)outbuf2, OFOS::strlen(instr2)/2+1 );
    outbuf2[len2] = 0;
    cout << outbuf2 << endl;
    UT_EVAL_EXPR(( !OFOS::strcmp( outbuf2, instr1 ) ) );
    delete [] instr2;
    delete [] outbuf2;
    UT_TEST_END;

    UT_TEST_START( 3, "str_token2()" );
    char buf3[] = "rootobject.users.<tim@su\\>ntail.net>";
    char out3[128];
    ofuint32 start = 0;
    ofuint32 count = 1;
    while ( OFUtility::str_token2( buf3, ".", "<>", &start, out3, 128 ) )
    {
        cout << "TOKEN:" << out3 << endl;
        switch ( count )
        {
        case 1:
            UT_EVAL_EXPR(( !OFOS::strcmp( out3, "rootobject" ) ) );
            break;
        case 2:
            UT_EVAL_EXPR(( !OFOS::strcmp( out3, "users" ) ) );
            break;
        case 3:
            UT_EVAL_EXPR(( !OFOS::strcmp( out3, "tim@su>ntail.net" ) ) );
            break;
        }
        count++;
    }
    UT_TEST_END;

    UT_TEST_START( 4, "str_token2 2" );
    const char *buf4 = " \"  Hello  \"  , World";
    ofuint32 start4 = 0;
    char token4[129];
    cout << "buffer: [" << buf4 << "]" << endl;
    OFUtility::str_token2( buf4, ",", "\"\"", &start4, token4, 128, 1 );
    cout << "Token4: [" << token4 << "]" << endl;
    UT_EVAL_EXPR(!OFOS::strcmp( token4, "  Hello  " ) );

    UT_TEST_END;

    UT_TEST_START( 5, "empty token" );
    const char *buf5 = " ";
    char token5[129];
    ofuint32 start5 = 0;
    OFUtility::str_token2( buf5, ",", "\"\"", &start5, token5, 128, 1 );

    UT_TEST_END;

    UT_TEST_START( 6, "readLine2" );
    
    OFFile file( "./ofutility.cpp", 0664, OFFILE_OPEN_EXISTING | OFFILE_READONLY );
    UT_EVAL_EXPR(file.isOpen() );
    if ( file.isOpen() )
    {
        for ( ofuint32 x6 = 0; x6 < 7; x6++ )
        {
            char *line;
            OFUtility::readLine2( &file, &line );
            if ( x6 == 6 )
            {
                UT_EVAL_EXPR(!OFOS::strcmp( line, "#include <ofsys/ofsys.h>" ) );
                cout << line << endl;
            }
            delete [] line;
        }
    }
    UT_TEST_END;

    UT_TEST_START(7, "readLinesCSV");

    char path7[OF_MAX_PATH_LEN+1];
    OFOS::snprintf(path7, OF_MAX_PATH_LEN,
                   "%s/libs/ofsys/testdata.csv",
                   getenv("OFSOURCEROOT"));
    OFFile file7(path7);
    OFUtility::readLinesCSV(&file7, cb1, (void*)0);
    UT_TEST_END;

    UT_TEST_START(8, "convertCommaToCharStar");
    char string8[] = "one,two,three,four";
    OFCHARSTARLIST list8;
    OFUtility::convertCommaToCharStar(string8, &list8);
    OFCHARSTARLIST::iterator i8 = list8.begin();
    i8++; i8++;
    cout << *i8 << endl;
    UT_EVAL_EXPR(!OFOS::strcmp(*i8, "three"));
    OFUtility::destroyCharStarList(&list8);

    UT_TEST_END;

    UT_BATCH_END;
    return 0;

}

#endif // #if defined(UNIT_TEST)

