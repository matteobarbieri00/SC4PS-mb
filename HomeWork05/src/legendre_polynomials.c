#include "legendre_polynomials.h"
#include <math.h>
#include <stdlib.h>

/* Rescale a contiguous tail of the temporary Miller sequence. */
static void rescale_double_tail(double *Q, int start, int end, double factor) {
    for (int index = start; index <= end; ++index) {
        Q[index] *= factor;
    }
}

/* Forward Bonnet recurrence with exact initial values P0=1, P1=x. */
void legendre_forward(double x, int lmax, double *P) {
    P[0] = 1.0;
    if (lmax == 0) {
        return;
    }

    P[1] = x;

    for (int l = 1; l < lmax; ++l) {
        double lp1 = (double)(l + 1);
        /* Bonnet formula: P_{l+1} from P_l and P_{l-1}. */
        double num = (2.0 * l + 1.0) * x * P[l] - (double)l * P[l - 1];
        P[l + 1] = num / lp1;
    }
}

/* Miller-style backward sweep with final normalization at l=0. */
void legendre_backward(double x, int lmax, int L, double *P) {
    double *Q = (double *)malloc((L + 2) * sizeof(double));
    if (!Q) {
        return;
    }

    /* Arbitrary start values at large degree. */
    Q[L + 1] = 0.0;
    Q[L] = 1.0;

    for (int l = L; l >= 1; --l) {
        double denom = (double)l;
        /* Algebraic backward form of the three-term recurrence. */
        double term1 = (2.0 * l + 1.0) * x * Q[l] / denom;
        double term2 = (double)(l + 1) * Q[l + 1] / denom;
        Q[l - 1] = term1 - term2;

        /* Keep intermediates finite; global scale is restored at the end. */
        if (fabs(Q[l - 1]) > 1.0e200) {
            rescale_double_tail(Q, l - 1, L + 1, 1.0e-200);
        }
    }

    if (Q[0] == 0.0) {
        free(Q);
        return;
    }

    /* Enforce P0=1 normalization and copy only the requested range. */
    double scale = 1.0 / Q[0];
    for (int l = 0; l <= lmax; ++l) {
        P[l] = scale * Q[l];
    }

    free(Q);
}

/* Reference evaluation by forward recurrence in long-double arithmetic. */
void legendre_reference(double x, int lmax, long double *Pref) {
    long double xl = (long double)x;
    long double *P = (long double *)malloc((size_t)(lmax + 1) * sizeof(*P));

    if (!P) {
        return;
    }

    P[0] = 1.0L;
    if (lmax >= 1) {
        P[1] = xl;
    }

    for (int l = 1; l < lmax; ++l) {
        P[l + 1] = (((long double)(2 * l + 1)) * xl * P[l]
                    - (long double)l * P[l - 1]) / (long double)(l + 1);
    }

    for (int l = 0; l <= lmax; ++l) {
        Pref[l] = P[l];
    }

    free(P);
}