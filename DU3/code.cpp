#ifndef __PROGTEST__
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <stdexcept>
using namespace std;
#endif /* __PROGTEST__ */

//=================================================================================================
// a dummy exception class, keep this implementation
class InvalidDateException : public invalid_argument
{
  public:
    InvalidDateException ( )
      : invalid_argument ( "invalid date or format" )
    {
    }
};
//=================================================================================================
// date_format manipulator - a dummy implementation. Keep this code unless you implement your
// own working manipulator.
ios_base & ( * date_format ( const char * fmt ) ) ( ios_base & x )
{
  return [] ( ios_base & ios ) -> ios_base & { return ios; };
}
//=================================================================================================
class CDate
{
  public:

    CDate ( int year, int month, int day ) 
    :m_day(day),
     m_month(month),
     m_year(year)
    {
      if (!dateCheck( year, month ,day ) )
        throw InvalidDateException::invalid_argument ( "invalid date or format" );
    }
    ~CDate ( void ) {}
//=============================================================================================
    friend CDate operator + ( const CDate & date, const int day ) {

      int curr_day = date.m_day;
      int curr_year = date.m_year;
      int curr_month = date.m_month;

      CDate new_date(2000, 1, 1);

      if ( day >= 0 ) {
        for ( int i = 0; i < day; i++ ) {
          curr_day++;
          if (!dateCheck ( curr_year, curr_month, curr_day ) ) {
            if ( curr_month == 12 ) {
              curr_year++;
              curr_month = 1;
              curr_day = 1;
            }
            else {
              curr_month++;
              curr_day = 1;
            }
          }
        }
      }
      else {
        for ( int i = 0; i > day; i-- ) {
          curr_day--;
          if (!dateCheck ( curr_year, curr_month, curr_day ) ) {
            if ( curr_month == 1 ) {
              curr_year--;
              curr_month = 12;
              curr_day = 31;
            }
            else {
              curr_month--;
              if ( curr_month == 2 && isLeap( curr_year ) )
                curr_day = 29;
              else if ( curr_month == 2 && !isLeap( curr_year ) )
                curr_day = 28;
              else if ( curr_month == 1 || curr_month == 3 || curr_month == 5 || curr_month == 7 || curr_month == 8 || curr_month == 10 || curr_month == 12 )
                curr_day = 31;
              else 
                curr_day = 30;
            }
          }
        }
      }

      new_date.m_year = curr_year;
      new_date.m_month = curr_month;
      new_date.m_day = curr_day;

      return new_date;
    }
    //=============================================================================================
    friend CDate operator - ( const CDate & date, const int day ) {
      CDate new_date ( 2000, 1, 1 );
      int new_day;

      if ( day < 0 ) {
        new_day = day*(-1);
        new_date = date + new_day;
      }
      else {
        new_date = date + -day;
      }

      return new_date;
    }
    //=============================================================================================
    friend int operator - ( const CDate date1, const CDate date2 ) {
      CDate curr_date = date1;
      int counter = 0;

      if (date1 == date2)
        return 0;

      if ( date1 > date2 ) {
        while ( curr_date != date2 ) {
          curr_date = curr_date - 1;
          counter++;
        }
      } 
      else {
        while ( curr_date != date2 ) {
          curr_date = curr_date + 1;
          counter++;
        }
      }
      return counter;
    }
    //=============================================================================================
    CDate operator   ++ ( int n ) {
      CDate old = *this;
      if ( n == 0 )
		    *this = *this + 1;
	    else
		    *this = *this + n;
	    return old;
    }
    //=============================================================================================
    CDate operator   -- ( int n ) {
      CDate old = *this;
      *this = *this - 1;
      return old;
    }
    //=============================================================================================
    CDate & operator ++ ( void ) {
      return *this = *this + 1;
    }
    //=============================================================================================
    CDate & operator -- ( void ) {
      return *this = *this - 1;
    }
    //=============================================================================================
    friend bool operator == ( const CDate date1, const CDate date2 ) {
      if ( (date1.m_year == date2.m_year) && (date1.m_day == date2.m_day) && (date1.m_month == date2.m_month) )
        return true;
      return false;
    }
    //=============================================================================================
    friend bool operator != ( const CDate date1, const CDate date2 ) {
      if ( date1 == date2 )
        return false;
      return true;
    }
    //=============================================================================================
    friend bool operator >  ( const CDate date1, const CDate date2 ) {
      if ( date1.m_year > date2.m_year )
        return true;
      else if ( date1.m_year < date2.m_year )
        return false;
      else if ( date1.m_month > date2.m_month )
        return true;
      else if ( date1.m_month < date2.m_month )
        return false;
      else if ( date1.m_day > date2.m_day )
        return true;
      return false;
    }
    //=============================================================================================
    friend bool operator <  ( const CDate date1, const CDate date2 ) {
      return date2 > date1;
    }
    //=============================================================================================
    friend bool operator >= ( const CDate date1, const CDate date2 ) {
      if ( date1 == date2 )
        return true;
      else if ( date1 > date2 )
        return true;
      return false;
    }
    //=============================================================================================
    friend bool operator <= ( const CDate date1, const CDate date2 ) {
      if ( date1 == date2 )
        return true;
      if ( date1 < date2 )
        return true;
      return false;
    }
    //=============================================================================================
    friend std::ostream & operator << ( std::ostream & os, const CDate & date ) {
      int year = date.m_year;
      int month = date.m_month;
      int day = date.m_day;

      os << year << "-" << setw(2) << setfill('0') << month << "-" << setw(2) << setfill('0') << day;
      return os;
    }
    //=============================================================================================
    friend std::istream & operator >> ( std::istream & is, CDate & date ) {
      
      int year = 0;
      int month = 0;
      int day = 0;

      is >> year;
      
      if ( is && is.peek() == '-' ) {
        is.ignore(1,2) >> month;
        if (  is && is.peek() == '-')
          is.ignore(1,2) >> day;
      }
      
      if ( !dateCheck( year, month ,day ) )
        is.setstate(std::ios::failbit);
      else {
        date.m_year = year;
        date.m_month = month;
        date.m_day = day;
      }
      
      return is;
    }
    //=============================================================================================
    void printDate ( void ) const {
      cout << m_year << "-" << m_month << "-" << m_day << endl; 
    }
    //=============================================================================================
  private:

    int m_day;
    int m_month;
    int m_year;

    static int leapYearCounter (int year) {
      return year / 4 + year / 400 - year / 100 - year / 4000;
    }

    static int isLeap ( int year) {
      if ( year % 4 == 0
      && ( year % 100 != 0 || year % 400 == 0 ) && ( year % 4000 != 0) ){
        return 1;
      }
      else {
        return 0;
      }
    }

    static bool dateCheck ( int year, int month, int day ) {
      int leap = isLeap( year );

      if ( (month == 1 && day > 31)
      || ( month == 2 && leap == 0 && day > 28 )
      || ( month == 2 && leap == 1 && day > 29 )
      || ( month == 3 && day > 31 )
      || ( month == 4 && day > 30 )
      || ( month == 5 && day > 31 )
      || ( month == 6 && day > 30 )
      || ( month == 7 && day > 31 )
      || ( month == 8 && day > 31 )
      || ( month == 9 && day > 30 )
      || ( month == 10 && day > 31  )
      || ( month == 11 && day > 30  )
      || ( month == 12 && day > 31  )
      || ( month > 12 || month < 1  )
      || ( day < 1 )
      || ( year < 2000 || year > 2030) )
      {
        return false;
      }

      return true;
    }

    
};

#ifndef __PROGTEST__
int main ( void )
{
  ostringstream oss;
  istringstream iss;

  CDate a ( 2000, 1, 2 );
  CDate b ( 2010, 2, 3 );
  CDate c ( 2004, 2, 10 );
  oss . str ("");
  oss << a;
  assert ( oss . str () == "2000-01-02" );
  oss . str ("");
  oss << b;
  assert ( oss . str () == "2010-02-03" );
  oss . str ("");
  oss << c;
  assert ( oss . str () == "2004-02-10" );
  a = a + 1500;
  oss . str ("");
  oss << a;
  assert ( oss . str () == "2004-02-10" );
  b = b - 2000;
  oss . str ("");
  oss << b;
  assert ( oss . str () == "2004-08-13" );
  assert ( b - a == 185 );
  assert ( ( b == a ) == false );
  assert ( ( b != a ) == true );
  assert ( ( b <= a ) == false );
  assert ( ( b < a ) == false );
  assert ( ( b >= a ) == true );
  assert ( ( b > a ) == true );
  assert ( ( c == a ) == true );
  assert ( ( c != a ) == false );
  assert ( ( c <= a ) == true );
  assert ( ( c < a ) == false );
  assert ( ( c >= a ) == true );
  assert ( ( c > a ) == false );
  a = ++c;
  oss . str ( "" );
  oss << a << " " << c;
  assert ( oss . str () == "2004-02-11 2004-02-11" );
  a = --c;
  oss . str ( "" );
  oss << a << " " << c;
  assert ( oss . str () == "2004-02-10 2004-02-10" );
  a = c++;
  oss . str ( "" );
  oss << a << " " << c;
  assert ( oss . str () == "2004-02-10 2004-02-11" );
  a = c--;
  oss . str ( "" );
  oss << a << " " << c;
  assert ( oss . str () == "2004-02-11 2004-02-10" );
  iss . clear ();
  iss . str ( "2015-09-03" );
  assert ( ( iss >> a ) );
  oss . str ("");
  oss << a;
  assert ( oss . str () == "2015-09-03" );
  a = a + 70;
  oss . str ("");
  oss << a;
  assert ( oss . str () == "2015-11-12" );

  CDate d ( 2000, 1, 1 );
  try
  {
    CDate e ( 2000, 32, 1 );
    assert ( "No exception thrown!" == nullptr );
  }
  catch ( ... )
  {
  }
  iss . clear ();
  iss . str ( "2000-12-33" );
  assert ( ! ( iss >> d ) );
  oss . str ("");
  oss << d;
  assert ( oss . str () == "2000-01-01" );
  iss . clear ();
  iss . str ( "2000-11-31" );
  assert ( ! ( iss >> d ) );
  oss . str ("");
  oss << d;
  assert ( oss . str () == "2000-01-01" );
  iss . clear ();
  iss . str ( "2000-02-29" );
  assert ( ( iss >> d ) );
  oss . str ("");
  oss << d;
  assert ( oss . str () == "2000-02-29" );
  iss . clear ();
  iss . str ( "2001-02-29" );
  assert ( ! ( iss >> d ) );
  oss . str ("");
  oss << d;
  assert ( oss . str () == "2000-02-29" );

  //-----------------------------------------------------------------------------
  // bonus test examples
  //-----------------------------------------------------------------------------
  CDate f ( 2000, 5, 12 );
  oss . str ("");
  oss << f;
  assert ( oss . str () == "2000-05-12" );
  oss . str ("");
  oss << date_format ( "%Y/%m/%d" ) << f;
  assert ( oss . str () == "2000/05/12" );
  oss . str ("");
  oss << date_format ( "%d.%m.%Y" ) << f;
  assert ( oss . str () == "12.05.2000" );
  oss . str ("");
  oss << date_format ( "%m/%d/%Y" ) << f;
  assert ( oss . str () == "05/12/2000" );
  oss . str ("");
  oss << date_format ( "%Y%m%d" ) << f;
  assert ( oss . str () == "20000512" );
  oss . str ("");
  oss << date_format ( "hello kitty" ) << f;
  assert ( oss . str () == "hello kitty" );
  oss . str ("");
  oss << date_format ( "%d%d%d%d%d%d%m%m%m%Y%Y%Y%%%%%%%%%%" ) << f;
  assert ( oss . str () == "121212121212050505200020002000%%%%%" );
  oss . str ("");
  oss << date_format ( "%Y-%m-%d" ) << f;
  assert ( oss . str () == "2000-05-12" );
  iss . clear ();
  iss . str ( "2001-01-1" );
  assert ( ! ( iss >> f ) );
  oss . str ("");
  oss << f;
  assert ( oss . str () == "2000-05-12" );
  iss . clear ();
  iss . str ( "2001-1-01" );
  assert ( ! ( iss >> f ) );
  oss . str ("");
  oss << f;
  assert ( oss . str () == "2000-05-12" );
  iss . clear ();
  iss . str ( "2001-001-01" );
  assert ( ! ( iss >> f ) );
  oss . str ("");
  oss << f;
  assert ( oss . str () == "2000-05-12" );
  iss . clear ();
  iss . str ( "2001-01-02" );
  assert ( ( iss >> date_format ( "%Y-%m-%d" ) >> f ) );
  oss . str ("");
  oss << f;
  assert ( oss . str () == "2001-01-02" );
  iss . clear ();
  iss . str ( "05.06.2003" );
  assert ( ( iss >> date_format ( "%d.%m.%Y" ) >> f ) );
  oss . str ("");
  oss << f;
  assert ( oss . str () == "2003-06-05" );
  iss . clear ();
  iss . str ( "07/08/2004" );
  assert ( ( iss >> date_format ( "%m/%d/%Y" ) >> f ) );
  oss . str ("");
  oss << f;
  assert ( oss . str () == "2004-07-08" );
  iss . clear ();
  iss . str ( "2002*03*04" );
  assert ( ( iss >> date_format ( "%Y*%m*%d" ) >> f ) );
  oss . str ("");
  oss << f;
  assert ( oss . str () == "2002-03-04" );
  iss . clear ();
  iss . str ( "C++09format10PA22006rulez" );
  assert ( ( iss >> date_format ( "C++%mformat%dPA2%Yrulez" ) >> f ) );
  oss . str ("");
  oss << f;
  assert ( oss . str () == "2006-09-10" );
  iss . clear ();
  iss . str ( "%12%13%2010%" );
  assert ( ( iss >> date_format ( "%%%m%%%d%%%Y%%" ) >> f ) );
  oss . str ("");
  oss << f;
  assert ( oss . str () == "2010-12-13" );

  CDate g ( 2000, 6, 8 );
  iss . clear ();
  iss . str ( "2001-11-33" );
  assert ( ! ( iss >> date_format ( "%Y-%m-%d" ) >> g ) );
  oss . str ("");
  oss << g;
  assert ( oss . str () == "2000-06-08" );
  iss . clear ();
  iss . str ( "29.02.2003" );
  assert ( ! ( iss >> date_format ( "%d.%m.%Y" ) >> g ) );
  oss . str ("");
  oss << g;
  assert ( oss . str () == "2000-06-08" );
  iss . clear ();
  iss . str ( "14/02/2004" );
  assert ( ! ( iss >> date_format ( "%m/%d/%Y" ) >> g ) );
  oss . str ("");
  oss << g;
  assert ( oss . str () == "2000-06-08" );
  iss . clear ();
  iss . str ( "2002-03" );
  assert ( ! ( iss >> date_format ( "%Y-%m" ) >> g ) );
  oss . str ("");
  oss << g;
  assert ( oss . str () == "2000-06-08" );
  iss . clear ();
  iss . str ( "hello kitty" );
  assert ( ! ( iss >> date_format ( "hello kitty" ) >> g ) );
  oss . str ("");
  oss << g;
  assert ( oss . str () == "2000-06-08" );
  iss . clear ();
  iss . str ( "2005-07-12-07" );
  assert ( ! ( iss >> date_format ( "%Y-%m-%d-%m" ) >> g ) );
  oss . str ("");
  oss << g;
  assert ( oss . str () == "2000-06-08" );
  iss . clear ();
  iss . str ( "20000101" );
  assert ( ( iss >> date_format ( "%Y%m%d" ) >> g ) );
  oss . str ("");
  oss << g;
  assert ( oss . str () == "2000-01-01" );

  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
