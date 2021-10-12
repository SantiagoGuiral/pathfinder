#ifndef TASK_DEFINITION_H_
#define TASK_DEFINITION_H_

#include <stdio.h>
#include <pthread.h>

pthread_mutex_t lock;


int peek();
int isEmpty();
int isFull();
int size();
void producer(int* c);
void consumer(int* c);


#endif /* TASK_DEFINITION_H_ */

