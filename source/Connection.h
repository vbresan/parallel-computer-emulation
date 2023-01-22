#ifndef CONNECTION_H
#define CONNECTION_H

#include <sys/types.h>		// pid_t

class Message;
class Exception;


enum read_rv
{
   read_OK,
   conn_dstrr,
   no_msgs,
   no_mem
};


enum write_rv
{
   write_OK,
   conn_dstrw,
   try_again
};


class Connection
{
 private:
        class Impl;
        Impl * const p;
   
 public:
        Connection(pid_t i_pid = 0, bool i_valid = false) throw();
   	virtual ~Connection()                 throw();
   
   	virtual bool    destroyed()     const throw() = 0;
   
   	virtual write_rv write(Message const & i_msg) = 0;
   	virtual read_rv  read(Message * o_msg) throw(Exception) = 0;
   
      	void  valid(bool new_state)     throw();
   	bool  valid()          	  const throw();
   
        void  pid(pid_t new_pid)	throw();
   	pid_t pid()	          const throw();
};


#endif
