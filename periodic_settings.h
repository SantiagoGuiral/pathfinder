/*************************************************/

// Prof. Luca Abeni. The Real-Time Operating Systems and Middleware course
// http://www.dit.unitn.it/~abeni/RTOS/periodic_tasks.h

/*************************************************/


/*************************************************/
//  - Code by Prof. Gustavo Patino  -
//    - Real-Time Systems (IEO 2547027) -
//     - University of Antioquia (UdeA) -
//      - Medellin, Colombia 2021 -
/*************************************************/


struct periodic_thread {
	struct timespec r;
	int period;
	int offset;
	int count;
};

void current_time();

void start_periodic_timer(struct periodic_thread* perthread);

void wait_next_activation(struct periodic_thread *t);
