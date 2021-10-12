#include <unistd.h>

#include "resource.h"

void sharedresource(){

	int sl;
	sl=1;
	rt_printf("Init of the shared resource\n")
	sleep(sl);
	rt_printf("End of the shared resource\n")
}
