#include "CircularPtr.h"


/*****************************************************************************
  
 Handle class
 
*****************************************************************************/

class CircularPtr::Impl
{
 public:
        char       * const lower_limit;
        char       * const upper_limit;
        char       *       current_value;
        unsigned int const min_block_size;
   
        Impl(char * lower, char * upper, unsigned int min_size) throw();
};


/*----------------------------------------------------------------------------
 konstruktor 
----------------------------------------------------------------------------*/

CircularPtr::Impl::Impl
  (
   char *       const lower,
   char *       const upper,
   unsigned int const min_size
  )
   throw()
:  lower_limit   (lower),
   upper_limit   (upper),
   current_value (lower),
   min_block_size(min_size)
{
}


/*****************************************************************************
  
 class CircularPtr 
 
*****************************************************************************/


/*----------------------------------------------------------------------------
 konstruktor 
----------------------------------------------------------------------------*/

CircularPtr::CircularPtr
  (
   char *       const lower,
   char *       const upper,
   unsigned int const min_size
  )
   throw()
:  p(new Impl(lower, upper, min_size))
{
}


/*----------------------------------------------------------------------------
 destruktor
----------------------------------------------------------------------------*/

CircularPtr::~CircularPtr()
   throw()
{
   delete p;
}


/*----------------------------------------------------------------------------
 public metode 
----------------------------------------------------------------------------*/

char * 
CircularPtr::current_value() 
   const
   throw()
{
   return p->current_value;
}


char * 
CircularPtr::upper_limit() 
   const 
   throw()
{
   return p->upper_limit;
}


CircularPtr & 
CircularPtr::operator+=
  (
   unsigned int const right
  ) 
   throw()
{
   p->current_value += right;
   if(p->current_value + p->min_block_size > p->upper_limit)
	p->current_value = p->lower_limit;
   
   return *this;
}
