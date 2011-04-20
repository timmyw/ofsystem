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

#if !defined(_OF_CONSTANTS_H_)
#define _OF_CONSTANTS_H_

/** \file ofconstants.h
    Contains the declaration of the OFConstant class.
*/

/** \class OFConstant ofconstants.h
    Provides commonly used constants.

    \nosubgrouping
    \ingroup ofsyscls
*/
class OFSYS_API OFConstant
{
 public:

    /** \name String length constants.
     */
    //@{

    /** The length of a carriage return and line feed ("\r\n")
     */
    static const ofuint32 lenCRLF;

    /** The length of a carriage return ("\n" );
     */
    static const ofuint32 lenCR;

    //@}
};

#endif // #if !defined(_OF_CONSTANTS_H_)
