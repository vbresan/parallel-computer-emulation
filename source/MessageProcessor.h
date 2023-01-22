#ifndef MESSAGEPROCESSOR_H
#define MESSAGEPROCESSOR_H

#include "Process.h"

class Message;


typedef void (*p_action)(unsigned int sender_no, Message const & msg);


class MessageProcessor : public Process
{
 private:
   	bool	 active_;
   	p_action users_proc_;
   
   	void process_message(conn_no_t            sender_no, 
			     Message const & msg) throw(Exception);
   	void ba_create_route(Message const & msg) throw();
        void ba_send(conn_no_t sender_no, Message const & msg) throw();
   	void ba_start_new(conn_no_t sender_no, Message const & msg)
     							throw(bad_alloc);
 public:
   	MessageProcessor(char const * i_name, p_action i_u_proc);
   
   	void loop_forever();
   	bool loop();
};


#endif
