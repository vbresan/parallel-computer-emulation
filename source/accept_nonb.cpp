#include "accept_nonb.h"

#include "Wrapper.h"
#include "Exception.h"
#include "txt.h"
#include <errno.h>
#include <fcntl.h>

using namespace Wrapper;


int
accept_nonb
  (
   int         const s,
   sockaddr  * const addr,
   socklen_t * const addrlen
  )
   throw (Exception)
{  int ret_val;
   int flags;
   
   while(true)
	if((ret_val = ::accept(s, addr, addrlen)) == -1)
	  {
	     if(errno != EINTR)
		  throw Exception(TXT_EXCPT_ACCEPT);
	  }
	else
	     break;

   flags = Throw::fcntl(ret_val, F_GETFL, 0);
   Throw::fcntl(ret_val, F_SETFL, flags | O_NONBLOCK);
   
   return ret_val;
}
