#include "pce.h"

#include <iostream>
#include <string.h>	// strcmp
#include <stdio.h>	// fgets
#include <ctype.h>      // isdigit

#define	BUFFER_LEN 256

Process * pProcess = NULL;


void process_arg(char * arg);


void 
process_arg
  (
   char * arg
  )
{
   if(!strcmp(arg, "start_new"))
	pProcess->start_new();
   else if(!strncmp(arg, "kill", strlen("kill")))
     {  int whom = atoi(arg + strlen("kill"));
	
	pProcess->kill(whom);
     }
   else if(!strncmp(arg, "route", strlen("route")))
     {  int    from   = atoi(arg + strlen("route"));
	int    to;
	char * second = arg + strlen("route");
		  
	while(isdigit(*second))
	     ++second;
	
	to = atoi(second + strlen("to"));
	pProcess->route(from, to);
     }
   else if(!strcmp(arg, "loop"))
	pProcess->loop();
   else if(!strcmp(arg, "loop_forever"))
	pProcess->loop_forever();
   else if(!strcmp(arg, ""))
	;
   else
	std::cerr << getpid() << " unknown command!" << std::endl;
}


int main(int argc, char **argv)
{  
   pProcess = start_new(argc, argv);
   
   if(pProcess->master())
     {
   try
     {
	if(argc > 1)
	  for(int i = 1; i < argc; ++i)
	       process_arg(argv[i]);
	else
	  {  char buffer[BUFFER_LEN];
	     
	     std::cout << "@ ";
	     while(fgets(buffer, BUFFER_LEN, stdin))
	       {  char * nl;
		  
		  if((nl = strchr(buffer, '\n')))
		       *nl = '\0';
		       
		  process_arg(buffer);
		  std::cout << "@ ";
	       }
	     std::cout << std::endl;
	  }
     }
   catch(exception & e) {
	std::cerr << e.what() << std::endl; }
     }
   else
	pProcess->loop_forever();
   
   delete pProcess;
   
   return 0;
}
