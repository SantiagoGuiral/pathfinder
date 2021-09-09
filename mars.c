#include <stdio.h>
#include <stdlib-h>
#include <signal.h>
#include <unistd.h>

#include <alchemy/task.h>
#include <alchemy/timer.h>
#include <alchemy/sem.h>
#include <alchemy/mutex.h>

#include "coarsemm.h"
#include "encode.h"
#include "utils.h"

#define NTASKS 4

RT_TASK task[NTASKS];

RT_MUTEX resource;

void encoder(void *arg){

	rt_printf("%d Task encode running \n",num);
	encodeVideo();

}

void mm(void *arg){
	int num = * (int *)arg;

	char *fname = "matrices_test.dat";
	FILE *fh;

	double **a, **b, ***coarse;
	double ****data;
	int matrixSize;
	int nmats;
	int threads = 2;

	fh = fopen(fname,"r");
	fscanf(fh, "%d %d\n", &nmats, &matrixSize);
	data=allocateData(matrixSize,nmats);
	storeData(fh,data,matrixSize,nmats);
	fclose(fh);

	a = allocateMatrix(matrixSize);
	b = allocateMatrix(matrixSize);
	coarse = allocate3DMatrix(matrixSize,threads);

	matmulcoarse(a,b,coarse,data,threads,matrixSize,nmats);

	freeMemory(a,b);
	freeCoarse(coarse,matrixSize,nmats);
	freeData(data,matrixSize,nmats);
	
	rt_printf("%d Task coarse mm running \n",num);
}

void f3(void *arg){
	int num = * (int *)arg;
	rt_printf("%d Task f3 running \n",num);
}

void f4(void *arg){
	int num = * (int *)arg;
	rt_printf("%d Task f4 running \n",num);
}



int main (int argc, char* argv[]){
	int i;
	
	rt_mutex_create (&resource, "Semaphore");

	char[10] task0 ="Data";	
	char[10] task1 ="Video";	
	char[10] task2 ="Radio";	
	char[10] task3 ="Measure";	

	rt_task_create(&task[0], task0, 0,50,0);
	rt_task_create(&task[1], task1, 0,50,0);
	rt_task_create(&task[2], task2, 0,50,0);
	rt_task_create(&task[3], task3, 0,50,0);

	for (i=0;i<NTASKS;i++){
		switch(i){
			case 0:
				rt_task_start(&task[i], &encoder, &i);
				break;
			case 1:
				rt_task_start(&task[i], &mm, &i);
				break;
			case 2:
				rt_task_start(&task[i], &f3, &i);
				break;
			case 3:
				rt_task_start(&task[i], &f4, &i);
				break;
		}
	}
	pause();
}
