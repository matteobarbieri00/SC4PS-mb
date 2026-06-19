# Scientific Computing for Physics Students 25/26
In this repository, there are the solutions to the homework assigned during the PhD courde of *Scientific Computing for Physics Students*. 

Seven directories, corresponding to the seven assigned homeworks are present. In each directory there is the source code needed for the assignement and a MarkDown file containing the answers to the given questions and the instructions to compile and execute the code.

Moreover, there is also a directory named *Tim*, where I have some code for the part of the course given by Tim Mattson, evn though this is not a part of the assigned homework.

The repository also contains the binary files of the executables and the Python virtual environments, where needed. These will be removed in the final commit. The outputs will not be removed, since they are needed for the MorkDown files.

## Homework 1

In the directory *HomeWork01* a MarkDown file and a basic `.sh` script are present. These are abut the first lesson on Linux.

## Homework 2

In the directory *HomeWork02* there are two `.c` files, both solving the problem of summing two vectors. One allocates memory statically and one dynamically. The answers to the assignement are given in the MarkDown file. 

## Homework 3

In the directory *HomeWork03* there is a `.c` code for matrix multiplication, with both a naive and a cache-friendly implementation. The MarkDown file describes the code structure and reports the benchmarking and correctness checks.

## Homework 4

In the directory *HomeWork04* there is the MarkDown file with the answers to the assignement on FFT, sampling and aliasing, together with the discussion of coupled oscillators and the related outputs.

## Homework 5

In the directory *HomeWork05* there is a more structured project with `src`, `include`, `scripts`, `outputs` and a `Makefile`. The task is about numerical stability of Legendre polynomials (forward/backward recurrence) and spherical harmonics, with C computations and Python plots.

## Homework 6

In the directory *HomeWork06* there is an advanced version of `sum2vec` that reads parameters from a config file and stores vectors, chunks and metadata in an HDF5 output file. The MarkDown file explains the implementation details and build instructions.

## Homework 7

In the directory *HomeWork07* there is a C project on random number generation and Monte Carlo methods (coin tosses, estimate of $\pi$, change of variables and empirical CDF), plus a Python script for plotting all results.

## Tim

In the directory *Tim* there are additional C exercises from the lectures by Tim Mattson, including serial and parallel versions of numerical examples.
