#ifndef SORTING_H
#define SORTING_H

#include <stdio.h>  // printf
#include <stdlib.h> // malloc, free
#include <string.h> // strcpy
#include <omp.h>    // OpenMP for parallelism

// Adjustable threshold for switching to sequential sorting in parallel algorithms
// Tuning parameter to reduce overhead of task creation and synchronization in 
// divide-and-conquer parallel algorithms. It is recommended to experiment with
// different values (e.g., 512, 1024, 2048) based on the system and input size.
#define SEQ_THRESHOLD 1024

/*************************************
 *   SEQUENTIAL SORTING ALGORITHMS   *
 *************************************/

// ----    COMPARATIVE  ALGORITHMS    -----
// Odd-Even Sort O(n²)
void odd_even_sort_s(int arr[], int n);
// Shell Sort O(n log² n)
void shell_sort_s(int arr[], int n);

// ----  NON-COMPARATIVE  ALGORITHMS  -----
// Counting Sort O(n + k)
void counting_sort_s(int arr[], int n);

// ---- DIVIDE AND CONQUER ALGORITHMS -----  
// Merge Sort O(n logn)
void merge_sort_s(int arr[], int temp[], int l, int r);
void merge_s(int arr[], int temp[], int l, int m, int r);
// Quick Sort O(n logn)
void quick_sort_s(int arr[], int low, int high);
int partition(int arr[], int low, int high);

// ----    NET-ORDERING ALGORITHMS    -----
// Bitonic Sort O(n log²n)
void bitonic_sort_s(int arr[], int low, int cnt, int dir);

/*************************************
 *    PARALLEL SORTING ALGORITHMS    *
 *************************************/
 
// ----    COMPARATIVE  ALGORITHMS    -----
// Odd-Even Sort O()
void odd_even_sort_p(int arr[], int n, int t);
// Shell Sort O()
void shell_sort_p(int arr[], int n, int t);

// ----  NON-COMPARATIVE  ALGORITHMS  -----
// Counting Sort O()
void counting_sort_p(int arr[], int n, int t);

// ---- DIVIDE AND CONQUER ALGORITHMS -----  
// Merge Sort O()
void call_merge_sort_p(int arr[], int n, int t);
void merge_sort_p(int arr[], int temp[], int l, int r);
// Quick Sort O()
void call_quick_sort_p(int arr[], int n, int t);
void quick_sort_p(int arr[], int low, int high, int t);

// ----    NET-ORDERING ALGORITHMS    -----
// Bitonic Sort O()
void bitonic_merge_p(int arr[], int low, int cnt, int dir, int t);
void bitonic_sort_p(int arr[], int low, int cnt, int dir, int t);

/*************************************
 *      MISCELLANEOUS FUNCTIONS      *
 *************************************/

void populate_array(int arr[], int n);
void swap(int *a, int *b);
void print_stats(const char *sort_name, int n, int t, double times[]);
void testing_unit();

#endif // SORTING_H