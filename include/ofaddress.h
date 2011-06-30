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
