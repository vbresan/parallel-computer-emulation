#include "debug.h"

#include <string.h>

#include <iostream>

using namespace std;

void *
Debug::memcpy
  (
   void       * const dest,
   void const * const src,
   size_t       const n
  )
{
   cerr << "Kopiram memoriju na lokaciju " << dest 
        << " sa lokacije "                 << src
        << " (" 			   << n
        << "B)"                            << endl;
   
   return ::memcpy(dest, src, n);
}


void *
Debug::memset
  (
   void * const s,
   int    const c,
   size_t const n
  )
{
   cerr << "Postavljam memoriju na lokaciji " << s
        << " na "                             << c
        << " (" 			      << n
        << "B)"                               << endl;
   
   return ::memset(s, c, n);
}
