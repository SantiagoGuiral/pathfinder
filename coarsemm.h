#ifndef COARSEMM_H_
#define COARSEMM_H_

#include <stdint.h>

#include "pt_queue.h"

//Thread data structure
typedef struct{
	uint8_t T;				//Thread number
	uint32_t matrixSize;	//Matrix Size
	uint32_t nmats;			//Number of pair matrices
	uint32_t nmatrices;		//Number of pairs to calculate by thread
	uint16_t idx;			//Matrix Index
	double **A;				//Input
	double **B;				//Input
	double ***coarse;		//Ouput
	double ****data;		//Data from file
	pt_queue_t msg_in;		//Main message to thread
	pt_queue_t* msg_out;	//Thread message work is done
}thread_data_coarse;

void matmulcoarse(double **A, double **B, double ***coarse, double ****data, uint8_t T, uint32_t matrixSize, uint32_t nmats);

#endif /* COARSEMM_H_ */
