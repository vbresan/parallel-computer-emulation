#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <exception>


class Exception : public exception
{
 private:
        class  Impl;
        Impl * p;
   
 public:
   	Exception(char const * i_msg)	throw();
        ~Exception()			throw();
   
   	char const * what() 	  const throw();
};

#endif
