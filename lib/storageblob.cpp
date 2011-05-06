/* $Revision: 2.0 $

   $Id: storageblob.cpp,v 2.0 2006-01-01 19:26:29 timmy Exp $
*/

#include <ofsys.h>
#include <storageblob.h>

void StorageBlob::writeDateTime(const OFDateTime *time)
{
     writeInt64(time->getDateTime());
}

void StorageBlob::readDateTime(OFDateTime *time)
{
    time->setDateTime(readInt64());
}
