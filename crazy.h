
#ifndef MAPH_CRAZY_INCLUDED
#define MAPH_CRAZY_INCLUDED   1

// This is for testing the polymorphism in C.

#include <vector>
#include <iostream>
#include "minmax.h"

struct crazychar
{
   char c;

   constexpr explicit crazychar( char c )
      :c(c) 
   { } 

   crazychar& operator ++ ( ) 
   {
      ++ c;
      return *this;
   }
};

inline bool operator == ( crazychar c1, crazychar c2 )
{
   return c1.c == c2.c;
}

inline bool operator != ( crazychar c1, crazychar c2 )
{
   return c1.c != c2.c;
}

inline bool operator < ( crazychar c1, crazychar c2 )
{
   return c1.c < c2.c;
}

inline bool operator > ( crazychar c1, crazychar c2 )
{
   return c1.c > c2.c;
}

inline bool operator <= ( crazychar c1, crazychar c2 )
{
   return c1.c <= c2.c;
}


inline std::ostream& operator << ( std::ostream& out, crazychar c )
{
   out << c.c;
   return out;
}


namespace lexing
{
   template<> crazychar minval( )
      { return crazychar( 'a' ); }

   template<> crazychar maxval( )
      { return crazychar( 'z' ); }
}


struct crazyreader
{
   std::string buffer; 
   size_t p;

   crazyreader( const std::string& s )  
      : buffer(s),
        p(0) 
   { }

   crazyreader( std::string&& s )
      : buffer( std::move(s) ),
        p(0)  
   { }
 
   crazyreader( const char* c )
      : buffer( std::string(c) ),
        p(0) 
   { }

   crazychar peek( size_t i ) const
      { return crazychar( buffer[ p + i ] ); }

   bool has( size_t len ) const
      { return p + len < buffer. size( ); }

   void commit( size_t len )
      { p += len; }
};

#endif 


