/*************************************************/
//          - Code by Prof. Gustavo Patino  -
//          - Real-Time Systems (IEO 2547027) -
//          - University of Antioquia (UdeA) -
//          - Medellin, Colombia 2021 -
//          - Modified for practice 5 by:
//          - Santiago Rios - Emmanuel Gomez
/*************************************************/

#include <stdio.h>
#include <pthread.h>

extern pthread_mutex_t lock;

void producer_thread();
void consumer_thread();

