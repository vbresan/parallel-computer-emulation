#include "Wrapper.h"

#include "error_funct.h"
#include "exit_errors.h"
#include "connect_nonb.h"
#include "accept_nonb.h"

#include <unistd.h>	// execvp, gethostname
#include <fcntl.h>	// fcntl
#include <signal.h>	// sigaction


/*----------------------------------------------------------------------------
 ispisuju poruku o pogresci
----------------------------------------------------------------------------*/

int 
Wrapper::shmctl
  (
   int        shmid, 
   int        cmd, 
   shmid_ds * buf
  )
   throw()
{  int ret_val = ::shmctl(shmid, cmd, buf);
   
   if(ret_val == -1)
	displ_error("shmctl");
   
   return ret_val;
}


int 
Wrapper::shmdt
  (
   char * shmaddr
  )
   throw()
{  int ret_val = ::shmdt(shmaddr);
   
   if(ret_val == -1)
	displ_error("shmdt");
   
   return ret_val;
}


/*----------------------------------------------------------------------------
 bacaju iznimku
----------------------------------------------------------------------------*/

pid_t 
Wrapper::Throw::fork()
   throw(Exception)
{  pid_t ret_val = ::fork();

   if(ret_val == -1)
	throw_excpt("fork");
   
   return ret_val;
}


int 
Wrapper::Throw::kill
  (
   pid_t pid,
   int   sig
  )
   throw(Exception)
{  int ret_val = ::kill(pid, sig);
   
   if(ret_val == -1)
	throw_excpt("kill");

   return ret_val;
}


int 
Wrapper::Throw::shmget
  (
   key_t key, 
   int   size, 
   int   shmflg
  )
   throw(Exception)
{  int ret_val = ::shmget(key, size, shmflg);

   if(ret_val == -1)
	throw_excpt("shmget");
   
   return ret_val;
}


char * 
Wrapper::Throw::shmat
  (
   int    shmid, 
   char * shmaddr, 
   int    shmflg
  )
   throw(Exception)
{  char * ret_val = (char *) ::shmat(shmid, shmaddr, shmflg);
   
   if(ret_val == (char *) -1)
     {
        Wrapper::shmctl(shmid, IPC_RMID, 0);
	throw_excpt("shmat");
     }
   
   return ret_val;
}


int
Wrapper::Throw::socket
  (
   int const domain,
   int const type,
   int const protocol
  )
   throw(Exception)
{  int ret_val = ::socket(domain, type, protocol);
   
   if(ret_val == -1)
	throw_excpt("socket");

   return ret_val;
}


int
Wrapper::Throw::bind
  (
   int        const sockfd,
   sockaddr * const my_addr,
   socklen_t  const addrlen
  )
   throw(Exception)
{  int ret_val = ::bind(sockfd, my_addr, addrlen);
   
   if(ret_val == -1)
	throw_excpt("bind");
   
   return ret_val;
}


int
Wrapper::Throw::getsockname
  (
   int         const s,
   sockaddr  * const name,
   socklen_t * const namelen
  )
   throw(Exception)
{  int ret_val = ::getsockname(s, name, namelen);
   
   if(ret_val == -1)
	throw_excpt("getsockname");
   
   return ret_val;
}


int
Wrapper::Throw::listen
  (
   int const s,
   int const backlog
  )
   throw(Exception)
{  int ret_val = ::listen(s, backlog);
   
   if(ret_val == -1)
	throw_excpt("listen");
   
   return ret_val;
}


int
Wrapper::Throw::connect
  (
   int              const sockfd,
   sockaddr const * const serv_addr,
   socklen_t        const addrlen,
   int              const nsec
  )
   throw(Exception)
{  int ret_val = ::connect_nonb(sockfd, serv_addr, addrlen, nsec);
   
   if(ret_val == -1)
	throw_excpt("connect_nonb");
   
   return ret_val;
}


int
Wrapper::Throw::accept
  (
   int         const s,
   sockaddr  * const addr,
   socklen_t * const addrlen
  )
   throw(Exception)
{  int ret_val = ::accept_nonb(s, addr, addrlen);
   
   if(ret_val == -1)
	throw_excpt("accept_nonb");
   
   return ret_val;
}


int
Wrapper::Throw::gethostname
  (
   char * const name,
   size_t const namelen
  )
   throw(Exception)
{  int ret_val = ::gethostname(name, namelen);
   
   if(ret_val == -1)
	throw_excpt("gethostname");
   
   return ret_val;
}


hostent *
Wrapper::Throw::gethostbyname
  (
   char const * const hostname
  )
   throw(Exception)
{  hostent * ret_val = ::gethostbyname(hostname);
   
   if(ret_val == NULL)
	throw_excpt("gethostbyname");
   
   return ret_val;
}


int
Wrapper::Throw::fcntl
  (
   int  const fd,
   int  const cmd,
   long const arg
  )
   throw(Exception)
{  int ret_val = ::fcntl(fd, cmd, arg);
   
   if(ret_val == -1)
	throw_excpt("fcntl");

   return ret_val;
}


int
Wrapper::Throw::select
  (
   int       const fd,
   fd_set  * const readfds,
   fd_set  * const writefds,
   fd_set  * const exceptfds,
   timeval * const timeout
  )
   throw(Exception)
{  int ret_val = ::select(fd, readfds, writefds, exceptfds, timeout);
   
   if(ret_val == -1)
	throw_excpt("select");
   
   return ret_val;
}


/*----------------------------------------------------------------------------
 izlaze iz programa
----------------------------------------------------------------------------*/

int 
Wrapper::Exit::execvp
  (
   char const * const file, 
   char const * const argv[]
  )
   throw()
{  int ret_val = ::execvp(file, (char * const *) argv);
   
   if(ret_val == -1)
     {
	displ_error("execvp");
	exit(err_execvp);
     }
   
   return ret_val;
}


/* Prema rijecima W. Richarda Stevensa:					  *
 * 	'signal' je povijesna funkcija koja prethodi Posix.1 i razlicite  *
 * 	implementacije ponasaju se razlicito osiguravajuci 'backward      *
 *      compability' dok Posix.1 eksplicitno navodi ponasanje kada je     *
 *      'sigaction' pozvana. 	                                          */

void 
(*Wrapper::Exit::signal(int signum, void (*handler)(int)))(int)
   throw()
{  struct sigaction act;
   struct sigaction oact;
   
   act.sa_handler = handler;
   sigemptyset(&act.sa_mask);
   act.sa_flags = 0;
   
   if(signum == SIGALRM)
     {
#ifdef SA_INTERRUPT	
	act.sa_flags |= SA_INTERRUPT;		// SunOS 4.x
#endif	
     }
   else
     {
#ifdef SA_RESTART
	act.sa_flags |= SA_RESTART;		// SVR4, 4.4BSD
#endif	
     }
   if(sigaction(signum, &act, &oact) < 0)
     {
	displ_error("signal");
	exit(err_signal);
     }
   
   return oact.sa_handler;
}


char * 
Wrapper::Exit::shmat
  (
   int    shmid, 
   char * shmaddr, 
   int    shmflg
  )
   throw()
{  char * ret_val = (char *) ::shmat(shmid, shmaddr, shmflg);
   
   if(ret_val == (char *) -1)
     {
        displ_error("shmat");
	exit(err_shmat);
     }
   
   return ret_val;
}
