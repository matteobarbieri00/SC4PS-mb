#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
bool test_ok(double d[], double x[], double y[], double a, int n){
	for(int i = 0; i < n; i++){
		if(d[i]  != a*x[i] + y[i]) {
			fprintf(stderr, "Component %i of vector d is not correct\n", i);
			return false;
		}
	}
	return true;
}

void test_specific_values(){
	int n = 10;
	double a = 3;
	double x = 0.1;
	double y = 7.1;
	double X[n], Y[n];
	for (int i = 0; i < n; i++){
		X[i] = x;
		Y[i] = y;
	}
	double D[n];
        for (int j = 0; j < n; j++){
        	D[j] = a*X[j]+Y[j];
        }
	if (test_ok(D,X,Y,a,n)== true){
		fprintf(stdout,"For a = %f, x = %f, y = %f and n = %i, I get the correct values\n", a,x,y,n);
	} else {
		 fprintf(stdout,"For a = %f, x = %f, y = %f and n = %i, I DO NOT get the correct values\n", a				,x,y,n);
	}
}

int main(int argc, char *argv[])
{	
	test_specific_values();	

	if (argc != 4) {
		fprintf(stderr, "Usage: %s a x y\n",argv[0]);
		return 1;
	}

	int N[3] = {10, (int)1e6, (int)1e8};
	double a, x, y;

	// N = (int)strtol(argv[1], NULL, 10);
	a = strtod(argv[1], NULL);
	x = strtod(argv[2], NULL);
	y = strtod(argv[3], NULL);
	
	for(int i = 0; i < 3; i++){
		int n = N[i];
		double X[n];
		double Y[n];
		for(int j = 0; j < n; j++){
			X[j] = x;
			Y[j] = y;
		}
		double D[n];
		for (int j = 0; j < n; j++){
			D[j] = a*X[j]+Y[j];
		}
		if (test_ok(D,X,Y,a,n) == true){
			fprintf(stdout,"for N = %i, the computation is correct\n",n);
		} else {
			fprintf(stdout,"for N = %i, the computation is NOT correct\n",n);
		}
	} 
	return 0;
}
