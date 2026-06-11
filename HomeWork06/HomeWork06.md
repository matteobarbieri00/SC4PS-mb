# HomeWork06

This version of `sum2vec` reads all runtime parameters from a text config file and stores the chunked vectors plus partial sums in an HDF5 file.

## Implementation summary

The implementation in `sum2vec.c` is organized into small helper functions.

- `trim`: removes leading and trailing spaces from a string read from the config file.
- `parse_double_value`: converts a string into a `double` and checks that the input is valid.
- `parse_int_value`: converts a string into a positive integer and validates the result.
- `parse_int_list`: reads the list of vector sizes from the config file.
- `init_config`: initializes the `Config` structure and sets the default HDF5 output file name.
- `parse_config_file`: reads the text configuration file, extracts all parameters, and checks that required values are present.
- `compute_chunk_size`: returns the chunk size to use for a given vector length.
- `compute_num_chunks`: computes the number of chunks from the vector size and chunk size.
- `test_ok`: verifies that the computed vector satisfies `d[i] = a * x[i] + y[i]` within a numerical tolerance.
- `fill_vector`: fills a vector with a constant value.
- `sum2vec_reference`: computes the full reference result without chunking.
- `sum2vec_chunk_accumulate`: performs the chunked computation, stores each chunk in auxiliary arrays, computes the partial sum of each chunk, and returns the total accumulated sum.
- `write_double_dataset_1d` and `write_int_dataset_1d`: write one-dimensional HDF5 datasets.
- `write_double_dataset_2d`: writes the chunk matrices as two-dimensional HDF5 datasets.
- `write_int_attribute` and `write_double_attribute`: store scalar metadata such as `N`, `chunk_size`, `a`, `x`, and `y` as HDF5 attributes.
- `write_run_to_hdf5`: creates one HDF5 group for each tested vector size and stores vectors, chunks, partial sums, and metadata.
- `allocate_vector` and `allocate_int_vector`: allocate heap memory for the arrays used in the computation.
- `main`: loads the config file, allocates memory, runs the reference and chunked computations for each vector size, checks correctness, and saves the results in the HDF5 output file.

## Config format

Each line must follow:

```text
Variable = Value
```

Supported keys are:

```text
a = 3.0
x = 5.0
y = 7.0
vector_sizes = 10, 100, 1000
chunk_size = 8
output_file = sum2vec_output.h5
```

Instead of `chunk_size`, you can provide:

```text
num_chunks = 4
```

In that case, the program computes the chunk size for each vector length.

## Build
The current `Makefile` works on cloud veneto. If I wish to run the code on mac I need to replace the lines:
- `CC ?= gcc` with  `H5CC ?= /opt/homebrew/bin/h5cc`;
- `LDLIBS ?= -lm -lhdf5` with `LDLIBS ?= -lm`;
- `$(CC) $(CFLAGS) $< -o $@ $(LDLIBS)` with `$(H5CC) $(CFLAGS) $< -o $@ $(LDLIBS)`

Once edited the `Makefile` I just need torun 

```sh
make
```

and then

```sh
make run
```

The HDF5 file stores, for each configured vector size:

- the full vectors `X`, `Y`, `D_reference`, `D_chunked`
- the chunked matrices `X_chunks`, `Y_chunks`, `D_chunks`
- the `partial_chunk_sums`
- the `chunk_lengths`
