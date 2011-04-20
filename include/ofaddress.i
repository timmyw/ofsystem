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

inline
OFAddress::OFAddress( ofint16 port, const char *host ) :
    m_valid( 0 )
{
    set( port, host );
}

inline
OFAddress::OFAddress( const OFAddress &copy )
{
    memcpy( &m_addr, &copy.m_addr, sizeof(m_addr) );
    m_valid = copy.m_valid;
}

inline
OFAddress::~OFAddress( )
{
}

inline
void
OFAddress::ipaddress( char *buffer )
{
    OFOS::strcpy( buffer, inet_ntoa( m_addr.sin_addr ) );
}

inline
ofuint16
OFAddress::port( ) const
{
    return ntohs( m_addr.sin_port );
}

inline
const sockaddr_in *
OFAddress::address()
{
    return &m_addr;
}

inline
const bool
OFAddress::isValid() const
{
    return m_valid;
}
