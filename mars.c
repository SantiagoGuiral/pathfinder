#include <stdio.h>
#include <signal.h>
#include <unistd.h>

#include <alchemy/task.h>
#include <alchemy/timer.h>
#include <alchemy/sem.h>
#include <alchemy/mutex.h>

#define NTASKS 4

RT_TASK task[NTASKS];

int main (int argc, char* argv[]){
	int i;
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
				rt_task_start(&task[i], &f1, &i);
				break;
			case 1:
				rt_task_start(&task[i], &f2, &i);
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
