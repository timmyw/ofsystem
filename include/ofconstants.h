/*
* $OFPREPARED$
*
* $Revision: 2.0 $
*
* $Id: ofconstants.h,v 2.0 2006-01-01 20:08:32 timmy Exp $
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
