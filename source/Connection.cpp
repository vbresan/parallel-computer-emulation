#include "Connection.h"

#include "Wrapper.h"

#include <iostream>
#include <unistd.h>	// getpid
#include <signal.h>	// SIGIO


/*****************************************************************************
  
 Handle class 
  
*****************************************************************************/

class Connection::Impl
{
 public:
       pid_t pid;
       bool  valid;
   
       Impl(pid_t i_pid, bool i_valid) throw();
};


/*----------------------------------------------------------------------------
 konstruktor 
----------------------------------------------------------------------------*/

inline
Connection::Impl::Impl
  (
   pid_t const i_pid,
   bool  const i_valid
  )
   throw()
:  pid  (i_pid),
   valid(i_valid)
{
}


/*****************************************************************************
  
 Connection 
  
*****************************************************************************/

/*----------------------------------------------------------------------------
 konstruktor
----------------------------------------------------------------------------*/

Connection::Connection
  (
   pid_t const i_pid,
   bool  const i_valid
  ) 
   throw()
:  p(new Impl(i_pid, i_valid))
{
/*   std::cerr << getpid() << " -> " << p->pid 
             << " Connection ctor" 
             << std::endl;*/
}


/*----------------------------------------------------------------------------
 destruktor
----------------------------------------------------------------------------*/

Connection::~Connection()
   throw()
{  
   delete p;
/*   std::cerr << getpid() << " -> " << p->pid << " Connection dtor" 
             << std::endl;*/
}


/*----------------------------------------------------------------------------
 public metode
----------------------------------------------------------------------------*/

write_rv
Connection::write
  (
   Message const &
  )
{
  ::kill(p->pid, SIGUSR1);

   return write_OK;
}


void 
Connection::valid
  (
   bool const new_state
  ) 
   throw()
{
   p->valid = new_state;
}


bool 
Connection::valid() 
   const 
   throw()
{
   return p->valid;
}


void 
Connection::pid
  (
   pid_t const new_pid
  ) 
   throw()
{
   p->pid = new_pid;
}


pid_t 
Connection::pid() 
   const 
   throw()
{
   return p->pid;
}
