#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define getClock() ((double)clock()/CLOCKS_PER_SEC)

void matmul(int n, int A[n][n], int B[n][n], int C[n][n]) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
                C[i][j] = 0; // Initialize C[i][j] to zero
            for (int k = 0; k < n; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

//Optimized matrix multiplication (cache-friendly)
void matmul_optimized(int n, int A[n][n], int B[n][n], int C[n][n]) {
    for (int i = 0; i < n; i++) {
        for (int k = 0; k < n; k++) {
            for (int j = 0; j < n; j++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

bool is_correct(int n, int A[n][n], int B[n][n], int C[n][n]) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int c = 0; // Initialize c to zero for each element
            for (int k = 0; k < n; k++) {
                c += A[i][k] * B[k][j];
            }
            if (C[i][j] != c) {
                return false; // If any element is incorrect, return false
            }
        }
    }
    return true;
}

bool freivalds(int n, int A[n][n], int B[n][n], int C[n][n]) {
    int *r = malloc(n * sizeof(int));
    int *Br = calloc(n, sizeof(int));
    int *Cr = calloc(n, sizeof(int));
    int *ABr = calloc(n, sizeof(int));

    if (!r || !Br || !Cr || !ABr) return false;

    // random vector r ∈ {0,1}
    for (int i = 0; i < n; i++) {
        r[i] = rand() % 2;
    }

    // Br = B * r
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            Br[i] += B[i][j] * r[j];
        }
    }

    // ABr = A * (Br)
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            ABr[i] += A[i][j] * Br[j];
        }
    }

    // Cr = C * r
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            Cr[i] += C[i][j] * r[j];
        }
    }

    // compare ABr and Cr
    for (int i = 0; i < n; i++) {
        if (ABr[i] != Cr[i]) {
            free(r); free(Br); free(Cr); free(ABr);
            return false;
        }
    }

    free(r); free(Br); free(Cr); free(ABr);
    return true;
}

bool check_fast(int n, int A[n][n], int B[n][n], int C[n][n]) {
    for (int i = 0; i < 5; i++) {
        if (!freivalds(n, A, B, C)) {
            return false;
        }
    }
    return true;
}


int main(int argc, char** argv){
    if (argc != 5) {
		fprintf(stderr, "Usage: %s a b N output_file\n", argv[0]);
		return 1;
	}
    int a_, b_;
    a_ = strtol(argv[1], NULL, 10);
	b_ = strtol(argv[2], NULL, 10);
	int n = strtol(argv[3], NULL, 10);
    char* fileout = argv[4];

	if (n <= 0) {
		fprintf(stderr, "Error: N must be a positive integer\n");
		return 1;
	}
	double tstart = getClock();
    int rows = n;
    int cols = rows;

    int (*a)[n] = malloc(sizeof(int[n][n]));
    int (*b)[n] = malloc(sizeof(int[n][n]));
    int (*c)[n] = malloc(sizeof(int[n][n]));
    int (*c_opt)[n] = malloc(sizeof(int[n][n]));

    if (a == NULL || b == NULL || c == NULL || c_opt == NULL) {
        fprintf(stderr, "Error allocating matrix memory\n");
        free(a);
        free(b);
        free(c);
        free(c_opt);
        return 1;
    }
    printf("The total memory allocated is %7.3lf GB\n.", 4.0 * sizeof(int) * (rows * cols) / 1024 / 1024 / 1024);
	
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            a[i][j] = a_;
            b[i][j] = b_;
            c[i][j] = 0;
            c_opt[i][j] = 0;
        }
    }
    FILE *fp = fopen(fileout, "w");
    if (fp == NULL) {
        fprintf(stderr, "Error opening file %s for writing\n", fileout);
        return 1;
    }

    fprintf(fp, "a = %d, b = %d, N = %d\n", a_, b_, n);

    double tend_init = getClock();
    double tinit = tend_init - tstart;
	fprintf(fp, "Time for initialization: %f seconds\n", tinit);
    
    double tstart_mult = getClock();
    matmul(n, a, b, c);
    double tend_mult = getClock();
    double tmult = tend_mult - tstart_mult;
    fprintf(fp, "Time for matrix multiplication: %f seconds\n", tmult);

    double tstart_check = getClock();
    if (is_correct(n, a, b, c)) {
        fprintf(fp, "The matrix multiplication is correct.\n");
    } else {
        fprintf(fp, "The matrix multiplication is NOT correct.\n");
    }
    double tend_check = getClock();
    double tcheck = tend_check - tstart_check;
    fprintf(fp, "Time for correctness check: %f seconds\n", tcheck);

    double tstart_opt = getClock();
    matmul_optimized(n, a, b, c_opt);
    double tend_opt = getClock();
    double topt = tend_opt - tstart_opt;
    fprintf(fp, "Time for optimized matrix multiplication: %f seconds\n", topt);

    double tstart_check_opt = getClock();
    if (is_correct(n, a, b, c_opt)) {
        fprintf(fp, "The matrix multiplication with the optimized method is correct.\n");
    } else {
        fprintf(fp, "The matrix multiplication with the optimized method is NOT correct.\n");
    }
    double tend_check_opt = getClock();
    double tcheck_opt = tend_check_opt - tstart_check_opt;
    fprintf(fp, "Time for correctness check of optimized method: %f seconds\n", tcheck_opt);

    double tstart_check_fast = getClock();
    if (check_fast(n, a, b, c)) {
        fprintf(fp, "The matrix multiplication with the original method is correct (fast check).\n");
    } else {
        fprintf(fp, "The matrix multiplication with the original method is NOT correct (fast check).\n");
    }
    double tend_check_fast = getClock();
    double tcheck_fast = tend_check_fast - tstart_check_fast;
    fprintf(fp, "Time for fast correctness check of original method: %f seconds\n", tcheck_fast);

    double tstart_check_fast_opt = getClock();
    if (check_fast(n, a, b, c_opt)) {
        fprintf(fp, "The matrix multiplication with the optimized method is correct (fast check).\n");
    } else {
        fprintf(fp, "The matrix multiplication with the optimized method is NOT correct (fast check).\n");
    }
    double tend_check_fast_opt = getClock();
    double tcheck_fast_opt = tend_check_fast_opt - tstart_check_fast_opt;
    fprintf(fp, "Time for fast correctness check of optimized method: %f seconds\n", tcheck_fast_opt);

    double tinit_write = getClock();
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            fprintf(fp, "%d ", c[i][j]);
        }
        fprintf(fp, "\n");
    }
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            fprintf(fp, "%d ", c_opt[i][j]);
        }
        fprintf(fp, "\n");
    }
    double tend_write = getClock();
    double twrite = tend_write - tinit_write;

    double tend_total = getClock();
    double ttotal = tend_total - tstart;
    fprintf(fp, "Total time: %f seconds\n", ttotal);
    fprintf(fp, "Time for writing output matrix: %f seconds\n", twrite);
    fclose(fp);
    free(a);
    free(b);
    free(c);
    free(c_opt);

	return 0;
}
