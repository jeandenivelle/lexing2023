
// Written by Dina Muktubayeva and
// Hans de Nivelle, March/April 2021.

#ifndef MAPH_LEXING_MINMAX_
#define MAPH_LEXING_MINMAX_  1

#include <limits>
#include <iostream>
#include <set>
#include <stack>
#include "flatmap.h"
#include "state.h"

namespace lexing
{
   template< typename C > C minval( ) 
      { return std::numeric_limits<C> :: lowest( ); } 

   template< typename C > C maxval( ) 
      { return std::numeric_limits<C> :: max( ); } 

}

#endif

