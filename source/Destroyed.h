#ifndef DESTROYED_H
#define DESTROYED_H

#include "Connection.h"
#include <unistd.h>  // pid_t

class Message;
class Exception;


class Destroyed : public Connection
{
 public:
        Destroyed(pid_t pid);
   
   	bool    destroyed()      const throw();
   
        write_rv write(Message const &) throw(Exception);
        read_rv  read(Message *)        throw(Exception);
};


#endif
