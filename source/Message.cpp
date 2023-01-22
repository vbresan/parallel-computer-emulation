#include "Message.h"

#include "txt.h"
#include "Exception.h"

/*****************************************************************************
  
 Handle class 
  
*****************************************************************************/

class Message::Impl
{
 public:
   	int    mid;
   	size_t len;
   	char * data;
   
   	Impl(int i_mid, size_t i_len);
};


/*----------------------------------------------------------------------------
 konstruktor 
----------------------------------------------------------------------------*/

inline
Message::Impl::Impl
  (
   int    const i_mid,
   size_t const i_len
  )
:  mid(i_mid),
   len(i_len)
{
}


/*****************************************************************************
  
 Message 
  
*****************************************************************************/

/*----------------------------------------------------------------------------
 konstruktor(i) 
----------------------------------------------------------------------------*/

Message::Message
  (
   int          const i_mid,
   size_t       const i_len,
   char const * const i_data
  )
   throw(bad_alloc)
:  p(new Impl(i_mid, i_len))
{
   if(p->len)
     {
	p->data = new char [p->len];
	if(i_data != NULL)
	     memcpy(p->data, i_data, p->len);
     }
   else
	p->data = NULL;
}


Message::Message
  (
   Message const & other
  )
   throw(bad_alloc)
:  p(new Impl(other.p->mid, other.p->len))
{
   if(p->len)
     {
	p->data = new char [p->len];
	memcpy(p->data, other.p->data, p->len);
     }
   else
	p->data = NULL;
}


Message::Message
  (
   Message const & other,
   int     const   new_mid
  )
   throw(bad_alloc)
:  p(new Impl(new_mid, other.p->len))    
{
   if(p->len)
     {
	p->data = new char [p->len];
	memcpy(p->data, other.p->data, p->len);
     }
   else
	p->data = NULL;
}


Message::Message
  (
   Message      const & other,
   int          const   new_mid,
   unsigned int const   offset,
   size_t	const   size
  )
   throw(Exception, bad_alloc)
:  p(new Impl(new_mid, size))    
{
   if(offset + size <= other.p->len)
	if(p->len)
	  {
	     p->data = new char [p->len];
             memcpy(p->data, other.p->data + offset, p->len);
	  }
	else
	     p->data = NULL;
   else
	throw Exception(TXT_EXCPT_MSG_INVLD);
}


/*----------------------------------------------------------------------------
 destruktor 
----------------------------------------------------------------------------*/

Message::~Message()
   throw()
{
   delete [] p->data;
   delete p;
}


/*----------------------------------------------------------------------------
 public metode 
----------------------------------------------------------------------------*/

Message &
Message::operator=
  (
   Message const & other
  )
   throw(bad_alloc)
{
   if(this != &other)
     {
	p->mid = other.p->mid;
	p->len = other.p->len;
	delete [] p->data;
	
	if(p->len)
	  {
	     p->data = new char [p->len];
             memcpy(p->data, other.p->data, p->len);
	  }
	else
	     p->data = NULL;
     }
   
   return *this;
}


void *
Message::copy_data_to
  (
   void * const to
  )
   const
   throw()
{
   return memcpy(to, p->data, p->len);
}


int
Message::compare_data
  (
   void const * const with,
   size_t const       len
  )
   const
   throw(Exception)
{
   if(p->len < len)
	throw Exception(TXT_EXCPT_INVLD_REF);
   
   return memcmp(p->data, with, len);
}


int
Message::int_data_at
  (
   unsigned int const at
  )
   const
   throw(Exception)
{
   if((at + 1) * sizeof(int) > p->len)
	throw Exception(TXT_EXCPT_INVLD_REF);
   
   return ((int *) p->data)[at];
}


char
Message::char_data_at
  (
   unsigned int const at
  )
   const
   throw(Exception)
{
   if((at + 1) * sizeof(char) > p->len)
	throw Exception(TXT_EXCPT_INVLD_REF);
   
   return ((char *) p->data)[at];
}


int
Message::mid()
   const
   throw()
{
   return p->mid;
}


size_t
Message::len()
   const
   throw()
{
   return p->len;
}
