#include "legendre_polynomials.h"
#include "spherical_harm.h"
#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <sys/stat.h>


int main(void) {
    int lmax = 50;
    int backward_start = lmax + 50;
    double xs[] = {0.1, 0.5, 0.9, 0.99};
    int nx = sizeof(xs) / sizeof(xs[0]);
    const char *output_dir = "outputs";
    const char *output_path = "outputs/legendre_errors.csv";

    double Pf[lmax + 1], Pb[lmax + 1];
    double Yf[lmax + 1], Yb[lmax + 1];
    long double Pref[lmax + 1];
    long double Yref[lmax + 1];

    if (mkdir(output_dir, 0755) != 0 && errno != EEXIST) {
        perror("mkdir");
        return 1;
    }

    FILE *fp = fopen(output_path, "w");
    if (!fp) {
        perror("fopen");
        return 1;
    }

    fprintf(fp, "x,l,abs_err_forward,rel_err_forward,abs_err_back,rel_err_back,rel_err_sph_forward,rel_err_sph_back\n");

    for (int ix = 0; ix < nx; ++ix) {
        double x = xs[ix];
        double theta = acos(x);
        double phi = 0.0;

        legendre_reference(x, lmax, Pref);
        legendre_forward(x, lmax, Pf);
        legendre_backward(x, lmax, backward_start, Pb);
        spherical_harmonic_l0(theta, phi, lmax, Pf, Yf);
        spherical_harmonic_l0(theta, phi, lmax, Pb, Yb);
        spherical_harmonic_l0_reference(theta, phi, lmax, Pref, Yref);

        for (int l = 0; l <= lmax; ++l) {
            long double ref = Pref[l];
            long double yref = Yref[l];

            long double err_f = (long double)Pf[l] - ref;
            long double err_b = (long double)Pb[l] - ref;

            long double abs_err_f = fabsl(err_f);
            long double abs_err_b = fabsl(err_b);

            long double rel_err_f = fabsl(ref) > 0.0L ? abs_err_f / fabsl(ref) : 0.0L;
            long double rel_err_b = fabsl(ref) > 0.0L ? abs_err_b / fabsl(ref) : 0.0L;
        long double rel_err_yf = fabsl(yref) > 0.0L ? fabsl((long double)Yf[l] - yref) / fabsl(yref) : 0.0L;
        long double rel_err_yb = fabsl(yref) > 0.0L ? fabsl((long double)Yb[l] - yref) / fabsl(yref) : 0.0L;

        fprintf(fp, "%g,%d,%.15Le,%.15Le,%.15Le,%.15Le,%.15Le,%.15Le\n",
            x, l, abs_err_f, rel_err_f, abs_err_b, rel_err_b, rel_err_yf, rel_err_yb);
        }
    }

    fclose(fp);
    return 0;
}