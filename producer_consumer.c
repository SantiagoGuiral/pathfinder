/*************************************************/
//      - Code modified by Prof. Gustavo Patino  -
//      - Real-Time Systems (IEO 2547027) -
//      - University of Antioquia (UdeA) -
//      - Medellin, Colombia 2021 -
//      - Modified for Practice 5
//      - Santiago Rios - Emmanuel Gomez
/*************************************************/

#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "threads_definition.h"
#include "producer_consumer.h"

//#define DEBUG_MODE
#define SECONDS_TO_RUN 10

static int thread_producer_count;
static int thread_consumer_count;

pthread_mutex_t lock;

struct periodic_thread {
	sigset_t sigset;
	struct itimerspec t;
	struct sigevent sigev;
	timer_t timer;
	int period;
	};

void current_time()
{
	struct timeval tv;

	gettimeofday(&tv, 0);
	printf("The current time measure is: %ld.%ld\n", tv.tv_sec, tv.tv_usec / 1000);
}

struct periodic_thread* start_periodic_timer(uint64_t offs, int period, short unsigned sigrt)
{
	int res;
	struct periodic_thread *p;
		
	int signotif = SIGRTMIN + sigrt;                    // RT SIGNALS

	p =(periodic_thread* )malloc(sizeof(struct periodic_thread));
	if (p == NULL) {
		return NULL;
	}

	memset(&p->t, 0, sizeof(p->t));
	memset(&p->sigev, 0, sizeof(p->sigev));

	sigemptyset(&p->sigset);
	sigaddset(&p->sigset, signotif);

	p->sigev.sigev_notify = SIGEV_SIGNAL;
	p->sigev.sigev_signo = signotif;

	res = timer_create(CLOCK_REALTIME, &p->sigev, &p->timer);
	if (res < 0) {
		perror("Timer Create");
		exit(-1);
	}

	p->t.it_value.tv_sec = offs / 1000000;
	p->t.it_value.tv_nsec = (offs % 1000000) * 1000;
	p->t.it_interval.tv_sec = period / 1000000;
	p->t.it_interval.tv_nsec = (period % 1000000) * 1000;

#ifdef DEBUG_MODE
	printf("\nBefor timer initialization, the current time is: \n");
	current_time();
#endif

	printf("This process has a spected period of: %d us. \n", period);
	printf("The process offset is %ld us.\n\n", offs);

	res = timer_settime(p->timer, 0 /*TIMER_ABSTIME*/, &p->t, NULL);

	if (res < 0) {
		perror("Timer_settime");
		exit(-1);
	}

	p->period = period;
	return p;
}


static void wait_next_activation(struct periodic_thread *t)
{
	int sig;

	int result = sigwait(&t->sigset, &sig);
	if (result < 0) {
		perror("Sigwait");
		exit(-1);
	}
#ifdef DEBUG_MODE
	else if(result == 0)
		printf("The System Call sigwait() gain the signal: %d\n", sig);
#endif
}


static void *thread_producer(void *arg)
{
	int period = 400000;        // Period of 100ms
	struct periodic_thread *t1;

	pthread_mutex_lock(&lock);

	printf("Thread Producer 1: Period %d ms.\n", period/1000);
	t1=start_periodic_timer(200000, period, 1);

	pthread_mutex_unlock(&lock);

	while (1) {
		wait_next_activation(t1);
        producer_thread();
		++thread_producer_count;
	}
	return NULL;
}

static void *thread_consumer(void *arg)
{
	int period = 400000; // Period of 500ms
	struct periodic_thread *t2;

	pthread_mutex_lock(&lock);

	printf("Thread Consumer: Period %d ms.\n", period/1000);
	t2=start_periodic_timer(300000, period, 2);

	pthread_mutex_unlock(&lock);

	while (1) {
		wait_next_activation(t2);
        consumer_thread();
		++thread_consumer_count;
	}
	return NULL;
}


int prod_cons(){

	pthread_t t_1;          // producer 1
	pthread_t t_2;          // consumer
	sigset_t alarm_sigset;
	int i;

	/* Block all real time signals so they can be used for the timers.
	   Note: this has to be done in main() before any threads are created
	   so they all inherit the same mask. Doing it later is subject to
	   race conditions */

	sigemptyset(&alarm_sigset);
	for (i = SIGRTMIN; i <= SIGRTMAX; i++)
		sigaddset(&alarm_sigset, i);
		
	sigprocmask(SIG_BLOCK, &alarm_sigset, NULL);

	pthread_create(&t_1, NULL, thread_producer, NULL);
	pthread_create(&t_2, NULL, thread_consumer, NULL);

	sleep(SECONDS_TO_RUN);

	pthread_mutex_lock(&lock);
	printf("In %d seconds executed where: \n\n", SECONDS_TO_RUN);
	printf("%d iterations of Producer 1.\n", thread_producer_count);
	printf("%d iteracions of Consumer.\n", thread_consumer_count);
	pthread_mutex_unlock(&lock);

	pthread_mutex_destroy(&lock);

	return 0;
}
