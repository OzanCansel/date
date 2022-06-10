#pragma once

#include <iosfwd>
#include <ctime>
#include <tuple>
#include <stdexcept>
#include <string_view>
#include <string>
#include <cassert>

namespace project
{

class date
{

public:

    static constexpr int BASE_YEAR     = 1900;
    static constexpr int RAND_MIN_YEAR = 1940;
    static constexpr int RAND_MAX_YEAR = 2020;

    enum class day
    {
        sunday    ,
        monday    ,
        tuesday   ,
        wednesday ,
        thursday  ,
        friday    ,
        saturday
    };

    static date random();
    static constexpr bool is_leap( int year );

    date();
    date( int day , int month , int year );
    date( std::string_view );
    date( std::time_t );
    [[nodiscard]] int month_day() const;
    [[nodiscard]] int month() const;
    [[nodiscard]] int year() const;
    [[nodiscard]] int year_day() const;
    [[nodiscard]] day week_day() const;

    date& set_month_day( int );
    date& set_month( int );
    date& set_year( int );
    date& set( int day , int month , int year );

    date  operator+ ( int day ) const;
    date& operator+=( int day );
    date  operator- ( int day ) const;
    date& operator-=( int day );
    date& operator++();
    date  operator++( int );
    date& operator--();
    date  operator--( int );
    
    friend bool operator< ( const date& , const date& );
    friend bool operator==( const date& , const date& );
    friend bool operator!=( const date& , const date& );
    friend std::ostream& operator<<( std::ostream& os , const date& );
    friend std::istream& operator>>( std::istream& is ,       date& );

private:

    [[nodiscard]] int n_days( int month , int year ) const;
    [[nodiscard]] int days_from_0( int year ) const;
    [[nodiscard]] int year_from_days( int days ) const;
    [[nodiscard]] day anchor_day( int year ) const;
    void validate_day( int ) const;
    void validate_month( int ) const;
    void validate_year( int ) const;

    int m_day;
    int m_month;
    int m_year;
};

constexpr bool date::is_leap( int year )
{
    return year % 4   == 0 &&
           year % 100 != 0 ||
           year % 400 == 0;
}

date::date()
    :   m_day   { 1 }
    ,   m_month { 1 }
    ,   m_year  { BASE_YEAR }
{}

date::date( int day , int month , int year )
    :   m_day   { day }
    ,   m_month { month }
    ,   m_year  { year }
{
    validate_year( m_year );
    validate_day( m_day );
    validate_month( m_month );
}

date::date( std::string_view v )
{
    m_day = stoi(
        std::string { v.substr( 0 , 2 ) }
    );

    m_month = stoi(
        std::string { v.substr( 3 , 2 ) }
    );

    m_year = stoi(
        std::string { v.substr( 6 , 4 ) }
    );
}

date::date( std::time_t gmt )
{
    auto tm = std::gmtime( &gmt );

    if ( !tm )
        throw std::runtime_error { "gmtime fail." };

    m_year  = tm->tm_year + 1900;
    m_month = tm->tm_mon + 1;
    m_day   = tm->tm_mday;
}

int date::month_day() const
{
    return m_day;
}

int date::month() const
{
    return m_month;
}

int date::year() const
{
    return m_year;
}

int date::year_day() const
{
    int days {};

    switch( m_month - 1 )
    {
        case 12 : days += n_days( 12 , m_year );
        case 11 : days += n_days( 11 , m_year );
        case 10 : days += n_days( 10 , m_year );
        case 9  : days += n_days( 9  , m_year );
        case 8  : days += n_days( 8  , m_year );
        case 7  : days += n_days( 7  , m_year );
        case 6  : days += n_days( 6  , m_year );
        case 5  : days += n_days( 5  , m_year );
        case 4  : days += n_days( 4  , m_year );
        case 3  : days += n_days( 3  , m_year );
        case 2  : days += n_days( 2  , m_year );
        case 1  : days += n_days( 1  , m_year );
        case 0  : days += m_day;
    }

    return days;
}

inline date::day date::week_day() const
{
    int n_digits { int( std::floor( std::log10( m_year ) ) ) + 1 };

    assert( n_digits >= 2 );

    int td { m_year % int( std::pow( 10 , n_digits - 2 ) ) };
    int c1 { td / 12 };
    int c2 { td - c1 * 12 };
    int c3 { c2 / 4 };

    day dooms_week_day {
        ( c1 + c2 + c3 + int( anchor_day( m_year ) ) ) % 7
    };
    date dooms_day { 4 , 4 , m_year };

    int diff {
        year_day() - dooms_day.year_day()
    };
    
    return day(
        ( ( diff % 7 ) + 7 + int( dooms_week_day ) ) % 7
    );
}

inline date& date::set_month_day( int day )
{
    validate_day( day );

    m_day = day;

    return *this;
}

inline date& date::set_month( int month )
{
    validate_month( month );

    m_month = month;

    return *this;
}

inline date& date::set_year( int year )
{
    validate_year( year );

    m_year = year;

    return *this;
}

inline date& date::set( int day , int month , int year )
{
    set_year( year );
    set_month( month );
    set_month_day( day );

    return *this;
}

inline date date::operator+( int day ) const
{
    if ( !day )
        return *this;

    date target_date { *this };

    target_date += day;

    return target_date;
}

inline date& date::operator+=( int day )
{
    if ( !day )
        return *this;

    int current_days { days_from_0( m_year ) + year_day() - 1 };
    int target_days  { current_days + day };
    int target_year  { year_from_days( target_days ) };
    int surplus_days { target_days - days_from_0( target_year ) };
    int target_month;

    for (
            target_month = 1 ;
            target_month < 13 && surplus_days >= n_days( target_month , target_year ); 
            surplus_days -= n_days( target_month , target_year ) , ++target_month
        );

    target_year  += target_month == 13;
    target_month  = target_month == 13 ? 1 : target_month;

    m_day   = surplus_days + 1;
    m_month = target_month;
    m_year  = target_year;

    return *this;
}

inline date date::operator-( int day ) const
{
    return operator+( -day );
}

inline date& date::operator-=( int day )
{
    return operator+=( -day );
}

inline date& date::operator++()
{
    return operator+=( 1 );
}

inline date date::operator++( int )
{
    date curr { *this };

    operator++();

    return curr;
}

inline date& date::operator--()
{
    return operator-=( 1 );
}

inline date date::operator--( int )
{
    date curr { *this };

    operator--();

    return curr;
}

inline int date::n_days( int month , int year ) const
{
    assert( month > 0 && month <= 12 );

    switch( month )
    {
        case 12 : return 31;
        case 11 : return 30;
        case 10 : return 31;
        case 9  : return 30;
        case 8  : return 31;
        case 7  : return 31;
        case 6  : return 30;
        case 5  : return 31;
        case 4  : return 30;
        case 3  : return 31;
        case 2  : return 28 + is_leap( year );
        case 1  : return 31;
    }
}

inline int date::days_from_0( int year ) const
{
    year -= 1;
    return year * 365 + year / 400 - year / 100 + year / 4;
}

inline int date::year_from_days( int days ) const
{
    return days * 400 / 146097 + 1;
}

inline date::day date::anchor_day( int year ) const
{
    auto century = ( ( year - 200 ) % 400 / 100 );

    switch( century )
    {
        case 0 : return day::friday;
        case 1 : return day::wednesday;
        case 2 : return day::tuesday;
        case 3 : return day::sunday;
    }

    assert( false && "Should not reach here" );
}

inline void date::validate_day( int day ) const
{
    assert( day  > 0 );
    assert( day  <= n_days( m_month , m_year ) );
}

inline void date::validate_month( int month ) const
{
    assert( month > 0 );
    assert( month <= 12 );
}

inline void date::validate_year( int year ) const
{
    assert( year  > 1900 );
}

inline bool operator<( const date& x , const date& y )
{
    return std::tie(
        x.m_year  ,
        x.m_month ,
        x.m_day
    ) <
    std::tie(
        y.m_year  ,
        y.m_month ,
        y.m_day
    );
}

inline bool operator<=( const date& x , const date& y )
{
    return !( y < x );
}

inline bool operator>( const date& x , const date& y )
{
    return !( x <= y );
}

inline bool operator>=( const date& x , const date& y )
{
    return !( x < y );
}

inline bool operator==( const date& x , const date& y )
{
    return x <= y && y <= x;
}

inline bool operator!=( const date& x , const date& y )
{
    return !( x == y );
}

inline std::ostream& operator<<( std::ostream& os , const date& )
{
    return os;
}

inline std::istream& operator>>( std::istream& is , date& )
{
    return is;
}

}