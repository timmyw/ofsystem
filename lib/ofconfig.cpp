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

#include <ofsys.h>
#include <ofplatform.h>
#include <ofconfig.h>
#include <ofconfig_int.h>
#include <ofutility.h>
#include <ofos.h>
#include <ofplatform.h>
#include <oflogservice.h>

const char *OFConfigurator::rootGroup = "root";

#if defined(OFOPSYS_WIN32)
#pragma warning(disable:4800)
#endif

OFConfigurator::OFConfigurator( ) :
    m_stack_level(0),
    m_opened( false ),
    m_dirty( false ),
    m_readOnly( false ),
    m_auto_path(0)
{
	m_datafile[0] = '\0';
    for (ofuint32 c = 0; c<OFCONFIG_MAX_INCLUDE; c++)
	{
        m_files[c] = 0;
		m_filenames[c][0] = '\0';
	}
}

OFConfigurator::OFConfigurator( const char* fileName, bool readOnly /*= true*/ , bool autoPath /*= true*/ ): 
    m_stack_level(0),
    m_opened( false ),
    m_dirty( false ),
    m_readOnly( false ),
    m_auto_path(autoPath)
{
	m_datafile[0] = '\0';
    for (ofuint32 c = 0; c<OFCONFIG_MAX_INCLUDE; c++)
	{
        m_files[c] = 0;
		m_filenames[c][0] = '\0';
	}
    open(fileName, readOnly, autoPath);
}

OFConfigurator::~OFConfigurator( )
{
    close_();
}

bool
OFConfigurator::isWellFormed(const char* fileName)
{
    OFFile f(fileName, 0664 ,OFFILE_READONLY | OFFILE_OPEN_EXISTING);
    if (!f.isOpen())
        return false; // can't find file

    char line[1024];
    while ( OFUtility::readLine( &f, line ) )
    {
        OFUtility::leftTrimString( line );
        OFUtility::rightTrimString( line );
    
        int length = OFOS::strlen( line );
        if ( length > 0 )
        {
            if (line[0] == '#' || line[0] == ';')
                ; // comment
            else if (line[0] == '[' && line[length-1] == ']')
                ; // group
            else if (OFOS::strchr(line,'='))
                ; // term (contains single =, not in column 0)
            else
                return false; // don't recognise this line as a valid one
        }
    }
    return true; // seems ok
}

bool
OFConfigurator::open( const char* fileName, bool readOnly /*= true*/ , bool autoPath /*= true*/)
{
	// #4797 convert delimiter to appropriate ones for platform
	char fixedFileName[OF_MAX_PATH_LEN+1];
    OFOS::strncpy2(fixedFileName, fileName, OF_MAX_PATH_LEN);
	OFPlatform::fixPath( fixedFileName );

    if (isOpen())
        close_();

    m_stack_level = 0;
    m_files[++m_stack_level] = new OFFile;
    open_file_(fixedFileName, readOnly, autoPath, *m_files[m_stack_level], m_filenames[m_stack_level]);
    OFOS::strncpy2(m_datafile, m_filenames[m_stack_level], OF_MAX_PATH_LEN);
    m_opened = m_files[m_stack_level]->isOpen();
    
    group_iter i = m_groups.begin();
    while ( i != m_groups.end() )
    {
        OFConfigGroup *grp = *i;
        delete grp;
        i++;
    }
    m_groups.clear();

    // Only if open was successful
    if (m_opened)
        readFile_( );
    m_dirty = false;
        
    return m_opened;
}

void
OFConfigurator::close_( )
{
    flush( );
    for ( vector<OFConfigGroup*>::iterator i = m_groups.begin(); i != m_groups.end(); i++ )
        delete (*i);
    m_groups.clear( );
    /*
    if (m_opened)
        m_files[m_stack_level]->close();
    */
    for (ofuint32 c = 0; c < OFCONFIG_MAX_INCLUDE; c++)
        delete m_files[c];
}

ofuint32
OFConfigurator::open_file_(const char* path, bool read_only, bool auto_path, OFFile& file, char* real_path)
{
    char full_path[OF_MAX_PATH_LEN+1];
    if (auto_path)
    {
        char config_path[OF_MAX_PATH_LEN+1];
        OFPlatform::configPath(config_path);
        OFOS::snprintf(full_path, OF_MAX_PATH_LEN, "%s/%s", config_path, path);
        if (!OFOS::strstr(full_path, ".conf"))
            OFOS::strcat(full_path, ".conf");
    }
    else
        OFOS::strncpy2(full_path, path, OF_MAX_PATH_LEN);

    // Deal with a relative path from the include, with an absolute
    // path that was passed in to the constructor
    if (!auto_path && !OFOS::strchr(path, OFPlatform::PathSeparator))
    {
        char dir[OF_MAX_PATH_LEN+1];
        OFPlatform::retrieveDirectory(m_datafile, dir,OF_MAX_PATH_LEN);
        OFOS::snprintf(full_path, OF_MAX_PATH_LEN, "%s/%s", dir, path);
    }

    OFPlatform::fixPath(full_path);

    ofuint32 ret = file.open(full_path, 0664,
                             OFFILE_SHAREALL|OFFILE_OPEN_ALWAYS|
                             (read_only?OFFILE_READONLY:OFFILE_READWRITE));
    if (real_path)
        OFOS::strncpy2(real_path, full_path, OF_MAX_PATH_LEN);
    return ret;
}

void
OFConfigurator::readFile_()
{
    // Assumes that there is already a current file opened,
    // ready for reading.
    OFConfigGroup *group= new OFConfigGroup(rootGroup, m_stack_level);
    m_groups.push_back( group );
  
    char* line;
    while (m_stack_level)
    {
        while (OFUtility::readLine2(m_files[m_stack_level], &line))
        {
            OFUtility::leftTrimString(line);
            OFUtility::rightTrimString(line);
    
            // Only if there is any thing left
            if (OFOS::strlen(line))
            {
                if (line[0] == '[')
                {
                    char szName[1025];
                    OFOS::strncpy2( szName, line+1, 1024 );
                    if ( szName[ OFOS::strlen(szName)-1] == ']' )
                        szName[OFOS::strlen(szName)-1] = 0;
                    if (!(group = retrieveGroup(szName)))
                    {
                        group = new OFConfigGroup(szName, m_stack_level);
                        m_groups.push_back(group);
                    }
                }
                else
                    if (line[0] != '#' && line[0] != ';')
                    {
                        assert(group);

                        // Check if this is an include
                        if (line[0] == '$' && OFOS::strlen(line) > 9 &&
                            !OFOS::strnicmp(line, "$include", 8))
                        {
                            if (m_stack_level < OFCONFIG_MAX_INCLUDE)
                            {
                                m_stack_level++;
                                m_files[m_stack_level] = new OFFile;
                                char *file_name = line+8;
                                while (*file_name && (*file_name == ' ' || *file_name == '\t'))
                                    file_name++;
                                if (open_file_(file_name, m_readOnly, (bool)m_auto_path, *m_files[m_stack_level]))
                                {
                                    // We failed to open the include file.
                                    delete m_files[m_stack_level];
                                    m_files[m_stack_level] = 0;
                                    m_stack_level--;
                                }
                                else
                                {
                                    OFConfigInclude* include = new OFConfigInclude(file_name);
                                    group->addInclude(include);
                                    OFOS::strncpy2(m_filenames[m_stack_level], file_name, OF_MAX_PATH_LEN);
                                }

                            }
                        }
                        else
                            // Add a new value
                            group->addValue(line, !OFOS::stricmp(m_filenames[m_stack_level], m_datafile));
                    }
                if ((line[0] == '#' || line[0] == ';') && m_stack_level == OFCONFIG_BASE_FILE)
                {
                    // Add a comment, to either the last item, or group if it
                    // Doesn't exist
                    if (group->getItemCount())
                        group->getLastItem()->addComment(line);
                    else
                        group->addComment(line);
        
                }
            }
            else
            {
                group->addBlank( );
            }
            delete [] line;
        }

        // If we are in an included file, we need to jump up a level
        if (m_stack_level)
        {
            delete m_files[m_stack_level];
            m_files[m_stack_level] = 0;
            m_stack_level--;
        }
    }


    if (!retrieveGroup( rootGroup ))
    {
        group = new OFConfigGroup(rootGroup, m_stack_level);
        m_groups.push_back( group );
    }

    m_dirty = false;
}

ofuint32
OFConfigurator::flatten(OFFile& file)
{
    if (!file.isOpen())
        return ERR_INVAL_FILE_HANDLE;
    for (group_iter i = m_groups.begin(); i != m_groups.end(); i++)
    {
        OFConfigGroup *group = *i;
        group->dump(file, 1);
    }
    return ERR_SUCCESS;
}


void
OFConfigurator::flush( )
{
    if ( m_dirty && !m_readOnly )
    {
        // Create a new file, then delete old, and rename new
        //    assert( !m_readOnly );
    
        if (m_files[OFCONFIG_BASE_FILE])
        {
            m_files[OFCONFIG_BASE_FILE]->close( );
            OFPlatform::deleteFile( m_filenames[OFCONFIG_BASE_FILE] );
        }
        m_files[OFCONFIG_BASE_FILE] = new OFFile;
        m_files[OFCONFIG_BASE_FILE]->open(m_datafile, 0664, OFFILE_CREATE_ALWAYS | OFFILE_READWRITE);

        for (group_iter i = m_groups.begin(); i != m_groups.end(); i++)
        {
            OFConfigGroup *group = *i;
            if (group->level() == OFCONFIG_BASE_FILE)
                group->dump( *m_files[OFCONFIG_BASE_FILE] );
        }
    }
    m_dirty = false;
}

void OFConfigurator::clear()
{
    vector<OFConfigGroup*>::iterator i = m_groups.begin();
    while ( i != m_groups.end() )
    {
        OFConfigGroup *group = *i;
        delete group;
        i++;
    }
    m_groups.clear( );

    m_dirty = true;
}

OFConfigGroup *
OFConfigurator::retrieveGroup( const char *groupName )
{
    OFConfigGroup *group = NULL;

    group_iter i = m_groups.begin( );
    while ( i != m_groups.end() && !group )
    {
        OFConfigGroup *tgrp = *i;
        if ( !OFOS::stricmp( groupName, tgrp->name() ) )
            group = tgrp;
        i++;
    }
    return group;
}

bool
OFConfigurator::readYesNo(const char* groupName, const char* itemName, bool boolDefault)
{
    char buf[129];
    readValue(groupName, itemName, buf, "null");
    if (!OFOS::stricmp(buf, "null"))
        return boolDefault;
    if (!OFOS::stricmp(buf, "yes"))
        return true;
    if (!OFOS::stricmp(buf, "no"))
        return false;
    if (!OFOS::stricmp(buf, "true"))
        return true;
    if (!OFOS::stricmp(buf, "false"))
        return false;
    if (OFOS::atol(buf))
        return true;
    return false;
}

const char *
OFConfigurator::readValue( const char *groupName, const char * itemName, const char * defaultValue )
{
    const char *val = defaultValue;
    OFConfigGroup *group = retrieveGroup( groupName );
    if ( group )
    {
        OFConfigItem *item = group->retrieveItem( itemName );
        if ( item )
            val = item->value( );
    }
    return val;
}

bool
OFConfigurator::readValue( const char *groupName, const char * itemName, char * value, const char *defaultValue, bool expandEnvVars /* = true */ )
{
    bool found = false;
    OFConfigGroup *group = retrieveGroup( groupName );
    if ( group )
    {
        OFConfigItem *item = group->retrieveItem( itemName );
        if ( item )
        {
            item->value( value );
            found = true;
        }
    }
    if ( !found )
        OFOS::strcpy( value, defaultValue );
    if ( expandEnvVars && OFOS::strstr( value, "$(" ) )
    {
        char temp[4096];
        OFUtility::expandEnvVars( value, temp );
        OFOS::strcpy( value, temp );
    }
    return found;
}

bool
OFConfigurator::readValue( const char *groupName, const char * itemName, ofuint32 * pdwValue, ofuint32 dwDefault )
{
    char defaultValue[256];
    char value[256];
    OFOS::snprintf( defaultValue, 19, "%lu", (ofuint32)dwDefault );
    bool found = readValue( groupName, itemName, value, defaultValue );
    *pdwValue = OFUtility::strToULong( value );
    return found;
}

bool
OFConfigurator::readValue( const char *groupName, const char * itemName, double *dblValue, double dblDefault )
{
    char value[129];
    char defaultValue[20];
    OFOS::snprintf( defaultValue, 128, "%.7g", dblDefault );

    bool found = readValue( groupName, itemName, value, defaultValue );
    *dblValue = atof( value );
    return found;
}

bool
OFConfigurator::writeValue( const char *groupName, const char *itemName, const char *value )
{
	assert( groupName && itemName && value );

    if ( m_readOnly )
        return false;
  
    OFConfigGroup *group = retrieveGroup( groupName );
    if ( !group )
    {
        group = new OFConfigGroup(groupName, OFCONFIG_BASE_FILE);
        m_groups.push_back( group );
    }
    else
        group->level(OFCONFIG_BASE_FILE);
    OFConfigItem *item = group->retrieveItem( itemName );
    if ( !item )
    {
        item = new OFConfigItem( itemName, value, 1 );
        group->addValue( item );
    }
    else
    {
        item->setValue( value );
    }
    m_dirty = true;
    return true;
}

bool
OFConfigurator::writeValue( const char *groupName, const char *itemName, ofuint32 dwValue )
{
        char value[21];
        OFOS::snprintf (value, 20, "%lu", (ofpointer)dwValue);
        return writeValue( groupName, itemName, value );
}

bool
OFConfigurator::writeValue( const char *groupName, const char *itemName, double dblValue )
{
        char value[21];
        OFOS::snprintf( value, 20, "%.7g", dblValue );
        return writeValue( groupName, itemName, value );
}

bool 
OFConfigurator::writeCommentForGroup( const char *groupName, const char *comment)
{
        if ( m_readOnly )
                return false;
  
        OFConfigGroup *group = retrieveGroup( groupName );
        if ( !group )
                return false;

        char line[1025];
        OFOS::snprintf( line, 1024, "#%s", comment );
        group->addComment(line);
        m_dirty = true;

        return true;
}

bool 
OFConfigurator::writeCommentForItem( const char *groupName, const char *itemName, const char *comment)
{
    OFConfigGroup *group = retrieveGroup( groupName );
    if ( !group )
        return false;
    OFConfigItem *item = group->retrieveItem( itemName );
    if ( !item )
        return false;
    char line[1024];
    sprintf( line, "#%s", comment );
    item->addComment(line);
    m_dirty = true;

    return true;
}

void 
OFConfigurator::getItemsInGroup( const char *groupName, OFCHARSTARLIST *list )
{
    OFConfigGroup *grp = retrieveGroup( groupName );
    list->clear( );
    if ( grp )
        grp->getItems( list );
}

void
OFConfigurator::getNameAndValuesInGroup(const char* groupName,
                                        OFCHARSTARLIST* names,
                                        OFCHARSTARLIST* values)
{
    OFConfigGroup *grp = retrieveGroup(groupName);
    names->clear();
    values->clear();
    if (grp)
        grp->getNameAndValues(names, values);
}

void 
OFConfigurator::getValuesInGroup( const char *groupName, OFCHARSTARLIST *list )
{
    OFConfigGroup *grp = retrieveGroup( groupName );
    list->clear( );
    if ( grp )
        grp->getValues( list );
}

bool 
OFConfigurator::hasGroup( const char *groupName )
{
    return retrieveGroup( groupName ) != NULL;
}

void
OFConfigurator::getActualValuesInGroup( const char *groupName, OFCHARSTARLIST *list )
{
    OFConfigGroup *grp = retrieveGroup( groupName );
    list->clear( );
    if ( grp )
        grp->getActualValues( list );
}

bool 
OFConfigurator::deleteItem( const char *groupName, const char * itemName )
{
    if ( m_readOnly )
        return false;

    OFConfigGroup * group = retrieveGroup( groupName );
    if ( !group )
        return false;
    
    if ( !group->deleteItem( itemName ) )
        return false;
    
    m_dirty = true;

    return true;
}

// //////////////////////////////////////////////
// OFConfigGroup

OFConfigGroup::~OFConfigGroup( )
{
    item_iter i = m_items.begin();
    while ( i != m_items.end() )
    {
        OFConfigItem *item = *i;
        delete item;
        i++;
    }

    comment_iter i2 = m_comments.begin( );
    while ( i2 != m_comments.end() )
    {
        OFConfigComment *comm = *i2;
        delete comm;
        i2++;
    }

    for ( vector<OFConfigBlank*>::iterator blank_i = m_blanks.begin(); blank_i != m_blanks.end(); blank_i++ )
        delete (*blank_i);
}

void
OFConfigGroup::addValue( const char *line, char top_level)
{
    // Take from NAME=VALUE
    if (OFOS::strchr(line, '='))
    {
        OFConfigItem *item = new OFConfigItem(line, top_level);
        m_items.push_back(item);
    }
}

OFConfigItem *
OFConfigGroup::retrieveItem( const char *theName )
{
    OFConfigItem *item = NULL;
    item_iter i = m_items.begin();

    while ( i != m_items.end() && !item )
    {
        OFConfigItem *titem = *i;
        if ( !OFOS::stricmp( theName, titem->name() ) )
            item = titem;
        i++;
    }
    return item;
}

bool 
OFConfigGroup::deleteItem( const char *name )
{   
    item_iter it = m_items.begin();
    while ( it != m_items.end() )
    {
        OFConfigItem *item = *it;
        if ( !OFOS::stricmp( name, item->name() ) )
        {
            m_items.erase( it );
            return true;
        }
        it++;
    }
    
    return false;
}


void
OFConfigGroup::dump(OFFile &file, char force_all /*= 0*/)
{
    char name[201];
  
    if (OFOS::stricmp(m_name, "root"))
    {
        OFOS::snprintf(name, 200, "[%s]%s", m_name, OFPlatform::CRLF);
        if (file.write(name, OFOS::strlen(name)) <= 0)
            OFLogService::errorLine("Error writing group name");
    }
 
    comment_iter i1 = m_comments.begin();
    while ( i1 != m_comments.end() )
    {
        OFConfigComment *comm = *i1;
        comm->dump( file );
        i1++;
    }

    for (item_iter i2 = m_items.begin( ); i2 != m_items.end(); i2++)
    {
        if ((*i2)->top_level() || force_all)
            (*i2)->dump(file);
    }
    OFOS::snprintf( name, 200, OFPlatform::CRLF );
    file.write( name, OFOS::strlen(name) );
    for ( vector<OFConfigBlank*>::iterator i3 = m_blanks.begin(); i3 != m_blanks.end(); i3++ )
    {
        (*i3)->dump( file );
    }

    for (vector<OFConfigInclude*>::iterator inc_i = m_includes.begin();
         inc_i != m_includes.end();
         inc_i++)
    {
        (*inc_i)->dump(file);
    }
}

OFConfigItem *
OFConfigGroup::getLastItem( )
{
    OFConfigItem *item = NULL;
    if ( m_items.size( ) )
        item = m_items[( m_items.size()-1 )];
    return item;
}

void
OFConfigGroup::addComment( const char *line )
{
    OFConfigComment *comment = new OFConfigComment( line );
    m_comments.push_back( comment );
}

void
OFConfigGroup::addBlank( )
{
    OFConfigBlank *blank = new OFConfigBlank;
    m_blanks.push_back( blank );
}

void
OFConfigGroup::addInclude(OFConfigInclude* include)
{
    m_includes.push_back(include);
}

void 
OFConfigGroup::getItems( OFCHARSTARLIST *list )
{
    item_iter i = m_items.begin( );
    while ( i != m_items.end() )
    {
        OFConfigItem *item = *i;
        
        ofuint32 len = OFOS::strlen( item->name() ) + item->itemSize() + 2;
        char *line = new char[len];
        char *val = new char[item->itemSize()+1];
        item->value( val );
        sprintf( line, "%s=%s", item->name(), val );
        list->push_back( line );
        delete [] val;
        i++;
    }
}

void
OFConfigGroup::getNameAndValues(OFCHARSTARLIST* names,
                                OFCHARSTARLIST* values)
{
    
    for (item_iter i = m_items.begin( ); i != m_items.end(); i++)
    {
        OFConfigItem *item = *i;
        
        ofuint32 name_len = OFOS::strlen(item->name());
        ofuint32 value_len = OFOS::strlen(item->value());
        char* s_name = new char[name_len+1];
        char* s_value = new char[value_len+1];
        OFOS::strncpy2(s_name, item->name(), name_len);
        OFOS::strncpy2(s_value, item->value(), value_len);
        names->push_back(s_name);
        values->push_back(s_value);
    }
}

void 
OFConfigGroup::getValues( OFCHARSTARLIST *list )
{
    item_iter i = m_items.begin( );

    while ( i != m_items.end() )
    {
        OFConfigItem *item = *i;
        char *line = new char[OFOS::strlen( item->name() ) +1 ];
        OFOS::strcpy( line, item->name() );
        list->push_back( line );
        i++;
    }
}

void
OFConfigGroup::getActualValues( OFCHARSTARLIST *list )
{
    for ( item_iter i = m_items.begin(); i != m_items.end(); i++ )
    {
        char *line = new char[OFOS::strlen( (*i)->value() ) + 1];
        OFOS::strcpy( line, (*i)->value() );
        list->push_back( line );
    }
}

// ////////////////////////////////////////////
// OFConfigItem

OFConfigItem::~OFConfigItem( )
{
    delete [] m_value;
    comment_iter i = m_comments.begin( );
    while ( i != m_comments.end() )
    {
        delete *i;
        i++;
    }
    m_comments.clear( );
}

OFConfigItem::OFConfigItem(const char * line, char top_level):
    m_value(NULL)
{
    // Find the separator
    const char *sepPos = OFOS::strchr( line, '=' );
    assert( sepPos );
  
    // Retrieve the first part, the name
    char * temp = new char[OFOS::strlen( line ) +1 ];
    OFOS::strcpy( temp, line );
    char* eqpos = (char*)OFOS::strchr(temp, '=');
    *eqpos = 0;

    OFOS::strcpy( m_name, temp );
    OFUtility::rightTrimString( m_name );
    OFUtility::leftTrimString( m_name );
  
    // Retrieve the value, anything after the separator.
    m_value = new char[OFOS::strlen(sepPos) +1];
    OFOS::strcpy( m_value, sepPos+1 );
    OFUtility::rightTrimString( m_value );
    OFUtility::leftTrimString( m_value );

    m_top_level = top_level;

    delete [] temp;
}

OFConfigItem::OFConfigItem( const char * itemName, const char * theValue, char top_level)
{
    OFOS::strcpy( m_name, itemName );
    m_value = new char[OFOS::strlen( theValue ) +1 ];
    OFOS::strcpy( m_value, theValue );
    m_top_level = top_level;
}

void
OFConfigItem::dump( OFFile &file )
{
        char line[1025];
        OFOS::snprintf( line, 1024, "%s=", m_name );
        file.write( line, OFOS::strlen( line ) );
        file.write( m_value, OFOS::strlen(m_value) );
        file.write( OFPlatform::CRLF, OFOS::strlen(OFPlatform::CRLF) );

        comment_iter i = m_comments.begin( );
        while ( i != m_comments.end() )
        {
                OFConfigComment *comm = *i;
                comm->dump( file );
                i++;
        }
        for ( vector<OFConfigBlank*>::iterator i3 = m_blanks.begin(); i3 != m_blanks.end(); i3++ )
        {
                (*i3)->dump( file );
        }
}

void
OFConfigItem::setValue( const char *theValue )
{
    if ( m_value )
        delete [] m_value;

    m_value = new char[OFOS::strlen( theValue ) +1 ];
    OFOS::strcpy( m_value, theValue );
}

void
OFConfigItem::addComment( const char * line )
{
    OFConfigComment *comm = new OFConfigComment( line );
    m_comments.push_back( comm );
}

void
OFConfigItem::addBlank( )
{
    OFConfigBlank *blank = new OFConfigBlank;
    m_blanks.push_back( blank );
}

void
OFConfigBlank::dump( OFFile &file )
{
//     char line[1024];
//     sprintf( line, "\n" );
//     file.write( line, OFOS::strlen( line ) );
}

#if defined(UNIT_TEST)

#include "UnitTest.h"

int main(int argc, char* argv[])
{
    UT_BATCH_START("OFConfigurator");

    UT_TEST_START(1, "OFConfigurator::OFConfigurator()");
    char path[OF_MAX_PATH_LEN+1];
    OFOS::snprintf(path, OF_MAX_PATH_LEN, "%s/libs/ofsys/test.conf", getenv("OFSOURCEROOT"));
    OFConfigurator config(path, false, false);
    UT_EVAL_EXPR(config.isOpen());
    UT_TEST_END;

    UT_TEST_START(2, "OFConfigurator::dataFile()");
    UT_EVAL_EXPR(OFOS::strstr(config.dataFile(), "/libs/ofsys/test.conf"));
    UT_TEST_END;

    UT_TEST_START(3, "OFConfigurator::getAndValuesInGroup()");
    OFCHARSTARLIST names;
    OFCHARSTARLIST values;
    config.getNameAndValuesInGroup("main", &names, &values);
    UT_EVAL_EXPR(names.size()==4);
    OFCHARSTARLIST::iterator name_i, value_i;
    ofuint32 j = 0;
    for (name_i = names.begin(), value_i = values.begin(); name_i != names.end(); name_i++, value_i++)
    {
        char* s_name = *name_i;
        char* s_value = *value_i;
        switch (j++)
        {
        case 0:
            {
                UT_EVAL_EXPR(!OFOS::stricmp(s_name, "item1") && !OFOS::stricmp(s_value, "value"));
            }
            break;
        case 1:
            {
                UT_EVAL_EXPR(!OFOS::stricmp(s_name, "item2") && !OFOS::stricmp(s_value, "23.5"));
            }
            break;
        case 2:
            {
                UT_EVAL_EXPR(!OFOS::stricmp(s_name, "item4") && !OFOS::stricmp(s_value, "fig"));
            }
            break;
        case 3:
            {
                UT_EVAL_EXPR(!OFOS::stricmp(s_name, "item3") && !OFOS::stricmp(s_value, "banana"));
            }
            break;
        }
    }
    UT_TEST_END;

    for_each(names.begin(), names.end(), delete_elem_array<char>);
    for_each(values.begin(), values.end(), delete_elem_array<char>);

    UT_BATCH_END;
    return 0;
}

#endif // UNIT_TEST

#if defined(UNIT_TESTX)

#include "UnitTest.h"
#include <fstream>

ofint32  main(ofint32 argc, char *argv[] )
{
    // -- time stamp

    UNIT_TEST_START("OFConfigurator");

    // delete existing files
    string f1;
    {
        OFConfigurator dummy1("c1");
        f1 = dummy1.dataFile();
    } // file closes
    ::remove(f1.c_str());
    
    string f2;
    {
        OFConfigurator dummy2("c2");
        f2 = dummy2.dataFile();
    } // file closes
    ::remove(f2.c_str());
    
#ifdef _WIN32
    COMMENT("check file path (auto and non-auto");

    EXPECT_VALUE(stricmp(f1.c_str(),"c:\\winnt\\c1.conf") == 0);

    string f3;
    {
        OFConfigurator dummy3("c:\\c3",false,false);
        f3 = dummy3.dataFile();
    } // file closes
    ::remove(f3.c_str());

    string f4;
    {
        OFConfigurator dummy4("c4",false,false);
        f4 = dummy4.dataFile();
    } // file closes
    ::remove(f4.c_str());


    EXPECT_VALUE(stricmp(f3.c_str(),"c:\\c3") == 0);
#endif

    try
    {

        // -- create file and access while open
            
        {
            COMMENT("default ctor");

            OFConfigurator c1;
            
            COMMENT("open");
            EXPECT_VALUE(c1.isOpen() == false);

            EXPECT_VALUE(c1.isDirty() == false);

            // NB! This will fail if user doesn't have write access to where
            // the is being written
            EXPECT_VALUE(c1.open("c1",false) == true);
            std::cout << "file = " << c1.dataFile() << std::endl;

            EXPECT_VALUE(c1.isOpen() == true);

            EXPECT_VALUE(c1.isDirty() == false);

            EXPECT_VALUE(c1.isReadOnly() == false);

            // -- write values

            COMMENT("test config file is well formed");
            EXPECT_VALUE(OFConfigurator::isWellFormed(c1.dataFile()) == true);

            COMMENT("write string");
            EXPECT_VALUE(c1.writeValue("group1","item1","xxx") == true);

            COMMENT("overwrite string");
            EXPECT_VALUE(c1.writeValue("group1","item1","value1") == true);
                        
            COMMENT("write ofuint32");
            EXPECT_VALUE(c1.writeValue("group2","item1",ofuint32(123)) == true);

            COMMENT("write double");
            EXPECT_VALUE(c1.writeValue("group1","item2",double(4.567)) == true);

            COMMENT("write double into root");
            EXPECT_VALUE(c1.writeValue("root","double",double(-1234.567)) == true);

            COMMENT("getValuesInGroup");
            vector<string> valuesInGroup1;
            c1.getValuesInGroup("group1",&valuesInGroup1);
            EXPECT_VALUE(valuesInGroup1.size() == 2);
            EXPECT_VALUE(valuesInGroup1[0] == string("item1"));
            EXPECT_VALUE(valuesInGroup1[1] == string("item2"));
            vector<string> valuesInGroup2;
            c1.getValuesInGroup("group2",&valuesInGroup2);
            EXPECT_VALUE(valuesInGroup2.size() == 1);
            vector<string> valuesInGroup3;
            c1.getValuesInGroup("group3",&valuesInGroup3);
            EXPECT_VALUE(valuesInGroup3.size() == 0);
            vector<string> itemsInRoot;
            c1.getValuesInGroup("root",&itemsInRoot);
            EXPECT_VALUE(itemsInRoot.size() == 1);

            COMMENT("write comments");
            EXPECT_VALUE(c1.writeCommentForGroup("group1","group1 comment") == true);
            EXPECT_VALUE(c1.writeCommentForGroup("group1","# another group1 comment") == true);
            EXPECT_VALUE(c1.writeCommentForGroup("group2","group2 comment") == true);
            EXPECT_VALUE(c1.writeCommentForGroup("root","root comment") == true);
            EXPECT_VALUE(c1.writeCommentForGroup("group_unknown","group_unknown comment") == false);

            EXPECT_VALUE(c1.writeCommentForItem("group1","item1","item1 comment") == true);
            EXPECT_VALUE(c1.writeCommentForItem("group1","item1","# another group1 comment") == true);
            EXPECT_VALUE(c1.writeCommentForItem("group2","item1","group2 comment") == true);
            EXPECT_VALUE(c1.writeCommentForItem("group_unknown","item1","group_unknown comment") == false);
            EXPECT_VALUE(c1.writeCommentForItem("group1","item_unknown","item_unknown comment") == false);

            EXPECT_VALUE(c1.isDirty() == true);

            COMMENT("test config file is well formed");
            EXPECT_VALUE(OFConfigurator::isWellFormed(c1.dataFile()) == true);

            // -- read stored

            COMMENT("read string");
            string resString;
            EXPECT_VALUE(c1.readValue("group1","item1",resString,"default") == true);
            EXPECT_VALUE(resString == "value1");

            COMMENT("read string again");
            EXPECT_VALUE(c1.readValue("group1","item1",resString,"default") == true);
            EXPECT_VALUE(resString == "value1");

            COMMENT("read ofuint32");
            ofuint32 resWord;
            EXPECT_VALUE(c1.readValue("group2","item1",&resWord,999) == true);
            EXPECT_VALUE(resWord == 123);

            COMMENT("read double");
            double resDouble;
            EXPECT_VALUE(c1.readValue("group1","item2",&resDouble,-999.999) == true);
            EXPECT_VALUE(resDouble == 4.567);

            COMMENT("read double from root");
            EXPECT_VALUE(c1.readValue("root","double",&resDouble,-999.999) == true);
            EXPECT_VALUE(resDouble == -1234.567);

            // -- read as wrong type - !! causes a crash

            /*COMMENT("read string as float");
              double resDouble;
              EXPECT_VALUE(c1.readValue("group1","item1",&resDouble,-FLT_MAX) == false);
              EXPECT_VALUE(resDouble == -FLT_MAX;*/

            // -- read not-stored

            EXPECT_VALUE(c1.readValue("unknown_group","item1",resString,"default") == false);
            EXPECT_VALUE(resString == "default");

            EXPECT_VALUE(c1.readValue("group1","unknown_item",resString,"default") == false);
            EXPECT_VALUE(resString == "default");
        }

        // -- re-read file

        COMMENT("re-read");

        {
            OFConfigurator c1("c1");

            EXPECT_VALUE(c1.isReadOnly() == true);
                                    
            COMMENT("read ofuint32");
            ofuint32 resWord;
            EXPECT_VALUE(c1.readValue("group2","item1",&resWord,999) == true);
            EXPECT_VALUE(resWord == 123);

            COMMENT("read double");
            double resDouble;
            EXPECT_VALUE(c1.readValue("group1","item2",&resDouble,-999.99) == true);
            EXPECT_VALUE(resDouble == 4.567);

            string resString;
            EXPECT_VALUE(c1.readValue("group1","item1",resString,"default") == true);
            EXPECT_VALUE(resString == "value1");

            EXPECT_VALUE(c1.readValue("group1","unknown_item",resString,"default") == false);
            EXPECT_VALUE(resString == "default");

#ifdef _WIN32
            EXPECT_VALUE(stricmp(c1.dataFile().c_str(),"c:\\winnt\\c1.conf") == 0);
#else
            EXPECT_VALUE(stricmp(c1.dataFile().c_str(),"/etc/c1.conf") == 0);
#endif

            /* ! not implemented
               EXPECT_VALUE(c1.doesValueExist("group1","unknown_item") == false);

               EXPECT_VALUE(c1.doesValueExist("group1","item1") == true);*/
                }

        // -- change file open

        COMMENT("re-open");

        {
            OFConfigurator c1("c2",false);

            EXPECT_VALUE(c1.isOpen() == true);

            EXPECT_VALUE(c1.isReadOnly() == false);

            COMMENT("write string to read-only");
            EXPECT_VALUE(c1.writeValue("group1","item1","value1") == true);

            COMMENT("open as read-only");
            EXPECT_VALUE(c1.open("c2") == true);

            EXPECT_VALUE(c1.isOpen() == true);
            
            EXPECT_VALUE(c1.isReadOnly() == true);

            COMMENT("write string");
            EXPECT_VALUE(c1.writeValue("groupx","itemx","value1") == false);

            EXPECT_VALUE(c1.writeCommentForGroup("groupx","comment") == false);

            EXPECT_VALUE(c1.writeCommentForItem("groupx","itemx","comment") == false);

            COMMENT("test config file is well formed");
            EXPECT_VALUE(OFConfigurator::isWellFormed(c1.dataFile()) == true);

            COMMENT("open different file");
            EXPECT_VALUE(c1.open("c1",false) == true);

            string resString;
            EXPECT_VALUE(c1.readValue("group1","item1",resString,"default") == true);
            EXPECT_VALUE(resString == "value1");

            EXPECT_VALUE(c1.readValue("groupx","itemx",resString,"default") == false);
            EXPECT_VALUE(resString == "default");

        }

        // -- read empty file and re-output twice (should be unchanged)

        COMMENT("open & close empty file");
        string path;
        {
            OFConfigurator c1("c3",false,false);
            path = c1.dataFile();

            EXPECT_VALUE(c1.isOpen() == true);
                        
            COMMENT("write ofuint32 into x");
            EXPECT_VALUE(c1.writeValue("x","x",ofuint32(123)) == true);

            COMMENT("write ofuint32 into root");
            EXPECT_VALUE(c1.writeValue("root","x",ofuint32(1)) == true);

            c1.flush();

            COMMENT("test config file is well formed");
            EXPECT_VALUE(OFConfigurator::isWellFormed(c1.dataFile()) == true);

            c1.open("c3",false,false);

            EXPECT_VALUE(c1.isOpen() == true);
                        
            COMMENT("re-write ofuint32");
            EXPECT_VALUE(c1.writeValue("x","x",ofuint32(456)) == true);

            c1.flush();

            COMMENT("test config file is well formed");
            EXPECT_VALUE(OFConfigurator::isWellFormed(c1.dataFile()) == true);
        }

        {
            COMMENT("check file contents");
            ifstream f(path.c_str());
            char c;
            string buffer;
            while (f.get(c))
            {
                buffer += c;
            }
            f.close();
            EXPECT_VALUE(buffer == "x=1\n\n[x]\nx=456\n\n");
        }

        {
            COMMENT("before clear");
            OFConfigurator c1("c3",false,false);

            string resString;
            EXPECT_VALUE(c1.readValue("x","x",resString,"default") == true);
            cout << "resString = " << resString << "\n";
            EXPECT_VALUE(resString == "456");

            c1.clear();

            COMMENT("after clear");
            EXPECT_VALUE(c1.readValue("x","x",resString,"default") == false);
            EXPECT_VALUE(resString == "default");
            EXPECT_VALUE(c1.writeValue("x","x",ofuint32(123)) == true);
        }

        {
            COMMENT("very large ofuint32s");
            OFConfigurator c1("c3",false);
            ofuint32 big = 4278190080ul;
            EXPECT_VALUE(c1.writeValue("dwords","big",big) == true);

            ofuint32 returned;
            // read big value
            EXPECT_VALUE(c1.readValue("dwords","big",&returned,0) == true);
            EXPECT_VALUE(returned == 4278190080ul);
 
            // get big default
            EXPECT_VALUE(c1.readValue("dwords","non-existant",&returned,4294967295ul) == false);             
            EXPECT_VALUE(returned == 4294967295ul);
        } 

        {
            COMMENT("config file with no blank line at end");
            ofstream of(f4.c_str());
            EXPECT_VALUE(!of == false);
            of << "[x]\na=b\nc=d\ny=z";
            of.close();
           
            OFConfigurator c1("c4",false,false);
           
            string resString;
            EXPECT_VALUE(c1.readValue("x","y",resString,"default") == true);
            cout << "resString = " << resString << "\n";
            EXPECT_VALUE(resString == "z");
        }
       

    }
    catch (exception& e)
    {
        cout << "\n\nFAILURE uncaught exception: " << e.what() << std::endl;
    }  

    UNIT_TEST_END("OFConfigurator");

    return 0;
}

#endif // UNIT_TESTS
