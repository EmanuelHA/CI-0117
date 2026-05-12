#ifndef PARAMS_H
#define PARAMS_H

// Centralize all configuration parameters for the arrays and threads.
typedef struct {
    int arr_length;
    int num_threads;
} params_t;

// Parses command-line arguments and populates the execution parameters structure.
void parse_arguments(int argc, char *argv[], params_t *params);

#endif // PARAMS_H