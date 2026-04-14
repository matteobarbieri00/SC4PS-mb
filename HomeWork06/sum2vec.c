#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <hdf5.h>

#define MAX_LINE_LENGTH 512
#define MAX_PATH_LENGTH 256
#define MAX_VECTOR_SIZES 32

typedef struct {
	double a;
	double x;
	double y;
	int vector_sizes[MAX_VECTOR_SIZES];
	int vector_sizes_count;
	int chunk_size;
	int num_chunks;
	char output_file[MAX_PATH_LENGTH];
} Config;

static char *trim(char *text)
{
	char *start = text;
	char *end = NULL;

	while (*start != '\0' && isspace((unsigned char)*start)) {
		start++;
	}

	if (*start == '\0') {
		return start;
	}

	end = start + strlen(start) - 1;
	while (end > start && isspace((unsigned char)*end)) {
		*end = '\0';
		end--;
	}

	return start;
}

static bool parse_double_value(const char *text, double *value)
{
	char *end_ptr = NULL;

	errno = 0;
	*value = strtod(text, &end_ptr);
	if (errno != 0 || end_ptr == text) {
		return false;
	}

	while (*end_ptr != '\0') {
		if (!isspace((unsigned char)*end_ptr)) {
			return false;
		}
		end_ptr++;
	}

	return true;
}

static bool parse_int_value(const char *text, int *value)
{
	char *end_ptr = NULL;
	long parsed_value = 0;

	errno = 0;
	parsed_value = strtol(text, &end_ptr, 10);
	if (errno != 0 || end_ptr == text || parsed_value <= 0 || parsed_value > INT_MAX) {
		return false;
	}

	while (*end_ptr != '\0') {
		if (!isspace((unsigned char)*end_ptr)) {
			return false;
		}
		end_ptr++;
	}

	*value = (int)parsed_value;
	return true;
}

static bool parse_int_list(char *text, int *values, int *count)
{
	char *token = strtok(text, ",");
	int parsed_count = 0;

	while (token != NULL) {
		int parsed_value = 0;
		char *entry = trim(token);

		if (parsed_count >= MAX_VECTOR_SIZES || !parse_int_value(entry, &parsed_value)) {
			return false;
		}

		values[parsed_count] = parsed_value;
		parsed_count++;
		token = strtok(NULL, ",");
	}

	if (parsed_count == 0) {
		return false;
	}

	*count = parsed_count;
	return true;
}

static void init_config(Config *config)
{
	memset(config, 0, sizeof(*config));
	strncpy(config->output_file, "sum2vec_output.h5", sizeof(config->output_file) - 1);
}

static bool parse_config_file(const char *config_path, Config *config)
{
	FILE *config_file = fopen(config_path, "r");
	char line[MAX_LINE_LENGTH];
	bool found_a = false;
	bool found_x = false;
	bool found_y = false;
	bool found_sizes = false;

	if (config_file == NULL) {
		fprintf(stderr, "Cannot open config file: %s\n", config_path);
		return false;
	}

	while (fgets(line, sizeof(line), config_file) != NULL) {
		char *separator = NULL;
		char *key = NULL;
		char *value = NULL;
		char *comment = strchr(line, '#');

		if (comment != NULL) {
			*comment = '\0';
		}

		key = trim(line);
		if (*key == '\0') {
			continue;
		}

		separator = strchr(key, '=');
		if (separator == NULL) {
			fprintf(stderr, "Invalid config line: %s\n", key);
			fclose(config_file);
			return false;
		}

		*separator = '\0';
		value = trim(separator + 1);
		key = trim(key);

		if (strcmp(key, "a") == 0) {
			found_a = parse_double_value(value, &config->a);
		} else if (strcmp(key, "x") == 0) {
			found_x = parse_double_value(value, &config->x);
		} else if (strcmp(key, "y") == 0) {
			found_y = parse_double_value(value, &config->y);
		} else if (strcmp(key, "vector_sizes") == 0 || strcmp(key, "N") == 0) {
			found_sizes = parse_int_list(value, config->vector_sizes, &config->vector_sizes_count);
		} else if (strcmp(key, "chunk_size") == 0) {
			if (!parse_int_value(value, &config->chunk_size)) {
				fprintf(stderr, "Invalid chunk_size value\n");
				fclose(config_file);
				return false;
			}
		} else if (strcmp(key, "num_chunks") == 0) {
			if (!parse_int_value(value, &config->num_chunks)) {
				fprintf(stderr, "Invalid num_chunks value\n");
				fclose(config_file);
				return false;
			}
		} else if (strcmp(key, "output_file") == 0) {
			if (*value == '\0') {
				fprintf(stderr, "output_file cannot be empty\n");
				fclose(config_file);
				return false;
			}
			strncpy(config->output_file, value, sizeof(config->output_file) - 1);
			config->output_file[sizeof(config->output_file) - 1] = '\0';
		} else {
			fprintf(stderr, "Unknown config key: %s\n", key);
			fclose(config_file);
			return false;
		}

		if ((strcmp(key, "a") == 0 && !found_a) ||
		    (strcmp(key, "x") == 0 && !found_x) ||
		    (strcmp(key, "y") == 0 && !found_y) ||
		    ((strcmp(key, "vector_sizes") == 0 || strcmp(key, "N") == 0) && !found_sizes)) {
			fprintf(stderr, "Invalid value for key: %s\n", key);
			fclose(config_file);
			return false;
		}
	}

	fclose(config_file);

	if (!found_a || !found_x || !found_y || !found_sizes) {
		fprintf(stderr, "Config file must define a, x, y and vector_sizes\n");
		return false;
	}

	if (config->chunk_size <= 0 && config->num_chunks <= 0) {
		fprintf(stderr, "Config file must define chunk_size or num_chunks\n");
		return false;
	}

	return true;
}

static int compute_chunk_size(const Config *config, int vector_size)
{
	if (config->chunk_size > 0) {
		return config->chunk_size;
	}

	return (vector_size + config->num_chunks - 1) / config->num_chunks;
}

static int compute_num_chunks(int vector_size, int chunk_size)
{
	return (vector_size + chunk_size - 1) / chunk_size;
}

static bool test_ok(const double *d, const double *x, const double *y, double a, int n)
{
	const double tolerance = 1e-12;

	for (int index = 0; index < n; index++) {
		double expected_value = a * x[index] + y[index];

		if (fabs(d[index] - expected_value) > tolerance) {
			fprintf(stderr, "Component %d of vector d is not correct\n", index);
			return false;
		}
	}

	return true;
}

static void fill_vector(double *vector, int size, double value)
{
	for (int index = 0; index < size; index++) {
		vector[index] = value;
	}
}

static void sum2vec_reference(double *d, const double *x, const double *y, double a, int n)
{
	for (int index = 0; index < n; index++) {
		d[index] = a * x[index] + y[index];
	}
}

static double sum2vec_chunk_accumulate(
	double *d,
	const double *x,
	const double *y,
	double *x_chunks,
	double *y_chunks,
	double *d_chunks,
	double *partial_chunk_sums,
	int *chunk_lengths,
	double a,
	int n,
	int chunk_size,
	int num_chunks)
{
	double total_sum = 0.0;

	for (int chunk_index = 0; chunk_index < num_chunks; chunk_index++) {
		int start = chunk_index * chunk_size;
		int end = start + chunk_size;
		double accumulation_in_chunk = 0.0;

		if (end > n) {
			end = n;
		}

		chunk_lengths[chunk_index] = end - start;
		for (int offset = 0; offset < chunk_size; offset++) {
			int flat_index = chunk_index * chunk_size + offset;

			x_chunks[flat_index] = NAN;
			y_chunks[flat_index] = NAN;
			d_chunks[flat_index] = NAN;
		}

		for (int element_index = start; element_index < end; element_index++) {
			int offset = element_index - start;
			int flat_index = chunk_index * chunk_size + offset;

			d[element_index] = a * x[element_index] + y[element_index];
			x_chunks[flat_index] = x[element_index];
			y_chunks[flat_index] = y[element_index];
			d_chunks[flat_index] = d[element_index];
			accumulation_in_chunk += d[element_index];
		}

		partial_chunk_sums[chunk_index] = accumulation_in_chunk;
		total_sum += accumulation_in_chunk;
	}

	return total_sum;
}

static int write_double_dataset_1d(hid_t parent, const char *name, const double *data, hsize_t size)
{
	hsize_t dims[1] = {size};
	hid_t dataspace = H5Screate_simple(1, dims, NULL);
	hid_t dataset = -1;
	int status = -1;

	if (dataspace < 0) {
		return -1;
	}

	dataset = H5Dcreate2(parent, name, H5T_NATIVE_DOUBLE, dataspace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
	if (dataset >= 0) {
		status = H5Dwrite(dataset, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, data);
		H5Dclose(dataset);
	}

	H5Sclose(dataspace);
	return status;
}

static int write_int_dataset_1d(hid_t parent, const char *name, const int *data, hsize_t size)
{
	hsize_t dims[1] = {size};
	hid_t dataspace = H5Screate_simple(1, dims, NULL);
	hid_t dataset = -1;
	int status = -1;

	if (dataspace < 0) {
		return -1;
	}

	dataset = H5Dcreate2(parent, name, H5T_NATIVE_INT, dataspace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
	if (dataset >= 0) {
		status = H5Dwrite(dataset, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, data);
		H5Dclose(dataset);
	}

	H5Sclose(dataspace);
	return status;
}

static int write_double_dataset_2d(hid_t parent, const char *name, const double *data, hsize_t rows, hsize_t cols)
{
	hsize_t dims[2] = {rows, cols};
	hid_t dataspace = H5Screate_simple(2, dims, NULL);
	hid_t dataset = -1;
	int status = -1;

	if (dataspace < 0) {
		return -1;
	}

	dataset = H5Dcreate2(parent, name, H5T_NATIVE_DOUBLE, dataspace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
	if (dataset >= 0) {
		status = H5Dwrite(dataset, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, data);
		H5Dclose(dataset);
	}

	H5Sclose(dataspace);
	return status;
}

static int write_int_attribute(hid_t object, const char *name, int value)
{
	hsize_t dims[1] = {1};
	hid_t dataspace = H5Screate_simple(1, dims, NULL);
	hid_t attribute = -1;
	int status = -1;

	if (dataspace < 0) {
		return -1;
	}

	attribute = H5Acreate2(object, name, H5T_NATIVE_INT, dataspace, H5P_DEFAULT, H5P_DEFAULT);
	if (attribute >= 0) {
		status = H5Awrite(attribute, H5T_NATIVE_INT, &value);
		H5Aclose(attribute);
	}

	H5Sclose(dataspace);
	return status;
}

static int write_double_attribute(hid_t object, const char *name, double value)
{
	hsize_t dims[1] = {1};
	hid_t dataspace = H5Screate_simple(1, dims, NULL);
	hid_t attribute = -1;
	int status = -1;

	if (dataspace < 0) {
		return -1;
	}

	attribute = H5Acreate2(object, name, H5T_NATIVE_DOUBLE, dataspace, H5P_DEFAULT, H5P_DEFAULT);
	if (attribute >= 0) {
		status = H5Awrite(attribute, H5T_NATIVE_DOUBLE, &value);
		H5Aclose(attribute);
	}

	H5Sclose(dataspace);
	return status;
}

static bool write_run_to_hdf5(
	hid_t file_id,
	int run_index,
	int n,
	int chunk_size,
	int num_chunks,
	const Config *config,
	const double *x,
	const double *y,
	const double *d_reference,
	const double *d_chunked,
	const double *x_chunks,
	const double *y_chunks,
	const double *d_chunks,
	const double *partial_chunk_sums,
	const int *chunk_lengths)
{
	char group_name[64];
	hid_t group_id = -1;
	int status = 0;

	snprintf(group_name, sizeof(group_name), "run_%02d_n_%d", run_index, n);
	group_id = H5Gcreate2(file_id, group_name, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
	if (group_id < 0) {
		return false;
	}

	status |= write_int_attribute(group_id, "vector_size", n);
	status |= write_int_attribute(group_id, "chunk_size", chunk_size);
	status |= write_int_attribute(group_id, "num_chunks", num_chunks);
	status |= write_double_attribute(group_id, "a", config->a);
	status |= write_double_attribute(group_id, "x", config->x);
	status |= write_double_attribute(group_id, "y", config->y);

	status |= write_double_dataset_1d(group_id, "X", x, (hsize_t)n);
	status |= write_double_dataset_1d(group_id, "Y", y, (hsize_t)n);
	status |= write_double_dataset_1d(group_id, "D_reference", d_reference, (hsize_t)n);
	status |= write_double_dataset_1d(group_id, "D_chunked", d_chunked, (hsize_t)n);
	status |= write_double_dataset_2d(group_id, "X_chunks", x_chunks, (hsize_t)num_chunks, (hsize_t)chunk_size);
	status |= write_double_dataset_2d(group_id, "Y_chunks", y_chunks, (hsize_t)num_chunks, (hsize_t)chunk_size);
	status |= write_double_dataset_2d(group_id, "D_chunks", d_chunks, (hsize_t)num_chunks, (hsize_t)chunk_size);
	status |= write_double_dataset_1d(group_id, "partial_chunk_sums", partial_chunk_sums, (hsize_t)num_chunks);
	status |= write_int_dataset_1d(group_id, "chunk_lengths", chunk_lengths, (hsize_t)num_chunks);

	H5Gclose(group_id);
	return status >= 0;
}

static double *allocate_vector(int size)
{
	return malloc((size_t)size * sizeof(double));
}

static int *allocate_int_vector(int size)
{
	return malloc((size_t)size * sizeof(int));
}

int main(int argc, char *argv[])
{
	Config config;
	hid_t hdf5_file = -1;

	if (argc != 2) {
		fprintf(stderr, "Usage: %s config.txt\n", argv[0]);
		return 1;
	}

	init_config(&config);
	if (!parse_config_file(argv[1], &config)) {
		return 1;
	}

	hdf5_file = H5Fcreate(config.output_file, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
	if (hdf5_file < 0) {
		fprintf(stderr, "Cannot create HDF5 file: %s\n", config.output_file);
		return 1;
	}

	for (int run_index = 0; run_index < config.vector_sizes_count; run_index++) {
		int n = config.vector_sizes[run_index];
		int chunk_size = compute_chunk_size(&config, n);
		int num_chunks = compute_num_chunks(n, chunk_size);
		double *x = allocate_vector(n);
		double *y = allocate_vector(n);
		double *d_reference = allocate_vector(n);
		double *d_chunked = allocate_vector(n);
		double *x_chunks = allocate_vector(num_chunks * chunk_size);
		double *y_chunks = allocate_vector(num_chunks * chunk_size);
		double *d_chunks = allocate_vector(num_chunks * chunk_size);
		double *partial_chunk_sums = allocate_vector(num_chunks);
		int *chunk_lengths = allocate_int_vector(num_chunks);
		double expected_sum = 0.0;
		double accumulated_sum = 0.0;

		if (x == NULL || y == NULL || d_reference == NULL || d_chunked == NULL ||
		    x_chunks == NULL || y_chunks == NULL || d_chunks == NULL ||
		    partial_chunk_sums == NULL || chunk_lengths == NULL) {
			fprintf(stderr, "Memory allocation failed for vector size %d\n", n);
			free(x);
			free(y);
			free(d_reference);
			free(d_chunked);
			free(x_chunks);
			free(y_chunks);
			free(d_chunks);
			free(partial_chunk_sums);
			free(chunk_lengths);
			H5Fclose(hdf5_file);
			return 1;
		}

		fill_vector(x, n, config.x);
		fill_vector(y, n, config.y);
		sum2vec_reference(d_reference, x, y, config.a, n);
		accumulated_sum = sum2vec_chunk_accumulate(
			d_chunked,
			x,
			y,
			x_chunks,
			y_chunks,
			d_chunks,
			partial_chunk_sums,
			chunk_lengths,
			config.a,
			n,
			chunk_size,
			num_chunks);

		for (int index = 0; index < n; index++) {
			expected_sum += d_reference[index];
		}

		if (!test_ok(d_chunked, x, y, config.a, n)) {
			fprintf(stderr, "Chunked computation failed for N = %d\n", n);
			free(x);
			free(y);
			free(d_reference);
			free(d_chunked);
			free(x_chunks);
			free(y_chunks);
			free(d_chunks);
			free(partial_chunk_sums);
			free(chunk_lengths);
			H5Fclose(hdf5_file);
			return 1;
		}

		if (fabs(accumulated_sum - expected_sum) > 1e-12) {
			fprintf(stderr, "Accumulated chunk sum failed for N = %d\n", n);
			free(x);
			free(y);
			free(d_reference);
			free(d_chunked);
			free(x_chunks);
			free(y_chunks);
			free(d_chunks);
			free(partial_chunk_sums);
			free(chunk_lengths);
			H5Fclose(hdf5_file);
			return 1;
		}

		if (!write_run_to_hdf5(
			hdf5_file,
			run_index,
			n,
			chunk_size,
			num_chunks,
			&config,
			x,
			y,
			d_reference,
			d_chunked,
			x_chunks,
			y_chunks,
			d_chunks,
			partial_chunk_sums,
			chunk_lengths)) {
			fprintf(stderr, "Failed to write HDF5 datasets for N = %d\n", n);
			free(x);
			free(y);
			free(d_reference);
			free(d_chunked);
			free(x_chunks);
			free(y_chunks);
			free(d_chunks);
			free(partial_chunk_sums);
			free(chunk_lengths);
			H5Fclose(hdf5_file);
			return 1;
		}

		printf(
			"N = %d, chunk_size = %d, num_chunks = %d, sum = %.12f\n",
			n,
			chunk_size,
			num_chunks,
			accumulated_sum);

		free(x);
		free(y);
		free(d_reference);
		free(d_chunked);
		free(x_chunks);
		free(y_chunks);
		free(d_chunks);
		free(partial_chunk_sums);
		free(chunk_lengths);
	}

	H5Fclose(hdf5_file);
	printf("Saved chunk data and partial sums to %s\n", config.output_file);
	return 0;
}
