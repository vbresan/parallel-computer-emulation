#ifndef BUILTIN_MSG_H
#define BUILTIN_MSG_H

#include <limits.h>


//	NMO    - not memory owner (nije vlasnik memorije)
//      MO     - memory owner (vlasnik memorije)
// 	*      - bilo koji proces


enum builtin_msg 
{
   bm_attach = INT_MIN,		// MASTER   -> NMO   
   bm_attached,			// NMO      -> MO 

   bm_route_req,		// SLAVE    -> MASTER
   bm_create_route,		// MASTER   -> MO 
   
   bm_send,			// *        -> *
   
   bm_start_new,		// SLAVE    -> MASTER

   bm_kill,			// SLAVE    -> MASTER
   bm_terminate,		// MASTER   -> SLAVE(1)
   bm_terminating,		// SLAVE(1) -> SLAVE(2)
   bm_terminating_ack,		// SLAVE(2) -> SLAVE(1)
   bm_terminated,		// SLAVE(1) -> SLAVE(2)
   
   bm_no_such_pid,		// MASTER   -> SLAVE
   
   bm_padd			// *        -> *
};


#endif
