#ifndef PROCESS_H
#define PROCESS_H

#include <deque>
//#include <vector>
#include <iosfwd>
#include <unistd.h>

class Connection;
class Message;
class Exception;


enum send_rv
{
   sent,
   destroyed
};


typedef unsigned int conn_no_t;

class Process
{
   
 private:
   	class Impl;
   	Impl * const p;
   
 protected:
        std::deque<Connection *> connection_;
   
	void destroy_connection(conn_no_t which) throw();
   
 public:
   	Process(char const * i_name);
   	virtual ~Process();
   
        char const * name() const throw();
   
   	send_rv send(conn_no_t conn_no, Message const & i_msg);
   	bool    receive(conn_no_t conn_no, Message * o_msg);
   
   	virtual void start_new()                throw(Exception) = 0;
   	virtual void start_new(char * cstr_rsh) throw(Exception) = 0;
      	virtual void kill (conn_no_t whom)      throw(Exception) = 0;
   	virtual bool route(conn_no_t from, 
			   conn_no_t to)        throw() = 0;
   
	virtual bool master() const throw() = 0;
   	virtual bool slave()  const throw() = 0;

   	virtual bool loop()         = 0;
        virtual void loop_forever() = 0;
};


std::ostream & operator<<(std::ostream & out, Process const & p);


#endif
