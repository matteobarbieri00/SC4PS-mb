# Homewor05 - Stability of Legendre Polynomials
For this homework I here report the answers to the various points in order to include all the information regarding the questions at this [link](https://codimd.infn.it/s/J2RN0fKY5).

## 1: Project Structure

The `src/` directory contains the C source files used for the numerical experiment. In particular, `compute.c` drives the generation of the error table, while `legendre_polynomials.c` implements the forward recurrence, the backward recurrence, and the reference evaluation used for the comparison.

The `include/` directory stores the public header `legendre_polynomials.h`. This file exposes the function prototypes shared by the source files and keeps the interface separated from the implementation.

The `scripts/` directory contains the Python post-processing utilities. At the moment it includes `plot_errors.py`, which reads the generated CSV file and produces the plots of absolute and relative errors for the selected values of $x$.

The `outputs/` directory is used for generated results. It collects the CSV table with the computed errors together with the image files produced by the plotting script, so numerical data and visualizations remain separate from the source code.

Finally, the `Makefile` coordinates the full workflow: compilation of the C code, execution of the program, creation of the Python virtual environment, and generation of the plots.

To reproduce the workflow from the `HomeWork05/` directory:

```bash
# Compile the C program
make

# Run the executable and generate outputs/legendre_errors.csv
make run

# Create the local virtual environment for plotting dependencies
make venv

# Run the full pipeline: execute C code and generate all plots
make plot-python
```

Optional cleanup commands:

```bash
# Remove build artifacts, binaries, and generated outputs
make clean

# Remove the Python virtual environment
make clean-venv
```

## 2 & 3: Plots of the absolute and relative errors for foreward and backward recurrence
I give here the pots for the absulute and relative errors values of the foreward and backward recurrence versus the high-precision reference for values of $x \in \{0.1,0.5,0.9,0.99\}$.

<figure>
	<img src="outputs/legendre_abs_errors_x_0.1.png" alt="legendre absolute errors for x=0.1">
	<figcaption>Figure 1. Absolute errors for the forward and backward recurrence at x = 0.1.</figcaption>
</figure>

<figure>
	<img src="outputs/legendre_errors_x_0.1.png" alt="legendre relative errors for x=0.1">
	<figcaption>Figure 2. Relative errors for the forward and backward recurrence at x = 0.1.</figcaption>
</figure>

<figure>
	<img src="outputs/legendre_abs_errors_x_0.5.png" alt="legendre absolute errors for x=0.5">
	<figcaption>Figure 3. Absolute errors for the forward and backward recurrence at x = 0.5.</figcaption>
</figure>

<figure>
	<img src="outputs/legendre_errors_x_0.5.png" alt="legendre relative errors for x=0.5">
	<figcaption>Figure 4. Relative errors for the forward and backward recurrence at x = 0.5.</figcaption>
</figure>

<figure>
	<img src="outputs/legendre_abs_errors_x_0.9.png" alt="legendre absolute errors for x=0.9">
	<figcaption>Figure 5. Absolute errors for the forward and backward recurrence at x = 0.9.</figcaption>
</figure>

<figure>
	<img src="outputs/legendre_errors_x_0.9.png" alt="legendre relative errors for x=0.9">
	<figcaption>Figure 6. Relative errors for the forward and backward recurrence at x = 0.9.</figcaption>
</figure>

<figure>
	<img src="outputs/legendre_abs_errors_x_0.99.png" alt="legendre absolute errors for x=0.99">
	<figcaption>Figure 7. Absolute errors for the forward and backward recurrence at x = 0.99.</figcaption>
</figure>

<figure>
	<img src="outputs/legendre_errors_x_0.99.png" alt="legendre relative errors for x=0.99">
	<figcaption>Figure 8. Relative errors for the forward and backward recurrence at x = 0.99.</figcaption>
</figure>

### Comments on the plots

For $x = 0.1$, it is clear Figure 1 and 2, that the forward recurrence is accurate, given that the absolute error is in the order of $10^{-16}$ and the relative error is of order $10^{-14}$

Going to Figure 2 and 4, one can see that the absolute and relative errors of the backward recurrence are relatively high, whereas for the foreward recursion the errors scale exponentially as $l$ increases. This behaviour also shows up in Figures 5 and 6, for $x = 0.9$ and in Figures 7 and 8, for $x=0.99$.

Overall, backward recurrence has higher absolute and relative errors compared with the foreward recurrence.

## 4: Explanation of the Numerical Behaviour

### Two Independent Solutions of the Recurrence

The three-term recurrence relation for Legendre polynomials,
$$P_{\ell+1}(x) = \frac{2\ell+1}{\ell+1}x\,P_\ell(x) - \frac{\ell}{\ell+1}P_{\ell-1}(x),$$
is a second-order linear recurrence. Mathematically, it admits **two linearly independent solutions**. One is the physical Legendre polynomial $P_\ell(x)$ that we seek. The other is a second solution that satisfies the same recurrence but is not selected by the physical boundary conditions ($P_0 = 1$ and $P_1 = x$).

### Miller's Algorithm: Dominant and Minimal Solutions

Miller's key observation is that the behaviour of the two solutions depends on the **direction of propagation**:

- **Forward direction** (increasing $\ell$): The physical solution $P_\ell(x)$ is the *minimal* (slowly growing) solution for $|x| < 1$, while the unwanted solution is *dominant* (rapidly growing).
- **Backward direction** (decreasing $\ell$ from large $L$): The roles reverse: the unwanted solution is now minimal (decays) while the physical solution remains slowly varying.

Miller's algorithm exploits this by starting the backward sweep at an artificially high degree $L$ with arbitrary seeds, then normalizing at $\ell=0$. As the sweep goes backward, the dominant (unwanted) component is exponentially damped, leaving only the physical solution.

### Failure of Dominant/Minimal Separation for Ordinary Legendre Polynomials on $|x|<1$

**However**, for ordinary Legendre polynomials $P_\ell(x)$ with $|x| < 1$ (interior of the interval), the dominant/minimal mechanism **does not produce a clean separation**. The two solutions do not have clearly separated growth rates that would guarantee robust recovery of $P_\ell$ via a single normalization condition ($P_0 = 1$) alone.

In fact, a three-term recurrence requires **two initial values** to uniquely specify a solution. Miller's algorithm enforces only one constraint ($P_0 = 1$) at the end of the backward sweep. This is insufficient to fully determine the physical solution when the two linearly independent solutions have comparable magnitude in the range of interest. The backward sweep with arbitrary seeds followed by one-point normalization does not reliably eliminate contamination from the unwanted mode across all degrees.

### Forward Recurrence is Stable for the Tested Values

By contrast, the **forward recurrence initialized with the exact physical values** $P_0(x) = 1$ and $P_1(x) = x$ directly selects the correct solution. Both initial conditions are enforced from the start.

For the tested values $x \in \{0.1, 0.5, 0.9, 0.99\}$ (all interior to $[-1, 1]$), the forward recurrence propagates the physical solution cleanly. With the corrected reference evaluation, the forward absolute and relative errors remain close to machine precision (typically around $10^{-15}$ or smaller) across the degree range, while the backward recurrence seeded with arbitrary values remains noticeably less accurate.

### Backward Recurrence Fails to Recover $P_\ell$ Reliably

Conversely, the arbitrary backward recurrence does **not reliably recover $P_\ell(x)$** for the same values. The plots show that the backward errors are significantly larger than the forward errors across all tested degrees and $x$ values. This is not because of algorithmic instability in the backward direction (the recurrence itself is stable backwards), but because:

1. The arbitrary initial seeds at $L=100$ do not encode the true physical solution in a recoverable way.
2. A single normalization at $\ell=0$ is insufficient to suppress all contamination from the unwanted linearly independent solution.
3. The contamination persists across the entire degree range.

### Conclusion

For Legendre polynomials on $|x|<1$, the numerical experiment confirms that **algorithmic stability and mathematical solution selection are distinct issues**. The forward recurrence is the reliable method because it directly encodes the physics through exact initial conditions. Miller's algorithm, while theoretically elegant and powerful for other special functions (e.g., Bessel functions near the turning point), does not provide automatic advantage for ordinary Legendre polynomials in this parameter regime. The practical lesson is that starting with the correct boundary values is more robust than relying on asymptotic damping of unwanted modes.

## 5: Spherical Harmonics

For spherical harmonics I observe the following relative errors:

<figure>
	<img src="outputs/spherical_harm_errors_x_0.1.png" alt="spherical harmonic relative errors for x=0.1">
	<figcaption>Figure 9. Relative errors for the spherical harmonics computed from the forward and backward recurrence at x = 0.1.</figcaption>
</figure>

<figure>
	<img src="outputs/spherical_harm_errors_x_0.5.png" alt="spherical harmonic relative errors for x=0.5">
	<figcaption>Figure 10. Relative errors for the spherical harmonics computed from the forward and backward recurrence at x = 0.5.</figcaption>
</figure>

<figure>
	<img src="outputs/spherical_harm_errors_x_0.9.png" alt="spherical harmonic relative errors for x=0.9">
	<figcaption>Figure 11. Relative errors for the spherical harmonics computed from the forward and backward recurrence at x = 0.9.</figcaption>
</figure>

<figure>
	<img src="outputs/spherical_harm_errors_x_0.99.png" alt="spherical harmonic relative errors for x=0.99">
	<figcaption>Figure 12. Relative errors for the spherical harmonics computed from the forward and backward recurrence at x = 0.99.</figcaption>
</figure>

## 6
The three-term recurrence for Legendre polynomials admits two linearly independent solutions. One is the physical solution $P_l(x)$ that we want, while the other is a second solution of the same recurrence that is not selected by the initial conditions of the mathematical problem. In recurrence language, one of these behaves as a dominant solution and the other as a minimal solution, depending on the direction in which the recurrence is iterated. When the recurrence is run forward in finite precision, roundoff introduces a small contamination from the dominant mode, and that component is amplified step after step. For this reason the forward recurrence can become unstable even if the exact polynomial values themselves are perfectly regular.

Miller's idea is to reverse the direction of the computation. In backward recurrence, the unstable component is no longer amplified; instead it is damped as the recursion moves from large $l$ down to small $l$. Starting from arbitrary values at a sufficiently large index $L$ and then normalizing the sequence at the end suppresses the unwanted dominant component and recovers the minimal, physically relevant solution. This is why backward recurrence is usually the stable way to evaluate the sequence when forward propagation would amplify roundoff.

This also clarifies the difference between conditioning and stability. The Legendre polynomials $P_l(x)$ near $x = 1$ are not themselves badly conditioned objects: a small perturbation in the data does not necessarily imply a huge change in the exact value. However, an algorithm can still be unstable if it magnifies floating-point perturbations internally. In this homework the loss of accuracy is therefore not mainly a property of the function being evaluated, but of the forward recurrence algorithm in finite precision. The problem is algorithmic stability, not intrinsic conditioning of the Legendre polynomials.
