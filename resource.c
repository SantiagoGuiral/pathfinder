#include <unistd.h>
#include <stdio.h>

#include "resource.h"

void sharedresource(int n){

	printf("Init of the shared resource\n");
	sleep(n);
	printf("End of the shared resource\n");
}
