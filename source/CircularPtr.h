#ifndef CIRCULARPTR_H
#define CIRCULARPTR_H


class CircularPtr
{
 private:
        class Impl;
        Impl * const p;
   
 public:
   	CircularPtr(char       * i_lower_limit, 
		    char       * i_upper_limit, 
		    unsigned int i_min_block_size)       throw();
   	~CircularPtr() throw();
   
   	CircularPtr & operator+=(unsigned int right)     throw();
   
   	char * current_value() const throw();
   	char * upper_limit()   const throw();
};


#endif
