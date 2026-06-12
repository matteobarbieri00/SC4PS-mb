#include <omp.h>
#include <stdio.h>

static long num_steps = 100000000;
double step;
#define PAD 8
#define MAX_THREADS 8
int main(void)
{   
    int threads[MAX_THREADS];
    for (int i = 1; i < MAX_THREADS+1; i++) threads[i-1] = i;
    double times[MAX_THREADS];
    double times_padding[MAX_THREADS];
    double times_critical[MAX_THREADS];
    printf("No Padding\n");
    int NUM_THREADS = 1;
    for (NUM_THREADS = 1; NUM_THREADS <= MAX_THREADS; NUM_THREADS++) {
        double tdata = omp_get_wtime();
        int i, nthreads;
        double pi, sum[NUM_THREADS];
        omp_set_num_threads(NUM_THREADS);
        step = 1.0 / (double)num_steps;
        #pragma omp parallel 
        {
            int i, id, numthreads;
            double x;
            id = omp_get_thread_num();
            numthreads = omp_get_num_threads();
            if (id == 0) nthreads = numthreads;
            for (i = id, sum[id] = 0.0; i < num_steps; i += numthreads) {
                x = (i + 0.5) * step;
                sum[id] = sum[id] + 4.0 / (1.0 + x * x);
            }

        }
        for (i = 0, pi = 0.0; i < nthreads; i++) pi += sum[i] * step;
        tdata = omp_get_wtime() - tdata;
        times[NUM_THREADS-1] = tdata;
        printf("pi = %f\n", pi);
        printf("Time taken with %d threads: %f seconds to get pi = %f\n", NUM_THREADS, tdata, pi);
    }
    printf("With Padding\n");
    for (NUM_THREADS = 1; NUM_THREADS <= MAX_THREADS; NUM_THREADS++) {
        double tdata = omp_get_wtime();
        int i, nthreads;
        double pi, sum[NUM_THREADS][PAD];
        omp_set_num_threads(NUM_THREADS);
        step = 1.0 / (double)num_steps;
        #pragma omp parallel 
        {
            int i, id, numthreads;
            double x;
            id = omp_get_thread_num();
            numthreads = omp_get_num_threads();
            if (id == 0) nthreads = numthreads;
            for (i = id, sum[id][0] = 0.0; i < num_steps; i += numthreads) {
                x = (i + 0.5) * step;
                sum[id][0] += 4.0 / (1.0 + x * x);
            }

        }
        for (i = 0, pi = 0.0; i < nthreads; i++) pi += sum[i][0] * step;
        tdata = omp_get_wtime() - tdata;
        times_padding[NUM_THREADS-1] = tdata;
        printf("pi = %f\n", pi);
        printf("Time taken with %d threads: %f seconds to get pi = %f\n", NUM_THREADS, tdata, pi);
    }
    printf("Threads\tTime without padding\tTime with padding\n");
    for (int i = 0; i < MAX_THREADS; i++) {
        printf("%d\t%f\t%f\n", threads[i], times[i], times_padding[i]);
    }
    printf("With Critical\n");
    for (NUM_THREADS = 1; NUM_THREADS <= MAX_THREADS; NUM_THREADS++) {
        double tdata = omp_get_wtime();
        int i, nthreads;
        double pi, sum = 0.0;
        omp_set_num_threads(NUM_THREADS);
        step = 1.0 / (double)num_steps;
        #pragma omp parallel 
        {
            int i, id, numthreads;
            double x;
            id = omp_get_thread_num();
            numthreads = omp_get_num_threads();
            if (id == 0) nthreads = numthreads;
            for (i = id; i < num_steps; i += numthreads) {
                x = (i + 0.5) * step;
                sum += 4.0 / (1.0 + x * x);
            }
            #pragma omp critical
            pi = sum * step;
        }
        
        tdata = omp_get_wtime() - tdata;
        times_critical[NUM_THREADS-1] = tdata;
        printf("pi = %f\n", pi);
        printf("Time taken with %d threads: %f seconds to get pi = %f\n", NUM_THREADS, tdata, pi);
    }
    printf("Threads\tNo padding\tPadding\tCritical\n");
    for (int i = 0; i < MAX_THREADS; i++) {
        printf("%d\t%f\t%f\t%f\n", threads[i], times[i], times_padding[i], times_critical[i]);
    }
    printf("With Loop\n");
    double times_loop[MAX_THREADS];
    for (NUM_THREADS = 1; NUM_THREADS <= MAX_THREADS; NUM_THREADS++) {
        double tdata = omp_get_wtime();
        int i, nthreads;
        double pi, sum = 0.0;
        omp_set_num_threads(NUM_THREADS);
        step = 1.0 / (double)num_steps;
        #pragma omp parallel for reduction(+:sum)
        for (i = 0; i < num_steps; i++) {
            double x = (i + 0.5) * step;
            sum += 4.0 / (1.0 + x * x);
        }
        pi = sum * step;
        tdata = omp_get_wtime() - tdata;
        times_loop[NUM_THREADS-1] = tdata;
        printf("pi = %f\n", pi);
        printf("Time taken with %d threads: %f seconds to get pi = %f\n", NUM_THREADS, tdata, pi);
    }
    printf("Threads\tNo padding\tPadding\tCritical\tLoop\n");
    for (int i = 0; i < MAX_THREADS; i++) {
        printf("%d\t%f\t%f\t%f\t%f\n", threads[i], times[i], times_padding[i], times_critical[i], times_loop[i]);
    }
    return 0;
}