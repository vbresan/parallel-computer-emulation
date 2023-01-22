#include "non_blocking.h"

#include "txt.h"
#include "Exception.h"

#include <unistd.h>
#include <errno.h>

#include <iostream>


write_rv
writen
  (
   int          const fd, 
   const void * const vptr, 
   size_t       const n
  )
   throw(Exception)
{  size_t	nleft;
   ssize_t	nwritten;
   const char * ptr;
   
   ptr   = (char const *) vptr;
   nleft = n;
   while(nleft > 0) 
     {
	if((nwritten = write(fd, ptr, nleft)) <= 0) 
	  {
	     if(errno == EINTR)
		  nwritten = 0;
	     else if(errno == EAGAIN)
	       {
		  if(nleft == n)
		       return try_again;
		  else
		    {
		       nwritten = 0;
		       usleep(1);
		    }
	       }
	     else if(errno == EPIPE)
		  return conn_dstrw;
	     else
		  throw Exception(TXT_EXCPT_WRITE);
	  }
	
	nleft -= nwritten;
	ptr   += nwritten;
     }
   
   return write_OK;
}


read_rv
readn
  (
   int    const fd, 
   void * const vptr, 
   size_t const n
  )
   throw(Exception)
{  size_t  nleft;
   ssize_t nread;
   char	*  ptr;
   
   ptr   = (char *) vptr;
   nleft = n;
   while(nleft > 0) 
     {
	if((nread = read(fd, ptr, nleft)) < 0) 
	  {
	     if(errno == EINTR)
		  nread = 0;
	     else if(errno == EAGAIN)
	       {
		  if(nleft == n)
		       return no_msgs;
		  else
		    {
		       nread = 0;
		       usleep(1);
		    }
	       }
	     else
		  throw Exception(TXT_EXCPT_READ);
	  } 
	else if(nread == 0)
	     return conn_dstrr;		/* EOF */
	
	nleft -= nread;
	ptr   += nread;
     }
   
   return read_OK;
}
