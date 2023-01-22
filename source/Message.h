#ifndef MESSAGE_H
#define MESSAGE_H

#include <stdio.h>	// NULL
#include <exception>

class Exception;


class Message
{
 private:
   	class Impl;
   	Impl * const p;
   
 public:
   	Message(int          i_mid = 0, 
		size_t       i_len = 0, 
		char const * i_data = NULL) 		throw(bad_alloc);
   	Message(Message const & other)	    		throw(bad_alloc);
   	Message(Message const & other, int new_mid)	throw(bad_alloc);
   	Message(Message const & other, int new_mid, 
		unsigned int offset,
		size_t       size)	     throw(Exception, bad_alloc);
   	~Message() throw();
   
   	Message & operator=(Message const & other)      throw(bad_alloc);
   
   	void * copy_data_to(void * to)         const throw();
   	int    compare_data(void const * with,
			    size_t       len)  const throw(Exception);
   
        int    int_data_at(unsigned int at)  const throw(Exception);
   	char   char_data_at(unsigned int at) const throw(Exception);
   	int    mid()                         const throw();
   	size_t len()		             const throw();
};


#endif
