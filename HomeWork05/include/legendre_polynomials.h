#ifndef LEGENDRE_POLYNOMIALS_H
#define LEGENDRE_POLYNOMIALS_H

/* Compute P_l(x) for l=0..lmax by forward recurrence (double). */
void legendre_forward(double x, int lmax, double *P);

/* Compute P_l(x) for l=0..lmax by backward (Miller) recurrence. */
void legendre_backward(double x, int lmax, int L, double *P);

/* Reference computed in long double precision. */
void legendre_reference(double x, int lmax, long double *Pref);

#endif /* LEGENDRE_POLYNOMIALS_H */