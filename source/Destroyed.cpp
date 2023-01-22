#include "Destroyed.h"

#include "txt.h"
#include "Exception.h"


/*****************************************************************************
  
 Destroyed 
  
*****************************************************************************/

/*----------------------------------------------------------------------------
 konstruktor
----------------------------------------------------------------------------*/

Destroyed::Destroyed
  (
   pid_t pid
  )
:  Connection(pid)
{
}


/*----------------------------------------------------------------------------
 public metode
----------------------------------------------------------------------------*/

bool
Destroyed::destroyed()
   const
   throw()
{
   return true;
}
 

write_rv
Destroyed::write
  (
   Message const &
  )
   throw(Exception)
{
   throw Exception(TXT_EXCPT_W_DSTRY);
}


read_rv 
Destroyed::read
  (
   Message *
  )
   throw(Exception)
{
   throw Exception(TXT_EXCPT_R_DSTRY);
}
