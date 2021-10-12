#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>

#include <alchemy/task.h>
#include <alchemy/timer.h>
#include <alchemy/sem.h>
#include <alchemy/mutex.h>

#include "coarsemm.h"
#include "utils.h"
#include "procons.h"
#include "image.h"

#define NTASKS 4

RT_TASK task[NTASKS];

RT_MUTEX resource;

void radio(void *arg){
	//int num = * (int *)arg;
	int r;
	float period;
	
	RTIME now;
    period=10e9;
    rt_task_set_periodic(NULL,TM_INFINITE,period);
    now=rt_timer_read();

    while(1){

		r=pc();
		if (r<0){
			rt_printf("Error producer-consumer");
			exit(-1);
		}

		//rt_printf("%d Task Radio running \n",num);
		RT_TASK_INFO curtaskinfo;
        rt_task_inquire(NULL,&curtaskinfo);
        rt_printf("%s task - cycle time: %.5f s\n",curtaskinfo.name,(rt_timer_read()-now)/1000000000.0);

        rt_task_wait_period(NULL);
	}
}

void mm(void *arg){
	//int num = * (int *)arg;
	float period;
	RTIME now;
	period=6e9;
	rt_task_set_periodic(NULL,TM_INFINITE,period);
	now=rt_timer_read();

	while(1){
		char *fname = "matrices_test.dat";
		FILE *fh;

		double **a, **b, ***coarse;
		double ****data;
		int matrixSize;
		int nmats;
		int threads = 1;

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
		
		//rt_printf("%d Task coarse mm running \n",num);
		RT_TASK_INFO curtaskinfo;
	    rt_task_inquire(NULL,&curtaskinfo);
		rt_printf("%s task - cycle time: %.5f s\n",curtaskinfo.name,(rt_timer_read()-now)/1000000000.0);

		rt_task_wait_period(NULL);
	}

}

void image_gen(void *arg){
	//int num = * (int *)arg;
	int width,height;
	width=1920;
	height=1080;

	float period;
	RTIME now;
	period=6e9;
	rt_task_set_periodic(NULL,TM_INFINITE,period);
	now=rt_timer_read();

	while(1){
		createImage("colorscale.bmp", width, height);
		//rt_printf("%d Task Data running \n",num);
		RT_TASK_INFO curtaskinfo;
	    rt_task_inquire(NULL,&curtaskinfo);
		rt_printf("%s task - cycle time: %.5f s\n",curtaskinfo.name,(rt_timer_read()-now)/1000000000.0);

		rt_task_wait_period(NULL);
	}
}

void f4(void *arg){
	int num = * (int *)arg;
	rt_printf("%d Task f4 running \n",num);
}


int main (int argc, char* argv[]){

	//rand(time(NULL));
	int i;
	
	rt_mutex_create(&resource, "Semaphore");

	char task0[] ="Radio";	
	char task1[] ="Data";	
	char task2[] ="Image Decoder";	
	char task3[] ="f4";	

	rt_task_create(&task[0], task0, 0,50,0);
	rt_task_create(&task[1], task1, 0,50,0);
	rt_task_create(&task[2], task2, 0,50,0);
	rt_task_create(&task[3], task3, 0,50,0);

	for (i=0;i<NTASKS;i++){
		switch(i){
			case 0:
				rt_task_start(&task[i], &radio, &i);
				break;
			case 1:
				rt_task_start(&task[i], &mm, &i);
				break;
			case 2:
				rt_task_start(&task[i], &image_gen, &i);
				break;
			case 3:
				rt_task_start(&task[i], &f4, &i);
				break;
		}
	}
	pause();
}
