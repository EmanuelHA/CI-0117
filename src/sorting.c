#include "../include/sorting.h"
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <stdbool.h>

/*************************************
 *   SEQUENTIAL SORTING ALGORITHMS   *
 *************************************/

// ----    COMPARATIVE  ALGORITHMS    -----
void odd_even_sort_s(int arr[], int n) {
    int is_ordered = 0;
    // Keep iterating until the array is ordered (Optimization: if no swaps were made in a complete pass, the array is ordered)
    while (!is_ordered) {
        is_ordered = 1;
        // Compare all odd indexed elements with their next even indexed element and swap if they are in the wrong order
        for (int i = 1; i <= n - 2; i += 2) {
            if (arr[i] > arr[i + 1]) {
                int temp = arr[i];
                arr[i] = arr[i + 1];
                arr[i + 1] = temp;
                is_ordered = 0;
            }
        }
        // Compare all even indexed elements with their next odd indexed element and swap if they are in the wrong order
        for (int i = 0; i <= n - 2; i += 2) {
            if (arr[i] > arr[i + 1]) {
                int temp = arr[i];
                arr[i] = arr[i + 1];
                arr[i + 1] = temp;
                is_ordered = 0;
            }
        }
    }
}

void shell_sort_s(int arr[], int n) {
    // Start with a N/2 - default shell-sort [not optimized (like Sedgewick or Knuth)] gap, then reduce the gap
    for (int gap = n / 2; gap > 0; gap /= 2) {
        // Make a "insertion sort" for the elements at the current gap
        for (int i = gap; i < n; i++) {
            int j, temp = arr[i];
            for (j = i; (j >= gap) && (arr[j - gap] > temp); j -= gap) {
                arr[j] = arr[j - gap];
            }
            // Insert temp (arr[i]) in its correct position
            arr[j] = temp;
        }
    }
}

// ----  NON-COMPARATIVE  ALGORITHMS  -----
void counting_sort_s(int arr[], int n) {
    int k = 0; 
    // k is the max. value in arr[] (assuming all values are non-negative for mental peace)
    for (int i = 0; i < n; i++) {
        if (arr[i] > k) {
            k = arr[i];
        }
    }
    // Reserve memory for the count and the output arrays
    int *count = (int *)calloc(k + 1, sizeof(int));
    int *output = (int *)malloc(n * sizeof(int));
    // Count the occurrences of each unique object in the input array (counting phase)
    for (int i = 0; i < n; i++) {
        count[arr[i]]++;
    }
    // Sum up the counts to get the cumulative count (prefix sum phase)
    for (int i = 1; i <= k; i++) {
        count[i] += count[i - 1];
    }
    // Colocate the elements in the output array based on the cumulative count (placement phase)
    for (int i = n - 1; i >= 0; i--) {
        output[count[arr[i]] - 1] = arr[i];
        count[arr[i]]--;
    }
    // Copy the sorted output back to arr[] (copy back / mem bulk phase)
    // Using memcpy for bulk copy (Optimization with AVX2 and other SIMD instructions)
    memcpy(arr, output, n * sizeof(int));

    free(count);
    free(output);
}

// ---- DIVIDE AND CONQUER ALGORITHMS -----  
void merge_sort_s(int arr[], int temp[], int l, int r) {
    // If left < right, we can split the array into halves, we can merge them in sorted order
    if (l < r) {
        // Find the middle point to divide the array into two halves
        int m = l + (r - l) / 2;
        // Call merge_sort_s for first half and second half
        merge_sort_s(arr, temp, l, m);
        merge_sort_s(arr, temp, m + 1, r);
        // Merge the sorted halves
        merge_s(arr, temp, l, m, r);
    }
}

void merge_s(int arr[], int temp[], int l, int m, int r) {
    // Set the indices of the two subarrays to be merged
    int i = l;
    int j = m + 1;
    int k = l;
    // Merge the two halves [(l, m) and (m + 1, r)] into temp[]
    while (i <= m && j <= r) {
        if (arr[i] <= arr[j]) {
            temp[k] = arr[i];
            i++;
        } else {
            temp[k] = arr[j];
            j++;
        }
        k++;
    }
    // Copy the remaining elements of left half, if there are any
    while (i <= m) {
        temp[k] = arr[i];
        i++;
        k++;
    }
    // Copy the remaining elements of right half, if there are any
    while (j <= r) {
        temp[k] = arr[j];
        j++;
        k++;
    }
    // Using memcpy for bulk copy (Optimization with AVX2 and other SIMD instructions)
    memcpy(&arr[l], &temp[l], (r - l + 1) * sizeof(int));
}

void quick_sort_s(int arr[], int low, int high) {
    if (low < high) {
        // Search and adjust the pivot element
        int pivot = partition(arr, low, high);

        // Recursively sort elements before and after the partition
        quick_sort_s(arr, low, pivot - 1);
        quick_sort_s(arr, pivot + 1, high);
    }
}

// Partition function for Quick Sort using Lomuto partition scheme (simple)
// Use Hoare's partition scheme for better performance in some cases (duplicates, already sorted, etc.)
int partition(int arr[], int low, int high) {
    int pivot = arr[high]; 
    int i = (low - 1); // Index of smaller element
    for (int j = low; j < high; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

// ----    NET-ORDERING ALGORITHMS    -----
void bitonic_sort_s(int arr[], int low, int cnt, int dir) {

}

/*************************************
 *    PARALLEL SORTING ALGORITHMS    *
 *************************************/
 
// ----    COMPARATIVE  ALGORITHMS    -----
void odd_even_sort_p(int arr[], int n, int t) {
    int phase, i, temp;
    // Parallel region with shared access to arr and n, and private copies of i, temp, and phase for each thread
    // to avoid open and close the parallel region in each iteration of the outer loop (OPTIMIZATION)
    // NOTE: sync is implied at the end of each #pragma omp for.
    #pragma omp parallel default(none) shared(arr, n) private(i, temp, phase) num_threads(t)
    {
        for (phase = 0; phase < n; phase++) {
            
            if (phase % 2 != 0) {    // Odd phase
                #pragma omp for
                for (i = 1; i <= n - 2; i += 2) {
                    if (arr[i] > arr[i + 1]) {
                        swap(&arr[i], &arr[i + 1]);
                    }
                }
            } else {                // Even phase
                #pragma omp for
                for (i = 0; i <= n - 2; i += 2) {
                    if (arr[i] > arr[i + 1]) {
                        swap(&arr[i], &arr[i + 1]);
                    }
                }
            }
        }
    }
}

void shell_sort_p(int arr[], int n, int t) {
    int gap, i, j, k, temp;
    // The external loop needs to be sequential to ensure the correct order of gaps
    for (gap = n / 2; gap > 0; gap /= 2) {
        // There are gap independent sub-arrays to be sorted, so we can parallelize the inner loop (OPTIMIZATION AND NO RC)
        #pragma omp parallel for private(j, k, temp) shared(arr, n, gap)
        for (i = 0; i < gap; i++) {
            // Each thread will perform an insertion sort on its assigned sub-array (starting at index i and jumping by gap)
            for (j = i + gap; j < n; j += gap) {
                temp = arr[j];
                k = j;
                
                // Look at the correct pos. in the sorted sub-array and insert temp there
                while (k >= gap && arr[k - gap] > temp) {
                    arr[k] = arr[k - gap];
                    k -= gap;
                }
                arr[k] = temp;
            }
        }
    }
}

// ----  NON-COMPARATIVE  ALGORITHMS  -----
void counting_sort_p(int arr[], int n, int t) {
    int k = 0;
     // Reduction to find the maximum value in arr[] in parallel
    #pragma omp parallel for reduction(max:k)
    for (int i = 0; i < n; i++) {
        if (arr[i] > k) {
            k = arr[i];
        }
    }
    int *count = (int *)calloc(k + 1, sizeof(int));
    int *output = (int *)malloc(n * sizeof(int));
    // When reduction(+:count[0:k+1]) is enabled, OpenMP creates a private copy of the count
    // array for each thread, initialized to zero. Each thread updates its private count array
    // independently. At the end of the parallel region, OpenMP automatically sums all the
    // private count arrays into the original count array to ensure that the final count
    // reflects the total occurrences from all threads without avoiding race conditions.
    #pragma omp parallel for reduction(+:count[0:k+1])    // Parallel counting (OpenMP 4.5+ feature)
    for (int i = 0; i < n; i++) {
        count[arr[i]]++;
    }
    // Sum of prefixes
    for (int i = 1; i <= k; i++) {
        count[i] += count[i - 1];
    }

    // Loose stability if we parallelize this loop, so we keep it sequential (OPTIMIZATION)
    for (int i = n - 1; i >= 0; i--) {
        output[count[arr[i]] - 1] = arr[i];
        count[arr[i]]--;
    }

    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        arr[i] = output[i];
    }

    free(count);
    free(output);
}

// ---- DIVIDE AND CONQUER ALGORITHMS -----
void call_merge_sort_p(int arr[], int n, int t) {
    int *temp = (int *)malloc(n * sizeof(int));
    // Parallel region with t threads
    #pragma omp parallel num_threads(t)
    {
        // Single thread initiates the sorting (single) and allows others to work on tasks (nowait)
        #pragma omp single nowait
        {
            merge_sort_p(arr, temp, 0, n - 1);
        }
    }
    
    free(temp);
}

void merge_sort_p(int arr[], int temp[], int l, int r) {
    if (l < r) {
        // If the size of the subarray is below the threshold, perform sequential merge sort (optimization)
        if ((r - l) < SEQ_THRESHOLD) {
            merge_sort_s(arr, temp, l, r); 
            return;
        }

        int m = l + (r - l) / 2;
        // Create a task for the left half with shared access to arr and temp, and private copies of l and m
        #pragma omp task shared(arr, temp) firstprivate(l, m)
        {
            merge_sort_p(arr, temp, l, m);
        }
        // Create a task for the right half with *shared access to arr and temp, and *private copies of m and r
        #pragma omp task shared(arr, temp) firstprivate(m, r)
        {
            merge_sort_p(arr, temp, m + 1, r);
        }
        // Wait for tasks to complete before merging
        #pragma omp taskwait
        // Merge the sorted halves
        merge_s(arr, temp, l, m, r);
    }
}

void call_quick_sort_p(int arr[], int n, int t) {
    int *temp = (int *)malloc(n * sizeof(int));

    #pragma omp parallel num_threads(t) // Parallel region with t threads
    {
        #pragma omp single nowait       // Single thread initiates the sorting (single) and allows others to work on tasks (nowait)
        {
            merge_sort_p(arr, temp, 0, n - 1);
        }
    }
    
    free(temp);
}

void quick_sort_p(int arr[], int low, int high, int t) {
    if (low < high) {
        // Treshold optimization to avoid excessive task creation for small subarrays
        if ((high - low) < SEQ_THRESHOLD) {
            quick_sort_s(arr, low, high);
            return;
        }
        // Sequential partitioning (the partition function is not thread-safe, so we do it sequentially)
        int pi = partition(arr, low, high);
        // Create tasks for the two halves of the array with shared access to arr and private copies of low, pi and high
        #pragma omp task shared(arr) firstprivate(low, pi)
        {
            quick_sort_p(arr, low, pi - 1, t);
        }

        #pragma omp task shared(arr) firstprivate(pi, high)
        {
            quick_sort_p(arr, pi + 1, high, t);
        }
        // Synchronize tasks to ensure both halves are sorted before returning
        #pragma omp taskwait
    }
}

// ----    NET-ORDERING ALGORITHMS    -----
void bitonic_sort_p(int arr[], int low, int cnt, int dir, int t) {

}

/*************************************
 *      MISCELLANEOUS FUNCTIONS      *
 *************************************/

void populate_array(int arr[], int n) {
    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 10000;
    }
}

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Helper function to calculate average time and print the results
void print_stats(const char* name, int n, int t, double times[5]) {
    double sum = 0.0;
    for(int i = 0; i < 5; i++) {
        sum += times[i];
    }
    
    // Calculate the average execution time
    double avg = sum / 5.0;
    
    // t == 0 indicates a sequential run
    if (t == 0) {
        printf("Alg: %-18s | N: %8d | T: Secuencial | Promedio: %.6f s\n", name, n, avg);
    } else {
        printf("Alg: %-18s | N: %8d | T: %10d | Promedio: %.6f s\n", name, n, t, avg);
    }
}

void testing_unit() {
    printf("==============================================================\n");
    printf("         INICIANDO TESTING UNIT DE ORDENAMIENTO               \n");
    printf("==============================================================\n\n");

    // Array sizes for O(n^2) algorithms (limited to 100K to prevent excessive execution time)
    int sizes_O2[] = {1000, 10000, 100000};
    int num_sizes_O2 = 3;
    
    // Array sizes for O(n log n) algorithms
    int sizes_Ologn[] = {1000, 10000, 100000, 1000000, 10000000}; 
    int num_sizes_Ologn = 5;
    
    // Array sizes strictly bounded to powers of 2 (required for Bitonic Sort)
    int sizes_bitonic[] = {1024, 16384, 131072, 1048576, 8388608};
    int num_sizes_bitonic = 5;

    // Thread configurations to evaluate
    int threads_config[] = {1, 2, 4, 8, 12};
    int num_threads_config = 5;

    // Global memory allocation to avoid malloc/free overhead during benchmarks
    int max_N = 10000000;
    int *arr = (int *)malloc(max_N * sizeof(int));
    int *temp = (int *)malloc(max_N * sizeof(int)); // Temporary array needed for Merge Sort

    if (arr == NULL || temp == NULL) {
        printf("Error de memoria. No se pudo reservar los arreglos.\n");
        return;
    }

    // Temporary array to store execution times for each iteration
    double times[5]; 

    // =========================================================
    // 1. SEQUENTIAL TESTS
    // =========================================================
    printf("\n--- SECUENCIALES ---\n");

    // SEQUENTIAL ODD-EVEN SORT
    for (int s = 0; s < num_sizes_O2; s++) {
        int n = sizes_O2[s];
        for (int iter = 0; iter < 5; iter++) {
            populate_array(arr, n);
            double start = omp_get_wtime();
            odd_even_sort_s(arr, n);
            double end = omp_get_wtime();
            times[iter] = end - start;
        }
        print_stats("Odd-Even Sec", n, 0, times);
    }

    // SEQUENTIAL MERGE SORT
    for (int s = 0; s < num_sizes_Ologn; s++) {
        int n = sizes_Ologn[s];
        for (int iter = 0; iter < 5; iter++) {
            populate_array(arr, n);
            double start = omp_get_wtime();
            merge_sort_s(arr, temp, 0, n - 1);
            double end = omp_get_wtime();
            times[iter] = end - start;
        }
        print_stats("Merge Sort Sec", n, 0, times);
    }

    // SEQUENTIAL BITONIC SORT
    for (int s = 0; s < num_sizes_bitonic; s++) {
        int n = sizes_bitonic[s];
        for (int iter = 0; iter < 5; iter++) {
            populate_array(arr, n);
            double start = omp_get_wtime();
            bitonic_sort_s(arr, 0, n, 1);
            double end = omp_get_wtime();
            times[iter] = end - start;
        }
        print_stats("Bitonic Sort Sec", n, 0, times);
    }

    // =========================================================
    // 2. PARALLEL TESTS
    // =========================================================
    printf("\n--- PARALELOS ---\n");

    // PARALLEL ODD-EVEN SORT
    for (int s = 0; s < num_sizes_O2; s++) {
        int n = sizes_O2[s];
        for(int t_idx = 0; t_idx < num_threads_config; t_idx++) {
            int t = threads_config[t_idx];
            for (int iter = 0; iter < 5; iter++) {
                populate_array(arr, n);
                double start = omp_get_wtime();
                odd_even_sort_p(arr, n, t); // Passing 't' thread count
                double end = omp_get_wtime();
                times[iter] = end - start;
            }
            print_stats("Odd-Even Par", n, t, times);
        }
    }

    // PARALLEL MERGE SORT
    for (int s = 0; s < num_sizes_Ologn; s++) {
        int n = sizes_Ologn[s];
        for(int t_idx = 0; t_idx < num_threads_config; t_idx++) {
            int t = threads_config[t_idx];
            for (int iter = 0; iter < 5; iter++) {
                populate_array(arr, n);
                double start = omp_get_wtime();
                call_merge_sort_p(arr, n, t);
                double end = omp_get_wtime();
                times[iter] = end - start;
            }
            print_stats("Merge Sort Par", n, t, times);
        }
    }

    // PARALLEL BITONIC SORT
    for (int s = 0; s < num_sizes_bitonic; s++) {
        int n = sizes_bitonic[s];
        for(int t_idx = 0; t_idx < num_threads_config; t_idx++) {
            int t = threads_config[t_idx];
            for (int iter = 0; iter < 5; iter++) {
                populate_array(arr, n);
                double start = omp_get_wtime();
                bitonic_sort_p(arr, 0, n, 1, t);
                double end = omp_get_wtime();
                times[iter] = end - start;
            }
            print_stats("Bitonic Sort Par", n, t, times);
        }
    }

    // Free the globally allocated memory blocks
    free(arr);
    free(temp);

    printf("\n==============================================================\n");
    printf("                 TESTING UNIT FINALIZADO                      \n");
    printf("==============================================================\n");
}

int main(int argc, char *argv[]) {
    testing_unit();
    return 0;
}