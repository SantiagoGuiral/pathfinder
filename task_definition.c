/*************************************************/
//  - Code by Prof. Gustavo Patino  -
//    - Real-Time Systems (IEO 2547027) -
//     - University of Antioquia (UdeA) -
//      - Medellin, Colombia 2021 -
/*************************************************/


#include "task_definition.h"


void producer(int* c)
{
	pthread_mutex_lock(&lock);
	printf("Thread Producer: %d\n", (*c)++);
	pthread_mutex_unlock(&lock);
}


void consumer(int* c)
{
	pthread_mutex_lock(&lock);
	printf("Thread Consumer: %d\n", (*c)++);
	pthread_mutex_unlock(&lock);
}

