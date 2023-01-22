#ifndef SLAVEPROCESS_H
#define SLAVEPROCESS_H

#include "MessageProcessor.h"

class Exception;


class SlaveProcess : public MessageProcessor
{
 public:
   	SlaveProcess(char const * i_name, 
		     int          i_shmid, 
		     p_action     i_u_proc) throw();
   	SlaveProcess(char const * i_name, 
		     char const * i_servaddr, 
		     p_action     i_u_proc) throw();
   	~SlaveProcess()                     throw();
   
   	void start_new()                  throw();
   	void start_new(char * cstr_rsh) throw();
   
   	void kill(conn_no_t whom) 	throw(Exception);
   	bool route(conn_no_t from,
		   conn_no_t to)	throw();
   
   	bool master() const throw();
   	bool slave()  const throw();
};


#endif
