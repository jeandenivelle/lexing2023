
// Composition of two types that have a total 3-way comparison
// on them. Written by Hans de Nivelle, March 2023. 

#include <concepts>
#include <compare>


#ifndef MAPH_LEXING_COMP_
#define MAPH_LEXING_COMP_   1

namespace lexing
{

   // For minimization, the classifications must be 3-way comparable. 
   // All reasonable classifications that I can think of
   // (integral types, enums, std::string) have this property. 

   template< typename T1, typename T2 >
   requires std::three_way_comparable< T1, std::weak_ordering > &&
            std::three_way_comparable< T2, std::weak_ordering > 
   struct comp
   {
      T1 first;
      T2 second;

      comp( ) = delete; 

      comp( const T1& first, const T2& second ) 
         : first( first ), second( second )
      { }

      comp( T1&& first, T2&& second ) 
         : first( std::move( first )), second( std::move( second ))
      { }

   };

   template< typename T1, typename T2 >
   requires std::three_way_comparable< T1, std::weak_ordering > &&
            std::three_way_comparable< T2, std::weak_ordering >
   inline std::weak_ordering 
   operator <=> ( const comp< T1, T2 > & c1, const comp< T1, T2 > & c2 )
   {
      auto cmp = ( c1. first <=> c2. first ); 
      if( !is_eq( cmp )) return cmp; 
   
      cmp = ( c1. second <=> c2. second ); 
      return cmp;
   }

   template< typename T1, typename T2 >
   requires std::equality_comparable< T1 > && std::equality_comparable< T2 >
   inline bool
   operator == ( const comp< T1, T2 > & c1, const comp< T1, T2 > & c2 ) 
   {
      return is_eq( c1 <=> c2 );
   }

   template< typename T1, typename T2 > 
   requires requires( std::ostream& out, T1 first, T2 second ) 
   {
      { out << first } -> std::convertible_to< std::ostream& > ; 
      { out << second } -> std::convertible_to< std::ostream& > ; 
   }
   std::ostream& operator << ( std::ostream& out, const comp< T1, T2 > & c )
   {
      out << "( " << c. first << ", " << c. second << " )";
      return out; 
   }
}

#endif

