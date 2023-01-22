#include "error_funct.h"

#include "txt.h"
#include "Exception.h"

#include <stdio.h>		// snprintf, perror
#include <errno.h>		// errno
#include <unistd.h>		// getpid
#include <string.h>             // strerror

#define BUFFER_SIZE 	256


void 
displ_error
  (
   char const * const fname
  ) 
   throw()
{  char buffer[BUFFER_SIZE];
	
   snprintf(buffer, BUFFER_SIZE, "%s - [%i] %s %s", 
	    TXT_ERROR, getpid(), TXT_IN_CALL, fname);
   perror(buffer);
}


void 
throw_excpt
  (
   char const * const fname
  ) 
   throw(Exception)
{  char buffer[BUFFER_SIZE];
   
   snprintf(buffer, BUFFER_SIZE, "%s %s: %s",  
	    TXT_IN_CALL, fname, strerror(errno));
   throw Exception(buffer);
}
