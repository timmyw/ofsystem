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

#include <ofdatetime.h>
#include <ofos.h>

static const long FIRST_DAY = 2361222;  // Julian day for 17520914
static const int  FIRST_YEAR    = 1752;     // ### wrong for many countries
static const unsigned long SECS_PER_DAY  = 86400;
static const unsigned long MSECS_PER_DAY = 86400000;
static const unsigned long SECS_PER_HOUR = 3600;
static const unsigned long MSECS_PER_HOUR= 3600000;
static const unsigned long SECS_PER_MIN  = 60;
static const unsigned long MSECS_PER_MIN = 60000;

static short monthDays[] ={0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

const char *OFDate::monthNames[] = {
    "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

const char *OFDate::weekdayNames[] ={
    "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"
};

//  OFDate::OFDate()
//  {
//      m_jd=0;
//  }

//  OFDate::OFDate( int y, int m, int d )
//  {
//      setYMD( y, m, d );
//  }

//  OFDate::OFDate(const OFDate& rhs )
//  {
//      m_jd = rhs.m_jd ;
//  } ;

void 
OFDate::clear()
{
    m_jd = 0;
}

bool   OFDate::isNull()  const
{
    return m_jd == 0;
}

bool OFDate::isValid() const
{
    return m_jd >= FIRST_DAY;
}

int OFDate::year() const
{
    int y, m, d;
    jul2greg( m_jd, y, m, d );
    return y;
}


int OFDate::month() const
{
    int y, m, d;
    jul2greg( m_jd, y, m, d );
    return m;
}

int OFDate::day() const
{
    int y, m, d;
    jul2greg( m_jd, y, m, d );
    return d;
}

int OFDate::dayOfWeek() const
{
    return (((m_jd+1) % 7) + 6)%7 + 1;
}

int OFDate::dayOfYear() const
{
    return m_jd - greg2jul(year(), 1, 1) + 1;
}

int OFDate::daysInMonth() const
{
    int y, m, d;
    jul2greg( m_jd, y, m, d );
    if ( m == 2 && leapYear(y) )
        return 29;
    else
        return monthDays[m];
}

int OFDate::daysInYear() const
{
    int y, m, d;
    jul2greg( m_jd, y, m, d );
    return leapYear(y) ? 366 : 365;
}

string
OFDate::monthName( int month ) const
{
    if ( month < 1 || month > 12 )
    {
        month = 1;
    }

    return string( monthNames[month-1] );
}

string OFDate::dayName( int weekday) const
{
    if ( weekday < 1 || weekday > 7 )
        weekday = 1;
    return string(weekdayNames[weekday-1]);
}

void 
OFDate::getGregorian( ofint32 *y, ofint32 *m, ofint32 *d ) const
{
    *y = year();
    *m = month();
    *d = day();
}

/* todo - return null if error parsing ?
*/
const char *
OFDate::assignISO( const char *s )
{
    const char *cp = s;
    const char *ret;

    if ( cp )
    {
        ofint32 field[] = { 0, 0, 0 };                                /* YMD */
                     /* points to char after field incase we need to retrace */
        const char *charafterfield[] = { 0, 0, 0 };
        int maxi = 2;
        int i = 0;                             /* which field were busy with */
        int j = -1;      /* set to i each time we do something with field[i] */
        int t = -1;                          /* first time field encountered */
        int sign = 1;

        /* date is upto time separator */
        for ( ; *cp && i <= maxi && t < 0; ++cp )      /* 2 is the max field */
        {
            ret = cp;             /* keep track of next char for time parser */

            char c = *cp;

            if ( c == '-' && j < i && i == 0 )        /* could be a -ve year */
            {
                sign = -1;
                j = i;
                charafterfield[i] = cp + 1;
            }
            else if ( c >= '0' && c <= '9' )
            {
                if ( j == i )                            /* been here before */
                {
                    field[i] *= 10;
                    field[i] += c - '0';
                }
                else
                {
                    field[i] = ( c - '0' ) * sign;
                    j = i;
                    sign = 1;
                }
                charafterfield[i] = cp + 1;
            }
            else if ( c == '-' || c == '/' )      /* lenient, but may change */
            {
                ++i;                                   /* explicit new field */
            }
            else if ( c == ':' ) /* last field worked on was actually a time */
            {
                t = j;
            }
            else if ( c == 'T' )             /* explicit start of time parts */
            {
                t = ++i;
            }
            else if ( j == i )  /* had started work on the field, so move on */
            {
                t = ++i;
            }
        }

        assert( j <= maxi );

        int lastvalidfield = j;
        if ( t > 0 && t <= lastvalidfield )
            lastvalidfield = t - 1;

        if ( lastvalidfield < 0 )
        {
            clear();

            ret = s;                /* next parser must start from beginning */
        }
        else
        {
            if ( lastvalidfield < 2 )
                field[2] = 1;
            if ( lastvalidfield < 1 )
                field[1] = 1;

            setYMD( field[0], field[1], field[2] );

            ret = charafterfield[ lastvalidfield ];
        }
    }
    else
    {
        clear();
        ret = s;
    }

    return ret;
}

std::string 
OFDate::toStringISO() const
{
    std::string s;

    if ( isNull() )
    {
        s = "";
    }
    else
    {
        ofint32 y, m, d; // int for %d
        getGregorian( &y, &m, &d );

        char temp[15]; // 7+1 for y
#if !defined(NDEBUG)
        int i = 
#endif
            sprintf( temp, "%.4"LFORMAT"d-%.2"LFORMAT"d-%.2"LFORMAT"d", y, m, d );
        assert( i < 15 );

        s = temp;
    }

    return s;
}

string OFDate::toString() const
{
    int y, m, d;
    char temp[10] ;

    jul2greg( m_jd, y, m, d );
    string buf = dayName(dayOfWeek());
    buf += ' ';
    buf += monthName(m);
    sprintf(temp," %d %d", d, y);
    buf += temp ;
    return buf;
}

bool
OFDate::setYMD( int y, int m, int d )
{
    if ( isValid( y, m, d ) )
    {
        m_jd = greg2jul( y, m, d );
        //assert( year() == y && month() == m && day() == d );
        return true;
    }
    else
    {
        clear(); 
        return false;
    }
}

OFDate OFDate::addDays( int ndays ) const
{
    OFDate d;
    d.m_jd = m_jd + ndays;
    return d;
}

int OFDate::daysTo( const OFDate &d ) const
{
    return d.m_jd - m_jd;
}

OFDate OFDate::currentDate()
{
#if defined(OFOPSYS_WIN32)

    SYSTEMTIME t;
    GetLocalTime( &t );
    OFDate d;
    d.m_jd = greg2jul( t.wYear, t.wMonth, t.wDay );
    return d;

#else

    time_t ltime;
    time( &ltime );
    tm *t = localtime( &ltime );
    OFDate d;
    d.m_jd = greg2jul( t->tm_year + 1900, t->tm_mon + 1, t->tm_mday );
    return d;

#endif
}

bool OFDate::isValid( int y, int m, int d )
{
    if ( y >= 0 && y <= 99 )
        y += 1900;
    else if ( y < FIRST_YEAR || (y == FIRST_YEAR && (m < 9 ||
                                                     (m == 9 && d < 14))) )
        return  false;
    return (d > 0 && m > 0 && m <= 12) &&
        (d <= monthDays[m] || (d == 29 && m == 2 && leapYear(y)));
}

bool OFDate::leapYear( int y )
{
    return (y % 4 == 0 && y % 100 != 0) || y % 400 == 0;
}

long OFDate::greg2jul( int y, int m, int d )
{
    long c, ya;
    if ( y <= 99 )
        y += 1900;
    if ( m > 2 )
    {
        m -= 3;
    }
    else
    {
        m += 9;
        y--;
    }
    c = y;                  // NOTE: Sym C++ 6.0 bug
    c /= 100;
    ya = y - 100*c;
    return 1721119 + d + (146097*c)/4 + (1461*ya)/4 + (153*m+2)/5;
}

void OFDate::jul2greg( long jd, int &y, int &m, int &d )
{
    long x;
    long j = jd - 1721119;
    y = (j*4 - 1)/146097;
    j = j*4 - 146097*y - 1;
    x = j/4;
    j = (x*4 + 3) / 1461;
    y = 100*y + j;
    x = (x*4) + 3 - 1461*j;
    x = (x + 4)/4;
    m = (5*x - 3)/153;
    x = 5*x - 3 - 153*m;
    d = (x + 5)/5;
    if ( m < 10 )
    {
        m += 3;
    }
    else
    {
        m -= 9;
        y++;
    }
}

int operator == ( const OFDate &lhs, const OFDate &rhs )
{
    return lhs.m_jd == rhs.m_jd;
}

int operator != ( const OFDate &lhs, const OFDate &rhs )
{
    return lhs.m_jd != rhs.m_jd;
}

int operator < ( const OFDate &lhs, const OFDate &rhs )
{
    return lhs.m_jd < rhs.m_jd;
}

int operator <= ( const OFDate &lhs, const OFDate &rhs )
{
    return lhs.m_jd <= rhs.m_jd;
}

int operator > ( const OFDate &lhs, const OFDate &rhs )
{
    return lhs.m_jd > rhs.m_jd;
}

int operator >= ( const OFDate &lhs, const OFDate &rhs )
{
    return lhs.m_jd >= rhs.m_jd;
}

OFDate operator+( const OFDate &lhs,  const OFDate &rhs )
{
    OFDate temp ;

    temp.m_jd = lhs.m_jd + rhs.m_jd ;
    return temp ;
}

OFDate operator-( const OFDate &lhs,  const OFDate &rhs )
{
    OFDate temp ;

    temp.m_jd = lhs.m_jd - rhs.m_jd ;
    return temp ;
}

/*****************************************************************************
OFTime member functions
*****************************************************************************/

//  OFTime::OFTime()
//  {
//      m_ds=0;
//  }

//  OFTime::OFTime( int h, int m, int s, int ms )
//  {
//      setHMS( h, m, s, ms );
//  }

//  OFTime::OFTime( long init )
//  {
//      m_ds = init ;
//  }

//  OFTime::OFTime(const OFTime& rhs )
//  {
//      m_ds = rhs.m_ds ;
//  }

void 
OFTime::clear()
{
    m_ds = 0;
    //m_tz = nulltz;
}

bool   OFTime::isNull()  const
{
    return m_ds == 0;
}

bool OFTime::isValid() const
{
    return m_ds < MSECS_PER_DAY;
}

int OFTime::hour() const
{
    return m_ds / MSECS_PER_HOUR;
}

int OFTime::minute() const
{
    return (m_ds % MSECS_PER_HOUR)/MSECS_PER_MIN;
}

int OFTime::second() const
{
    return (m_ds / 1000)%SECS_PER_MIN;
}


int OFTime::msec() const
{
    return m_ds % 1000;
}

std::string
OFTime::toStringISO (bool z/*=true*/) const
{
        char temp[18]; // extra 4 for debugging

        int h, m, s, ms;
        h = hour();
        m = minute();
        s = second();
        ms = msec();

        int i;

        if (ms)
                i = OFOS::snprintf (temp, 17, "%.2d:%.2d:%.2d.%-.3d", h, m, s, ms);
        else
                i = OFOS::snprintf (temp, 17,  "%.2d:%.2d:%.2d", h, m, s);

        std::string ret( temp );

        if (z)
        {
                /*
                  if ( hasTimeZone() )
                  {
                  h = m_tz / 60;
                  m = m_tz - ( h * 60 );
                  sprintf( temp, "%+.2d:%2d", h, m );
                  ret += temp;
                  }
                  else
                  {
                */
                ret += 'Z';
                /*
                  }
                */
        }

        return ret;
}

string OFTime::toString() const
{
    string buf;

    char temp[10] ; // 10 is arbitrary

    OFOS::snprintf(temp, 10, "%.2d", hour());
    buf = temp ;
    OFOS::snprintf(temp, 10, ":%.2d", minute());
    buf += temp ;
    OFOS::snprintf(temp, 10, ":%.2d", second());
    buf += temp ;

    return buf;
}

const char *
OFTime::assignISO( const char *s, bool atTimeChars )
{
    const char *cp = s;
    const char *ret;

    /* By default, assumes time starts after last time separator 'T',
       ( or space ) immediately followed by a digit or sign.
    */
    if ( cp && !atTimeChars )
    {
        const char *start = 0;
        const char *ps = 0;
        for ( ; *cp; ++cp )      /* 2 is the max field */
        {
            char c = *cp;
            if ( ps )
            {
                if ((c >= 0 && c <= '9') || (c == '-' || c == '+'))
                    start = cp;
                else
                    ps = 0;
            }
            if ( c == ' ' || c == 'T' )
                ps = cp;
        }
        if ( start )
            cp = start;
        else
            cp = s;
    }

    if ( cp )
    {
        bool z = false;   /* optional 'Z' Zulu time zone designator present? */
        bool tz = false;                     /* has time zone other than 'Z' */
        bool msec = false;                    /* optional '.' msecs present? */
        int sign = 1;
        int mseci = 3;
        int tzdi  = 4;
        ofint32 field[] = { 0, 0, 0, 0, 0, 0 };       /* hh:mm:ss.msec+HH:MM */
        int maxi = 5;
        int i = 0;                             /* which field were busy with */
        int j = -1;      /* set to i each time we do something with field[i] */

        for ( ; *cp && i <= maxi; ++cp )               /* 2 is the max field */
        {
            ret = cp;             /* keep track of next char for time parser */

            char c = *cp;

            if ( c == '-' && j < i ) /* must not have passed digits in field */
            {
                sign = -1;
                j = i;                   /* entered into this field properly */
            }
            else if ( c >= '0' && c <= '9' )
            {
                if ( j == i )   /* been here before, accumulate old * 10 + c */
                {
                    field[i] *= 10;
                    field[i] += c - '0';
                }
                else
                {
                    field[i] = ( c - '0' ) * sign;
                    j = i;               /* entered into this field properly */
                    sign = 1;
                }
            }
            else if ( c == '-' )                     /* time zone designator */
            {
                tz = true;
                i = tzdi;
            }
            else if ( c == '+' )                     /* time zone designator */
            {
                tz = true;
                i = tzdi;
            }
            else if ( c == '.' )                                     /* msec */
            {
                ++i;
                if ( i == mseci )                         /* definitely msec */
                {
                    msec = true;
                }                              /* tolerates hh.mm.ss for now */
            }
            else if ( c == ':' )    /* last field worked on was a time field */
            {
                ++i; sign = 1;
            }
            else if ( c == 'Z' )                 /* UTC time zone designator */
            {
                z = true;
                i = maxi + 1;                                       /* leave */
            }
            else if ( j == i )  /* had started work on the field, so move on */
            {
                i = tzdi;               /* tolerate space before tzd for now */
            }
        }

        /* tolerate +0100 (no colon) tzd for now */
        if ( j == tzdi )
        {
            field[j+1] = field[j] - ( field[j] / 60 ) * 60;
            field[j] /= 60;
        }

        assert( j <= maxi );

        setHMS( field[0], field[1], field[2], field[3] );
        if ( !z )
        {
            // todo store time zone
            // setTimeZone( field[4], field[5] );
            // "12:00Z and 13:00+01:00 and 0700-0500 all indicate the same point in time"
            // days might overflow ?
            setLong( addSecs( - ((field[4]*60)+field[5])*60 ).getLong() );
        }

        ret = cp;
    }
    else
    {
        clear();
        ret = 0;
    }

    return ret;
}

bool OFTime::setHMS( int h, int m, int s, int ms )
{
    if ( !isValid(h,m,s,ms) )
    {
        return false;
    }
    m_ds = (h*SECS_PER_HOUR + m*SECS_PER_MIN + s)*1000 + ms;
    return true;
}

OFTime OFTime::addSecs( int nsecs ) const
{
    return addMSecs(nsecs*1000);
}

int OFTime::secsTo( const OFTime &t ) const
{
    return ((int)t.m_ds - (int)m_ds)/1000;
}

OFTime OFTime::addMSecs( int ms ) const
{
    OFTime t;
    if ( ms < 0 )
    {
        // % not well-defined for -ve, but / is.
        int negdays = (MSECS_PER_DAY-ms) / MSECS_PER_DAY;
        t.m_ds = ((int)m_ds + ms + negdays*MSECS_PER_DAY)
            % MSECS_PER_DAY;
    }
    else
    {
        t.m_ds = ((int)m_ds + ms) % MSECS_PER_DAY;
    }
    return t;
}

int OFTime::msecsTo( const OFTime &t ) const
{
    return (int)t.m_ds - (int)m_ds;
}


OFTime OFTime::currentTime()
{
    OFTime ct;
    currentTime( &ct );
    return ct;
}

bool OFTime::currentTime( OFTime *ct )
{
    if ( !ct )
        return false;

#if !defined(OFOPSYS_WIN32)
    struct timeval tv;
    gettimeofday( &tv, 0 );
    time_t ltime = tv.tv_sec;
    tm *t = gmtime( &ltime );
    ct->m_ds = (long)( MSECS_PER_HOUR*t->tm_hour + MSECS_PER_MIN*t->tm_min +
                       1000*t->tm_sec + tv.tv_usec/1000 );
    return (t->tm_hour== 0 && t->tm_min == 0);
#else
    SYSTEMTIME t;
    GetSystemTime( &t );
    ct->m_ds = MSECS_PER_HOUR*t.wHour + MSECS_PER_MIN*t.wMinute +
        1000*t.wSecond + t.wMilliseconds;
    return (t.wHour == 0 && t.wMinute == 0);
#endif
}

bool OFTime::isValid( int h, int m, int s, int ms )
{
    return (long)h < 24 && (long)m < 60 && (long)s < 60 && (long)ms < 1000;
}

void OFTime::start()
{
    *this = currentTime();
}

int OFTime::restart()
{
    OFTime t = currentTime();
    int n = msecsTo( t );
    if ( n < 0 )                // passed midnight
        n += 86400*1000;
    *this = t;
    return n;
}

int OFTime::elapsed()
{
    int n = msecsTo( currentTime() );
    if ( n < 0 )                // passed midnight
        n += 86400*1000;
    return n;
}

void OFTime::setLong(long value)
{
    m_ds = value ;
}

long OFTime::getLong() const
{
    return m_ds ;
}

// Friend relational and equality operators for OFTime

int operator ==( const OFTime &lhs, const OFTime &rhs)
{
    return lhs.m_ds == rhs.m_ds ;
}

int operator!=( const OFTime& lhs, const OFTime& rhs )
{
    return lhs.m_ds != rhs.m_ds;
}
int operator<( const OFTime& lhs, const OFTime& rhs )
{
    return lhs.m_ds < rhs.m_ds;
}
int operator<=( const OFTime& lhs, const OFTime& rhs )
{
    return lhs.m_ds <= rhs.m_ds;
}
int operator>( const OFTime& lhs, const OFTime& rhs )
{
    return lhs.m_ds > rhs.m_ds;
}
int operator>=( const OFTime& lhs, const OFTime& rhs )
{
    return lhs.m_ds >= rhs.m_ds;
}

OFTime operator+( const OFTime &lhs,  const OFTime &rhs )
{
    OFTime temp ;

    temp.m_ds = lhs.m_ds + rhs.m_ds ;
    return temp ;
}

OFTime operator-( const OFTime &lhs,  const OFTime &rhs )
{
    OFTime temp ;

    temp.m_ds = lhs.m_ds - rhs.m_ds ;
    return temp ;
}

/*****************************************************************************
OFDateTime member functions
*****************************************************************************/

//  OFDateTime::OFDateTime()
//  {

//  }

//  OFDateTime::OFDateTime( const OFDate &date )
//      : m_d(date)
//  {
//  }

//  OFDateTime::OFDateTime( const OFDate &date, const OFTime &time )
//      : m_d(date), m_t(time)
//  {
//  }

//  OFDateTime::OFDateTime( const OFDateTime &dt )
//      :m_d(dt.m_d), m_t(dt.m_t)
//  {
//  }

//  OFDateTime::OFDateTime( time_t tt )
//  {
//      this->setTime_t( tt ) ;
//  }

bool   OFDateTime::isNull() const
{
    return m_d.isNull() && m_t.isNull();
}

bool   OFDateTime::isValid()    const
{
    return m_d.isValid() && m_t.isValid();
}

OFDate  OFDateTime::date()  const
{
    return m_d;
}

OFTime  OFDateTime::time()  const
{
    return m_t;
}

void   OFDateTime::setDate( const OFDate &date )
{
    m_d=date;
}

void   OFDateTime::setTime( const OFTime &time )
{
    m_t=time;
}

void OFDateTime::setTime_t( long secsSince1Jan1970UTC )
{
    time_t tmp = (time_t) secsSince1Jan1970UTC;
    //tm *tM = localtime( &tmp ); // originally
    tm *tM = gmtime( &tmp );
    if ( !tM )
    {
        tM = gmtime( &tmp );
        if ( !tM )
        {
            m_d.m_jd = OFDate::greg2jul( 1970, 1, 1 );
            m_t.m_ds = 0;
            return;
        }
    }
    m_d.m_jd = OFDate::greg2jul( tM->tm_year + 1900, tM->tm_mon + 1, tM->tm_mday );
    m_t.m_ds = MSECS_PER_HOUR*tM->tm_hour + MSECS_PER_MIN*tM->tm_min +
        1000*tM->tm_sec;
}

long OFDateTime::getLong( ) const
{
    return (m_d.m_jd - OFDate::greg2jul( 1970, 1, 1 ))*SECS_PER_DAY + m_t.m_ds / 1000 ;
}

void
OFDateTime::setDateTime( ofuint64 value )
{
     m_t.m_ds = value;
     m_d.m_jd = ( value >> 32 );
}

ofuint64
OFDateTime::getDateTime( ) const
{
     return ((ofuint64)(m_t.m_ds)) | (((ofuint64)(m_d.m_jd)) << 32);
}

std::string 
OFDateTime::toStringISO() const
{
    std::string s( m_d.toStringISO() );
    s += "T";
    s += m_t.toStringISO( true );
    return s;
}

std::string
OFDateTime::toRFC1123() const
{
    // Sun, 06 Nov 1994 08:49:37 GMT
    string buf = m_d.dayName(m_d.dayOfWeek());
    buf += ", ";
    char cp[5] ;
    sprintf(cp, "%02d", m_d.day() ) ;
    buf += cp;
    buf += " ";
    buf += m_d.monthName(m_d.month());
    buf += " ";
    sprintf(cp, "%d", m_d.year() ) ;
    buf += cp;
    buf += " ";
    buf += m_t.toString();
    buf += " GMT";
    return buf;
}

std::string
OFDateTime::toCookieString() const
{
   // Mon, 07-Apr-2003 22:39:21 GMT
   string buf = m_d.dayName(m_d.dayOfWeek());
   buf += ", ";
   char cp[5] ;
   sprintf(cp, "%02d", m_d.day() ) ;
   buf += cp;
   buf += "-";
   buf += m_d.monthName(m_d.month());
   buf += "-";
   sprintf(cp, "%d", m_d.year() ) ;
   buf += cp;
   buf += " ";
   buf += m_t.toString();
   buf += " GMT";
   return buf;
}

string
OFDateTime::toStringAsctime() const
{
    char cp[5] ;
    string buf = m_d.dayName(m_d.dayOfWeek());
    buf += ' ';
    buf += m_d.monthName(m_d.month());
    buf += ' ';
    sprintf(cp, "%2d", m_d.day() ) ;
    buf += cp ;
    buf += ' ';
    buf += m_t.toString();
    buf += ' ';
    sprintf(cp, "%d", m_d.year() ) ;
    buf += cp ;

    return buf;
}

ofint32 month_to_int(const char *m)
{
    if ((m[0] == 'J' || m[0] == 'j') && m[1] == 'a') return 0;
    if ((m[0] == 'F' || m[0] == 'f') && m[1] == 'e') return 1;
    if ((m[0] == 'M' || m[0] == 'm') && m[1] == 'a' && m[2] == 'r') return 2;
    if ((m[0] == 'A' || m[0] == 'a') && m[1] == 'p') return 3;
    if ((m[0] == 'M' || m[0] == 'm') && m[1] == 'a' && m[2] == 'y') return 4;
    if ((m[0] == 'J' || m[0] == 'j') && m[1] == 'u' && m[2] == 'n') return 5;
    if ((m[0] == 'J' || m[0] == 'j') && m[1] == 'u' && m[2] == 'l') return 6;
    if ((m[0] == 'A' || m[0] == 'a') && m[1] == 'u') return 7;
    if ((m[0] == 'S' || m[0] == 's') && m[1] == 'e') return 8;
    if ((m[0] == 'O' || m[0] == 'o') && m[1] == 'c') return 9;
    if ((m[0] == 'N' || m[0] == 'n') && m[1] == 'o') return 10;
    if ((m[0] == 'D' || m[0] == 'd') && m[1] == 'e') return 11;
    return -1;
}

void
OFDateTime::assignRFC1123(const char* s)
{
    if (OFOS::strlen(s) < 29)
        return;

    //           1         2
    // 01234567890123456789012345678
    // Thu, 23 Sep 2004 18:15:31 GMT

    static ofint32 zero[] = {7, 11, 16, 19, 22, 25};
    static ofint32 offsets[] = {5, 8, 12, 17, 20, 23};
    
    enum { elem_day, elem_month, elem_year, elem_hour, elem_minute, elem_second, elem_tz};
    int _day, _month, _year, _hour, _minute, _second;

    char tdate[40];
    OFOS::strncpy2(tdate, s, 40);

    tdate[zero[elem_second]] = 0; _second = atoi(tdate+offsets[elem_second]);
    tdate[zero[elem_minute]] = 0; _minute = atoi(tdate+offsets[elem_minute]);
    tdate[zero[elem_hour]] = 0; _hour = atoi(tdate+offsets[elem_hour]);

    if (_second < 0 || _second > 59 ||
        _minute < 0 || _minute > 59 ||
        _hour < 0 || _hour > 23)
        return;

    m_t.setHMS(_hour, _minute, _second);
    
    tdate[zero[elem_year]] = 0; _year = atoi(tdate+offsets[elem_year]);
    tdate[zero[elem_month]] = 0; _month = month_to_int(tdate+offsets[elem_month]) + 1;
    tdate[zero[elem_day]] = 0; _day = atoi(tdate+offsets[elem_day]);

    if (_day < 1 || _day > 31 ||
        _month < 1 || _month > 12 ||
        _year < 1)
        return;

    m_d.setYMD(_year, _month, _day);
}

void 
OFDateTime::assignISO( const char *s )
{
    const char *cp =
      m_d.assignISO( s );
    // OFTime.assign() can handle a null string
    m_t.assignISO( cp, true );
}

string
OFDateTime::toString() const
{
    return toStringISO();
}

void 
OFDateTime::assign( const char *s )
{
    assignISO( s );
}

OFDateTime OFDateTime::addDays( int ndays ) const
{
    OFDateTime x;
    x.m_d = m_d.addDays(ndays);
    x.m_t = m_t;
    return x;
}

OFDateTime OFDateTime::addSecs( int nsecs ) const
{
    long dd = m_d.m_jd;
    int  tt = m_t.m_ds;
    int  sign = 1;
    if ( nsecs < 0 )
    {
        nsecs = -nsecs;
        sign = -1;
    }
    if ( nsecs >= (int)SECS_PER_DAY )
    {
        dd += sign*(nsecs/SECS_PER_DAY);
        nsecs %= SECS_PER_DAY;
    }
    tt += sign*nsecs*1000;
    if ( tt < 0 )
    {
        tt = MSECS_PER_DAY - tt - 1;
        dd -= tt / MSECS_PER_DAY;
        tt = tt % MSECS_PER_DAY;
        tt = MSECS_PER_DAY - tt - 1;
    }
    else if ( tt >= (int)MSECS_PER_DAY )
    {
        dd += ( tt / MSECS_PER_DAY );
        tt = tt % MSECS_PER_DAY;
    }
    OFDateTime ret;
    ret.m_t.m_ds = tt;
    ret.m_d.m_jd = dd;
    return ret;
}

int OFDateTime::daysTo( const OFDateTime &dt ) const
{
    return m_d.daysTo( dt.m_d );
}

int
OFDateTime::secsTo( const OFDateTime &dt ) const
{
    return m_t.secsTo(dt.m_t) + m_d.daysTo(dt.m_d)*SECS_PER_DAY;
}

int
operator ==( const OFDateTime &lhs, const OFDateTime &rhs )
{
    return  static_cast<int>( lhs.m_t == rhs.m_t && lhs.m_d == rhs.m_d );
}

int operator!=( const OFDateTime &lhs,  const OFDateTime &rhs )
{
    return  !(lhs == rhs) ;
}

int operator<( const OFDateTime &lhs,  const OFDateTime &rhs )
{
    if ( lhs.m_d < rhs.m_d )
        return 1 ;
    return lhs.m_d == rhs.m_d ? lhs.m_t < rhs.m_t : 0;
}

int operator<=( const OFDateTime &lhs,  const OFDateTime &rhs )
{
    if ( lhs.m_d < rhs.m_d )
        return 1;
    return lhs.m_d == rhs.m_d ? lhs.m_t <= rhs.m_t : 0 ;
}

int operator>( const OFDateTime &lhs,  const OFDateTime &rhs )
{
    if ( lhs.m_d > rhs.m_d )
        return 1;
    return lhs.m_d == rhs.m_d ? lhs.m_t > rhs.m_t : 0;
}


int operator>=( const OFDateTime &lhs,  const OFDateTime &rhs )
{
    if ( lhs.m_d > rhs.m_d )
        return 1;
    return lhs.m_d == rhs.m_d ? lhs.m_t >= rhs.m_t : 0;
}

OFDateTime operator+( const OFDateTime &lhs,  const OFDateTime &rhs )
{
    OFDateTime temp ;

    temp.m_d = lhs.m_d + rhs.m_d ;
    temp.m_t = lhs.m_t + rhs.m_t ;
    return temp ;
}

OFDateTime operator-( const OFDateTime &lhs,  const OFDateTime &rhs )
{
    OFDateTime temp ;

    temp.m_d = lhs.m_d - rhs.m_d ;
    temp.m_t = lhs.m_t - rhs.m_t ;
    return temp ;
}

OFDateTime OFDateTime::currentDateTime()
{
    OFDateTime x;
    x.m_d = OFDate::currentDate();
    if ( OFTime::currentTime( &x.m_t ) )     // too close to midnight?
        x.m_d = OFDate::currentDate();     // YES! time for some midnight
    // voodoo, fetch date again
    return x;
}


#if defined(UNIT_TEST)

#include <UnitTest.h>

int
main( int argc, char *argv[] )
{
    UT_BATCH_START( "ofdatetime" );

    UT_TEST_START(1, "OFDateTime:assignRFC1123");

    OFDateTime dt1;
    dt1.assignRFC1123("Thu, 23 Sep 2004 18:15:31 GMT");

    UT_EVAL_EXPR(dt1.toString() == "2004-09-23T18:15:31Z");

    UT_TEST_END;
    UT_BATCH_END;
    return 0;
}

#endif // UNIT_TEST

#if defined(UNIT_TESTX)

#include "UnitTest.h"

int Test_OFDate()
{
    COMMENT( "Test_OFDate" ) ;
    OFDate ofd ;
    OFDate ofd1 ;

    ofd.setYMD(2010, 11,20) ;
    ofd1 = ofd.addDays(200) ;

    EXPECT_EXCEPTION(ofd.setYMD(2002, 13, 23)) ;
    EXPECT_VALUE( ofd1.daysTo(ofd) == -200 ) ;

    return 0 ;
}

int Test_OFTime()
{
    COMMENT( "Test_OFTime" ) ;

    OFTime oft( 13, 23, 30, 200) ;
    OFTime oft1(OFTime::currentTime()) ;

    long test = oft1.getLong() ;
    OFTime oft2(test) ;

    oft2.setLong(test) ;
    EXPECT_VALUE( oft1.getLong() == oft2.getLong() ) ;

    EXPECT_VALUE( oft.isValid() == true ) ;
    EXPECT_VALUE( oft1.isNull() == false ) ;
    EXPECT_VALUE( oft.setHMS(23, 58, 59, 999) == true ) ;

    return 0 ;

}

int Test_OFDateTime()
{

    COMMENT( "Test_OFDateTime" ) ;

    OFDateTime ofdt(OFDateTime::currentDateTime()) ;
    COMMENT( ofdt.toString() ) ;

    long test = ofdt.getLong() ;

    ofdt.setTime_t( test ) ;
    EXPECT_VALUE( ofdt.getLong() == test ) ;
    COMMENT( ofdt.toString() ) ;
    OFDateTime ofdt5(test) ;
    EXPECT_VALUE( ofdt5.getLong() == test ) ;

    COMMENT( "Two tests to check dates around feb and april ")

        test = (31*365+6 + 31 + 29 )*24*3600 + 13*3600 + 1 ;
    ofdt.setTime_t( test ) ;
    EXPECT_VALUE( ofdt.getLong() == test ) ;
    COMMENT( ofdt.toString() ) ;

    test = (31*365+6 + 31 + 29 + 31 + 29 )*24*3600 + 13*3600 + 1 ;
    ofdt.setTime_t( test ) ;
    EXPECT_VALUE( ofdt.getLong() == test ) ;
    COMMENT( ofdt.toString() ) ;

    EXPECT_VALUE( ofdt.isValid() == true ) ;
    EXPECT_VALUE( ofdt.isNull() == false ) ;

    COMMENT( "Constructors" ) ;

    OFDate ofd(OFDate::currentDate()) ;
    OFTime oft(OFTime::currentTime()) ;

    OFDateTime ofdt1(ofd) ;
    OFDateTime ofdt3(ofd, oft) ;

    EXPECT_VALUE( ofdt1.date() == ofd ) ;
    EXPECT_VALUE( ofdt3.time() == oft ) ;

    ofdt.setDate( ofd ) ;
    EXPECT_VALUE( ofdt.date() == ofd ) ;
    ofdt.setTime( oft ) ;
    EXPECT_VALUE( ofdt.time() == oft ) ;

    COMMENT( " Assignment operator and Equality operator" )

        OFDateTime ofdt4 ;
    ofdt4 = ofdt ;

    EXPECT_VALUE( ofdt4 == ofdt ) ;

    ofdt = OFDateTime::currentDateTime() ;
    ofdt4.setTime_t( 123456789 ) ;

    COMMENT( " Relational and equality operators" ) ;
    COMMENT( " ofdt4 = " << ofdt4.toString() ) ;
    COMMENT( " ofdt = " << ofdt.toString() ) ;

    EXPECT_VALUE( (ofdt4 == ofdt) == false ) ;
    EXPECT_VALUE( (ofdt4 != ofdt) == 1 ) ;
    EXPECT_VALUE( (ofdt4 < ofdt) == 1 ) ;
    EXPECT_VALUE( (ofdt4 > ofdt) == false ) ;
    EXPECT_VALUE( (ofdt4 <= ofdt) == 1 ) ;
    EXPECT_VALUE( (ofdt4 >= ofdt) == false ) ;

    COMMENT( " Adding days and seconds " )
        ofdt = ofdt4.addDays(3) ;
    EXPECT_VALUE( (ofdt.daysTo(ofdt4) == -3) ) ;

    ofdt = ofdt4.addSecs(13) ;
    EXPECT_VALUE( (ofdt4.secsTo(ofdt) == 13)) ;

    OFDateTime ofdt6(LONG_MAX) ;
    COMMENT( " Max date/time = " << ofdt6.toString() ) ;
    return 0 ;
}

int main(int argc, char *argv[] )
{
    // -- time stamp

    UNIT_TEST_START("ofdatetime");

    try
    {

        Test_OFDate()  ;

        Test_OFTime() ;

        Test_OFDateTime()  ;

    }
    catch (Exception& e)
    {
        cout << "\n\nFAILURE uncaught exception: " << e.message() << "\n";
    }
    catch (std::exception& e)
    {
        cout << "\n\nFAILURE uncaught std::exception: " << e.what() << "\n";
    }

    UNIT_TEST_END("ofdatetime");

    cout << "\n sizeof(unsigned int) = " << sizeof(unsigned int) << " ; sizeof(long) = " << sizeof(long) << "\n" << endl ;

    return 0 ;
}


#endif // UNIT_TESTS
