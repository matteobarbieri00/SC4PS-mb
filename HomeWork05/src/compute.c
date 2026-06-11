#include "legendre_polynomials.h"
#include "spherical_harm.h"
#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <sys/stat.h>


int main(void) {
    /* Maximum degree of Legendre polynomials to evaluate. */
    int lmax = 50;

    /* The backward (Miller) recurrence is seeded at a degree higher than lmax
     * so that the unnormalised sequence has converged by the time it reaches
     * the degrees of interest. */
    int backward_start = lmax + 50;

    /* Evaluation points in [-1, 1]. */
    double xs[] = {0.1, 0.5, 0.9, 0.99};
    int nx = sizeof(xs) / sizeof(xs[0]);

    const char *output_dir  = "outputs";
    const char *output_path = "outputs/legendre_errors.csv";

    /* Arrays for the three Legendre methods and the two spherical-harmonic
     * variants (forward/backward Legendre base). */
    double      Pf[lmax + 1], Pb[lmax + 1];
    double      Yf[lmax + 1], Yb[lmax + 1];
    long double Pref[lmax + 1];
    long double Yref[lmax + 1];

    /* Create the output directory if it does not already exist. */
    if (mkdir(output_dir, 0755) != 0 && errno != EEXIST) {
        perror("mkdir");
        return 1;
    }

    FILE *fp = fopen(output_path, "w");
    if (!fp) {
        perror("fopen");
        return 1;
    }

    /* CSV header:
        *   abs/rel_err_forward      – forward Bonnet recurrence vs. reference
        *   abs/rel_err_back         – backward (Miller) recurrence vs. reference
        *   rel_err_sph_forward/back – spherical harmonics Y_{l,0} errors
     */
    fprintf(fp, "x,l,"
                "abs_err_forward,rel_err_forward,"
                "abs_err_back,rel_err_back,"
                "rel_err_sph_forward,rel_err_sph_back\n");

    for (int ix = 0; ix < nx; ++ix) {
        double x     = xs[ix];
        double theta = acos(x);   /* polar angle corresponding to x = cos(theta) */
        double phi   = 0.0;

        /* Evaluate all three Legendre methods at this x. */
        legendre_reference(x, lmax, Pref);
        legendre_forward(x, lmax, Pf);
        legendre_backward(x, lmax, backward_start, Pb);

        /* Evaluate spherical harmonics Y_{l,0} using each Legendre base. */
        spherical_harmonic_l0(theta, phi, lmax, Pf,   Yf);
        spherical_harmonic_l0(theta, phi, lmax, Pb,   Yb);
        spherical_harmonic_l0_reference(theta, phi, lmax, Pref, Yref);

        for (int l = 0; l <= lmax; ++l) {
            long double ref  = Pref[l];
            long double yref = Yref[l];

            /* --- Legendre polynomial errors --- */

            /* Forward recurrence error vs. long-double reference. */
            long double abs_err_f = fabsl((long double)Pf[l] - ref);
            long double rel_err_f = fabsl(ref) > 0.0L
                                    ? abs_err_f / fabsl(ref) : 0.0L;

            /* Backward (Miller) recurrence error vs. long-double reference. */
            long double abs_err_b = fabsl((long double)Pb[l] - ref);
            long double rel_err_b = fabsl(ref) > 0.0L
                                    ? abs_err_b / fabsl(ref) : 0.0L;

            /* --- Spherical harmonic errors --- */

            long double rel_err_yf = fabsl(yref) > 0.0L
                                     ? fabsl((long double)Yf[l] - yref) / fabsl(yref) : 0.0L;
            long double rel_err_yb = fabsl(yref) > 0.0L
                                     ? fabsl((long double)Yb[l] - yref) / fabsl(yref) : 0.0L;

                fprintf(fp, "%g,%d,%.15Le,%.15Le,%.15Le,%.15Le,%.15Le,%.15Le\n",
                    x, l,
                    abs_err_f, rel_err_f,
                    abs_err_b, rel_err_b,
                    rel_err_yf, rel_err_yb);
        }
    }

    fclose(fp);
    return 0;
}