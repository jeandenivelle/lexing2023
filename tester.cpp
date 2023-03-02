

#include <iostream>
#include <fstream>
#include "filereader.h"

#include "includes.h"
#include "reachfunc.h"
#include "comp.h"

lexing::classifier< char, std::string > calculator( )
{
   using namespace lexing;

   classifier< char, std::string > cl( "#error" ); 

   auto idfirst = range( 'a', 'z' ) | range( 'A', 'Z' );

   auto idnext = range('a', 'z') | range('A', 'Z') |
                 just('_') | range('0', '9');

   auto digit = range( '0', '9' ); 

   cl. insert( idfirst * idnext.star(), "#identifier" );

   auto optsign = ( just( '-' ) | just( '+' )). optional( );

   auto exp = ( just( 'e' ) | just( 'E' )) *
              ( just( '-' ) | just( '+' )). optional( ) *
                digit. plus( );

   cl. insert( optsign * digit. plus( ) * exp. optional( ), "#double" );
   cl. insert( optsign * digit. plus( ) * just( '.' ) *
                 digit. star( ) * exp. optional( ), "#double" );
   cl. insert( optsign * digit. star( ) * just( '.' ) *
                 digit. plus( ) * exp. optional( ), "#double" );

   auto intconst = (just('0') |
               ( digit. without( '0' ) * digit .star() ));

   cl. insert( optsign * intconst, "#integer" );

   cl. insert( just( '+' ), "#add" );
   cl. insert( just( '-' ), "#sub" );
   cl. insert( just( '*' ), "#mul" );
   cl. insert( just( '/' ), "#div" );
   cl. insert( just( '*' ), "#mod" );

   cl. insert( just( '(' ), "#lpar" );
   cl. insert( just( ')' ), "#rpar" );
   
   cl. insert( just( ';' ), "#semicolon" );

   cl. insert( ( just( ' ' ) | just( '\t') | just( '\n' ) |
                 just( '\v' ) | just( '\f' ) | just( '\r' )). plus( ),
                  "#whitespace" );

   auto linecomment = word( "//" ) *
                    ( every<char>(). without( '\n' ) ).star();
   cl. insert( linecomment, "#whitespace" ); 

   auto blockcomment = word( "/*" ) *
          ( every<char>().without('*') |
            (just('*').plus() * every<char>().without('/').without('*')).star()).star()
             * just('*').plus( ) * just('/');

   cl. insert( blockcomment, "#whitespace" );

   cl. insert( just( '#' ), "#eof" );


   cl = make_deterministic( cl );

   std::cout << cl << "\n";

   cl = minimize( cl );

   std::ofstream file( "DFA.h" );
   lexing::printcode<char,std::string> (
   "char", "std::string", {},
    cl, file,
    []( std::ostream& out, char c ) { out << (int)c; },
        // print characters as integers. Doing different creates troubles.
    []( std::ostream& out, const std::string& t ) 
           { out << "\"" << t << "\""; } );
   file. close( );
   return cl; 
}

#include "DFA.h" 

namespace
{
   char hex( char c )
   {
      if( c >= 10 )
         return 'A' + ( c - 10 );
      else
         return '0' + c;
   }
}


int main( int argc, char* argv[] )
{
   auto cls = calculator( ); 
   // auto cls = 37;           // Replace this if you want to run the automaton.

   std::cout << cls << "\n";

   filereader read( &std::cin, "stdin" );

   constexpr bool thestarsareshiningandtheworldkeepsturning = true; 

   std::cout << "type a token, or # to quit\n";

   while( thestarsareshiningandtheworldkeepsturning ) 
   {
      if( !read. has(1) )
      {
         std::cout << "end of input\n";
	 return 0;
      }

      std::pair< std::string, size_t > p = readandclassify( cls, read );

      std::cout << "classification = " << p.first << "/" << p.second;
      std::cout << "    ";
      for( char c : read. view( p . second ))
      {
         if( isprint(c))
            std::cout << c;
         else
            std::cout << "{" << hex( ( c >> 4 ) & 0X0F ) << hex( c & 0X0F ) << "}";
      }
      std::cout << "\n";

      if( p. first == "#eof" ) 
      {
         std::cout << "end of file reached in line " << read. line << "\n";
         return 0;
      }

      if( p. second == 0 )
         read. commit(1);
      else 
         read. commit( p. second );
   }

}

