#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H

#include "Connection.h"

#include <unistd.h>	// pid_t
#include <exception>
#include <string>

class Message;


class TCPConnection : public Connection
{
 private:
   	class Impl;
   	Impl * const p;
   
 public:
   	TCPConnection();
   	TCPConnection(char const * const i_servaddr);
   	~TCPConnection() throw();
   
   	bool destroyed() const throw();
   	
   	write_rv write(Message const &) throw(bad_alloc);
   	read_rv  read(Message *);
   
   	void connfd(int i_connfd) throw();
   
   	std::string const & servaddr() const throw();
   	int  listenfd() 	       const throw();
   	void close_listenfd()	       const throw();
};

#endif
