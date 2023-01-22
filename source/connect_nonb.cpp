#include "connect_nonb.h"

#include "Wrapper.h"
#include "Exception.h"
#include "txt.h"

#include <unistd.h>
#include <stdio.h>
#include <sys/select.h>
#include <sys/time.h>
#include <errno.h>
#include <fcntl.h>

using namespace Wrapper;


int
connect_nonb
  (
   int              const sockfd, 
   sockaddr const * const saptr, 
   socklen_t        const salen, 
   int              const nsec
  )
   throw(Exception)
{  int       flags, n, error;
   socklen_t len;
   fd_set    rset, wset;
   timeval   tval;

   flags = Throw::fcntl(sockfd, F_GETFL, 0);
   Throw::fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);

   error = 0;
   if((n = connect(sockfd, (struct sockaddr *) saptr, salen)) < 0)
	if(errno != EINPROGRESS)
	     return -1;

   if(n != 0)
     {
	FD_ZERO(&rset);
	FD_SET(sockfd, &rset);
	wset = rset;
	tval.tv_sec = nsec;
	tval.tv_usec = 0;

	if((n = Throw::select(sockfd+1, &rset, &wset, NULL, 
			      nsec ? &tval : NULL)) == 0) 
	  {
	     close(sockfd);		/* timeout */
	     errno = ETIMEDOUT;
	     return -1;
	  }

	if(FD_ISSET(sockfd, &rset) || FD_ISSET(sockfd, &wset)) 
	  {
	     len = sizeof(error);
	     if(getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, &len) < 0)
		  return -1;			/* Solaris pending error */
	  } 
	else
	     throw Exception(TXT_EXCPT_SELECT);
     }
   
//   Throw::fcntl(sockfd, F_SETFL, flags);	/* restore file status flags */
   
   if(error)
     {
	close(sockfd);		/* just in case */
	errno = error;
	return -1;
     }
   

   return 0;
}
