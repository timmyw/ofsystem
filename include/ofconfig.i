/*
*/

#include <ofos.h>

inline
bool
OFConfigurator::isDirty() const
{
     return m_dirty;
}

inline
bool
OFConfigurator::isOpen( ) const
{
     return m_opened;
}

inline
bool
OFConfigurator::isReadOnly( ) const
{
     return m_readOnly;
}

inline
const char *
OFConfigurator::dataFile( ) const
{
     return m_filenames[OFCONFIG_BASE_FILE];
}

inline
void OFConfigurator::changeFileName(const char* fileName)
{
     OFOS::strcpy(m_filenames[OFCONFIG_BASE_FILE], fileName);
}

