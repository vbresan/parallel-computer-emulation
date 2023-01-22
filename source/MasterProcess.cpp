#include "MasterProcess.h"

#include "SharedMem.h"
#include "TCPConnection.h"
#include "Child.h"
#include "Wrapper.h"
#include "builtin_msg.h"
#include "Message.h"
#include "Exception.h"
#include "txt.h"

#include <sys/types.h>		// pid_t
#include <signal.h>		// kill 
#include <iostream>
#include <string>

#include <netinet/in.h>
#include <arpa/inet.h>

using namespace Wrapper;


/*****************************************************************************
  
 MasterProcess 
  
*****************************************************************************/

/*----------------------------------------------------------------------------
 konstruktor 
----------------------------------------------------------------------------*/

MasterProcess::MasterProcess
  (
   char const * const i_name,
   p_action            i_u_proc
  )
   throw()
:  MessageProcessor(i_name, i_u_proc)
{
}


/*----------------------------------------------------------------------------
 destruktor
----------------------------------------------------------------------------*/

MasterProcess::~MasterProcess()
   throw()
{  Message const msg(bm_terminate);
   int           last;

   while((last = connection_.size() - 1) >= 0)
     {
	if(!connection_[last]->destroyed())
	  {
	     send(last, msg);
	     while(::kill(connection_[last]->pid(), 0) != -1)
	       {
		  while(loop())
		    ;
		  usleep(1);
	       }
	  }
	
	delete connection_[last];
	connection_.erase(connection_.begin() + last);
     }
}


/*----------------------------------------------------------------------------
 public metode 
----------------------------------------------------------------------------*/

void 
MasterProcess::start_new() 
   throw(Exception)
{  SharedMem * pSharedMem = new SharedMem;
   
   try
     {  pid_t child_pid = Wrapper::Throw::fork();

	if(child_pid > 0)
          {
             pSharedMem->pid(child_pid);
	     connection_.push_back(pSharedMem);
          }
	else
	  {  Child   child(name(), pSharedMem->id());
	     Child * dummy;
	     
	     dummy = &child;   // da bi izbjegli warning compilera
	  }
     }
   catch(exception &)
     {
	delete pSharedMem;
	throw;
     }
}


void
MasterProcess::start_new
  (
   char * const cstr_rsh
  )
   throw(Exception)
{  TCPConnection * pTCPConnection = new TCPConnection;
   
   try
     {  pid_t child_pid = Throw::fork();
	
	if(child_pid > 0)
          {  sockaddr_in cliaddr;
	     socklen_t   len = sizeof(cliaddr);
	     char        buffer[256];
	     
	     pTCPConnection->connfd(Throw::accept(pTCPConnection->listenfd(), 
						  (sockaddr *) &cliaddr, 
						  &len));
	
	     std::cerr << "Prihvatio klijenta!" 
	               << inet_ntop(AF_INET, &cliaddr.sin_addr, buffer, 256) 
		       << ':' << ntohs(cliaddr.sin_port) << std::endl;
	     
	     pTCPConnection->close_listenfd();
	     pTCPConnection->pid(child_pid);
	     connection_.push_back(pTCPConnection);
          }
	else
	  {  Child   child(cstr_rsh, pTCPConnection->servaddr());
	     Child * dummy;
	     
	     dummy = &child;   // da bi izbjegli warning compilera
	  }
     }
   catch(exception &)
     {
	delete pTCPConnection;
	throw;
     }
}


void 
MasterProcess::kill
  (
   conn_no_t const whom
  ) 
   throw(Exception)
{  
   if(whom < connection_.size() && !connection_[whom]->destroyed())
	send(whom, Message(bm_terminate));
   else
	throw Exception(TXT_EXCPT_KILL);
   
}


bool 
MasterProcess::route
  (
   conn_no_t const from,
   conn_no_t const to
  )
   throw()
{
   if((from != to) && ((from > to)? from : to) <= connection_.size() - 1)
     {  int data = connection_[to]->pid();
	
	send(from, Message(bm_create_route, sizeof(data), (char *) &data));
	return true;
     }
   else
        return false;
}


bool 
MasterProcess::master() 
   const 
   throw()
{
   return true;
}


bool
MasterProcess::slave()
   const
   throw()
{
   return false;
}
