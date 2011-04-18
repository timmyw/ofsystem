/* $Revision: 2.2 $

$Id: ofsocket.h,v 2.2 2007-06-26 11:31:56 timmy Exp $
*/

#if !defined(_OF_SOCKET_H_)
#define _OF_SOCKET_H_

#if defined(OFOPSYS_WIN32)
typedef ofuint32 (__stdcall *OF_CLIENT_FUNC)(void *);
#else
typedef void * (*OF_CLIENT_FUNC)(void *);
#endif // #if defined(OFOPSYS_WIN32)

#include <ofaddress.h>

/**
   \class OFSocket ofsocket.h
   Implementation of a cross platform socket. This class
   wraps the differences between standard socket (bsd) and
   Winsock sockets.
*/
class OFSYS_API OFSocket
{
 public:
    /** Empty constructor.
     */
    OFSocket( );

    /** Constructs and initialises the address.
     */
    OFSocket( const OFAddress &remote );

    /** Constructs and initialises the socket handle
     */
    OFSocket( OFOS::of_socket_t socketHandle );

    /** Destructor.
     */
    virtual ~OFSocket( );
  
    /** Connect to the specified address.
     */
    virtual ofint32 connect( const OFAddress &remote );

    /** Connect to the address the socket was constructed with.
     */
    virtual ofint32 connect( );
  
    /** Listen on the supplied port and address.
     */
    virtual ofint32 listen( );
  
    /** Close the connection.
     */
    virtual void closeSocket( );
  
    /** Is the socket open
     */
    bool isConnected() const;
  
    /** Is the socket valid.
        Checks the created socket against OFOS::INVALID_SOCKET.
    */
    bool isValid( ) const;

    /** Send the supplied buffer.
    */
    virtual ofint32 send( void *buffer, ofuint32 bufferLength,
			  bool *terminate = NULL,
			  ofuint32 chunkSize = 2048, 
			  ofuint32 timeOut = 30 );
  
    /** Receive from remote address.
     */
    virtual ofint32 recv( void *buffer, ofuint32 bufferLength,
			  ofuint32 &bytesReceived,
			  bool *terminate = NULL,
			  ofuint32 chunkSize = 2048,
			  ofuint32 timeOut = 30 );
    /** Retrieve the peer's name

        Retrieves, if possible, the ip address of the remote peer.
     */
    virtual ofuint32 getpeername(char* peerName,
                                 ofuint32 peerNameLen,
                                 char useDNS = 0);

    /** Bind, listen and accept incoming requests.
        This method will create a new OFThread for each accepted
        incoming request, using the supplied function as the thread's
        starting point.
    
        Returns -1 if any part of the process fails.
    */
    ofint32 actAsServer( OF_CLIENT_FUNC clientFunc, ofuint32 timeOut );
    

    static void startup( );

    static void closedown();

    /** Access the underlying handle.
     */
    OFOS::of_socket_t handle( ) const;

    /** Retrieve the ip address of the local machine.
        This is returned in network byte order.
     */
    static ofuint32 getLocalIP( );

    /** Retrieve the local hostname
     */
    static void getHostName( char *host, ofuint32 length );

    /** Retrieve a name based on an address
     */
    static ofuint32 getnamefromaddr(char* peerName,
                                    ofuint32 peerNameLen,
                                    struct in_addr* addr,
                                    ofuint32 addrLen,
                                    char useDNS =0);

 protected:
    /** \name Internal helper methods
     */
    //@{

    /** Set the socket options
     */
    void set_options_();

    //@}

    OFAddress m_addr;
    bool m_open;
    OFOS::of_socket_t m_socket;
};

#include <ofsocket.i>

#endif // #if !defined(_OF_SOCKET_H_)

