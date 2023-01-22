#ifndef SHM_LOCK_H
#define SHM_LOCK_H

#include <signal.h>


struct shm_lock_t
{
   volatile int turn;
   volatile int status[2];
};


enum shm_lock_id_t
{ 
   writer,
   reader 
};



void lock  (shm_lock_t volatile * plock, shm_lock_id_t id) throw();
void unlock(shm_lock_t volatile * plock, shm_lock_id_t id) throw();


#endif

