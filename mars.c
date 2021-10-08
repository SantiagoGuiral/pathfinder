#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#include <alchemy/task.h>
#include <alchemy/timer.h>
#include <alchemy/sem.h>
#include <alchemy/mutex.h>

#include "coarsemm.h"
#include "utils.h"
#include "producer_consumer.h"

#define NTASKS 4

RT_TASK task[NTASKS];

RT_MUTEX resource;

void f1(void *arg){
	
	int num = * (int *)arg;
	//encodeVideo();
	rt_printf("%d Task encode running \n",num);

}

void mm(void *arg){
	int num = * (int *)arg;

	char *fname = "matrices_test.dat";
	FILE *fh;

	double **a, **b, ***coarse;
	double ****data;
	int matrixSize;
	int nmats;
	int threads = 3;

	fh = fopen(fname,"r");
	fscanf(fh, "%d %d\n", &nmats, &matrixSize);
	data=allocateData(matrixSize,nmats);
	storeData(fh,data,matrixSize,nmats);
	fclose(fh);
	a = allocateMatrix(matrixSize);
	b = allocateMatrix(matrixSize);
	coarse = allocate3DMatrix(matrixSize,threads);

	matmulcoarse(a,b,coarse,data,threads,matrixSize,nmats);
	printCoarse(coarse,matrixSize,threads);

	freeMemory(a,b);
	freeCoarse(coarse,matrixSize,threads);
	freeData(data,matrixSize,nmats);
	rt_printf("%d Task coarse mm running \n",num);
}

void pc(void *arg){
	int num = * (int *)arg;
	rt_printf("%d Task f3 running \n",num);
	prod_cons();
}

void f4(void *arg){
	int num = * (int *)arg;
	rt_printf("%d Task f4 running \n",num);
}



int main (int argc, char* argv[]){
	int i;
	
	rt_mutex_create (&resource, "Semaphore");

	char task0[] ="f1";	
	char task1[] ="Data";	
	char task2[] ="Radio";	
	char task3[] ="f4";	

	rt_task_create(&task[0], task0, 0,50,0);
	rt_task_create(&task[1], task1, 0,50,0);
	rt_task_create(&task[2], task2, 0,50,0);
	rt_task_create(&task[3], task3, 0,50,0);

	for (i=0;i<NTASKS;i++){
		switch(i){
			case 0:
				rt_task_start(&task[i], &f1, &i);
				break;
			case 1:
				rt_task_start(&task[i], &mm, &i);
				break;
			case 2:
				rt_task_start(&task[i], &pc, &i);
				break;
			case 3:
				rt_task_start(&task[i], &f4, &i);
				break;
		}
	}
	pause();
}
