#include "legendre_polynomials.h"
#include <math.h>
#include <stdlib.h>

static void rescale_double_tail(double *Q, int start, int end, double factor) {
    for (int index = start; index <= end; ++index) {
        Q[index] *= factor;
    }
}

void legendre_forward(double x, int lmax, double *P) {
    P[0] = 1.0;
    if (lmax == 0) {
        return;
    }

    P[1] = x;

    for (int l = 1; l < lmax; ++l) {
        double lp1 = (double)(l + 1);
        double num = (2.0 * l + 1.0) * x * P[l] - (double)l * P[l - 1];
        P[l + 1] = num / lp1;
    }
}

void legendre_backward(double x, int lmax, int L, double *P) {
    double *Q = (double *)malloc((L + 2) * sizeof(double));
    if (!Q) {
        return;
    }

    Q[L + 1] = 0.0;
    Q[L] = 1.0;

    for (int l = L; l >= 1; --l) {
        double denom = (double)l;
        double term1 = (2.0 * l + 1.0) * x * Q[l] / denom;
        double term2 = (double)(l + 1) * Q[l + 1] / denom;
        Q[l - 1] = term1 - term2;

        if (fabs(Q[l - 1]) > 1.0e200) {
            rescale_double_tail(Q, l - 1, L + 1, 1.0e-200);
        }
    }

    if (Q[0] == 0.0) {
        free(Q);
        return;
    }

    double scale = 1.0 / Q[0];
    for (int l = 0; l <= lmax; ++l) {
        P[l] = scale * Q[l];
    }

    free(Q);
}

void legendre_reference(double x, int lmax, long double *P) {
    long double xl = (long double)x;

    for (int l = 0; l <= lmax; ++l) {
        long double coeff = 1.0L;
        long double value = 0.0L;

        for (int j = 1; j <= l; ++j) {
            coeff *= (2.0L * (long double)j - 1.0L) / (long double)j;
        }

        for (int k = 0; k <= l / 2; ++k) {
            value += coeff * powl(xl, (long double)(l - 2 * k));

            if (k < l / 2) {
                long double numerator = (long double)(l - 2 * k) * (long double)(l - 2 * k - 1);
                long double denominator = 2.0L * (long double)(k + 1) * (2.0L * (long double)l - 2.0L * (long double)k - 1.0L);
                coeff *= -numerator / denominator;
            }
        }

        P[l] = value;
    }
}