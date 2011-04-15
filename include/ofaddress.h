/*
 * $Revision: 2.0 $
 *
 * $Id: ofaddress.h,v 2.0 2006-01-01 20:08:32 timmy Exp $
 */

#if !defined(_OF_ADDRESS_H_)
#define _OF_ADDRESS_H_

#include <ofos.h>

/** \class OFAddress ofaddress.h
    Implements an IP address and port pair.
*/
class OFSYS_API OFAddress
{
 public:

    /** Empty constructor.
     */
    OFAddress( );
  
    /** Constructor sets only the port. The address is set to 0.
     */
    OFAddress( ofint16 port );
  
    /** Set both the port and the host name.
        If the host parameter is an ip address in dot format, inet_aton() is used to
        retrieve the correct address, or the ip address is resolved using gethostbyname.
    */
    OFAddress( ofint16 port, const char *host );
  
    /** Set both the port and ip address.
     */
    OFAddress( ofint16 port, ofint32 ipAddress );
  
    /** Copy constructor.
     */
    OFAddress( const OFAddress &copy );
  
    /** Destructor.
     */
    ~OFAddress( );

    /** Set the host name and port.
     */
    void set( ofuint16 port, const char *host );

    /** Set the host name and port.
     */
    void set( ofuint16 port, sockaddr_in *addr );

    /** Retrieve the host name. 
     */
    const char *hostname();

    /** Retrieve the address in dot notation.
        This is generated using inet_ntoa().
    */
    void ipaddress( char *buffer );

    /** Retrieve the port number.
        This is returned in host order.
    */
    ofuint16 port( ) const;
  
    /** Retrieve the sockaddr_in structure.
     */
    const sockaddr_in *address();
  
    /** Check that the address is valid.
     */
    const bool isValid() const;

 protected:
    sockaddr_in m_addr;
    bool m_valid;
};

#include <ofaddress.i>

#endif // #if !defined(_OF_ADDRESS_H_)
