#include "pce.h"

#include "Wrapper.h"
#include "SlaveProcess.h"
#include "MasterProcess.h"

#include <string.h>		// strncmp
#include <unistd.h>		// getppid
#include <stdio.h>		// snprintf
#include <stdlib.h>		// atoi
#include <sys/types.h>		// waitpid
#include <sys/wait.h>
#include <signal.h>		// SIGCHLD

#define BUFFER_SIZE	256
#define ANY_CHILD	-1

void sigchld_hndlr(int signo);
void sigusr1_hndlr(int signo);


Process * start_new
  (
   int     argc, 
   char ** argv,
   p_action u_proc
  )
{  char buffer[BUFFER_SIZE];
   
   Wrapper::Exit::signal(SIGUSR1, sigusr1_hndlr);
   Wrapper::Exit::signal(SIGPIPE, SIG_IGN);
   
   snprintf(buffer, BUFFER_SIZE, "-%ishmid", getppid());
   for(int i = 1; i < argc; ++i)
	if(!strncmp(argv[i], buffer, strlen(buffer)))
	  {  int shmid = atoi(argv[i] + strlen(buffer));
	     
	     *argv[i] = '\0';
	     return new SlaveProcess(argv[0], shmid, u_proc);
	  }
   
   snprintf(buffer, BUFFER_SIZE, "-servaddr");
   for(int i = 1; i < argc; ++i)
	if(!strncmp(argv[i], buffer, strlen(buffer)))
	  {  
	     *argv[i] = '\0';
	     return new SlaveProcess(argv[0], 
				     argv[i] + strlen(buffer), u_proc);
	  }

   Wrapper::Exit::signal(SIGCHLD, sigchld_hndlr);
   return new MasterProcess(argv[0], u_proc);
}


void sigusr1_hndlr(int /*signo*/)
{
}


void sigchld_hndlr(int /*signo*/)
{  int   statloc;
   pid_t pid;
   
   while((pid = waitpid(ANY_CHILD, &statloc, WNOHANG)) > 0)
     {
	std::cout << "Child [" << pid << "] terminated" << std::endl;
     }
}
