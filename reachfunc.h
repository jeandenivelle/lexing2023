
// Written by Hans de Nivelle, May 2021.

#ifndef MAPH_LEXING_REACHFUNC_
#define MAPH_LEXING_REACHFUNC_  1

#include <map>

namespace lexing
{

   // This is called `reachability function' in the GANDALF 2023 paper,
   // but we limit the length. 

   template< typename T > 
   struct reachfunc
   {
      T t0; 
      std::vector< std::pair< T, size_t >> func;
         // Maps every reachable T to its nearest, positive distance.
         // We keep the highest maxsize T. 
         // We store in decreasing order, because it is more convenient. 

      static constexpr unsigned int maxsize = 3;

      reachfunc( const T& t0 )
         : t0( t0 )
      { } 

      void print( std::ostream& out ) const
      {
         out << t0 << "(";
         for( auto p = begin( ); p != end( ); ++ p )
         {
            if( p != func. begin( ))
               out << ", ";
            else
               out << " ";
            out << ( p -> first ) << "/" << ( p -> second );
         }
         out << " )";
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
   inline bool 
   operator == ( const reachfunc<T> & r1, const reachfunc<T> & r2 )
   {
      if( r1. t0 == r2. t0 && r1. size( ) == r2. size( ))
      {
         auto p1 = r1. begin( );
         auto p2 = r2. begin( );

         while( p1 != r1. end( ))
         {
            if( p1 -> first != p2 -> first || p1 -> second != p2 -> second )
               return false;

            ++ p1; ++ p2;
         }
         return true;
      }
      else
         return false; 
   }

   template< typename T >
   inline bool 
   operator != ( const reachfunc<T> & r1, const reachfunc<T> & r2 )
      { return ! ( r1 == r2 ); }  


   template< typename T >
   inline bool
   operator < ( const reachfunc<T> & r1, const reachfunc<T> & r2 )
   {
      if( r1. t0 != r2. t0 )
         return r1. t0 < r2. t0;

      if( r1. size( ) != r2. size( )) 
         return r1. size( ) < r2. size( ); 

      auto p1 = r1. begin( );
      auto p2 = r2. begin( );

      while( p1 != r1. end( ))
      {
         if( p1 -> first != p2 -> first )
            return ( p1 -> first ) < ( p2 -> first );

         if( p1 -> second != p2 -> second )
            return ( p1 -> second ) < ( p2 -> second );

         ++ p1; ++ p2;
      }
      return false;
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

