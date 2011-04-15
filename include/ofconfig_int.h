/*
 */

#if !defined(__OFCONFIG_INT_H_)
#define __OFCONFIG_INT_H_

class OFConfigItem;
class OFConfigComment;
class OFConfigBlank;
class OFConfigInclude;

class OFSYS_API OFConfigGroup
{
 public:
    /** Constructor
        Accepts the group name. Creates a new group with an empty
        list of items.
    */
    OFConfigGroup( const char *groupName, ofuint32 stack_level );
  
    /** Destructor
        Destroys any items and comments.
    */
    ~OFConfigGroup( );
  
    /** Adds a name/value pair to the group.
        This function will attempt to extract the name and value
        from the supplied string, using equals ('=') as a delimiter.
    */
    void addValue(const char *line, char top_level);
  
    /** Adds a name/value pair to the group.
        Performs a shallow copy and takes ownership of the item.
    */
    void addValue( OFConfigItem *item );
  
    /** Returns the name of the group.
     */
    const char *name( ) const;
  
    /** Retrieves a pointer to a #OFConfigItem identified by the supplied name.
        If the name cannot be found, NULL is returned.
    */
    OFConfigItem *retrieveItem( const char *name );
    
    /** Deletes the named item
        returns true if succeeded
    */
    bool deleteItem( const char *name );

    /** Writes the group out to an ASCII configuration file.
        The OFFile instance specified must have been opened with read/write
        privileges.  The function will dump group information (including comments)
        and then call OFConfigItem::Dump for each name/value pair in the group.
    */
    void dump( OFFile &file, char force_all = 0 );

    /** Returns the number of items in the list.
     */
    ofuint32 getItemCount();
  
    /** Retrieve a pointer to the last item in the list.
        Returns NULL if the item list is empty.
    */  
    OFConfigItem *getLastItem( );
  
    /** Adds a comment to the group.
        This function is used internally, although it can be used
        to add additional comments to the group.
    */
    void addComment( const char *line );

    /** Add a blank line
     */
    void addBlank( );

    /** Add an include
     */
    void addInclude(OFConfigInclude* inc);

    /** Retrieves a list of all the items in a group.
     */
    void getItems( OFCHARSTARLIST *list );
        
    /** Retrieves a list of all the values in a group.
     */
    void getValues( OFCHARSTARLIST *list );
  
    /** Retrieve a list of the actual values (not names)
     */
    void getActualValues( OFCHARSTARLIST *list );

    /** Retrieve the names and values in two lists

    Creates two lists, one containing the group item names and the
    other the corresponding values.
     */
    void getNameAndValues(OFCHARSTARLIST* names,
                          OFCHARSTARLIST* values);

    /** Retrieve the file this group was found in
     */
    const char* file() { return m_file; }

    /** Retrieve the level
     */
    ofuint32 level() {return m_stack_level;}

    /** Set the level
     */
    void level(ofuint32 l) {m_stack_level = l;}
 protected:

    char m_name[129];

    typedef std::vector<OFConfigItem*>::iterator item_iter;
    std::vector<OFConfigItem*> m_items;
    typedef std::vector<OFConfigComment*>::iterator comment_iter;
    std::vector<OFConfigComment*> m_comments;
    std::vector<OFConfigBlank*> m_blanks;
    std::vector<OFConfigInclude*> m_includes;

    char m_file[OF_MAX_PATH_LEN+1];

    ofuint32 m_stack_level;
};

/**
 * \par Created
 * Dec-99 by Tim Whelan
 * \par Last u.test update
 * Apr-00 by Andy Brice
 * \par Last code review
 * Apr-00 by Andy Brice
 * \brief
 * Stores information about each name/value pair in a OFConfigGroup.
 */
class OFSYS_API OFConfigItem
{
 public:
    /** Constructor.
        Attempts to extract the name/value information from the string
        provided.  The equals ('=') sign is used to delimit the string. */
    OFConfigItem( const char *line, char top_level );
  
    OFConfigItem( const char * itemName, const char * value, char top_level );
  
    OFConfigItem( const char * itemName, ofuint32 dwValue, char top_level );
  
    OFConfigItem( const char * itemName, double dblValue, char top_level );
  
    ~OFConfigItem( );

    bool value( char * value );
    bool value( ofuint32 *pdwValue );  
    bool value( double *pdblValue );
    const char *value( ) const;
  
    const char * name( ) const;
    void dump( OFFile &file );

    void setValue( const char *theValue );

    /** Add a comment to item
     */
    void addComment( const char *line );

    ofuint32 itemSize() const;

    /** Add a blank line
     */
    void addBlank( );

    /** Retrieve the top level flag
     */
    char top_level() const { return m_top_level; }
 protected:
    char m_name[129];
    char *m_value;
    typedef std::vector<OFConfigComment*>::iterator comment_iter;
    std::vector<OFConfigComment*> m_comments;
    std::vector<OFConfigBlank*> m_blanks;
    char m_top_level;
};

typedef vector<OFConfigItem*> OFConfigItemList;
typedef OFConfigItemList::iterator OFConfigItemIterator;

/**
 * \par  Created
 * Dec-99 by Tim Whelan
 * \par  Last u.test update
 * Apr-00 by Andy Brice
 * \par  Last code review
 * Apr-00 by Andy Brice
 * \brief
 * Stores a .... guess what.
 */
class OFSYS_API OFConfigComment
{
 public:
    OFConfigComment( const char * line );
    ~OFConfigComment( );
  
    void dump( OFFile &file );
 protected:
    char m_comment[1024];
};

class OFSYS_API OFConfigBlank
{
 public:
    void dump( OFFile &file );
};

class OFConfigInclude
{
 public:
    OFConfigInclude(const char* filename) { OFOS::strncpy2(m_filename, filename, OF_MAX_PATH_LEN+1);}
    void dump(OFFile &file);
 protected:
    char m_filename[OF_MAX_PATH_LEN+1];
};

#include <ofconfig_int.i>

#endif // #if !defined(__OFCONFIG_INT_H_)

