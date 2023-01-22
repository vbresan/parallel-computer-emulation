#include "pce.h"

#include "Message.h"

#define MAX_LEN 10000


Process * pProcess;
int     * data;
int       counter;

enum my_messages
{
   test_send,
   test_receive
};


void my_proc(unsigned int         /*sender_no*/,
	     Message const & message
	    )
{
   switch(message.mid())
     {
        case test_send:
	      pProcess->send(0, Message(message, test_receive));
	break;
        case test_receive:
	  {
	     if(message.compare_data(data + 2, 1847))
	       {
		  std::cout << "\aPrimio krivu poruku!" << counter 
		            << std::endl;
		  exit(11);
	       }
	     counter++;
	  }
	break;
        default:
	     std::cout << "Primio nepoznatu !" << std::endl;
	break;
     }
}


int main(int argc, char **argv)
{  
   pProcess = start_new(argc, argv, my_proc);
   
   if(pProcess->master())
     {
	pProcess->start_new();
	
	data = (int *) malloc(1847);
	memset(data, '?', 1847);
	Message const msg(test_send, 1847, (char *) data);
	
	for(int i = 0; i < MAX_LEN; ++i)
	  {  
	     pProcess->send(0, msg);
	     while(pProcess->loop())
		  ;
	  }
     }
   else
	pProcess->loop_forever();
   
   delete pProcess;
   return 0;
}
