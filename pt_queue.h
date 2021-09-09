/*
 *	pt_queue.h
 *
 *  Created on: Jul 20, 2021
 *	Author: ricardo
 */

#ifndef PT_QUEUE_H_
#define PT_QUEUE_H_

#include <pthread.h>
#include <stdint.h>

typedef struct{
	uint8_t dSize; // size in bytes of the data stored in the queue
	uint16_t qSize; // Queue size
	void* basePtr; // Base pointer of queue storage
	void* pullPtr; // Pointer to oldest entry in the queue
	void* pushPtr; // Pointer to next available queue entry
	int8_t full;
	int8_t empty;
	pthread_mutex_t mtx; // Mutex to guarantee
	pthread_cond_t queue_full; // Conditional variable to block writes when queue is full
	pthread_cond_t queue_empty; // Conditional variable to block reads when queue is empty
}pt_queue_t;

int pt_queue_init(pt_queue_t* Q, uint16_t qsize, uint8_t dsize);
int pt_queue_push(pt_queue_t* Q, void* data);
int pt_queue_pull(pt_queue_t* Q, void* data);
void pt_queue_clean(pt_queue_t* Q);
uint16_t pt_queue_size(pt_queue_t* Q);


#endif /* PT_QUEUE_H_ */
