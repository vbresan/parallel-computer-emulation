#include <iostream>
#include <sys/times.h>
#include <unistd.h>

using namespace std;

#define M 300
#define N 400

int main(int argc, char * argv[])
{  float (*pmatrix1)[N] = new float [M][N];
   float (*pmatrix2)[N] = new float [M][N];
   float (*pmatrix3)[M] = new float [M][M];
   
   for(unsigned int i = 0; i < M * N; ++i)
	pmatrix1[0][i] = i;
   for(unsigned int i = 0; i < M * M; ++i)
     {
	pmatrix3[0][i] = 0;
     }
   
   pmatrix2[0][0] = 0; pmatrix2[0][1] = 3; pmatrix2[0][2] = 6; pmatrix2[0][3] = 9;
   pmatrix2[1][0] = 1; pmatrix2[1][1] = 4; pmatrix2[1][2] = 7; pmatrix2[1][3] = 10;
   pmatrix2[2][0] = 2; pmatrix2[2][1] = 5; pmatrix2[2][2] = 8; pmatrix2[2][3] = 11;

   tms     dummy;
   clock_t stop;
   clock_t start = times(&dummy);
   

   for(unsigned int i = 0; i < M; ++i)  // od prvog do zadnjeg retka
	for(unsigned int j = 0; j < M; ++j)  // od prvog do zadnjeg stupca
	     for(unsigned int k = 0; k < N; ++k)
		  pmatrix3[i][j] += pmatrix1[i][k] * pmatrix2[j][k];
   
   stop = times(&dummy);
   
/*   for(unsigned int i = 0; i < M; ++i)
     {
	for(unsigned int j = 0; j < M; ++j)
	     printf("%f ", pmatrix3[i][j]);
	
	printf("\n");
     }*/
   
   cout << "Time: " << ((float) (stop - start)) / sysconf(_SC_CLK_TCK) 
        << endl;
   
   delete [] pmatrix3;
   delete [] pmatrix2;
   delete [] pmatrix1;
   
   return 0;
}
