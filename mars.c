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
#include "ping.h"
#include "resource.h"

#define NTASKS 4

RT_TASK task[NTASKS];

RT_MUTEX resource;

void radio(void *arg){
	RTIME now;
    now=rt_timer_read();
	//int num = * (int *)arg;
	int r;
	float period;
	
    period=12e9;
    rt_task_set_periodic(NULL,TM_INFINITE,period);

    while(1){

		r=pc();
		if (r<0){
			rt_printf("Error producer-consumer\n");
			exit(-1);
		}

		//rt_printf("%d Task Radio running \n",num);
		RT_TASK_INFO curtaskinfo;
        rt_task_inquire(NULL,&curtaskinfo);
        rt_printf("%s task - cycle time: %.5f s\n\n",curtaskinfo.name,(rt_timer_read()-now)/1000000000.0);

        rt_task_wait_period(NULL);
	}
}

void mm(void *arg){
	RTIME now;
	now=rt_timer_read();
	int num = * (int *)arg;
	float period;
	period=15e9;
	rt_task_set_periodic(NULL,TM_INFINITE,period);

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
	
		rt_printf("Task Data mm locks screen resource\n");
		rt_mutex_acquire(&resource,TM_INFINITE);
		sharedresource(num);
		rt_mutex_release(&resource);
		rt_printf("Task Data mm unlocks screen resource\n");
		
		//rt_printf("%d Task coarse mm running \n",num);
		RT_TASK_INFO curtaskinfo;
	    rt_task_inquire(NULL,&curtaskinfo);
		rt_printf("%s task - cycle time: %.5f s\n\n",curtaskinfo.name,(rt_timer_read()-now)/1000000000.0);

		rt_task_wait_period(NULL);
	}

}

void image_gen(void *arg){
	RTIME now;
	now=rt_timer_read();
	int num = * (int *)arg;
	int width,height;
	width=1920;
	height=1080;

	float period;
	period=24e9;
	rt_task_set_periodic(NULL,TM_INFINITE,period);

	while(1){
		createImage("colorscale.bmp", width, height);
		//rt_printf("%d Task Data running \n",num);

		rt_printf("Task Image Encoding locks screen resource\n");
		rt_mutex_acquire(&resource,TM_INFINITE);
		sharedresource(num);
		rt_mutex_release(&resource);
		rt_printf("Task Image Encoding unlocks screen resource\n");

		RT_TASK_INFO curtaskinfo;
	    rt_task_inquire(NULL,&curtaskinfo);
		rt_printf("%s task - cycle time: %.5f s\n\n",curtaskinfo.name,(rt_timer_read()-now)/1000000000.0);

		rt_task_wait_period(NULL);
	}
}

void ping(void *arg){
	RTIME now;
	now=rt_timer_read();
	int result;
	//int num = * (int *)arg;

	float period;
	period=20e9;
	rt_task_set_periodic(NULL,TM_INFINITE,period);

	while(1){
		result=nping();
		if (result!=0){
			rt_printf("Connection error when checking servers\n");
		}
		//rt_printf("%d Task f4 running \n",num);
		RT_TASK_INFO curtaskinfo;
	    rt_task_inquire(NULL,&curtaskinfo);
		rt_printf("%s task - cycle time: %.5f s\n\n",curtaskinfo.name,(rt_timer_read()-now)/1000000000.0);

		rt_task_wait_period(NULL);

	}
}


int main (int argc, char* argv[]){

	int i;
	
	rt_mutex_create(&resource, "Semaphore");

	char task0[] ="Radio";	
	char task1[] ="Data";	
	char task2[] ="Image Decoder";	
	char task3[] ="Connection";	

	rt_task_create(&task[0], task0, 0,52,0);
	rt_task_create(&task[1], task1, 0,53,0);
	rt_task_create(&task[2], task2, 0,54,0);
	rt_task_create(&task[3], task3, 0,51,0);

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
				rt_task_start(&task[i], &ping, &i);
				break;
		}
	}
	pause();
}
