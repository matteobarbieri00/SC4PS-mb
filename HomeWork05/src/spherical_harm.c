#include "spherical_harm.h"
#include <math.h>

void spherical_harmonic_l0(double theta, double phi, int lmax, const double *P, double *Y) {
    (void)theta;
    (void)phi;

    for (int l = 0; l <= lmax; ++l) {
        double norm = sqrt((2.0 * (double)l + 1.0) / (4.0 * M_PI));
        Y[l] = norm * P[l];
    }
}

void spherical_harmonic_l0_reference(double theta, double phi, int lmax, const long double *P, long double *Y) {
    (void)theta;
    (void)phi;

    for (int l = 0; l <= lmax; ++l) {
        long double norm = sqrtl((2.0L * (long double)l + 1.0L) / (4.0L * acosl(-1.0L)));
        Y[l] = norm * P[l];
    }
}