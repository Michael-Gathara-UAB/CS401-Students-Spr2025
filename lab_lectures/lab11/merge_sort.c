/**
 MacOS runtime: clang -Xclang -fopenmp -L/opt/homebrew/opt/libomp/lib -I/opt/homebrew/opt/libomp/include -lomp merge_sort.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h> 

void merge(int arr[], int left[], int right[], int left_size, int right_size) {
    int i = 0, j = 0, k = 0;
    
    // compare elements from both subarrays and place the smaller one in first
    while (i < left_size && j < right_size) {
        if (left[i] <= right[j]) {
            arr[k++] = left[i++];
        } else {
            arr[k++] = right[j++];
        }
    }
    
    // copy any remaining elements from the left subarray
    // just in case we run out of items from the right array
    while (i < left_size) {
        arr[k++] = left[i++];
    }
    
    // Copy any remaining elements from the right subarray
    // just in case we run out of items from the left array
    while (j < right_size) {
        arr[k++] = right[j++];
    }
}

void sequential_merge_sort(int arr[], int size) {
    if (size <= 1) {
        return;
    }
    
    int mid = size / 2;
    int left_size = mid;
    int right_size = size - mid;
    
    // create temporary arrays for the two halves
    int* left = (int*)malloc(left_size * sizeof(int));
    int* right = (int*)malloc(right_size * sizeof(int));
    
    // copy data to temporary arrays
    for (int i = 0; i < left_size; i++) {
        left[i] = arr[i];
    }
    for (int i = 0; i < right_size; i++) {
        right[i] = arr[mid + i];
    }
    
    sequential_merge_sort(left, left_size);
    sequential_merge_sort(right, right_size);
    
    merge(arr, left, right, left_size, right_size);
    
    free(left);
    free(right);
}


void parallel_merge_sort(int arr[], int size, int threads) {
    // base case: if array is small enough or we're down to 1 thread,
    // use sequential sort
    if (size <= 1 || threads <= 1) {
        sequential_merge_sort(arr, size);
        return;
    }
    
    int mid = size / 2;
    int left_size = mid;
    int right_size = size - mid;
    
    // create temporary arrays for the two halves
    int* left = (int*)malloc(left_size * sizeof(int));
    int* right = (int*)malloc(right_size * sizeof(int));
    
    // copy data to temporary arrays
    for (int i = 0; i < left_size; i++) {
        left[i] = arr[i];
    }
    for (int i = 0; i < right_size; i++) {
        right[i] = arr[mid + i];
    }
    
    // recursively sort the two halves in parallel
    #pragma omp parallel sections
    {
        #pragma omp section
        {
            // sort left half with half the available threads
            parallel_merge_sort(left, left_size, threads / 2);
        }
        
        #pragma omp section
        {
            // sort right half with the other half of the available threads
            parallel_merge_sort(right, right_size, threads - threads / 2);
        }
    }
    
    merge(arr, left, right, left_size, right_size);
    
    free(left);
    free(right);
}

/**
 * Alternative parallel implementation using task-based parallelism
 */
void task_parallel_merge_sort(int arr[], int size) {
    // Base case
    if (size <= 1) {
        return;
    }
    
    int mid = size / 2;
    int left_size = mid;
    int right_size = size - mid;
    
    int* left = (int*)malloc(left_size * sizeof(int));
    int* right = (int*)malloc(right_size * sizeof(int));
    
    for (int i = 0; i < left_size; i++) {
        left[i] = arr[i];
    }
    for (int i = 0; i < right_size; i++) {
        right[i] = arr[mid + i];
    }
    
    // use OpenMP tasks for recursive calls
    #pragma omp task shared(left) if(left_size > 1000)  // Only create tasks for larger arrays
    task_parallel_merge_sort(left, left_size);
    
    #pragma omp task shared(right) if(right_size > 1000)
    task_parallel_merge_sort(right, right_size);
    
    #pragma omp taskwait  // wait for both recursive calls to complete here
    
    merge(arr, left, right, left_size, right_size);
    
    free(left);
    free(right);
}

void test_sort() {
    const int size = 50000000;  
    int* data1 = (int*)malloc(size * sizeof(int));
    int* data2 = (int*)malloc(size * sizeof(int));
    
    srand(time(NULL));
    for (int i = 0; i < size; i++) {
        data1[i] = rand() % 10000;
        data2[i] = data1[i];  
    }
    
    printf("Array size: %d\n", size);
    
    double start_time = omp_get_wtime();
    sequential_merge_sort(data1, size);
    double end_time = omp_get_wtime();
    printf("Sequential merge sort time: %.2f seconds\n", end_time - start_time);
    
    int num_threads = omp_get_max_threads();
    printf("Number of threads available: %d\n", num_threads);
    
    start_time = omp_get_wtime();
    #pragma omp parallel
    {
        #pragma omp single
        task_parallel_merge_sort(data2, size);
    }
    end_time = omp_get_wtime();
    printf("Parallel merge sort time: %.2f seconds\n", end_time - start_time);
    
    for (int i = 0; i < size; i++) {
        if (data1[i] != data2[i]) {
            printf("ERROR: Sorting results do not match at index %d!\n", i);
            break;
        }
    }
    
    free(data1);
    free(data2);
}

int main() {
    test_sort();
    
    int arr[] = {38, 27, 43, 3, 9, 82, 10};
    int n = sizeof(arr) / sizeof(arr[0]);
    
    printf("\nSmall demo array before sorting: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    
    omp_set_num_threads(4);
    
    #pragma omp parallel
    {
        #pragma omp single
        task_parallel_merge_sort(arr, n);
    }
    
    printf("\nSmall demo array after sorting: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    
    return 0;
}