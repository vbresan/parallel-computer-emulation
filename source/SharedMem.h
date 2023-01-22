#ifndef SHAREDMEM_H
#define SHAREDMEM_H

#include <unistd.h>	// pid_t
#include "Connection.h"

class Message;


class SharedMem : public Connection
{
 private:
   	class  Impl;
     	Impl * const p;
   
 public:
   	SharedMem()                             throw();
   	SharedMem(int i_shmid, pid_t i_pid)     throw();
   	~SharedMem() 		   		throw();

        bool    destroyed() const throw();
   
        write_rv write(Message const & i_msg); 
   	read_rv  read(Message * o_msg)           throw();
   
   	int id() const throw();
};


#endif
