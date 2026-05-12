#include "../include/params.h"
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>


void parse_arguments(int argc, char *argv[], params_t *params) {
    // Default values
    params->arr_length      = 10000;
    params->num_threads     = 4;
    // Argument parsing
    int opt;
    static struct option long_options[] = {
        {"dna_length", required_argument, 0, 'n'},
        {"patterns",   required_argument, 0, 'k'},
        {"p_length",   required_argument, 0, 'l'},
        {"threads",    required_argument, 0, 't'},
        {0, 0, 0, 0}
    };
    
    while ((opt = getopt_long(argc, argv, "n:k:l:t:", long_options, NULL)) != -1) {
        switch (opt) {
            case 'n':
                params->arr_length = atoi(optarg);
                break;
            case 't':
                params->num_threads = atoi(optarg);
                break;
            case '?':
            default:
                fprintf(stderr, "Usage: %s -n [array length] -t [threads]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    printf("Executing with n=%d, t=%d\n", params->arr_length, params->num_threads);
}