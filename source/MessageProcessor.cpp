#include "MessageProcessor.h"

#include "txt.h"
#include "builtin_msg.h"
#include "SharedMem.h"
#include "Message.h"
#include "Exception.h"


#define PRINTF_1(msg)	std::cerr << *this << (msg) << std::endl
#define PRINTF_2(msg)	PRINTF_1(msg)

/*****************************************************************************
  
 MessageProcessor 
  
*****************************************************************************/

/*----------------------------------------------------------------------------
 private
----------------------------------------------------------------------------*/

void
MessageProcessor::ba_start_new
  (
   conn_no_t const   /*sender_no*/,
   Message   const & i_msg
  )
   throw(bad_alloc)
{
   PRINTF_1(TXT_STRTN_REQ);
   
   if(i_msg.len())
     {  char * cstr_rsh = new char [i_msg.len()];
	
	start_new((char *) i_msg.copy_data_to(cstr_rsh));
	delete [] cstr_rsh;
     }
   else
	start_new();
   
/*   if(master())
	if(!route(sender_no, connection_.size() - 1))
	     PRINTF_1(TXT_INVALID_ROUTE);*/
}
  

void 
MessageProcessor::ba_create_route
  (
   Message const & i_msg
  )
   throw()
{
   /*       prima      */     /* salje (master-u)  */
   /*------------------*/     /*-------------------*/
   /* [0]   peer pid   */     /* [0]   peer pid    */
   /*------------------*/     /* [1]   bm attach   */
                              /* [2]   own pid     */
                              /* [3]   shm ID      */
                              /*-------------------*/
	     
   bool create = true;
   
   for(unsigned int i = 0; i < connection_.size() && create; ++i)
	if(connection_[i]->pid() == i_msg.int_data_at(0))
	     create = false;
   
   if(create)
     {
	PRINTF_1(TXT_ROUTE_CREAT);

	SharedMem * pSharedMem = new SharedMem;
	pSharedMem->pid(i_msg.int_data_at(0));
             
	connection_.push_back(pSharedMem);
              
	int new_data[] = { i_msg.int_data_at(0), bm_attach, getpid(), 
	                   pSharedMem->id() };
	     
	std::cerr << new_data[3] << std::endl;
	
	send(0, Message(bm_send, sizeof(new_data), (char *) new_data));
     }
   else
	PRINTF_1(TXT_ERRC_ROUTE);
}


void 
MessageProcessor::ba_send
  (
   conn_no_t const   sender_no,
   Message const & i_msg
  )
   throw()
{
   /*     receives       */     /*        sends       */
   /*--------------------*/     /*--------------------*/
   /* [0]   to pid       */	/* [...] data to send */	/*top*/
   /* [1]   msg id       */	/*--------------------*/
   /* [...] data to send */
   /*--------------------*/     
   
   int to_pid = i_msg.int_data_at(0);
   unsigned int const offset  = 2 * sizeof(int);
   size_t       const new_len = i_msg.len() - offset;
   bool         receiver_found = false;
	     
   for(unsigned int i = 0; i < connection_.size() && !receiver_found; ++i)
	if(connection_[i]->pid() == to_pid)
	     if(send(i, Message(i_msg, i_msg.int_data_at(1), offset, new_len)) 
		== sent)
		  receiver_found = true;
   
   if(!receiver_found)
	send(sender_no, Message(bm_no_such_pid, 
				sizeof(int), (char *) &to_pid));
}

void 
MessageProcessor::process_message
  (
   conn_no_t const   sender_no,
   Message const & i_msg
  )
   throw(Exception)
{
   switch(i_msg.mid())
     {
      	case bm_kill:
	     for(unsigned int i = 0; i < connection_.size(); ++i)
		  if(connection_[i]->pid() == i_msg.int_data_at(0))
		    {
		       kill(i);
		       break;
		    }
	break;
      	case bm_route_req:
	  {  
	     for(unsigned int i = 0; i < connection_.size(); ++i)
		  if(connection_[i]->pid() == i_msg.int_data_at(0))
		    {
		       for(unsigned int j = 0; j < connection_.size(); ++j)
			    if(connection_[j]->pid() == i_msg.int_data_at(1))
			      {
				 route(i, j);
				 break;
		    	      }
		       break;
		    }
	  }
	break;
	case bm_attached:
	     connection_[sender_no]->valid(true);
	     PRINTF_1(TXT_CONN_VALID);
	break;
        case bm_create_route:
	     ba_create_route(i_msg);
	break;
      	case bm_send:
	     PRINTF_1(TXT_SEND_REQ);
	     ba_send(sender_no, i_msg);
	break;
      	case bm_no_such_pid:
	     for(unsigned int i = 0; i < connection_.size(); ++i)
		  if(connection_[i]->pid() == i_msg.int_data_at(0))
		    {
		       destroy_connection(i);
		       break;
		    }
	break;
      	case bm_attach:
          {  
	     /*     receives     */
	     /*------------------*/
	     /* [0]   peer pid   */
	     /* [1]   shm id     */
	     /*------------------*/
	     
	     if(active_)
	       {
		  PRINTF_1(TXT_ATTCH_REQ);
		  
		  SharedMem * pSharedMem = new SharedMem(i_msg.int_data_at(1), 
							 i_msg.int_data_at(0));
		  
		  connection_.push_back(pSharedMem);
		  send(connection_.size() - 1, Message(bm_attached));
	       }
          }
	break;
        case bm_start_new:
	     ba_start_new(sender_no, i_msg);
	break;
      	case bm_terminate:
	     PRINTF_1(TXT_TRMNT_REQ);
	     active_ = false;
	break;
        case bm_terminating:
	     PRINTF_1(TXT_TRMNT_NTC);
	     send(sender_no, Message(bm_terminating_ack));
	     connection_[sender_no]->valid(false);
	break;
        case bm_terminating_ack:
	     PRINTF_1(TXT_TRMNT_ACK);
	     send(sender_no, Message(bm_terminated));
	     destroy_connection(sender_no);
	break;
        case bm_terminated:
	     destroy_connection(sender_no);
	break;
        case bm_padd:
	break;
        default:
	     if(users_proc_)
		  users_proc_(sender_no, i_msg);
	     else
	       	  throw Exception(TXT_EXCPT_MSG_UNKNWN);
	break;
     }
}


/*----------------------------------------------------------------------------
 konstruktor
----------------------------------------------------------------------------*/

MessageProcessor::MessageProcessor
  (
   char const * const i_name,
   p_action	      i_u_proc
  )
:  Process    (i_name),
   active_    (true),
   users_proc_(i_u_proc)
{
}


/*----------------------------------------------------------------------------
 public metode 
----------------------------------------------------------------------------*/

void 
MessageProcessor::loop_forever()
{  
   while(active_)
     {  
	while(loop())
	     ;
	usleep(1);
     }
}


bool 
MessageProcessor::loop()
{  bool again = false;
   
   for(unsigned int i = 0; i < connection_.size(); ++i)
     {  Message msg;
	
	if(!connection_[i]->destroyed() && 
	    receive(i, &msg))
	  {
	     process_message(i, msg);
	     again = true;
	  }
     }
   
   return again;
}
