# HomeWork02: Program in C to compute $\vec{d} = a \vec{x} + \vec{y}$

In this directory there are two `.c` files which contain a program that does the same thing, but one uses static memory allocation for the arrays, while the other uses dynamical memory allocation, allowing to do the sum between two vectors whith more entries. Indeed using $N = 10^6$ gives a `segmentation fault`:

- `sum2vec.c` uses static memory allocation;

- `sum2vec_dynamical.c` uses dynamical memory allocation.

In the files there is a function that ensures the correctness of the result, as required by the Homework. Morover, one has to enter the values for $a,x$ and $y$ from command line. Indeed after running the compilation `gcc sum2vec.c -o sum2vec.x` (and respectively for `sum2vec_dynamical.c`) one can run the code with the command `./sum2vec.x a x y` giving numerical values for them, otherwise a message displaying the correct usage appears on the terminal.

Also there is a hard coded function that runs the sum for the values $a = , x =0.1, y = 7.1" and it works.
  
