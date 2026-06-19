# HomeWork03: Matrix Multiplication

In this homework task, I implemented matrix multiplication. This is done in the file `matmul.c`, which is to be compiled with the command `gcc matmul.c -o matmul.x` (the second argument is optional) and run with the command `./matmul.x a b N fileout.txt`, where the last argument is the file where to write the output. 

## Matrix multiplication functions
I have implemented two version for matrix multipliation:

- `matmul` which uses the naive way;

- `matmul_optimized` which uses a cash-friendly loop nesting.

## Check correctness functions
I have implemented two versions to check the correctness of matrix multiplication:

- `is_correct` which checks each entry of the output matix $c$;
- `check_fast` which uses Freivalds' algorithm to verify matrix multiplication, having an asymptotical speedup, namely $O(n^2)$ instead of $O(n^3)$. Freivald's algorithm is implemented in the function `freivalds`. I found this method looking on the web and asking to AI and also its implementation has been generated with it.

## main
In the main I take the times of each operation I perform:

- **initialization**: the time it takes to initialize the input matrices;

- **matrix multiplication**: the time for both the two matrix multiplication methods I have implemented;

- **check correctness**: the time for both the functions that check the correctness of matrix multiplication;

- **write time**: the time for writing the resulting matrices on file.

- **total time**: the total time needed for the whole main to run.

All these values are printed, together with the result matrices on a `.txt` file.

## Benchmarking results
I have taken the various times, as explained in the above section. When the matrix dimensions are small, the fast methods perform the same (or worst) then the **naive** methods, while a speedup can be seen for grater matrix sizes. Below a table with some results (execution times given in seconds). 

| N     | Initialization        | Multiplication        | Check                 | Opt Multiplication    | Opt Check             |
|---| --- | ---| ---| ---|---|
| 30    | $2.22 \times 10^{-2}$ | $8.30 \times 10^{-5}$ | $5.10 \times 10^{-5}$ | $8.30 \times 10^{-5}$ | $4.10 \times 10^{-5}$ |
| 1000  | $8.92 \times 10^{-3}$ | $2.98$                | $2.00 $               | $2.93$                | $3.35 \times 10^{-2}$ |
| 5000  | $1.57 \times 10^{-1}$ | $4.49 \times 10^2$    | $3.24 \times 10^2$    | $3.68 \times 10^2$    | $8.15 \times 10^{-1}$ |
