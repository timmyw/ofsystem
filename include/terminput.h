/*
 * $Id: terminput.h,v 2.0 2006-01-01 20:08:32 timmy Exp $
 */

#if !defined(_TERMINPUT_H_)
#define _TERMINPUT_H_

/** \file terminput.h
 */

enum
        {
                TI_ECHO_NORMAL = 0,
                TI_ECHO_PASS = 1
        };

const ofuint32 ti_buffer_size = 512;

/** \class TermInput terminput.h
Provides input utilities
\nosubgrouping
 */
class TermInput
{
 public:
        /** \name Constructors and destructors
         */
        //@{

        /** Default constructor
         */
        TermInput();

        /** Destructor
         */
        ~TermInput();

        //@}

        /** \name Input methods
         */
        //@{

        /** Retrieve a line of input 
            Returns a pointer to the entered
            line. This buffer is managed by the class and should not
            be manipulated or freed.
         */
        const char* get_line(const char*prompt = 0, ofuint32 max_len = 0);

        //@}

        /** \name Accessor methods
         */
        //@{

        /** Retrieve the echo mode
         */
        ofuint32 echo() { return m_echo; }

        /** Set the echo mode
         */
        void echo(ofuint32 e) {m_echo = e;}

        //@}

 protected:

        /** \name Helper methods
         */
        //@{

        /** Destroy/empty current line
         */
        void destroy_() {
                delete [] m_line;
                m_line = 0;
                m_length = 0;
        }

        /** Check size and extend if necessary
         */
        void check_size_(ofuint32 new_inc = 0);

        /** Retrieve a character
         */
        ofchar get_char_();

        //@}

        /** \name Data members
         */
        //@{

        /** Current buffer
         */
        char* m_line;

        /** Current allocated length
         */
        ofuint32 m_length;

        /** Current echo mode
         */
        ofuint32 m_echo;

        /** Keyboard buffer
         */
        char m_buffer[ti_buffer_size+1];

        /** Buffer space position
         */
        ofuint32 m_buffer_pos;

        /** Buffer read position
         */
        ofuint32 m_buffer_read;

        //@}

};

#endif // _TERMINPUT_H_
