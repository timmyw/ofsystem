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

#if !defined( OF_DATETIME_H_)
#define OF_DATETIME_H_

#include <ofsys.h>

/**
 * \brief
 *  The OFDate is based on the Gregorian (modern western) calendar.
 *
 *  This calendar was first adopted by England in 1752, and subsequently by
 *  most other western countries.  OFDate computes the wrong day of week
 *  for all dates earlier than 14th September 1752, and the day it
 *  computes is correct only in some countries for years between 1752
 *  and 1923.
 */
class OFSYS_API OFDate
{
 public:
    /** Sets the date to null.
     */
    void clear();

    /** Returns TRUE if the date is null.  A null date is invalid.*/
    bool isNull() const ;

    /** Returns TRUE if the date is valid.*/
    bool isValid() const;

    /** Returns the year (>= 1752) for this date. */
    int year() const;

    /** Returns the day of the month (1..31) for this date. */
    int month() const;         // 1..12
                     
    /** Returns the day of the week (1..7 (monday==1)) for this
        date. */
    int day() const;

    /** Returns the weekday (Monday=1 .. Sunday=7) for this date. */
    int dayOfWeek() const;

    /** Returns the day of the year (1..365) for this date.*/
    int dayOfYear() const;

    /** Returns the number of days in the month (28..31) for this
        date.*/
    int daysInMonth() const;

    /** Returns the number of days in the year (365 or 366) for this
        date.*/
    int daysInYear() const;

    /** Returns the name of the  Month 1 == "Jan", month 2 == "Feb" etc. */
    std::string monthName( int month ) const;

    /** Returns the name of the weekday. Weekday 1 == "Mon", day 2 ==
        "Tue" etc. */
    std::string dayName( int weekday ) const;

    /** get the components of the date in one call
	
        \todo merge 64bit implementation
    */
    void getGregorian( ofint32 *y, ofint32 *m, ofint32 *d ) const;

    /** \return the date in the format CCYY-MM-DD
     */
    std::string toStringISO() const;

    /** Returns the date as a string.The string format is "Sat May 20
        1995".*/
    std::string toString() const;

    /** Returns pointer to the character after the last character it
        consumed (accepted)

        Time could continue reading from here.
    */
    const char *assignISO( const char * );

    /** Sets the year y, month m and day d.
        
        Returns TRUE if the date is valid, otherwise FALSE.*/
    bool setYMD( int y, int m, int d );

    /** Returns this date plus ndays days.*/
    OFDate addDays( int days ) const;


    /** Returns the number of days from this date to d - negative if d
        is in the past.

       OFDate d1( 1995, 5, 17 );       // May 17th 1995
       OFDate d2( 1995, 5, 20 );       // May 20th 1995
       d1.daysTo( d2 );            // returns 3
       d2.daysTo( d1 );            // returns -3 */
    int daysTo( const OFDate &d ) const;

    friend int operator ==( const OFDate &lhs, const OFDate &rhs ) ;
    friend int operator !=( const OFDate &lhs, const OFDate &rhs ) ;
    friend int operator <( const OFDate &lhs, const OFDate &rhs ) ;
    friend int operator <=( const OFDate &lhs, const OFDate &rhs ) ;
    friend int operator >( const OFDate &lhs, const OFDate &rhs ) ;
    friend int operator >=( const OFDate &lhs, const OFDate &rhs ) ;

    /**
       Arithmetic operators +, - ; The / and * operators do not make sense */
    friend OFDate operator +( const OFDate &lhs, const OFDate &rhs ) ;
    friend OFDate operator -( const OFDate &lhs, const OFDate &rhs ) ;

    /**
       Returns the current date */
    static OFDate currentDate();

    /**
       Returns TRUE if the specified date is valid.
       Note that years 00-99 are treated as 1900-1999.*/
    static bool  isValid( int y, int m, int d );

    /**
       Returns TRUE if the specified year y is a leap year.*/
    static bool  leapYear( int year );

 protected:
    /**
       Converts a Gregorian date to a Julian day.
       This algorithm is taken from Communications of the ACM, Vol 6, No 8.*/
    static long  greg2jul( int y, int m, int d );

    /**
       Converts a Julian day to a Gregorian date.
       This algorithm is taken from Communications of the ACM, Vol 6, No 8.*/
    static void  jul2greg( long jd, int &y, int &m, int &d );

 private:
    static const char *monthNames[];
    static const char *weekdayNames[];
    long     m_jd;
    friend class OFDateTime;
};

/**
 * \brief
 * The OFTime class provides a time functions 24 hours a day.
 *
 * The time resolution of OFTime is a millisecond, although the accuracy
 * depends on the underlying operating system.  Some operating systems
 * (e.g. Linux and Window NT) support a one-millisecond resolution.
 */
class OFSYS_API OFTime
{
 public:
    /** Sets time equal to 00:00:00.000 UTC.
    */
    void clear();

    /**
       Returns TRUE if the time is equal to 00:00:00.000. A null time is valid.*/
    bool   isNull()  const  ;

    /**
       Returns TRUE if the time is valid, or FALSE if the time is invalid.
       The time 23:30:55.746 is valid, while 24:12:30 is invalid.*/
    bool   isValid()     const;

    /**
       Returns the hour part (0..23) of the time.*/
    int    hour()    const;
    /**
       Returns the minute part (0..59) of the time.*/
    int    minute()  const;
    /**
       Returns the second part (0..59) of the time.*/
    int    second()  const;
    /**
       Returns the millisecond part (0..999) of the time.*/
    int    msec()    const;

    /** uses the format "%.2d:%.2d:%.2d.%-.3d" 
	milliseconds will only be present if non-zero
	\todo 64 bit implementation includes timezone offset
	\param z = include the timezone in the returned string
     */
    std::string toStringISO( bool z = true ) const;

    /**
       Converts the date to a string, which is returned.  Milliseconds are
       not included. The string format is "03:40:13".*/
    std::string toString()   const;

    /** Reads the time from a c string
        atTimeChars can be used to indicate that the method doesn't need to
        skip a date portion to find the time.
        By default, assumes time starts after last time separator 'T',
        ( or space ) immediately followed by a digit or sign.

        \todo handle time zone
    */
    const char *assignISO( const char *s, bool atTimeChars=false );

    /**
       Sets the hour h, minute  m, seconds s and milliseconds ms.
       Returns TRUE if the time is valid, otherwise FALSE.*/
    bool   setHMS( int h, int m, int s, int ms=0 );

    /**
       Returns the time plus n secs seconds.*/
    OFTime  addSecs( int secs )     const;

    /**
       Returns the number of seconds from this time to t (which is
       negative if t is in the past).
       Since OFTime measures time within a day and there are 86400 seconds
       in a day, the result is between -86400 and 86400.*/
    int    secsTo( const OFTime & ) const;

    /**
       Returns the time plus ms milliseconds.*/
    OFTime  addMSecs( int ms )      const;

    /**
       Returns the number of milliseconds between this time and t.*/
    int    msecsTo( const OFTime &t )   const;

    /**
       Returns the current time.*/
    static OFTime currentTime();

    /**
       Fetches the current time and returns TRUE if the time is within one
       minute after midnight, otherwise FALSE. The return value is used by
       OFDateTime::currentDateTime() to ensure that the date there is correct.
       OFTime::isValid(21, 10, 30);        // returns TRUE
       OFTime::isValid(22, 5,  62);        // returns FALSE*/
    static bool  isValid( int h, int m, int s, int ms=0 );

    /**
       Sets the time to the current time, e.g. for timing:*/
    void   start();

    /**
       Sets *this to the current time, and returns the number of
       milliseconds that have elapsed since the last start() or restart().
       restart is guaranteed to be atomic, and so is very handy for
       repeated measurements; call start() to start the first measurement,
       then restart() for each later measurement.
       Note that the counter wraps to zero 24 hours after the last call to
       start() or restart().
       warning If the system's local time changes, the result is undefined.
       This can happen e.g. when daylight saving is turned on or off.*/
    int    restart();

    /**
       Returns the number of milliseconds that have elapsed since start() or
       restart() were called.*/
    int    elapsed();

    /**
       Sets the current stored time as long in msecs   */
    void setLong(long value)  ;

    /**
       Returns the current stored time as long */
    long getLong() const  ;

    friend int operator ==(const OFTime& lhs, const OFTime& rhs) ;
    friend int operator!=( const OFTime& lhs, const OFTime& rhs ) ;
    friend int operator<( const OFTime& lhs, const OFTime& rhs );
    friend int operator<=( const OFTime& lhs, const OFTime& rhs ) ;
    friend int operator>( const OFTime& lhs, const OFTime& rhs ) ;
    friend int operator>=( const OFTime& lhs, const OFTime& rhs ) ;

    /**
        Arithmetic operators +, - ; The / and * operators do not make sense */
    friend OFTime operator+( const OFTime& lhs, const OFTime& rhs ) ;
    friend OFTime operator-( const OFTime& lhs, const OFTime& rhs ) ;

 private:

    static bool currentTime( OFTime * );

    ofuint32 m_ds;
    friend class OFDateTime;
};


/**
 * \brief
 * The OFDateTime class combines OFDate and OFTime into a single class.
 *
 * OFDateTime provides high precision date and time functions since it can work
 * with Gregorian dates up to about year 8000.
 * Most countries that use the Gregorian calendar switched to it between 1550
 * and 1920.
 */
class OFSYS_API OFDateTime
{
 public:

    /** Returns TRUE if both the date and the time are null.  A null
        date is invalid.*/
    bool isNull() const ;

    /** Returns TRUE if both the date and the time are value.*/
    bool isValid() const ;

    /**
       Returns the date part of this datetime.*/
    OFDate  date()  const ;

    /**
       Returns the time part of this datetime.*/
    OFTime  time()  const ;

    /**
       Sets the date part of this datetime.*/
    void   setDate( const OFDate &date )  ;

    /**
       Sets the time part of this datetime.*/
    void   setTime( const OFTime &time )  ;

    /**
       Sets the local date and time given the number of seconds that have passed
       since 00:00:00 on January 1, 1970, Coordinated Universal Time (UTC).
       On systems that do not support timezones this function will behave as if
       local time were UTC.
       Note that Microsoft Windows supports only a limited range of values
       for secsSince1Jan1970UTC. This has been implemented using gmtime rather
       than localtime (from the include std::time) which caused problems over BST.*/
    void   setTime_t( long secsSince1Jan1970UTC );

    /**
       Returns the OFDateTime as a long in seconds */
    long getLong() const ;

    /**
       Sets the data/time from 64-bit integer
    */
    void setDateTime( ofuint64 value);

    /**
       returns date + time as a 64-bit integer
    */
    ofuint64 getDateTime() const;

    /**
       Returns the datetime as a string.
       The string format is from ISO8601
    */
    std::string toString()  const;

    /**
       Returns the datetime as a string.
       The string format is the ANSI c acstime() format
       "Sat May 20 03:40:13 1998" - ignores milliseconds.
    */
    std::string toStringAsctime()  const;

    /** Return the datetime in cookie format
        \warning This assumes that the time is GMT.
    */
    std::string toCookieString() const;

    /** Return the datetime in RFC1123 format.
        This format is required by HTTP Expires (and other header fields)
    */
    std::string toRFC1123() const;

    /** \return the datetime in the ISO format
	will become the default for toString, but I don't want to break things right now
    */
    std::string toStringISO() const;

    /** assign an ISO format date time string
    */
    void assignISO( const char *s );

    /** Assign an RFC1123 format date

        Thu, 23 Sep 2004 18:15:31 GMT
     */
    void assignRFC1123(const char* s);

    /** assign an ISO format date time string
    */
    void assign( const char *s );

    /**
       Returns the datetime plus ndays days.*/
    OFDateTime addDays( int days )  const;

    /**
       Returns the datetime plus nsecs seconds.*/
    OFDateTime addSecs( int secs )  const;

    /**
       Returns the number of days from this datetime to dt, which is
       negative if dt is in the past.*/
    int    daysTo( const OFDateTime & ) const;


    /**
       Returns the number of seconds from this datetime to dt, which is
       negative if t is in the past.
       OFDateTime dt = OFDateTime::currentDateTime();
       OFDateTime x( OFDate(dt.year(),12,24), OFTime(17,00) );*/
    int    secsTo( const OFDateTime & ) const;


    /**
       Friend relational and equality operators for OFDateTime*/

    friend int OFSYS_API operator==( const OFDateTime &lhs, const OFDateTime &rhs) ;
    friend int OFSYS_API operator!=( const OFDateTime &lhs, const OFDateTime &rhs) ;
    friend int OFSYS_API operator<( const OFDateTime &lhs, const OFDateTime &rhs)  ;
    friend int OFSYS_API operator<=( const OFDateTime &lhs, const OFDateTime &rhs) ;
    friend int OFSYS_API operator>( const OFDateTime &lhs, const OFDateTime &rhs)  ;
    friend int OFSYS_API operator>=( const OFDateTime &lhs, const OFDateTime &rhs) ;

    /**
        Arithmetic operators +, - ; The / and * operators do not make sense */
    friend OFDateTime operator+( const OFDateTime &lhs, const OFDateTime &rhs) ;
    friend OFDateTime operator-( const OFDateTime &lhs, const OFDateTime &rhs) ;

    static OFDateTime currentDateTime();

 private:
    OFDate  m_d;
    OFTime  m_t;
};

#endif // #if !defined( OF_DATETIME_H_ )
