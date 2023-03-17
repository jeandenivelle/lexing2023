
// Written by Hans de Nivelle, April 2021.
// It originally used string, but it was rewritten to use
// string_view as buffer in Feb. 2023.
// I also renamed the class from stringreader to viewreader,
// and put it in the namespace lexing.

#ifndef LEXING_VIEWREADER_ 
#define LEXING_VIEWREADER_   1

#include <iostream>
#include <string_view>

// This is an inputsource that reads from a string view. 
// For the rest, it has the same functionality as filereader. 
// 
// Since a string_view does not exist on its own, the underlying
// string must be stable as long as the reader exists.
// Also make sure that the string does not reallocated.

namespace lexing
{

   struct viewreader
   {
      std::string_view buffer;
         // We don't store position, because we modify the buffer.
         // That is cheap, because buffer is a string view.

      viewreader( ) = delete;

      // Never use a temporary string here: 

      explicit viewreader( const std::string& s )
         : buffer(s)
      { }

      explicit viewreader( std::string_view v )
         : buffer(v)
      { }

      explicit viewreader( const char* c )
         : buffer(c) 
      { }

      char peek( size_t i ) const
         { return buffer[i]; }

      bool has( size_t len ) const
         { return len <= buffer. size( ); }

      std::string_view view( size_t i ) const
         { return buffer. substr( 0, i ); } 

      void commit( size_t len )
         { buffer. remove_prefix( len ); }

      void print( std::ostream& out ) const
      {
         out << "viewreader: " << buffer; 
      }
   };

   inline 
   std::ostream& operator << ( std::ostream& out, const viewreader& read )
   {
      read. print( out );
      return out;
   }
}

#endif


