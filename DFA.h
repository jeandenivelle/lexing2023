
// This code was generated automatically.
// Generator written by Hans de Nivelle and Dina Muktubayeva.

namespace
{
   template< typename S >
   std::pair< std::string, size_t > readandclassify( int triv, S& inp )
   {
      auto latest = std::make_pair< std::string, size_t > ( "#error", 0 );
      if( !inp. has(1))
         return latest;
      char c = inp. peek(0);
      size_t i = 0;

   Q00:
      if( !inp. has(i+1))
         return latest;
      c = inp. peek(i);
      switch( c )
      {
      case 32:
         ++ i; goto Q01;
      case 35:
         ++ i; goto Q02;
      case 40:
         ++ i; goto Q03;
      case 41:
         ++ i; goto Q04;
      case 42:
         ++ i; goto Q05;
      case 43:
         ++ i; goto Q06;
      case 45:
         ++ i; goto Q07;
      case 46:
         ++ i; goto Q08;
      case 47:
         ++ i; goto Q09;
      case 48:
         ++ i; goto Q10;
      case 59:
         ++ i; goto Q12;
      case 44:
      case 58:
         return latest;
      }
      if( c >= 60 )
      {
         if( c >= 91 )
         {
            if( c >= 123 ) return latest;
            if( c >= 97 ) { ++ i; goto Q13; }
            if constexpr( true ) return latest;
         }
         else
         {
            if( c >= 65 ) { ++ i; goto Q13; }
            if constexpr( true ) return latest;
         }
      }
      else
      {
         if( c >= 49 ) { ++ i; goto Q11; }
         if( c >= 14 ) return latest;
         if( c >= 9 ) { ++ i; goto Q01; }
         if constexpr( true ) return latest;
      }
   Q01:
      latest. first = "#whitespace"; latest. second = i;
      if( !inp. has(i+1))
         return latest;
      c = inp. peek(i);
      if( c == 32 ) { ++ i; goto Q01; }
      if( c >= 14 ) return latest;
      if( c >= 9 ) { ++ i; goto Q01; }
      if constexpr( true ) return latest;
   Q02:
      latest. first = "#eof"; latest. second = i;
      return latest;
   Q03:
      latest. first = "#lpar"; latest. second = i;
      return latest;
   Q04:
      latest. first = "#rpar"; latest. second = i;
      return latest;
   Q05:
      latest. first = "#mod"; latest. second = i;
      return latest;
   Q06:
      latest. first = "#add"; latest. second = i;
      if( !inp. has(i+1))
         return latest;
      c = inp. peek(i);
      if( c == 46 ) { ++ i; goto Q08; }
      if( c == 48 ) { ++ i; goto Q10; }
      if( c == 47 ) return latest;
      if( c >= 58 ) return latest;
      if( c >= 49 ) { ++ i; goto Q11; }
      if constexpr( true ) return latest;
   Q07:
      latest. first = "#sub"; latest. second = i;
      if( !inp. has(i+1))
         return latest;
      c = inp. peek(i);
      if( c == 46 ) { ++ i; goto Q08; }
      if( c == 48 ) { ++ i; goto Q10; }
      if( c == 47 ) return latest;
      if( c >= 58 ) return latest;
      if( c >= 49 ) { ++ i; goto Q11; }
      if constexpr( true ) return latest;
   Q08:
      if( !inp. has(i+1))
         return latest;
      c = inp. peek(i);
      if( c >= 58 ) return latest;
      if( c >= 48 ) { ++ i; goto Q14; }
      if constexpr( true ) return latest;
   Q09:
      latest. first = "#div"; latest. second = i;
      if( !inp. has(i+1))
         return latest;
      c = inp. peek(i);
      if( c == 42 ) { ++ i; goto Q15; }
      if( c == 47 ) { ++ i; goto Q16; }
      if constexpr( true ) return latest;
   Q10:
      latest. first = "#integer"; latest. second = i;
      if( !inp. has(i+1))
         return latest;
      c = inp. peek(i);
      switch( c )
      {
      case 46:
         ++ i; goto Q14;
      case 69:
      case 101:
         ++ i; goto Q18;
      case 47:
         return latest;
      }
      if( c >= 58 ) return latest;
      if( c >= 48 ) { ++ i; goto Q17; }
      if constexpr( true ) return latest;
   Q11:
      latest. first = "#integer"; latest. second = i;
      if( !inp. has(i+1))
         return latest;
      c = inp. peek(i);
      switch( c )
      {
      case 46:
         ++ i; goto Q14;
      case 69:
      case 101:
         ++ i; goto Q18;
      case 47:
         return latest;
      }
      if( c >= 58 ) return latest;
      if( c >= 48 ) { ++ i; goto Q11; }
      if constexpr( true ) return latest;
   Q12:
      latest. first = "#semicolon"; latest. second = i;
      return latest;
   Q13:
      latest. first = "#identifier"; latest. second = i;
      if( !inp. has(i+1))
         return latest;
      c = inp. peek(i);
      if( c == 95 ) { ++ i; goto Q13; }
      if( c == 96 ) return latest;
      if( c >= 65 )
      {
         if( c >= 123 ) return latest;
         if( c >= 97 ) { ++ i; goto Q13; }
         if( c >= 91 ) return latest;
         if constexpr( true ) { ++ i; goto Q13; }
      }
      else
      {
         if( c >= 58 ) return latest;
         if( c >= 48 ) { ++ i; goto Q13; }
         if constexpr( true ) return latest;
      }
   Q14:
      latest. first = "#double"; latest. second = i;
      if( !inp. has(i+1))
         return latest;
      c = inp. peek(i);
      if( c == 69 || c == 101 ) { ++ i; goto Q18; }
      if( c >= 58 ) return latest;
      if( c >= 48 ) { ++ i; goto Q14; }
      if constexpr( true ) return latest;
   Q15:
      if( !inp. has(i+1))
         return latest;
      c = inp. peek(i);
      if( c == 42 ) { ++ i; goto Q19; }
      if constexpr( true ) { ++ i; goto Q15; }
   Q16:
      latest. first = "#whitespace"; latest. second = i;
      if( !inp. has(i+1))
         return latest;
      c = inp. peek(i);
      if( c == 10 ) return latest;
      if constexpr( true ) { ++ i; goto Q16; }
   Q17:
      latest. first = "#double"; latest. second = i;
      if( !inp. has(i+1))
         return latest;
      c = inp. peek(i);
      switch( c )
      {
      case 46:
         ++ i; goto Q14;
      case 69:
      case 101:
         ++ i; goto Q18;
      case 47:
         return latest;
      }
      if( c >= 58 ) return latest;
      if( c >= 48 ) { ++ i; goto Q17; }
      if constexpr( true ) return latest;
   Q18:
      if( !inp. has(i+1))
         return latest;
      c = inp. peek(i);
      if( c == 43 || c == 45 ) { ++ i; goto Q20; }
      if( c == 44 ) return latest;
      if( c >= 58 ) return latest;
      if( c >= 48 ) { ++ i; goto Q21; }
      if constexpr( true ) return latest;
   Q19:
      if( !inp. has(i+1))
         return latest;
      c = inp. peek(i);
      if( c == 42 ) { ++ i; goto Q19; }
      if( c == 47 ) { ++ i; goto Q22; }
      if constexpr( true ) { ++ i; goto Q15; }
   Q20:
      if( !inp. has(i+1))
         return latest;
      c = inp. peek(i);
      if( c >= 58 ) return latest;
      if( c >= 48 ) { ++ i; goto Q21; }
      if constexpr( true ) return latest;
   Q21:
      latest. first = "#double"; latest. second = i;
      if( !inp. has(i+1))
         return latest;
      c = inp. peek(i);
      if( c >= 58 ) return latest;
      if( c >= 48 ) { ++ i; goto Q21; }
      if constexpr( true ) return latest;
   Q22:
      latest. first = "#whitespace"; latest. second = i;
      return latest;
   }
}

