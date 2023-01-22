#ifndef WRAPPER_H
#define WRAPPER_H

#include <sys/time.h>
#include <sys/types.h>		// pid_t, shmat, waitpid, socket
#include <sys/ipc.h>		// shmget
#include <sys/shm.h>
#include <sys/socket.h>		// bind
#include <netdb.h>		// gethostbyname

class Exception;


namespace Wrapper
{
   // ispisuju poruku o pogresci
   
   int shmctl(int shmid, int cmd, shmid_ds * buf) throw();
   int shmdt(char * shmaddr)			  throw();
   
   namespace Throw // bacaju iznimke
     {
	pid_t fork()                   throw(Exception); 
	int   kill(pid_t pid, int sig) throw(Exception);
	
	int    shmget(key_t key, int size, int shmflg)      throw(Exception); 
	char * shmat(int shmid, char * shmaddr, int shmflg) throw(Exception);
	
	int socket(int domain, int type, int protocol)      throw(Exception);
	int bind(int sockfd, sockaddr * my_addr, 
		 socklen_t addrlen)                         throw(Exception);
	int getsockname(int s, sockaddr * name, 
			socklen_t * namelen)	    	    throw(Exception);
	int listen(int s, int backlog)                      throw(Exception);
	int connect(int sockfd, sockaddr const * serv_addr, 
		    socklen_t addrlen, int nsec = 0)        throw(Exception);
	int accept(int s, sockaddr * addr, 
		   socklen_t * addrlen)			    throw(Exception);
	int gethostname(char * name, size_t namelen)        throw(Exception);
	hostent * gethostbyname(char const * hostname)      throw(Exception);
	int fcntl(int fd, int cmd, long arg)		    throw(Exception);
	int select(int n, fd_set * readfds, fd_set * writefds,
		   fd_set * exceptfds, timeval * timeout)   throw(Exception);
     }
   
   namespace Exit  // izlaze iz programa
     {
	int    execvp(char const * file, char const * const argv[]) throw();
        void   (*signal(int signum, void (*handler)(int)))(int)     throw();
	char * shmat(int shmid, char * shmaddr, int shmflg)         throw();
     }
}


#endif
