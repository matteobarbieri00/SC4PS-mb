#ifndef SPHERICAL_HARM_H
#define SPHERICAL_HARM_H

/* Compute Y_l^0(theta, phi) from a precomputed sequence P_l(cos(theta)). */
void spherical_harmonic_l0(double theta, double phi, int lmax, const double *P, double *Y);

/* Compute a long-double reference sequence Y_l^0(theta, phi). */
void spherical_harmonic_l0_reference(double theta, double phi, int lmax, const long double *P, long double *Y);

#endif /* SPHERICAL_HARM_H */