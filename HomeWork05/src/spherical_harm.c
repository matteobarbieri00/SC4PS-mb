#include "spherical_harm.h"
#include <math.h>

/* For m=0, Y_{l,0} depends on theta only through P_l(cos(theta));
 * here P_l is already provided by the caller. */
void spherical_harmonic_l0(double theta, double phi, int lmax, const double *P, double *Y) {
    (void)theta;
    (void)phi;

    for (int l = 0; l <= lmax; ++l) {
        /* Normalization: sqrt((2l+1)/(4pi)). */
        double norm = sqrt((2.0 * (double)l + 1.0) / (4.0 * M_PI));
        Y[l] = norm * P[l];
    }
}

/* Long-double reference version of the same m=0 formula. */
void spherical_harmonic_l0_reference(double theta, double phi, int lmax, const long double *P, long double *Y) {
    (void)theta;
    (void)phi;

    for (int l = 0; l <= lmax; ++l) {
        /* acosl(-1) is pi in long-double precision. */
        long double norm = sqrtl((2.0L * (long double)l + 1.0L) / (4.0L * acosl(-1.0L)));
        Y[l] = norm * P[l];
    }
}