#include "Child.h"

#include "Wrapper.h"	// Wrapper::Exit::execvp  

#include <vector>
#include <stdio.h>	// NULL, snprintf
#include <unistd.h>	// getppid       


/*****************************************************************************
*****************************************************************************/

#define BUFFER_SIZE	256


/*****************************************************************************
  
 Child 
  
*****************************************************************************/

/*----------------------------------------------------------------------------
 konstruktor
----------------------------------------------------------------------------*/

Child::Child
  (
   char const * const name, 		// naziv procesa   
   int  const         shmid		// shared memory ID
  )
   throw()
{  char const * c_argv[] = { name, NULL, NULL};
   char         buffer[BUFFER_SIZE];
   
   snprintf(buffer, BUFFER_SIZE, "-%ishmid%i", getppid(), shmid);
   c_argv[1] = buffer;
   
   Wrapper::Exit::execvp(name, c_argv);
}


Child::Child
  (
   char         const * const cstr_rsh,
   std::string  const &       servaddr
  )
   throw(bad_alloc)
{  char       * const        cstr_copy = new char [strlen(cstr_rsh) + 1];
   char       *              cp_tmp    = cstr_copy;
   char const **             c_argv;
   char                      buffer[BUFFER_SIZE];
   std::vector<char const *> cp_arg;
   
   snprintf(buffer, BUFFER_SIZE, "-servaddr%s", servaddr.c_str());
   strcpy(cstr_copy, cstr_rsh);
   cp_arg.push_back("rsh");
   cp_arg.push_back("-n");
   cp_arg.push_back(cstr_copy);
   
   while((cp_tmp = strchr(cp_tmp, ' ')))
     {
	*cp_tmp = '\0';
	cp_arg.push_back(++cp_tmp);
     }
   cp_arg.push_back(buffer);
   cp_arg.push_back(NULL);
   
   c_argv = new char const * [cp_arg.size()];
   for(unsigned int i = 0; i < cp_arg.size(); ++i)
	c_argv[i] = cp_arg[i];
   
   Wrapper::Exit::execvp("rsh", c_argv);
}
