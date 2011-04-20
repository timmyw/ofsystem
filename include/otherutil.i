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

template<class T>
void
printbin( T x )
{
    T lastbit = (T)((T)1 << (T)(sizeof(T)*8-1));
    for ( char z = 0; z < sizeof(T)*8; z++ )
    {
        if ( x & lastbit )
            cout << "1";
        else
            cout << "0";
        x = x << 1;
    }
}

template<class T>
T
reversebits( T in )
{
    T ret = 0;
    T lastbit = (T)((T)1 << (T)(sizeof(T)*8-1));
    for ( int x = 0; x < sizeof(T)*8; x++ )
    {
        // Shift result (and everything else) down 1
        ret = ret >> 1;

        // Get the end bit and stick it in at the very top
        ret |= ( in & lastbit );

        // Pop off the top of the input
        in = in << 1;
    }
    return ret;
}
