#include "pce.h"
#include <time.h>

#include "Message.h"


enum my_messages { my_send, my_resend };

Process *    pProcess;
unsigned int counter;


void
my_proc
  (
   unsigned int const sender_no,
   Message const &    msg
  )
{
   switch(msg.mid())
     {
        case my_send:
	  {
	     pProcess->send(sender_no, Message(msg, my_resend));
	  }
	break;
      	case my_resend:
	  {
	     ++counter;
//	     std::cerr << counter << '\r';
	  }
	break;
	default:
	break;
     }
}


int main(int argc, char *argv[])
{  
   pProcess = start_new(argc, argv, my_proc);
   
   if(pProcess->master())
     {
	pProcess->start_new("matheu ~/source/diplomski/tcp_benchmark");
	for(unsigned int i = 1000; i <= 2024; i += 8)
	  {  char  * message_data = new char[i];
	     Message to_send(my_send, i, message_data);
	     Message to_recv;
	     time_t stop;
	     time_t start = time(NULL);
	     
	     for(unsigned int j = 1; j <= 1000; ++j)
	       {  unsigned int old_counter = counter;
		  
		  pProcess->send(0, to_send);
		  do
		    {
		       while(!pProcess->receive(0, &to_recv))
			    usleep(1);
		       
		       my_proc(0, to_recv);
		    }
		  while(old_counter == counter);
	       }

	     stop = time(NULL);
	     std::cout << i << ".\t" 
	               << difftime(stop, start) / (2 * 1000)
	               << "\n";
	     counter = 0;
	     delete [] message_data;
	  }
     }
   else
	pProcess->loop_forever();

   delete pProcess;
   return 0;
}
