
/* $Revision: 2.10 $

$Id: ofsocket.cpp,v 2.10 2007-09-27 12:59:57 timmy Exp $
*/

#include <ofsys/ofsys.h>
#include <ofsys/ofsocket.h>
#include <ofsys/ofutility.h>
#include <ofsys/oflogsvc.h>
#include <ofsys/ofplatform.h>

#if defined(OFOPSYS_WIN32)
typedef LINGER OFLINGER;
#else
typedef struct linger OFLINGER;
#if defined(OFOPSYS_LINUX)
#include <netinet/tcp.h>
#endif
#endif

OFSocket::OFSocket( ) :
    m_addr( 0 ),
    m_open( false )
{
    m_socket = socket( AF_INET, SOCK_STREAM, 0 );
    set_options_();
}
  
OFSocket::OFSocket( const OFAddress &remote ) :
    m_addr( remote ),
    m_open( false )
{
    m_socket = socket( AF_INET, SOCK_STREAM, 0 );
    set_options_();
}

OFSocket::OFSocket( OFOS::of_socket_t socketHandle ) :
    m_addr( 0 ),
    m_open( true )
{
    m_socket = socketHandle;
    set_options_();
}

OFSocket::~OFSocket( )
{
    closeSocket( );
}

ofint32
OFSocket::connect( const OFAddress &remote )
{
    m_addr = remote;
  
    if ( m_socket == -1 )
        return ERR_INVAL_SOCKET;
  
    sockaddr *addr = (sockaddr*)m_addr.address();
    ofint32 retv = ::connect( m_socket, addr, sizeof(sockaddr) );
    
    if ( retv == -1 )
        return OFUtility::translateLastSystemError( );
  
    m_open = true;

#if defined(OFOPSYS_WIN32)
    ofuint32 param = 1;
    if (ioctlsocket(m_socket, FIONBIO, &param) == SOCKET_ERROR)
#else
        ofuint32 param = fcntl(m_socket, F_GETFL);
    if (fcntl(m_socket, F_SETFL, param | O_NONBLOCK))
#endif
        LogService::debugLine("Error setting NBIO: %s",
                              retrieveMsg(OFUtility::translateLastSystemError()));
    return ERR_SUCCESS;
}

ofint32
OFSocket::listen( )
{
    ofint32 ret = 0;

    ofint32 temp = 1;
    ret = setsockopt( m_socket, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char *>(&temp), sizeof(temp) );
    if ( ret == -1 )
        return ret;
    
    ret = bind( m_socket, (struct sockaddr *)m_addr.address(), sizeof(*m_addr.address()) );
    if ( !ret )
        ret = ::listen( m_socket, 5 );
        
    return ret;
}

void
OFSocket::closeSocket( )
{
//     cout << "OFSocket::closeSocket" << endl;
    if ( m_open )
    {
        m_open = false;
        shutdown( m_socket, 2 );
#if defined(OFOPSYS_WIN32)
        closesocket( m_socket );
#else
        close( m_socket );
#endif
    }
    else
    {
#if defined(OFOPSYS_WIN32)
        if ( m_socket != OFOS::OF_INVALID_SOCKET )
        {
                shutdown( m_socket, 2 );
                closesocket( m_socket );
        }
#else
        if (m_socket != OFOS::OF_INVALID_SOCKET)
        {
                shutdown( m_socket, 2 );
                close( m_socket );
        }
#endif
    }
    m_socket = OFOS::OF_INVALID_SOCKET;
}

ofint32 
OFSocket::send( void *buffer,
		ofuint32 bufferLength,
		bool *terminate /* = NULL */,
		ofuint32 chunkSize /* = 2048 */,
		ofuint32 timeOut /* = 30 */ )
{
     ofint32 leftToSend = bufferLength;
     char *tempBuf = static_cast<char*>(buffer);
     ofint32 cursent = 0;

     time_t timenow;
     time_t timestart;
     time( &timestart );

     struct timeval tv;
     fd_set fdwrite;

     for(;;)
     {
	  ofint32 rc = 0;
	  FD_ZERO( &fdwrite );
	  FD_SET( m_socket, &fdwrite );
	  tv.tv_sec = 1;
	  tv.tv_usec = 0;
	  rc = select( m_socket+1, 0, &fdwrite, 0, &tv );
	  if ( terminate != 0 && *terminate ) return 1;
	  if ( rc > 0 && FD_ISSET( m_socket, &fdwrite ) )
	  {
	       time( &timestart );
	       ofint32 chunkToSend = (leftToSend < (ofint32)chunkSize)
		    ?leftToSend:chunkSize;
	       cursent = ::send( m_socket, tempBuf, chunkToSend, 0 );
	       if ( terminate != 0 && *terminate ) return 1;
	       if ( cursent < 0 ) return -1;
	       leftToSend -= cursent;
	       tempBuf += cursent;
	       if ( leftToSend <= 0 )
		    return 0;
	  }
	  time( &timenow );
	  if ( (ofuint32)timenow > timestart + timeOut ) break;
     }
     return 1;
}

ofint32
OFSocket::recv( void *buffer,
		ofuint32 bufferLength,
		ofuint32 &bytesReceived,
		bool *terminate /* = NULL */,
		ofuint32 chunkSize /* = 2048 */,
		ofuint32 timeOut /* = 30 */ )
{
    bytesReceived = 0;
    time_t timenow;
    time_t timestart;
    time( &timestart );

    struct timeval tv;
    fd_set fdread;

    for(;;)
    {
        ofint32 rc = 0;
        FD_ZERO( &fdread );
        FD_SET( m_socket, &fdread );
        tv.tv_sec = 1;
        tv.tv_usec = 0;
        rc = select( m_socket+1, &fdread, 0, 0, &tv );
        if ( terminate != 0 && *terminate ) return 1;
        if ( rc > 0 && FD_ISSET( m_socket, &fdread ) ) break;
        time( &timenow );
        if ( (ofuint32)timenow > timestart + timeOut ) return 1;
    }

    ofint32 chunkToRead = (bufferLength < (ofuint32)chunkSize)
        ?bufferLength:chunkSize;
    ofint32 curRecv = ::recv( m_socket, static_cast<char *>(buffer),
                              chunkToRead, 0 );
#if defined(VERBOSE_NETWORK)
    cout << "Socket:" << m_socket
         << " received " << curRecv
         << " bytes" << endl;
#endif
    if ( terminate != 0 && *terminate ) return 1;
    if (curRecv < 0)
    {
#if defined(VERBOSE_NETWORK)
        cout << "Socket:" << m_socket
             << " received -1" << endl;
        return -1;
#endif
    }
    if ( curRecv > 0 )
    {
        bytesReceived = curRecv;
        return 0;
    }
    return -1;
}

void 
OFSocket::startup( )
{
#if defined(OFOPSYS_WIN32)
    WSADATA data;
    WSAStartup( MAKEWORD(2,0), &data );
#endif
}

void 
OFSocket::closedown()
{
#if defined(OFOPSYS_WIN32)
    WSACleanup();
#endif
}

ofuint32 OFSocket::getnamefromaddr(char* peerName,
                                   ofuint32 peerNameLen,
                                   struct in_addr* addr,
                                   ofuint32 addrLen,
                                   char useDNS /*=0*/)
{
    const char* pn = "unknown";
    if (!useDNS)
    {
        pn = inet_ntoa(*addr);
    }
    else
    {
#if defined(OFOPSYS_WIN32)
        struct hostent* he = gethostbyaddr((char*)addr, addrLen, AF_INET);
#else
        struct hostent* he = gethostbyaddr(addr, addrLen, AF_INET);
#endif
        if (he && he->h_name)
            pn = he->h_name;
    }
    OFOS::strncpy2(peerName, pn, peerNameLen);
    return 0;
}

ofuint32 OFSocket::getpeername(char* peerName,
                               ofuint32 peerNameLen,
                               char useDNS /*= 0*/)
{
    struct sockaddr_in pa;
	OFOS::of_socklen_t sl = sizeof(pa);
    int r = ::getpeername(m_socket, (sockaddr*)&pa, &sl);
    if (r != -1)
    {
        OFSocket::getnamefromaddr(peerName, peerNameLen,
                                  &pa.sin_addr, sizeof(pa.sin_addr),
                                  useDNS);
    }
    return r==0?ERR_SUCCESS:OFUtility::translateSystemError(r);
}

/* static */
void
OFSocket::getHostName( char *host, ofuint32 length )
{
    RESETSTRING( host );
    char x[1024];
    gethostname( x, 1024 );
    struct hostent *he = gethostbyname( x );
    if ( he )
    {
        OFOS::strncpy2( host, he->h_name, length );
    }
}

/* static */
ofuint32
OFSocket::getLocalIP( )
{
    char name[1025];
    gethostname( name, 1024 );
    return OFPlatform::getHostIP(name);
}

void
OFSocket::set_options_()
{
#if defined(OFOPSYS_LINUX) || defined(OFOPSYS_WIN32)
    ofuint32 f = 1;
    setsockopt(m_socket, IPPROTO_TCP, TCP_NODELAY, (char *)&f, sizeof(f));
    setsockopt(m_socket, SOL_SOCKET, SO_KEEPALIVE, (char*)&f, sizeof(f));

    f = OF_SOCK_SEND_BUFSIZE;
    setsockopt(m_socket, SOL_SOCKET, SO_SNDBUF, (char*)&f, sizeof(f));
    f = OF_SOCK_RECV_BUFSIZE;
    setsockopt(m_socket, SOL_SOCKET, SO_RCVBUF, (char*)&f, sizeof(f));

    struct timeval tv;
    memset(&tv, 0, sizeof(tv));
    tv.tv_sec = 180;
    setsockopt(m_socket, SOL_SOCKET, SO_RCVTIMEO, (char*)&tv, sizeof(tv));
    setsockopt(m_socket, SOL_SOCKET, SO_SNDTIMEO, (char*)&tv, sizeof(tv));
#endif

}

#if defined(UNIT_TEST)

#include "UnitTest.h"

int
main( ofint32 argc, char *argv[] )
{

    UT_BATCH_START( "OFSocket class unit test" );
    
    UT_TEST_START( 1, "OFSocket::getLocalIP" );

    ofuint32 la = OFSocket::getLocalIP();
    printf("local address: %08lx", la);

    UT_BATCH_END;
    
    return 0;
    
}

#endif // #if defined(UNIT_TEST)
