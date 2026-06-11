from pathlib import Path

import matplotlib.pyplot as plt
import numpy as np

output_dir = Path("outputs")
data = np.genfromtxt(output_dir / "legendre_errors.csv", delimiter=",", names=True)

# Avoid log(0) in semilogy for difference plots.
tiny = np.finfo(float).tiny

x_vals = np.unique(data["x"])

for x in x_vals:
    subset = data[data["x"] == x]

    l = subset["l"]
    abs_err_f = subset["abs_err_forward"]
    abs_err_b = subset["abs_err_back"]
    err_f = subset["rel_err_forward"]
    err_b = subset["rel_err_back"]
    sph_err_f = subset["rel_err_sph_forward"]
    sph_err_b = subset["rel_err_sph_back"]

    plt.figure()
    plt.semilogy(l, abs_err_f, label="Forward")
    plt.semilogy(l, abs_err_b, label="Backward (Miller)")

    plt.title(f"Absolute error vs l (x={x})")
    plt.xlabel("l")
    plt.ylabel("Absolute error")
    plt.legend()
    plt.grid()

    plt.savefig(output_dir / f"legendre_abs_errors_x_{x}.png")
    plt.close()

    plt.figure()
    plt.semilogy(l, err_f, label="Forward")
    plt.semilogy(l, err_b, label="Backward (Miller)")

    plt.title(f"Relative error vs l (x={x})")
    plt.xlabel("l")
    plt.ylabel("Relative error")
    plt.legend()
    plt.grid()

    plt.savefig(output_dir / f"legendre_errors_x_{x}.png")
    plt.close()

    plt.figure()
    plt.semilogy(l, sph_err_f, label="Forward")
    plt.semilogy(l, sph_err_b, label="Backward (Miller)")

    plt.title(f"Relative spherical harmonic error vs l (x={x})")
    plt.xlabel("l")
    plt.ylabel("Relative error")
    plt.legend()
    plt.grid()

    plt.savefig(output_dir / f"spherical_harm_errors_x_{x}.png")
    plt.close()

    # Diagnostic: if Y_l0 errors are driven only by P_l errors, these
    # differences should stay near floating-point roundoff.
    diff_f = np.abs(sph_err_f - err_f)
    diff_b = np.abs(sph_err_b - err_b)

    plt.figure()
    plt.semilogy(l, np.maximum(diff_f, tiny), label="|rel_err_sph_forward - rel_err_forward|")
    plt.semilogy(l, np.maximum(diff_b, tiny), label="|rel_err_sph_back - rel_err_back|")

    plt.title(f"Propagation check for Y_l0 relative errors (x={x})")
    plt.xlabel("l")
    plt.ylabel("Absolute difference")
    plt.legend()
    plt.grid()

    plt.savefig(output_dir / f"spherical_harm_error_match_x_{x}.png")
    plt.close()