#include "Process.h"

#include "txt.h"
#include "builtin_msg.h"
#include "exit_errors.h"
#include "SharedMem.h"
#include "Destroyed.h"

#include <string>
#include <unistd.h>	// usleep, getpid
#include <sys/types.h>  // kill
#include <signal.h>     // kill


/*****************************************************************************
*****************************************************************************/

#define PRINTF_1(msg)	std::cerr << *this << (msg) << std::endl
#define PRINTF_2(msg)	PRINTF_1(msg)


/*****************************************************************************
  
 Implemenatcija 
  
*****************************************************************************/

class Process::Impl
{
 public:
   	std::string name;
   	static int  count;
   
   	Impl(char const * i_name);
};

int Process::Impl::count = 0;


/*----------------------------------------------------------------------------
 konstruktor 
----------------------------------------------------------------------------*/

Process::Impl::Impl
  (
   char const * const i_name
  )
:  name(i_name)  
{  
}


/*****************************************************************************
  
 Process 
  
*****************************************************************************/

/*----------------------------------------------------------------------------
 konstruktor 
----------------------------------------------------------------------------*/

Process::Process
  (
   char const * const i_name
  )
:  p(new Impl(i_name))
{
   if( ++(p->count) > 1)
     {
	std::cerr << TXT_EXIT_PROC_COUNT << std::endl;
	exit(err_proc_count);
     }
}


/*----------------------------------------------------------------------------
 destruktor
----------------------------------------------------------------------------*/

Process::~Process()
{  
   PRINTF_2("Process dtor");   
   --(p->count);
   delete p;
}


/*----------------------------------------------------------------------------
 protected metode 
----------------------------------------------------------------------------*/

void 
Process::destroy_connection
  (
   conn_no_t const conn_no
  )
   throw()
{  Connection * destroyed = new Destroyed(connection_[conn_no]->pid());
   
   delete connection_[conn_no];
   connection_[conn_no] = destroyed;
}


/*----------------------------------------------------------------------------
 public metode 
----------------------------------------------------------------------------*/

char const *
Process::name() 
   const 
   throw()
{
   return p->name.c_str();
}


send_rv 
Process::send
  (
   conn_no_t const   conn_no, 
   Message   const & i_msg
  )
{  send_rv  ret_val = sent;
   write_rv wret_val;
   
   while(!connection_[conn_no]->destroyed() && 
	 !connection_[conn_no]->valid())
	if(!loop())
	     usleep(1);
   
   wret_val = connection_[conn_no]->write(i_msg);
   while(!connection_[conn_no]->destroyed() && wret_val != write_OK)
     {
	if(wret_val == conn_dstrw)
	     destroy_connection(conn_no);
	else
	  {
//	     cerr << "pokusao upisati" << endl;
	     
	     if(!loop())
		  usleep(1);
	  }
	
	wret_val = connection_[conn_no]->write(i_msg);
     }
   
   if(connection_[conn_no]->destroyed())
	ret_val = destroyed;
   
   return ret_val;
}


bool
Process::receive
  (
   conn_no_t const conn_no,
   Message * const o_msg
  )
{  read_rv conn_ret_val;
   
   conn_ret_val = connection_[conn_no]->read(o_msg);
   if(conn_ret_val == read_OK)
	return true;
   else if(conn_ret_val == conn_dstrr)
	destroy_connection(conn_no);
   
   return false;
}


/*----------------------------------------------------------------------------
 ostalo 
----------------------------------------------------------------------------*/

std::ostream & 
operator<<
  (
   std::ostream  & out, 
   Process const & p
  )
{
   return out << p.name() << " [" << getpid() << "] ";
}
