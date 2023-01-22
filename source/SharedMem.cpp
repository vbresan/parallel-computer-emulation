#include "SharedMem.h"

#include "Exception.h"
#include "Wrapper.h"
#include "txt.h"
#include "CircularPtr.h"
#include "Message.h"
#include "builtin_msg.h"
#include "shm_lock.h"

#include <deque>
#include <sys/ipc.h>
#include <sys/shm.h>
//#include <string.h>		 memset
#include <unistd.h>		// getpagesize
#include <signal.h>		// kill

#include "debug.h"

//using namespace Debug;


/*****************************************************************************
*****************************************************************************/

#define SHM_MODE (SHM_R | SHM_W)


struct raw_message_t
{
   int          mid;
   unsigned int len;
};


struct shm_message_t
{
   shm_lock_t    lock;
   int		 fresh;
   raw_message_t raw_message;
};


using namespace Wrapper;


/*****************************************************************************
  
 Handle class 
  
*****************************************************************************/

class SharedMem::Impl
{
 public:
        bool               const owner;
        unsigned int       const size;
   	int                const id;
        char *             const addr;
   	CircularPtr              cptr_read;
	CircularPtr              cptr_write;
   	std::deque<char *>       msg_w_addr;
   
        Impl()			throw(Exception);
   	Impl(int i_shmid)	throw();
   
      	bool fresh(shm_message_t volatile * p)	      	const throw();
   
   	unsigned int padded_len(unsigned int len)     	const throw();

   	shm_message_t volatile * r_addr() 		  	const throw();
        shm_message_t volatile * w_addr(unsigned int size)   	throw(Exception);
   
   	void write_padd(Message const &)		throw();
};


/*----------------------------------------------------------------------------
 konstruktor(i)
----------------------------------------------------------------------------*/

SharedMem::Impl::Impl()
   throw(Exception)
:  owner     (true),
   size      (getpagesize()),
   id        (Throw::shmget(IPC_PRIVATE, size, SHM_MODE)),
   addr      (Throw::shmat(id, 0, 0)),
   cptr_read (CircularPtr(addr + size / 2, addr + size, sizeof(shm_message_t))),
   cptr_write(CircularPtr(addr, addr + size / 2,        sizeof(shm_message_t)))
{
}


SharedMem::Impl::Impl
  (
   int const i_shmid
  )
   throw()
:  owner     (false),
   size      (getpagesize()),
   id        (i_shmid), 
   addr      (Exit::shmat(id, 0, 0)),
   cptr_read (CircularPtr(addr, addr + size / 2,        sizeof(shm_message_t))),
   cptr_write(CircularPtr(addr + size / 2, addr + size, sizeof(shm_message_t)))
{
}


/*----------------------------------------------------------------------------
 public metode
----------------------------------------------------------------------------*/

bool 
SharedMem::Impl::fresh
  (
   shm_message_t volatile * const p
  ) 
   const
   throw()
{  bool is_fresh = false;

   lock( &(p->lock), writer);
   
   if(p->fresh)
	is_fresh = true;
 
   unlock( &(p->lock), writer);
   
   return is_fresh;
}


inline 
unsigned int 
SharedMem::Impl::padded_len
  (
   unsigned int const len
  ) 
   const 
   throw()
{  unsigned int padd_exc = len % sizeof(int);
   
   return (len + ((padd_exc)? (sizeof(int) - padd_exc) : (0)));
}


shm_message_t volatile * 
SharedMem::Impl::w_addr
  (
   unsigned int const msg_size
  ) 
   throw(Exception)
{  
   if(msg_size <= size / 2)
     {  char *       pwrite   = cptr_write.current_value();
   	char * const pwrite_e = cptr_write.upper_limit();
	
	while(msg_w_addr.size() && (pwrite <= msg_w_addr[0]) 
	      			 && (pwrite + msg_size > msg_w_addr[0]))
	     if(!fresh((shm_message_t volatile *) msg_w_addr[0]))
		  msg_w_addr.pop_front();
	     else
		  return NULL;
	
	if(pwrite + msg_size > pwrite_e)
	  {  
	     write_padd(Message(bm_padd,
				pwrite_e - pwrite - sizeof(shm_message_t)));
	     return w_addr(msg_size);
	  }
	
	msg_w_addr.push_back(pwrite);
	return (shm_message_t volatile *) pwrite;
     }
   else
	throw Exception(TXT_EXCPT_MSG_BIG);
}


inline 
shm_message_t volatile * 
SharedMem::Impl::r_addr() 
   const 
   throw()
{  
   return (shm_message_t volatile *) cptr_read.current_value();
}


void 
SharedMem::Impl::write_padd
  (
   Message const & i_msg
  ) 
   throw()
{  shm_message_t volatile * const pmsg = w_addr(sizeof(shm_message_t));
   
   lock( &(pmsg->lock), writer);
   
   pmsg->fresh             = 1;
   (pmsg->raw_message).mid = i_msg.mid();
   (pmsg->raw_message).len = i_msg.len();

   unlock( &(pmsg->lock), writer);
   
   cptr_write += sizeof(shm_message_t) + i_msg.len();
}


/*****************************************************************************
  
 SharedMem 
  
*****************************************************************************/

/*----------------------------------------------------------------------------
 konstruktor(i) 
----------------------------------------------------------------------------*/

SharedMem::SharedMem() 
   throw()
:  p(new Impl)
{
   /*Debug::*/memset(p->addr, 0, p->size);
}


SharedMem::SharedMem
  (
   int   const i_shmid,
   pid_t const i_pid
  )
   throw()
:  Connection(i_pid, true),
   p(new Impl(i_shmid))
{
}


/*----------------------------------------------------------------------------
 destruktor 
----------------------------------------------------------------------------*/

SharedMem::~SharedMem() 
   throw()
{
   if(p->owner)
	Wrapper::shmctl(p->id, IPC_RMID, 0);
   
   Wrapper::shmdt(p->addr);
   
   delete p;
}


/*----------------------------------------------------------------------------
 public metode 
----------------------------------------------------------------------------*/

bool
SharedMem::destroyed()
   const
   throw()
{
   return false;
}


int 
SharedMem::id() 
   const 
   throw()
{
   return p->id;
}


write_rv
SharedMem::write
  (
   Message const & i_msg
  ) 
{  write_rv                       ret_val = try_again;
   shm_message_t volatile * const pmsg    = p->w_addr(sizeof(shm_message_t) + 
						   p->padded_len(i_msg.len()));
   
   if(::kill(pid(), 0) == -1)
	return conn_dstrw;
    
   if(pmsg == (shm_message_t volatile *) NULL)
	return ret_val;

   lock( &(pmsg->lock), writer);
   
   pmsg->fresh             = 1;
   (pmsg->raw_message).mid = i_msg.mid();
   (pmsg->raw_message).len = i_msg.len();
   if(i_msg.len())
	i_msg.copy_data_to((void *) ( &(pmsg->raw_message).len + 1));
   
   ret_val = write_OK;

   unlock( &(pmsg->lock), writer);
   
   if(ret_val == write_OK)
     {
        p->cptr_write += sizeof(shm_message_t) + p->padded_len(i_msg.len());
	Connection::write(i_msg);
//	cerr << "upisao" << endl;
     }
   
   return ret_val;
}


read_rv 
SharedMem::read
  (
   Message * o_msg
  ) 
   throw()
{  read_rv                        ret_val = no_msgs;
   shm_message_t volatile * const pmsg    = p->r_addr();
   
   if(::kill(pid(), 0) == -1)
	return conn_dstrr;
   
   lock( &(pmsg->lock), reader);
   
   if(pmsg->fresh)
     {  
	*o_msg = Message((pmsg->raw_message).mid, (pmsg->raw_message).len,
			 (char const *) (&((pmsg->raw_message).len) + 1));
	memset((void *) pmsg, 0, sizeof(shm_message_t) + o_msg->len());
	ret_val = read_OK;
     }
   
   unlock( &(pmsg->lock), reader);
   
   if(ret_val == read_OK)
     {
//	cerr << "procitao " << (void *) pmsg << endl;
	p->cptr_read += sizeof(shm_message_t) + p->padded_len(o_msg->len());
	Connection::write(*o_msg);
     }
   
   return ret_val;
}
