#ifndef CHILD_H
#define CHILD_H

#include <string>
#include <exception>


class Child
{
 public:
   	Child(char const * name, int shmid) throw();
   	Child(char const * cstr_rsh, 
	      std::string const & servaddr) throw(bad_alloc);
};


#endif
