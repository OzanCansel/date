#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "date.hpp"
#include <sstream>

TEST_CASE( "date::is_leap( int year )" )
{
    REQUIRE( !project::date::is_leap( 2001 ) );
    REQUIRE(  project::date::is_leap( 2000 ) );
}

TEST_CASE( "date::date()" )
{
    project::date d;

    REQUIRE( d.year()      == 1900 );
    REQUIRE( d.month()     == 1    );
    REQUIRE( d.month_day() == 1    );
}

TEST_CASE( "date::date( int , int , int )" )
{
    project::date d { 5 , 6 , 2000 };

    REQUIRE( d.month_day() == 5    );
    REQUIRE( d.month()     == 6    );
    REQUIRE( d.year()      == 2000 );
}

TEST_CASE( "date::date( std::string_view )" )
{
    project::date d { "15/05/2022" };

    REQUIRE( d.month_day() == 15   );
    REQUIRE( d.month()     == 5    );
    REQUIRE( d.year()      == 2022 );
}

TEST_CASE( "date::date( std::time_t )" )
{
    std::time_t current;
    std::time( &current );

    std::tm* timeinfo = std::localtime( &current );

    timeinfo->tm_year = 120;
    timeinfo->tm_mon  = 1;
    timeinfo->tm_mday = 15;

    auto time = std::mktime( timeinfo );

    project::date d { time };

    REQUIRE( d.year()      == 2020 );
    REQUIRE( d.month()     == 2    );
    REQUIRE( d.month_day() == 15   );
}

TEST_CASE( "int date::year_day() const" )
{
    project::date x1 { 9 , 6 , 2022 };
    project::date x2 { 9 , 6 , 2000 };
    project::date x3 { 9 , 2 , 2000 };
    project::date x4 { 9 , 1 , 2005 };

    REQUIRE( x1.year_day() == 160 );
    REQUIRE( x2.year_day() == 161 );
    REQUIRE( x3.year_day() == 40  );
    REQUIRE( x4.year_day() == 9   );
}

TEST_CASE( "day date::week_day() const" )
{
    project::date x1 { 11 , 2  , 1978 };
    project::date x2 { 9  , 6  , 2022 };
    project::date x3 { 4  , 11 , 2001 };
    project::date x4 { 17 , 8  , 2564 };

    REQUIRE( x1.week_day() == project::date::day::saturday );
    REQUIRE( x2.week_day() == project::date::day::thursday );
    REQUIRE( x3.week_day() == project::date::day::sunday   );
    REQUIRE( x4.week_day() == project::date::day::friday   );
}

TEST_CASE( "date& date::set_month_day( int )" )
{
    project::date x1 { 11 , 2 , 1978 };
    project::date x2 { 11 , 2 , 2000 };

    x1.set_month_day( 24 );
    x2.set_month_day( 29 );

    REQUIRE( x1.month_day() == 24 );
    REQUIRE( x2.month_day() == 29 );
}

TEST_CASE( "date& date::set_month( int )" )
{
    project::date x { 11 , 2 , 1978 };

    x.set_month( 5 );

    REQUIRE( x.month() == 5 );
}

TEST_CASE( "date& date::set_year( int )" )
{
    project::date x { 11 , 2 , 1978 };

    x.set_year( 2005 );

    REQUIRE( x.year() == 2005 );
}

TEST_CASE( "date& date::set( int day , int month , int year )" )
{
    project::date x { 11 , 2 , 1978 };

    x.set( 12 , 3 , 1979 );

    REQUIRE( x.month_day() == 12   );
    REQUIRE( x.month()     == 3    );
    REQUIRE( x.year()      == 1979 );
}

TEST_CASE( "date date::operator+( int day )" )
{
    using namespace project;

    date x1  { 1  , 1 , 2000 };
    date x2  { 24 , 8 , 2022 };

    date y11 { x1 + 366    };
    date y21 { x1 + 4565   };
    date y31 { x1 + 5      };
    date y41 { x1 + 25     };
    date y51 { x1 + 97     };
    date y61 { x1 + 1      };
    date y71 { x1 + 0      };
    date y81 { x1 + -1     };
    date y91 { x1 + -55    };
    date y12 { x2 + 95332  };

    REQUIRE( y11 == date { 1  , 1  , 2001 } );
    REQUIRE( y21 == date { 1  , 7  , 2012 } );
    REQUIRE( y31 == date { 6  , 1  , 2000 } );
    REQUIRE( y41 == date { 26 , 1  , 2000 } );
    REQUIRE( y51 == date { 7  , 4  , 2000 } );
    REQUIRE( y61 == date { 2  , 1  , 2000 } );
    REQUIRE( y71 == date { 1  , 1  , 2000 } );
    REQUIRE( y81 == date { 31 , 12 , 1999 } );
    REQUIRE( y91 == date { 7  , 11 , 1999 } );
    REQUIRE( y12 == date { 28 , 8  , 2283 } );
}

TEST_CASE( "date date::operator+=( int day )" )
{
    using namespace project;

    REQUIRE( ( date { 1  , 1 , 2000 } += 366   ) == date { 1  , 1  , 2001 } );
    REQUIRE( ( date { 1  , 1 , 2000 } += 4565  ) == date { 1  , 7  , 2012 } );
    REQUIRE( ( date { 1  , 1 , 2000 } += 5     ) == date { 6  , 1  , 2000 } );
    REQUIRE( ( date { 1  , 1 , 2000 } += 25    ) == date { 26 , 1  , 2000 } );
    REQUIRE( ( date { 1  , 1 , 2000 } += 97    ) == date { 7  , 4  , 2000 } );
    REQUIRE( ( date { 1  , 1 , 2000 } += 1     ) == date { 2  , 1  , 2000 } );
    REQUIRE( ( date { 1  , 1 , 2000 } += 0     ) == date { 1  , 1  , 2000 } );
    REQUIRE( ( date { 1  , 1 , 2000 } += -1    ) == date { 31 , 12 , 1999 } );
    REQUIRE( ( date { 1  , 1 , 2000 } += -55   ) == date { 7  , 11 , 1999 } );
    REQUIRE( ( date { 24 , 8 , 2022 } += 95332 ) == date { 28 , 8  , 2283 } );
}

TEST_CASE( "date date::operator-( int day )" )
{
    using namespace project;

    date x1  { 1  , 1 , 2000 };
    date x2  { 24 , 8 , 2022 };

    date y11 { x1 - ( -366   ) };
    date y21 { x1 - ( -4565  ) };
    date y31 { x1 - ( -5     ) };
    date y41 { x1 - ( -25    ) };
    date y51 { x1 - ( -97    ) };
    date y61 { x1 - ( -1     ) };
    date y71 { x1 - ( -0     ) };
    date y81 { x1 - ( +1     ) };
    date y91 { x1 - ( +55    ) };
    date y12 { x2 - ( -95332 ) };

    REQUIRE( y11 == date { 1  , 1  , 2001 } );
    REQUIRE( y21 == date { 1  , 7  , 2012 } );
    REQUIRE( y31 == date { 6  , 1  , 2000 } );
    REQUIRE( y41 == date { 26 , 1  , 2000 } );
    REQUIRE( y51 == date { 7  , 4  , 2000 } );
    REQUIRE( y61 == date { 2  , 1  , 2000 } );
    REQUIRE( y71 == date { 1  , 1  , 2000 } );
    REQUIRE( y81 == date { 31 , 12 , 1999 } );
    REQUIRE( y91 == date { 7  , 11 , 1999 } );
    REQUIRE( y12 == date { 28 , 8  , 2283 } );
}

TEST_CASE( "date date::operator-=( int day )" )
{
    using namespace project;

    REQUIRE( ( date { 1  , 1 , 2000 } -= -366   ) == date { 1  , 1  , 2001 } );
    REQUIRE( ( date { 1  , 1 , 2000 } -= -4565  ) == date { 1  , 7  , 2012 } );
    REQUIRE( ( date { 1  , 1 , 2000 } -= -5     ) == date { 6  , 1  , 2000 } );
    REQUIRE( ( date { 1  , 1 , 2000 } -= -25    ) == date { 26 , 1  , 2000 } );
    REQUIRE( ( date { 1  , 1 , 2000 } -= -97    ) == date { 7  , 4  , 2000 } );
    REQUIRE( ( date { 1  , 1 , 2000 } -= -1     ) == date { 2  , 1  , 2000 } );
    REQUIRE( ( date { 1  , 1 , 2000 } -= -0     ) == date { 1  , 1  , 2000 } );
    REQUIRE( ( date { 1  , 1 , 2000 } -= +1     ) == date { 31 , 12 , 1999 } );
    REQUIRE( ( date { 1  , 1 , 2000 } -= +55    ) == date { 7  , 11 , 1999 } );
    REQUIRE( ( date { 24 , 8 , 2022 } -= -95332 ) == date { 28 , 8  , 2283 } );
}

TEST_CASE( "date& date::operator++()" )
{
    using namespace project;

    REQUIRE( ++date { 1  , 1  , 2000 } == date { 2 , 1 , 2000 } );
    REQUIRE( ++date { 31 , 1  , 2000 } == date { 1 , 2 , 2000 } );
    REQUIRE( ++date { 31 , 12 , 2000 } == date { 1 , 1 , 2001 } );
}

TEST_CASE( "date& date::operator++( int )" )
{
    using namespace project;

    date temp;

    REQUIRE( ( temp = date { 1  , 1  , 2000 } , temp++ ) == date { 1 , 1 , 2000 } );
    REQUIRE( temp == date { 2 , 1 , 2000 } );
    REQUIRE( ( temp = date { 31 , 1  , 2000 } , temp++ ) == date { 31 , 1 , 2000 } );
    REQUIRE( temp == date { 1 , 2 , 2000 } );
    REQUIRE( ( temp = date { 31 , 12 , 2000 } , temp++ ) == date { 31 , 12 , 2000 } );
    REQUIRE( temp == date { 1 , 1 , 2001 } );
}

TEST_CASE( "date& date::operator--()" )
{
    using namespace project;

    REQUIRE( --date { 2 , 1 , 2000 } == date { 1  , 1  , 2000 } );
    REQUIRE( --date { 1 , 2 , 2000 } == date { 31 , 1  , 2000 } );
    REQUIRE( --date { 1 , 1 , 2001 } == date { 31 , 12 , 2000 } );
}

TEST_CASE( "date& date::operator--( int )" )
{
    using namespace project;

    date temp;

    REQUIRE( ( temp = date { 2 , 1 , 2000 } , temp-- ) == date { 2 , 1 , 2000 } );
    REQUIRE( temp == date { 1 , 1 , 2000 } );
    REQUIRE( ( temp = date { 1 , 2 , 2000 } , temp-- ) == date { 1 , 2 , 2000 } );
    REQUIRE( temp == date { 31 , 1  , 2000 } );
    REQUIRE( ( temp = date { 1 , 1 , 2001 } , temp-- ) == date { 1 , 1 , 2001 } );
    REQUIRE( temp == date { 31 , 12 , 2000 } );
}

TEST_CASE( "operator<( const date& x , const date& y )" )
{
    project::date x  { 8 , 6 , 2022 };
    project::date y1 { 9 , 6 , 2022 };
    project::date y2 { 8 , 7 , 2022 };
    project::date y3 { 8 , 6 , 2023 };

    REQUIRE( x < y1 );
    REQUIRE( x < y2 );
    REQUIRE( x < y3 );
}

TEST_CASE( "operator<=( const date& x , const date& y )" )
{
    project::date x  { 8 , 6 , 2022 };
    project::date y1 { 9 , 6 , 2022 };
    project::date y2 { 8 , 7 , 2022 };
    project::date y3 { 8 , 6 , 2023 };
    project::date y4 { 8 , 6 , 2022 };

    REQUIRE( x <= y1 );
    REQUIRE( x <= y2 );
    REQUIRE( x <= y3 );
    REQUIRE( x <= y4 );
}

TEST_CASE( "operator>( const date& x , const date& y )" )
{
    project::date x  { 8 , 6 , 2022 };
    project::date y1 { 9 , 6 , 2022 };
    project::date y2 { 8 , 7 , 2022 };
    project::date y3 { 8 , 6 , 2023 };

    REQUIRE( y1 > x );
    REQUIRE( y2 > x );
    REQUIRE( y3 > x );
}

TEST_CASE( "operator>=( const date& x , const date& y )" )
{
    project::date x  { 8 , 6 , 2022 };
    project::date y1 { 9 , 6 , 2022 };
    project::date y2 { 8 , 7 , 2022 };
    project::date y3 { 8 , 6 , 2023 };
    project::date y4 { 8 , 6 , 2022 };

    REQUIRE( y1 >= x );
    REQUIRE( y2 >= x );
    REQUIRE( y3 >= x );
    REQUIRE( y4 >= x );
}

TEST_CASE( "operator==( const date& x , const date& y  )" )
{
    project::date x { 8 , 6 , 2022 };
    project::date y { 8 , 6 , 2022 };

    REQUIRE( x == y );
}

TEST_CASE( "operator!=( const date& x , const date& y )" )
{
    project::date x { 8 , 6 , 2022 };
    project::date y { 9 , 6 , 2022 };

    REQUIRE( x != y );
}

TEST_CASE( "std::ostream& operator<<( std::ostream& os , const date& )" )
{
    std::stringstream ss1;
    std::stringstream ss2;

    project::date x1 { 21 , 8 , 2022 };
    project::date x2 { 21 , 8 , 20223 };

    ss1 << x1;
    ss2 << x2;

    REQUIRE( ss1.str() == "21/08/2022"  );
    REQUIRE( ss2.str() == "21/08/20223" );
}

TEST_CASE( "std::ostream& operator>>( std::ostream& os , date& )" )
{
    std::stringstream ss1 , ss2;

    project::date x1 , x2;

    ss1 << "21/08/2022";
    ss1 >> x1;
    ss2 << "21/08/20223";
    ss2 >> x2;

    REQUIRE( x1 == project::date { 21 , 8 , 2022  } );
    REQUIRE( x2 == project::date { 21 , 8 , 20223 } );
}

TEST_CASE( "date date::random()" )
{
    bool correct { true };

    for ( auto i = 0; i < int( 1e6 ); ++i )
    {
        project::date d = project::date::random();

        correct = correct && d.year() >= project::date::RAND_MIN_YEAR;
        correct = correct && d.year() <= project::date::RAND_MAX_YEAR;
    }

    REQUIRE( correct );
}

TEST_CASE( "date date::days_since_111( int )" )
{
    REQUIRE( project::date::days_since_111( 2000 ) == 730119 );
}

TEST_CASE( "int operator-( const date& , const date& )" )
{
    using namespace project;

    REQUIRE( date { 11 , 4  , 2022 } - date { 1  , 12 , 2000 } == 7801   );
    REQUIRE( date { 1  , 10 , 2001 } - date { 23 , 5  , 2045 } == -15940 );
}

TEST_CASE( "date operator+( int n , const date& )" )
{
    using namespace project;

    REQUIRE( 5 + date { 1 , 10 , 2000 } == date { 6 , 10 , 2000 } );
}

TEST_CASE( "date::day& operator++( date::day& )" )
{
    using namespace project;

    auto day { date::day::saturday };

    REQUIRE( ++day == date::day::sunday );
    REQUIRE( ++day == date::day::monday );
    REQUIRE(   day == date::day::monday );
}

TEST_CASE( "date::day operator++( date::day& , int )" )
{
    using namespace project;

    auto day { date::day::saturday };

    REQUIRE( day++ == date::day::saturday );
    REQUIRE( day++ == date::day::sunday   );
    REQUIRE( day   == date::day::monday   );
}

TEST_CASE( "date::day& operator--( date::day& )" )
{
    using namespace project;

    auto day { date::day::sunday };

    REQUIRE( --day == date::day::saturday );
    REQUIRE( --day == date::day::friday   );
    REQUIRE(   day == date::day::friday   );
}

TEST_CASE( "date::day operator--( date::day& , int )" )
{
    using namespace project;

    auto day { date::day::sunday };

    REQUIRE( day-- == date::day::sunday   );
    REQUIRE( day-- == date::day::saturday );
    REQUIRE( day   == date::day::friday   );
}