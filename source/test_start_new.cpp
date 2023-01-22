#include "pce.h"


struct remote_t
{
   char * host_name;
   char * remote_dir;
};


int main(int argc, char * argv[])
{  Process * pProcess = start_new(argc, argv);
   
   if(pProcess->master())
     {
	pProcess->start_new("matheu ~/diplomski/source/test_start_new");
	pProcess->loop_forever();
     }
   else
     {
	pProcess->loop_forever();
     }
   
   delete pProcess;
   return 0;
}
