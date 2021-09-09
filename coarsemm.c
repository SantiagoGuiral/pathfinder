#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include "coarsemm.h"
#include "pt_queue.h"
#include "utils.h"

void* thread_coarse_mul(void* args){
	int i,j,k,pos;
	double sum;

	//Perform casting of void* argument to real argument type
	thread_data_coarse *dat = (thread_data_coarse *)args;

	//Define data sector that will process the thread
	pt_queue_push(dat->msg_out, (void *)&dat->idx);

	uint16_t idx;
	//Iterates to assing work to the threads unitl no more available
	for (;;){
		pt_queue_pull(&dat->msg_in, (void *)&idx);

		uint32_t start = dat->nmatrices*idx;
		uint32_t end = 0;
		if (idx == (dat->nmats-1)){
			end = dat->nmats;
		}
		else{
			end = dat->nmatrices*(idx+1);
		}

		//Thread work
		for (pos=start;pos<end;pos++){
			
			//Read matrices
			getMatrices(dat->data,dat->A,dat->B,dat->matrixSize,pos);

			//printf("Coarse... Multiplying two matrices...\n");
			for (i=0;i<dat->matrixSize;i++){
				for(j=0;j<dat->matrixSize;j++){
					sum=0.0;
					//dot product
					for(k=0;k<dat->matrixSize;k++){
						sum = sum + dat->A[i][k]*dat->B[k][j];
					}
					dat->coarse[dat->idx][i][j]=sum;
				}
			}
		}
		pt_queue_push(dat->msg_out, (void *)&dat->idx);
	}
	return NULL;
}

void matmulcoarse(double **A, double **B, double ***coarse, double ****data, uint8_t T, uint32_t matrixSize, uint32_t nmats){

	//Allocate the threads data structures
	thread_data_coarse* th_data = malloc(sizeof(thread_data_coarse)*T);
	pthread_t* th_ids=malloc(sizeof(pthread_t)*T);
	
	//Avoids stacking the Cache
	uint32_t max_matrices = ((uint32_t)(1<<16)/sizeof(double))/nmats;
	if (max_matrices<1) max_matrices=1;

	//Limits the number of threads
	if(nmats<T){
		T=nmats;
		printf("The number of threads used are %d",T);
	}

	//pairs is the number of matmul each thread performs
	uint32_t nmatrices=nmats/T;
	if (nmatrices>max_matrices) nmatrices=max_matrices;

	//Set queue for threads to message main when work is done
	pt_queue_t msg_out;
	pt_queue_init(&msg_out,500,sizeof(uint16_t));
	
	//Create threads
	for (int i=0;i<T;i++){
		th_data[i].idx = i;
		th_data[i].A = A;
		th_data[i].B = B ;
		th_data[i].coarse = coarse;
		th_data[i].data = data;
		th_data[i].T = T;
		th_data[i].matrixSize = matrixSize;
		th_data[i].nmats = nmats;
		th_data[i].nmatrices = nmatrices;
		pt_queue_init(&th_data[i].msg_in,50,sizeof(uint16_t));
		th_data[i].msg_out = &msg_out;
		pthread_create(&th_ids[i],NULL,thread_coarse_mul,&th_data[i]);
	}
	
	//Controls the number of blocks to work - sends work to thread
	for (int i=0;i<nmats/nmatrices;i++){
		uint16_t th_idx =0;
		pt_queue_pull(&msg_out,(void*)&th_idx);//read thread index
		if(th_idx>T){
			printf("Problem found, program will be terminated");
			break;
		}
		pt_queue_push(&th_data[th_idx].msg_in,(void *)&i);//main sends idx to work assigned to the thread
	}

	//Cancels the threads when there is no work available
	for (int i=0;i<T;i++){
		uint16_t th_idx=0;
		pt_queue_pull(&msg_out,(void*)&th_idx);
		pthread_cancel(th_ids[th_idx]);
	}
}
