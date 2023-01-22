#include "pce.h"

#include "Message.h"

#include <stdio.h>

#define I 	10000
#define MSG_LEN	2024


Process * pProcess;
int     * data;
int       counter;

enum my_messages
{
   test_send,
   test_receive
};


void 
my_proc
  (
   unsigned int         /*sender_no*/,
   Message const & message
  )
{
   switch(message.mid())
     {
        case test_send:
	  {
	     if(message.compare_data(data, MSG_LEN))
	       {
		  std::cout << "\aU dolasku primio krivu poruku! " << counter 
		            << std::endl;
		  for(unsigned int i = 0; i < message.len(); ++i)
		    {
		       std::cerr << (int) message.char_data_at(i) << ' ';
		    }
		  std::cerr << std::endl;
		  exit(11);
	       }
	     counter++;
	     std::cerr << counter << '\r';
/*	     
	     pProcess->send(0, Message(test_receive, 
                                       message.len(), message.data()););*/
	  }
	break;
/*        case test_receive:
	  {
	     if(memcmp(message.data, data, message.len))
	       {
		  std::cout << "\aU povratku primio krivu poruku! " << counter 
		            << std::endl;
		  for(int i = 0; i < MSG_LEN; ++i)
		    {
		       std::cerr << (int) *(((char *) message.data) + i)
			 	 << ' ';
		    }
		  std::cerr << std::endl;
		  exit(11);
	       }
	     counter++;
	  }
	break;*/
        default:
	     std::cout << "Primio nepoznatu poruku!" << std::endl;
	break;
     }
}


int main(int argc, char **argv)
{  
   pProcess = start_new(argc, argv, my_proc);
   
   data = (int *) malloc(MSG_LEN);
   memset(data, '?', MSG_LEN);
   
   if(pProcess->master())
     {  Message const msg(test_send, MSG_LEN, (char *) data);
	
	pProcess->start_new();
	for(int i = 0; i < I; ++i)
	  {  
	     pProcess->send(0, msg);
/*	     while(pProcess->loop())
		  ;*/
	  }
     }
   else
	pProcess->loop_forever();
   
   delete pProcess;
   return 0;
}
