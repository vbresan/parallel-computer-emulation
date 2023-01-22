#include "pce.h"
#include <iostream>
#include <sys/times.h>
#include <unistd.h>

#include "Message.h"

using namespace std;

#define NUM 1

#define M 500
#define N 500


enum my_messages { m_row, m_col, m_result };


Process * pProcess;
float    (*pmatrix)[N];
float    (*pmatrix3)[M];
float     buffer[N];
int	  idx;
int	  idxs[NUM];

void
my_proc
  (
   unsigned int const sender_no,
   Message const &    msg
  )
{
   switch(msg.mid())
     {
        case m_col:
	     msg.copy_data_to(pmatrix[idx++]);
	break;
        case m_row:
	  {  float result[M] = { 0 };
	     
	     msg.copy_data_to(buffer);
	     for(unsigned int i = 0; i < M; ++i)
		  for(unsigned int j = 0; j < N; ++j)
		       result[i] += buffer[j] * pmatrix[i][j];
	     
	     pProcess->send(0, 
		Message(m_result, sizeof(float) * M, (const char *) result));
	  }
	break;
      	case m_result:
	  {  
	     msg.copy_data_to(pmatrix3[idxs[sender_no]++ * NUM + sender_no]);
	     idx++;
	  }
	break;
        default:
	break;
     }
}


int main(int argc, char * argv[])
{
   pProcess = start_new(argc, argv, my_proc);
   
   if(pProcess->master())
     {  float (*pmatrix1)[N] = new float [M][N];
	float (*pmatrix2)[N] = new float [M][N];
	
	pmatrix3 = new float [M][M];
	
	for(unsigned int i = 0; i < M * N; ++i)
	     pmatrix1[0][i] = i;
	
	pmatrix2[0][0] = 0; pmatrix2[0][1] = 3; pmatrix2[0][2] = 6; pmatrix2[0][3] = 9;
	pmatrix2[1][0] = 1; pmatrix2[1][1] = 4; pmatrix2[1][2] = 7; pmatrix2[1][3] = 10;
	pmatrix2[2][0] = 2; pmatrix2[2][1] = 5; pmatrix2[2][2] = 8; pmatrix2[2][3] = 11;
	
	tms dummy;
	clock_t tstart = times(&dummy);
	
//	pProcess->start_new();
	pProcess->start_new("matheu ~/source/diplomski/pmatrix");
//	pProcess->start_new("matheu ~/source/diplomski/pmatrix");
	
	for(unsigned int i = 0; i < M; ++i)
	  {  Message const col(m_col, 
			       N * sizeof(float), (char const *) pmatrix2[i]);
	    
	     cerr << i << '\r';
	     for(unsigned int j = 0; j < NUM; ++j)
	       {
		  pProcess->send(j, col);
//		  usleep(1);
	       }
	  }
	cerr << endl;
	for(unsigned int i = 0; i < M; ++i)
	  {
	     cerr << i << '\r';
	     pProcess->send(i % NUM, Message(m_row, N * sizeof(float),
					   (char const *) pmatrix1[i]));
	     pProcess->loop();
	  }
	cerr << endl;
	while(idx != M)
	  {
	     pProcess->loop();
	  }
	
	clock_t tstop = times(&dummy);
	
	cout << "Time: " << ((float) (tstop - tstart)) / sysconf(_SC_CLK_TCK) 
	     << endl;
	
/*	for(unsigned int i = 0; i < M; ++i)
	  {
	     for(unsigned int j = 0; j < M; ++j)
	       {
		  cout << pmatrix3[i][j] << ' ';
	       }
	     cout << endl;
	  }*/
	
	delete [] pmatrix3;
	delete [] pmatrix2;
	delete [] pmatrix1;
     }
   else
     {
	pmatrix = new float [M][N];
	pProcess->loop_forever();
	
	delete [] pmatrix;
     }
   
   delete pProcess;
   return 0;
}
