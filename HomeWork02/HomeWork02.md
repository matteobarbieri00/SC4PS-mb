# HomeWork02: Program in C to compute $\vec{d} = a \vec{x} + \vec{y}$

In this directory there are two `.c` files which contain a program that does the same thing, but one uses static memory allocation for the arrays, while the other uses dynamical memory allocation, allowing to do the sum between two vectors whith more entries. Indeed using $N = 10^6$ gives a `segmentation fault`:

- `sum2vec.c` uses static memory allocation;

- `sum2vec_dynamical.c` uses dynamical memory allocation.

In the files there is a function that ensures the correctness of the result, as required by the Homework.

Also there is a hard coded function that runs the sum for the values $a = , x =0.1, y = 7.1" and it works.
  
