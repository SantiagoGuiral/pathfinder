/*************************************************/
//  - Code by Prof. Gustavo Patino  -
//    - Real-Time Systems (IEO 2547027) -
//     - University of Antioquia (UdeA) -
//      - Medellin, Colombia 2021 -
/*************************************************/

#include <stdio.h>
#include <pthread.h>

pthread_mutex_t lock;

void producer(int* c);
void consumer(int* c);

