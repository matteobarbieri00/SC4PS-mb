# HomeWork03: Matrix Multiplication

In this homework task, I implemented matrix multiplication. This is done in the file `matmul.c`, which is to be compiled with the command `clang matmul.c -o matmul.o` (the second argument is optional) and run with the command `./matmul.o a b fileout.txt`. 

## Matrix multiplication functions
I have implemented two version for matrix multipliation:

- `matmul` which uses the naive way;

- `matmul_optimized` which uses a cash-friendly loop nesting.

## Check correctness functions
I have implemented two versions to check the correctness of matrix multiplication:

- `is_correct` which checks each entry of the uotput matix c;
- `check_fast` which uses Freivalds' algorithm to verify matrix multiplication, having an asymptotical speedup, namely $O(n^2)$ instead of $O(n^3)$. Freivald's algorithm is implemented in the function `freivalds` (taken from AI).

## main
In the main I take the times of each operation I perform:

- **initialization**, namely the time it takes to initialize the input matrices;

- **matrix multiplication**, namely the time for both the two matrix multiplication methods I have implemented;

- **check correctness**, namely the time for both the functions that check the correctness of matrix multiplication;

- **write time**, namely the time for writing the resulting matrices on file.

- **total time**, namely the total time needed for the whole main to run.

All these values are printed, together with the result matrices on a `.txt` file.

## Benchmarking results
I have taken the various times, as explained in the above section. When the matrix dimensions are small, the fast methods perform the same (or worst) then the **naive** methods, while a speedup can be seen for grater matrix sizes.


