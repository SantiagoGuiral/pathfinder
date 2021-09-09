#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "utils.h"

double **allocateMatrix(int matrixSize) {
	//printf("The matrix size is: %d\n",matrixSize);
	int i;
	double *vals, **temp;

	// allocate space for values of a matrix
	vals = (double *) malloc (matrixSize * matrixSize * sizeof(double));

	// allocate vector of pointers to create the 2D array
	temp = (double **) malloc (matrixSize * sizeof(double*));

	for(i=0; i < matrixSize; i++)
		temp[i] = &(vals[i * matrixSize]);

	return temp;
}

double ****allocateData(int matrixSize, int nmats){
	int i,j,k;
	double ****data;
	int pair=2;

	//Allocate structure for the file data
	data = (double ****) malloc(nmats*sizeof(double***));

	//POINTER 4D array to iterate the data file
	for (i=0;i<nmats;i++){
		data[i]=(double ***) malloc(pair*sizeof(double**));
		if (data[i]==NULL) return NULL;
		for(j=0;j<pair;j++){
			data[i][j]=(double **) malloc(matrixSize*sizeof(double*));
			if (data[i][j]==NULL) return NULL;
			for(k=0;k<matrixSize;k++){
					data[i][j][k]=(double*) malloc(matrixSize*sizeof(double));
					if (data[i][j][k]==NULL) return NULL;
			}
		}
	}
	return data;
}

double ***allocate3DMatrix(int matrixSize, int threads){
	double ***coarse;
	int i,j;
	coarse = (double ***) malloc(threads*sizeof(double**));
	for (i=0;i<threads;i++){
		coarse[i]= (double **) malloc(matrixSize*sizeof(double*));
		if (coarse[i]==NULL) return NULL;
		for (j=0;j<matrixSize;j++){
			coarse[i][j]= (double*) malloc (matrixSize*sizeof(double));
			if (coarse[i][j]==NULL) return NULL;			
		}
	}
	return coarse;
}

void printCoarse(double ***coarse, int matrixSize, int threads){
	int i,j,k;
	for (i=0;i<threads;i++){
		for(j=0;j<matrixSize;j++){
			for (k=0;k<matrixSize;k++){
				printf("%lf ",coarse[i][j][k]);
			}
			printf ("\n");
		}
	}
}

int mat_diff_acum(double **A, double **B, int matrixSize){
	int ACC=0;
	for (int i=0;i<matrixSize;i++){
		for (int j=0;j<matrixSize;j++){
			if(A[i][j] != B[i][j]){
				ACC+=1;
			}
		}
	}
	return ACC;
}

void freeMemory(double **A, double **B){
	free(*A);
	free(*B);
	free(A);
	free(B);
}

void freeData(double ****data, int matrixSize, int nmats){
	int i,j,k;
	int pair=2;
	
	for(i=0;i<nmats;i++){
		for(j=0;j<pair;j++){
        	for(k=0;k<matrixSize;k++){
           		free(data[i][j][k]);
        	}
        	free(data[i][j]);
    	}
    	free(data[i]);
	}
	free(data);
}

void freeCoarse(double ***coarse, int matrixSize, int threads){
	int i,j;
	for (i=0;i<threads;i++){
		for (j=0;j<matrixSize;j++){
			free(coarse[i][j]);
		}
		free(coarse[i]);
	}
	free(coarse);
}

void storeData(FILE *fh, double ****data, int matrixSize, int nmats){
	int i,j,k;
	for(i=0;i<nmats;i++){
		for (j=0;j<matrixSize;j++){
			for(k=0;k<matrixSize;k++){
				fscanf(fh,"%lf",&data[i][0][j][k]);
			}
		}
		for (j=0;j<matrixSize;j++){
			for(k=0;k<matrixSize;k++){
				fscanf(fh,"%lf",&data[i][1][j][k]);
			}
		}
	}
}
