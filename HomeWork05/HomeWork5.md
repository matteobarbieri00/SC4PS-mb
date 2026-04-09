# Homewor05 - Stability of Legendre Polynomials
For this homework I here report the answers to the various points in order to include all the information regarding the questions at this [link](https://codimd.infn.it/s/J2RN0fKY5).

## 1: Project Structure

The `src/` directory contains the C source files used for the numerical experiment. In particular, `compute.c` drives the generation of the error table, while `legendre_polynomials.c` implements the forward recurrence, the backward recurrence, and the reference evaluation used for the comparison.

The `include/` directory stores the public header `legendre_polynomials.h`. This file exposes the function prototypes shared by the source files and keeps the interface separated from the implementation.

The `scripts/` directory contains the Python post-processing utilities. At the moment it includes `plot_errors.py`, which reads the generated CSV file and produces the plots of absolute and relative errors for the selected values of $x$.

The `outputs/` directory is used for generated results. It collects the CSV table with the computed errors together with the image files produced by the plotting script, so numerical data and visualizations remain separate from the source code.

Finally, the `Makefile` coordinates the full workflow: compilation of the C code, execution of the program, creation of the Python virtual environment, and generation of the plots.

## 2: Plots
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

## 3: Observe loss of significance and instability

For $x = 0.1$, it is clear Figure 1 and 2, that the forward recurrence is accurate, given that the absolute error is in the order of $10^{-16}$ and the relative error is of order $10^{-14}$

Looking at Figure 8 and 9 it is clear how forward recurrence gets unstable, given that it grows exponentially (linearly in the graph because of logarithmic scale on y axis).

However I do not observe the accuracy for backward recurrance, but I canno understand why.

## 4 & 5: Spherical Harmonics

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
