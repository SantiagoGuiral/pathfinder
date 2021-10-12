/*************************************************/

// Prof. Luca Abeni. The Real-Time Operating Systems and Middleware course
// http://www.dit.unitn.it/~abeni/RTOS/periodic_tasks.c

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

//#define DEBUG_MODE
#include "periodic_settings.h"

#define NSEC_PER_SEC 1000000000ULL


void current_time()
{
	struct timeval tv;

	gettimeofday(&tv, 0);
	printf("Current time is: %ld.%ld\n", tv.tv_sec, tv.tv_usec / 1000);
}


static inline void timespec_add_us(struct timespec *t, uint64_t d)
{
// El valor d que llega es en us, entonces se agrega al timespec *t en nanosegundos, multiplicado por 1000. (d*1000 nsec = d usec) 
// Por esto la funcion se llama "timespec_add_us", o sea, agrega us al timespec.

	#ifdef DEBUG_MODE
		printf("\ntimespec t, component sec =  %ld\n", t->tv_sec);
		printf("timespec t, component nsec =  %ld\n", t->tv_nsec);
	#endif

	d *= 1000;
	d += t->tv_nsec;
	while (d >= NSEC_PER_SEC) {
		d -= NSEC_PER_SEC;  // Por cada resta de NSEC_PER_SEC debe agregar un segundo al tv_sec.
		t->tv_sec += 1;
	}
	t->tv_nsec = d;
}


void wait_next_activation(struct periodic_thread *t)
{
    clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &t->r, NULL);
    timespec_add_us(&t->r, t->period);
}


void start_periodic_timer(struct periodic_thread* perthread)
{
	#ifdef DEBUG_MODE
	printf("\nBefore arming the timer: \n");
	current_time();
	#endif

	printf("Este hilo tiene un periodo esperado de : %d us. \n", perthread->period);
	printf("El offset de este hilo es de %d us.\n", perthread->offset);

	clock_gettime(CLOCK_REALTIME, &perthread->r);
	timespec_add_us(&perthread->r, perthread->offset);
}
