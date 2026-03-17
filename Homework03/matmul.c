#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define getClock() ((double)clock()/CLOCKS_PER_SEC)

int main(int argc, char** argv){
	double tstart = getClock();
	printf("The total memory allocated is %7.3lf GB\n.", 3.0 * sizeof(double)*(rows*columns)/1024/1024/1024);
	
	int i, j	
	return 0;
}
