/*
 * 	pt_queue.c
 *
 *  Created on: Jul 20, 2021
 *	Author: ricardo
 */

#include <stdlib.h>
#include <string.h>

#include "pt_queue.h"

int pt_queue_init(pt_queue_t* Q, uint16_t qsize, uint8_t dsize){
	Q->dSize = dsize;
	Q->qSize = qsize;
	Q->empty = -1;
	Q->full = 0;
	Q->basePtr = malloc(qsize*dsize);
	if (Q->basePtr == NULL)
		return -1;
	Q->pullPtr = Q->basePtr;
	Q->pushPtr = Q->basePtr;
	pthread_cond_init(&Q->queue_empty, NULL);
	pthread_cond_init(&Q->queue_full, NULL);
	pthread_mutex_init(&Q->mtx,	NULL);
	return 0;
}

int pt_queue_push(pt_queue_t* Q, void* data){
	pthread_mutex_lock(&Q->mtx);

	// Block the queue write in case of a full queue
	if(Q->full){
		pthread_cond_wait(&Q->queue_full, &Q->mtx);
	}

	// Write data in the queue
	memcpy(Q->pushPtr,data,Q->dSize);

	// Increment pushPtr checking for overflow
	if((Q->pushPtr + Q->dSize)==(Q->basePtr+(Q->dSize*Q->qSize)))
		Q->pushPtr = Q->basePtr;
	else
		Q->pushPtr = Q->pushPtr + Q->dSize;

	// Check for full queue and write flag when full
	if((Q->pushPtr - Q->pullPtr)==((Q->dSize*(Q->qSize-1)))||((Q->pushPtr + Q->dSize)==Q->pullPtr)){
		Q->full = -1;
	}

	if(Q->empty){
		pthread_cond_signal(&Q->queue_empty);
		Q->empty = 0;
	}

	pthread_mutex_unlock(&Q->mtx);
	return 0;
}

int pt_queue_pull(pt_queue_t* Q, void* data){
	pthread_mutex_lock(&Q->mtx);
	// block the queue read in case of an empty queue
	if(Q->empty){
		pthread_cond_wait(&Q->queue_empty, &Q->mtx);
	}

	// Read data
	memcpy(data,Q->pullPtr,Q->dSize);

	// Increment pullPtr checking for overflow
	if((Q->pullPtr + Q->dSize)==(Q->basePtr+(Q->dSize*Q->qSize)))
		Q->pullPtr = Q->basePtr;
	else
		Q->pullPtr = Q->pullPtr + Q->dSize;

	// Check for full queue and write flag when full
	if(Q->pushPtr == Q->pullPtr){
		Q->empty = -1;
	}

	if(Q->full){
		pthread_cond_signal(&Q->queue_full);
		Q->full = 0;
	}

	pthread_mutex_unlock(&Q->mtx);
	return 0;
}

void pt_queue_clean(pt_queue_t* Q){
	Q->empty = -1;
	Q->full = 0;
	Q->pullPtr = Q->basePtr;
	Q->pushPtr = Q->basePtr;
}

uint16_t pt_queue_size(pt_queue_t* Q){
	uint16_t size = 0;
	pthread_mutex_lock(&Q->mtx);
	if(Q->pushPtr > Q->pullPtr){
		size = (Q->pushPtr - Q->pullPtr)/Q->dSize;
	}
	else{
		size = (Q->dSize*(Q->qSize-1) - (Q->pullPtr - Q->pushPtr))/Q->dSize;
	}
	pthread_mutex_unlock(&Q->mtx);
	return size;
}
