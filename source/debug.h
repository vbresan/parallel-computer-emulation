#ifndef DEBUG_H
#define DEBUG_H

#include <stddef.h>

namespace Debug
{
   void * memcpy(void * dest, void const * src, size_t n);	// string.h
   void * memset(void * s,    int          c,   size_t n);	// string.h
};

#endif
