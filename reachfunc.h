
// Written by Hans de Nivelle, May 2021.

#ifndef MAPH_LEXING_REACHFUNC_
#define MAPH_LEXING_REACHFUNC_  1

#include <map>
#include <compare>

namespace lexing
{

   // This is called `reachability function' in the GANDALF 2023 paper,
   // but we limit the length. 

   template< typename T > 
   requires std::three_way_comparable< T, std::weak_ordering > 
   struct reachfunc
   {
      std::vector< std::pair< T, size_t >> func;
         // Maps every reachable T to its nearest, positive distance.
         // We keep the highest maxsize T. 
         // We store in decreasing order, because it is more convenient. 

      static constexpr unsigned int maxsize = 4;

      reachfunc( ) = default;

      void print( std::ostream& out ) const
      {
         out << "{";
         for( auto p = begin( ); p != end( ); ++ p )
         {
            if( p != func. begin( ))
               out << ", ";
            else
               out << " ";
            out << ( p -> first ) << "/" << ( p -> second );
         }
         out << " }";
      }

      size_t size( ) const { return func. size( ); }

      using iterator = 
         typename std::vector< std::pair< T, size_t >> :: iterator;

      using const_iterator = 
         typename std::vector< std::pair< T, size_t >> :: const_iterator; 

      iterator begin( ) { return func. begin( ); }
      iterator end( ) { return func. end( ); }

      const_iterator begin( ) const { return func. cbegin( ); }
      const_iterator end( ) const { return func. cend( ); }

      // Return true if this is the first distance for T, 
      // or an improvement of the existing distance for T.

      bool insert( const T& t, size_t dist )
      {
         auto p = begin( );  
         while( p != end( ) && p -> first > t )
            ++ p; 

         if( p != end( ) && p -> first == t )
         {
            if( p -> second > dist )
            {
               p -> second = dist;
               return true;
            }
            return false; 
         }

         if( p == end( ))
         {
            if( size( ) != maxsize )
            {
               func. push_back( std::pair( t, dist ));
               return true;
            }
            return false;
         }
        
         if( size( ) == maxsize )
            func. pop_back( );
         
         func. insert( p, std::pair( t, dist ));
         return true;
      }

   };


   template< typename T >
   requires std::three_way_comparable< T, std::weak_ordering > 
   inline std::weak_ordering 
   operator <=> ( const reachfunc<T> & r1, const reachfunc<T> & r2 )
   {
      auto p1 = r1. begin( );
      auto p2 = r2. begin( );

      while( p1 != r1. end( ) && p2 != r2. end( ))
      {
         auto c = ( p1 -> first <=> p2 -> first );
         if( !is_eq(c)) return c;

         c = ( p1 -> second <=> p2 -> second );
         if( !is_eq(c)) return c; 

         ++ p1; ++ p2;
      }
     
      if( p1 == r1. end( ) && p2 != r2. end( ))
         return std::weak_ordering::less;

      if( p1 != r1. end( ) && p2 == r2. end( ))
         return std::weak_ordering::greater;
 
      return std::weak_ordering::equivalent;
   }

   template< typename T >
   requires std::three_way_comparable< T, std::weak_ordering >
   inline bool
   operator == ( const reachfunc<T> & r1, const reachfunc<T> & r2 )
   {
      return is_eq( r1 <=> r2 ); 
   }

   template< typename T > 
   std::ostream& 
   operator << ( std::ostream& out, const lexing::reachfunc<T> & rf )
   {
      rf. print( out );
      return out;
   }

}

#endif

