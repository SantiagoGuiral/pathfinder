/*
 * Implement 3 periodic threads, with periods 50ms, 100ms, and 150ms.
 * The job bodies are functions named task1(), task2() and task3()
 * (defined in cyclic_test.c)
 *
 * WARNING!!! This program is incomplete: it does not set the threads
 * priorities! (threads are scheduled using the SCHED_OTHER policy)
 */

/*************************************************/
// Prof. Luca Abeni. The Real-Time Operating Systems and Middleware course
// http://www.dit.unitn.it/~abeni/RTOS/periodic_threads.c
/*************************************************/

/*************************************************/
//  - Code modified by Prof. Gustavo Patino  -
//    - Real-Time Systems (IEO 2547027) -
//     - University of Antioquia (UdeA) -
//      - Medellin, Colombia 2021 -
/*************************************************/


#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#include "periodic_settings.h"
#include "task_definition.h"

#define SECONDS_TO_RUN 1


pthread_mutex_t lock;


void *prod(void* p_d)

{
	pthread_mutex_lock(&lock);
	struct periodic_thread* temp = (struct periodic_thread*) p_d;

	temp->offset = 20000; // En unidades de us.
	temp->period = 200000; // En unidades de us.
	temp->count = 0;

	printf("\nThread 1 with period : %d ms. \n", temp->period/1000);
	start_periodic_timer(temp);

	pthread_mutex_unlock(&lock);

	while (1) {
		wait_next_activation(temp);
		producer(&(temp->count));
  	}
}


void *cons(void* p_d)
{
	pthread_mutex_lock(&lock);
	struct periodic_thread* temp = (struct periodic_thread*) p_d;

	temp->offset = 40000;
	temp->period = 200000;
	temp->count = 0;

	printf("\nThread 2 with period : %d ms. \n", temp->period/1000);
	start_periodic_timer(temp);

	pthread_mutex_unlock(&lock);

	while (1) {
		wait_next_activation(temp);
		consumer(&(temp->count));
  	}
}


int pc(){
	int err;
	pthread_t prod_id, cons_id;

	struct periodic_thread *pt1;
	struct periodic_thread *pt2;

	pt1 = malloc(sizeof(struct periodic_thread));
	pt2 = malloc(sizeof(struct periodic_thread));

	/* creation and activation of the new thread */

	err = pthread_create(&prod_id, NULL, prod, (void*) pt1);
	if (err != 0) {
	fprintf(stderr, "Cannot create pthread 1");
	}

	err = pthread_create(&cons_id, NULL, cons, (void*) pt2);
	if (err != 0) {
	fprintf(stderr, "Cannot create thread 2");
	}

	sleep(SECONDS_TO_RUN);

	pthread_mutex_lock(&lock);
	printf("\nThis code just ran for %d seconds.\n", SECONDS_TO_RUN);
	printf("Considering the offset of %d us, and the period of %d us, there were %d iterations of Thread 1.\n", pt1->offset, pt1->period, pt1->count);
	printf("Considering the offset of %d us, and the period of %d us, there were %d iterations of Thread 2.\n", pt2->offset, pt2->period, pt2->count);
	pthread_mutex_unlock(&lock);

	pthread_mutex_destroy(&lock);
	
	pthread_cancel(prod_id);
	pthread_cancel(cons_id);
	
	return 0;
}
