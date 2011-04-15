/*
 * $Header: /usr/local/cvs/source/libs/ofsys/terminput.cpp,v 2.2 2009-02-25 09:01:22 timmy Exp $
 */

#include <ofsys/ofsys.h>
#include <ofsys/terminput.h>
#include <ofsys/ofos.h>

TermInput::TermInput():
        m_line(0),
        m_length(0),
        m_echo(TI_ECHO_NORMAL),
        m_buffer_pos(0),
        m_buffer_read(0)
{
}

TermInput::~TermInput()
{
        destroy_();
}

ofchar
TermInput::get_char_()
{
        // Check for any outstanding reads on stdin
        ofuint32 fd = fileno(stdin);
        fd_set fdread; struct timeval tv;

        fprintf(stdout,"get_char_()");fflush(stdout);
        FD_ZERO(&fdread);
        FD_SET(fd, &fdread);
        tv.tv_sec = 0; tv.tv_usec = 100000;
        ofint32 rc;
        char stop = 0;
        while (!stop && (rc = select(fd+1, &fdread, 0, 0, &tv)) >= 0)
        {
                if (rc>0 && FD_ISSET(fd, &fdread))
                {
                        // There may be stuff to read in
                        ofuint32 space_left = (ti_buffer_size+m_buffer_read+m_buffer_pos-1)%ti_buffer_size;
                        fprintf(stdout, "[%"LFORMAT"d]", space_left);fflush(stdout);
                        if (space_left)
                        {
                                ofchar tbuf;
                                if (read(fd, &tbuf, sizeof(ofchar)) >0)
                                {
                                        m_buffer[m_buffer_pos++] = tbuf;
                                        m_buffer_pos %= ti_buffer_size;
                                }
                                else
                                        stop = 0;
                        }
                        else
                                stop = 0;
                }
        }

        // Get a character if available
        ofuint32 space_left = (ti_buffer_size+m_buffer_read+m_buffer_pos)%ti_buffer_size;
        ofchar c = 0;
        if (space_left < ti_buffer_size)
        {
                fprintf(stdout,"GOTCH");fflush(stdout);
                c = m_buffer[m_buffer_read++];
                m_buffer_read %= ti_buffer_size;
        }
        return c;
}

const char*
TermInput::get_line(const char*prompt /*= 0*/, ofuint32 max_len /*=0*/)
{
        //ofuint32 cur_pos = 0;
        if (prompt)
                fprintf(stdout, "%s", prompt);
        fflush(stdout);
        char stop = 0;
        check_size_();
        while (!stop)
        {
                ofchar c = get_char_();
                ofuint32 cur_len = OFOS::strlen(m_line);
                if (c != 0xd)
                {
                        if (cur_len < max_len)
                        {
                                check_size_(1);
                                m_line[cur_len++] = c;
                                m_line[cur_len] = 0;
                        }
                        else
                                stop = 1;
                }
                else
                        stop = 1;
        }
        return m_line;
}

void
TermInput::check_size_(ofuint32 new_inc /*= 0*/)
{
        if (!m_line)
        {
                m_length = maximum<ofuint32>(new_inc,100);
                m_line = new char[m_length+1];
                RESETSTRING(m_line);
                return;
        }
        if (OFOS::strlen(m_line)+new_inc > m_length)
        {
                m_length += maximum<ofuint32>(new_inc,100);
                char* line = new char[m_length+1];
                if (OFOS::strlen(m_line))
                        OFOS::strcpy(line, m_line);
                delete [] m_line;
                m_line = line;
        }
}

#if defined(UNIT_TEST)

#include "UnitTest.h"

int
main(ofint32 argc, char *argv[] )
{
        UT_BATCH_START("TermInput");

        TermInput input1;
        input1.echo(TI_ECHO_NORMAL);

        UT_TEST_START(1, "ofuint32 get_line(ofuint32)");
        const char* l = input1.get_line("ENTRY:");
        cout << "line:" << l << endl;
        UT_TEST_END;

        UT_BATCH_END;
        return 0;
}

#endif // UNIT_TEST
