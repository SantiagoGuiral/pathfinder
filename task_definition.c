#include <pthread.h>
#include <stdlib.h>

#include "task_definition.h"

#define MAX 20

int queue[MAX];
int front = 0;
int rear = -1;
int itemCount = 0;

int peek() {
   return queue[front];
}

int isEmpty() {
	int empty;
	if(itemCount == 0){
		empty=1;
	}else{
		empty=0;
	}
	return empty;
}

int isFull() {
	int full;
	if (itemCount == MAX){
		full=1;
	}else{
		full=0;
	}
	return full;
}

int size() {
   return itemCount;
}  

void producer(int* c){

	int data,f;
	pthread_mutex_lock(&lock);
	
	data = (rand()%100)+1;
	//printf("Data %d\n",data);
	f=isFull();
	
	if (f==0){
		if (rear==MAX-1){
			rear =-1;
		}
		queue[++rear]=data;
		itemCount++;
		printf("Thread Producer: %d - Data: %d\n", (*c)++,data);
	}else{
		printf("Queue Full");
	}
	
	pthread_mutex_unlock(&lock);
}

void consumer(int* c){
	int newData;

	pthread_mutex_lock(&lock);
	newData=queue[front++];
	
	if (front == MAX){
		front=0;
	}
	
	itemCount--;

	printf("Thread Consumer: %d - Data: %d\n", (*c)++,newData);
	pthread_mutex_unlock(&lock);
}
