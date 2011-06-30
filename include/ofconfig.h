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

/** 
    \file Declaration of the OFConfigurator class.
*/

#if !defined(__OFCONFIG_H_)
#define __OFCONFIG_H_

class OFConfigGroup;  // Forward
class OFConfigItem;    // Forward
class OFFile;

#define OFCONFIG_MAX_INCLUDE 10
#define OFCONFIG_BASE_FILE   1

/**
 * \par Created
 * Dec-99 by Tim Whelan
 * \par Last u.test update
 * 16-Jun-00 by Andy Brice
 * \par Last code review
 * Apr-00 by Andy Brice
 * \brief
 * Configuration file class wrapper.
 *
 * This class provides easy access to options written into a configuration file.

 The format of the file is similar to a windows .ini file, e.g.:

 ; comment
 [group]
 item1 = 1;
 item2 = abc

 # is also accepted for comments.
                         
 All names (group and item) are treated as case-sensitive.
 The group name 'root' is reserved for use by the class. NB/ Doesn't get written to configurator file.
 Be sure to read an item back as the same type it was saved as (string, double or ofuint32).
 TODO would be more robust if isOpen() and isReadOnly also checked status of file in operating system.
 <p>
*/
class OFSYS_API OFConfigurator 
{
 public:

    /** Use to check if a file is a well-formed config file before opening
        returns true if it is a well-formed config file */
    static bool isWellFormed(const char* fileName);

    /** Default constructor. 
        If you use this constructor, you must use Open() to specify the configuration
        file to use.
    */
    OFConfigurator( ) ;
    
    /** Constructor which accepts a filename to use as the configuration file
        and opens it.
        If autoPath is true it will take the fileName given, prepend a directory (as set by OFPlatform::configPath()) and append '.conf'.
        If autoPath is false you can specify your own path and extension.
    */
    OFConfigurator( const char* fileName, bool readOnly = true, bool autoPath = true );
    
    /** Destructor
        This will automatically flush and close the configuration file.
    */
    ~OFConfigurator( );
    
    /** Opens the specified file as the configuration file.
        This function will also load the groups and items within the file, ready
        for use.
        If autoPath is true it will take the fileName given, prepend a directory (as set by OFPlatform::configPath()) and append '.conf'.
        If autoPath is false you can specify your own path and extension.      
        Returns true for success.*/
    bool open( const char* fileName, bool readOnly = true, bool autoPath = true );
    
    /** This will rewrite the configuration file if anything has changed.
        Work needs to be done on this function to acquire a system-wide
        mutex, as it functions by deleting the file, and then re-writing it from
        scratch. 
    
        This function will also reset the dirty flag (accessed via IsDirty() )*/
    void flush( );
    
    /** Write the specified value to the specified group.
        The value is added if it is not present, and overwritten if it is.
        Nothing is done to the configuration file until OFConfigurator::flush() is called.
        Creates the supplied group if required.
        Returns true for success. */
    bool writeValue( const char *groupName, const char *itemName, const char *value );
    
    /**
       Write the specified ofuint32 into the configuration group.
       The value is added if it is not present, and overwritten if it is.
       Nothing is done to the configuration file until OFConfigurator::Flush() is called. 
       Creates the supplied group if required.
       Returns true for success. */    
    bool writeValue( const char *groupName, const char *itemName, ofuint32 dwValue );
    
    /**
       Write the specified double value (float) into the configuration group.
       The value is added if it is not present, and overwritten if it is.
       Nothing is done to the configuration file until OFConfigurator::Flush() is called.
       Creates the supplied group if required.
       Returns true for success. */
    bool writeValue( const char *groupName, const char *itemName, double dblValue );

    /**
       Writes a comment into the file for the group (adds '#' for you).
       Returns true for success. */
    bool writeCommentForGroup( const char *groupName, const char *comment);

    /**
       Writes a comment into the file for the item if it exists  (adds '#' for you).
       Returns true for success. */
    bool writeCommentForItem( const char *groupName, const char *itemName, const char *comment);
    
    /**
       Retrieves the value for the name specified (char *).
       If the value does not exist, the supplied default value is set and false is returned.
       \warning The size of buffer supplied in \c value is not checked.
    */
    bool readValue( const char *groupName, const char * itemName, char * value, const char * defaultValue, bool expandEnvVars = true );

    /** Retrieves the value for the name specified (char *).
        If the value does not exist, the supplied default value is returned.
    */
    const char *readValue( const char *groupName, const char * itemName, const char * defaultValue );

    /**
       Retrieves the value for the name specified (ofuint32).
       If the value does not exist, the supplied default value is set and false is returned.
    */
    bool readValue( const char *groupName, const char * itemName, ofuint32 * pdwValue, ofuint32 dwDefault );
    
    /**
       Retrieves the value for the name specified (double).
       If the value does not exist, the supplied default value is set and false is returned.
    */
    bool readValue( const char *groupName, const char * itemName, double *dblValue, double dblDefault );

    /** Read in a yes/no boolean value

        Interprets string values (yes/no and true/false as well as
        zero and non-zero numbers.)
     */
    bool readYesNo(const char* groupName, const char* itemName, bool boolDefault);

    /**
        Delete the item from the group
        returns true if was found and deleted 
     */
    bool deleteItem( const char *groupName, const char * itemName );
    
    /**
       Remove everything
    */
    void clear();

    /**
       Checks for the existance of the specified item name.
       Returns true if the item is found, and false if not.
    */
    // bool doesValueExist( PSTR pszGroup, PSTR pszName ); ! not implemented
    
    /**
       Checks if the configuration has been modified since it was last flushed
       using OFConfigurator::flush().
       Returns true if a call to OFConfigurator::flush() is required. This flag
       is automatically checked by the destructor. */
    bool isDirty( ) const;
    
    /** Checks the open status of the file.

    If the file is not open for some reason , returns false.
    */
    bool isOpen( ) const;
    
    /**
       Checks the readonly status of the underlying configuration file.

       This ultimately is decided on by the value passed into the
       constructor or to OFConfigurator::open();
    */
    bool isReadOnly( ) const;
    
    /** Access the item list for a group. e.g. "item1=value1", "item2=value2"

        It is the callers responsibility to destroy the elements of
        the list.
    */
    void getItemsInGroup( const char *groupName, OFCHARSTARLIST *list );

    /** Returns true if group exists and is not empty
     */
    bool hasGroup( const char *groupName );
    
    /** Access the name list for a group. e.g. "name1", "name2"
        It is the callers responsibility to destroy the elements of the list.
    */
    void getValuesInGroup( const char *groupName, OFCHARSTARLIST *list );

    /** Retrieve the 'actual' values in the group.

    This returns a list of the values, without any item names or
    separators.
     */
    void getActualValuesInGroup( const char *groupName, OFCHARSTARLIST *list );

    /** Retrieve the names and values in two lists

    Creates two lists, one containing the group item names and the
    other the corresponding values.
     */
    void getNameAndValuesInGroup(const char* groupName,
                                 OFCHARSTARLIST* names,
                                 OFCHARSTARLIST* values);

    /** Change the file name, doesn't flush or remove the original */
    void changeFileName( const char* fileName );

    /** Retrieve the name of the configuration file.
     */
    const char *dataFile( ) const;

    /** The root group, which always exists.
     */
    static const char * rootGroup;

    /** Flatten the configuration settings.
        Writes all the configuration settings to the specified file, ignoring
        all includes.
     */
    ofuint32 flatten(OFFile& file);

 private:
        
    /** copy constructor (not implemented)
     */
    OFConfigurator( const OFConfigurator& );

    /** operator = (not implemented)
     */
    OFConfigurator& operator = ( const OFConfigurator& );

    /** get the group with the given name, returns NULL if none.
     */
    OFConfigGroup *retrieveGroup( const char *groupName );

    /** Open the specified file
     */
    ofuint32 open_file_(const char* path, bool read_only, bool auto_path, OFFile& file, char* real_path = 0);

    void read_file_(OFFile *file);

    void readFile_( );
    void close_( );
    
    /** Stack of files
     */
    OFFile *m_files[OFCONFIG_MAX_INCLUDE+1];

    /** Stack of file names
     */
    char m_filenames[OFCONFIG_MAX_INCLUDE+1][OF_MAX_PATH_LEN+1];
    
    /** Current stack level
     */
    ofuint32 m_stack_level;

    typedef std::vector<OFConfigGroup*>::iterator group_iter;
    std::vector<OFConfigGroup*> m_groups;
    
    /** True if file is open.
     */
    bool m_opened;
    
    /** True if modified since last flush.
     */
    bool m_dirty;
    
    /** True if file is access through this class as read only (regardless of OS properties of file).
     */
    bool m_readOnly;

    /** Non-zero if we supplied the path
     */
    ofuint32 m_auto_path;
    
    /** The path of the config file.
     */
    char m_datafile[OF_MAX_PATH_LEN+1];
};

#include <ofconfig.i>

#endif
