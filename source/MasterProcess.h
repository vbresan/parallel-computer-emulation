#ifndef MASTERPROCESS_H
#define MASTERPROCESS_H

#include "MessageProcessor.h"

class Exception;


class MasterProcess : public MessageProcessor
{
 public:
   	MasterProcess(char const * const i_name, 
		      p_action            i_u_proc) throw(); 
   	~MasterProcess()                           throw();
   
   	void start_new()            	  throw(Exception);
   	void start_new(char * cstr_rsh) throw(Exception);
     
   	void kill(conn_no_t whom) throw(Exception);
        bool route(conn_no_t from, 
		   conn_no_t to)   throw();   
   
   	bool master() const throw();
   	bool slave()  const throw();
};


#endif
