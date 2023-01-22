#include "SlaveProcess.h"

#include "TCPConnection.h"
#include "SharedMem.h"
#include "builtin_msg.h"
#include "Message.h"
#include "Exception.h"
#include "txt.h"

#include <unistd.h>	// getppid


/*****************************************************************************
*****************************************************************************/

#define PRINTF_1(s)       std::cerr << *this << (s) << std::endl


/*****************************************************************************
  
 SlaveProcess 
  
*****************************************************************************/

/*----------------------------------------------------------------------------
 konstruktor(i)
----------------------------------------------------------------------------*/


SlaveProcess::SlaveProcess
  (
   char const * const i_name, 
   int	const	      i_shmid,
   p_action	      i_u_proc
  ) 
   throw()
:  MessageProcessor(i_name, i_u_proc)
{  
   connection_.push_back(new SharedMem(i_shmid, getppid()));
   send(0, Message(bm_attached));
}


SlaveProcess::SlaveProcess
  (
   char const * const i_name, 
   char const * const i_servaddr,
   p_action	      i_u_proc
  ) 
   throw()
:  MessageProcessor(i_name, i_u_proc)
{  
   connection_.push_back(new TCPConnection(i_servaddr));
}


/*----------------------------------------------------------------------------
 destruktor 
----------------------------------------------------------------------------*/

SlaveProcess::~SlaveProcess()
   throw()
{  Message msg(bm_terminating);
   int     last;
   
   for(int i = connection_.size() - 1; i > 0; --i)
	if(!connection_[i]->destroyed())
	     send(i, msg);
   
   while((last = connection_.size() - 1) >= 1)
	if(!connection_[last]->destroyed())
	  {
	     while(loop())
		  ;
	     usleep(1);
	  }
	else
	  {
	     delete connection_[last];
	     connection_.erase(connection_.begin() + last);
	  }

   PRINTF_1(TXT_DSCNCT_MASTER);
   
   delete connection_[0];
   connection_.pop_back();
}


/*----------------------------------------------------------------------------
 public metode
----------------------------------------------------------------------------*/

void 
SlaveProcess::start_new()
   throw()
{  
   send(0, Message(bm_start_new));
}


void
SlaveProcess::start_new
  (
   char * const cstr_rsh
  )
   throw()
{
   send(0, Message(bm_start_new, strlen(cstr_rsh) + 1, cstr_rsh));
}
   

void
SlaveProcess::kill
  (
   conn_no_t const whom 
  )
   throw(Exception)
{  
   if(whom < connection_.size() && !connection_[whom]->destroyed())
     {	int data = connection_[whom]->pid();
	
	send(0, Message(bm_kill, sizeof(data), (char *) &data));
     }
   else
	throw Exception(TXT_EXCPT_KILL);
}


bool
SlaveProcess::route
  (
   conn_no_t const from,
   conn_no_t const to
  )
   throw()
{  int data[] = { connection_[from]->pid(), connection_[to]->pid() };
   
   send(0, Message(bm_route_req, sizeof(data), (char *) data));
   return true;
}
 

bool
SlaveProcess::master()
   const
   throw()
{
   return false;
}


bool
SlaveProcess::slave()
   const
   throw()
{
   return true;
}
    
