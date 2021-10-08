/*************************************************/
//          - Code by Prof. Gustavo Patino  -
//          - Real-Time Systems (IEO 2547027) -
//          - University of Antioquia (UdeA) -
//          - Medellin, Colombia 2021 -
//          - Modified for Practice 5 by
//          - Santiago Rios - Emmanuel Gomez
/*************************************************/
#include <stdlib.h>

#include "threads_definition_v1.h"

//queue
int index=0
int ptq[30];

void producer_thread()
{
    int newNumber;

	pthread_mutex_lock(&lock);
    
    // Add a number to the queue
    newNumber = rand() % 100 + 1;

    //Input number to the queue

	ptq[index]=newNumber;
	index++;

    // Notify one thread that the condition variable might have changed
    printf("Producer 1 Added to queue: %d\n",newNumber);

	pthread_mutex_unlock(&lock);
}

void consumer_thread()
{
    int numberToProcess=0;

	pthread_mutex_lock(&lock);

	// Pop the values from the queue
	numberToProcess=ptq[index];
	index--;
    
	// Only process if there are numbers
    printf( "Consumer Pop number: %d\n ",numberToProcess);
    
    printf( "Queue size: %ld\n ",dataQueue.size());
	pthread_mutex_unlock(&lock);
}

