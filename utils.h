#ifndef UTILS_H_
#define UTILS_H_

double **allocateMatrix(int matrixSize);
double ****allocateData(int matrixSize, int nmats);
double ***allocate3DMatrix(int matrixSize, int threads);
void printCoarse(double ***coarse, int matrixSize, int threads);
int mat_diff_acum(double **A, double **B, int matrixSize);
void freeMemory(double **A, double **B);
void freeData(double ****data, int matrixSize, int nmats);
void freeCoarse(double ***coarse, int matrixSize, int threads);
void storeData(FILE *fh, double ****data, int matrixSize, int nmats);
void getMatrices(double ****data, double **a, double **b, int matrixSize, int index);

#endif /* UTILS_H_ */
