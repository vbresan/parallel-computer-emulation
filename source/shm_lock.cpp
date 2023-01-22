#include "shm_lock.h"

#include <unistd.h>	// usleep


enum state
{
   unlocked,
   locked
};


void 
lock
  (
   shm_lock_t    volatile * const plock,
   shm_lock_id_t            const id
  )
   throw()
{
   plock->status[id] = locked;
   while(plock->status[!id] == locked)
	if(plock->turn == !id)
	  {
	     plock->status[id] = unlocked;
	     while(plock->turn == !id)
		  usleep(1);
	     plock->status[id] = locked;
	  }
	else
	     usleep(1);
}


void 
unlock  
  (
   shm_lock_t volatile * const plock,
   shm_lock_id_t            const id
  )
   throw()
{
   plock->turn = !id;
   plock->status[id]  = unlocked;
}
