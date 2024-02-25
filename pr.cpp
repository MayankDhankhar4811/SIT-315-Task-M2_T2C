#include <chrono>
#include <cstdlib>
#include <iostream>
#include <time.h>
#include <omp.h> // Include OpenMP header

using namespace std::chrono;
using namespace std;

// Function to generate a random vector of integers
void generateRandomVector(int vector[], int size) {
    #pragma omp parallel for
    for (int i = 0; i < size; i++) {
        vector[i] = rand() % 100; // Generate a random number between 0 and 99 and assign it to the current element of the vector
    }
}

// Function to perform QuickSort algorithm
void performQuickSort(int arr[], int low, int high) {
    if (low < high) {
        int pivot = arr[high]; // Select the last element as the pivot
        int i = low - 1; // Initialize the index of the smaller element

        for (int j = low; j < high; j++) {
            if (arr[j] < pivot) {
                i++; // Increment the index of the smaller element
                std::swap(arr[i], arr[j]); // Swap the elements at indices i and j
            }
        }
        std::swap(arr[i + 1], arr[high]); // Swap the pivot element with the element at index i+1
        int pi = i + 1; // Store the index of the pivot element

        performQuickSort(arr, low, pi - 1); // Recursively sort the elements before the pivot
        performQuickSort(arr, pi + 1, high); // Recursively sort the elements after the pivot
    }
}

int main() {
    unsigned long size = 1000000; // Define the size of the vectors
    srand(time(0)); // Seed the random number generator with the current time
    int *vector1, *vector2, *vector3; // Declare pointers to the vectors
    auto start = high_resolution_clock::now(); // Get the current time

    // Allocate memory for the vectors
    vector1 = (int *)malloc(size * sizeof(int));
    vector2 = (int *)malloc(size * sizeof(int));
    vector3 = (int *)malloc(size * sizeof(int));

    // Generate random vectors
    generateRandomVector(vector1, size);
    generateRandomVector(vector2, size);

    // Sorting vector1 and vector2 using QuickSort algorithm in parallel
    #pragma omp parallel sections
    {
        #pragma omp section
        performQuickSort(vector1, 0, size - 1);
        
        #pragma omp section
        performQuickSort(vector2, 0, size - 1);
    }

    // Performing vector addition in parallel
    #pragma omp parallel for
    for (int i = 0; i < size; i++) {
        vector3[i] = vector1[i] + vector2[i]; // Add corresponding elements of vector1 and vector2 and store the result in vector3
    }

    auto stop = high_resolution_clock::now(); // Get the current time
    auto duration = duration_cast<microseconds>(stop - start); // Calculate the duration of the operation
    cout << "Time taken by function: " << duration.count() << " microseconds" << endl; // Print the duration

    // Free the memory allocated for the vectors
    free(vector1);
    free(vector2);
    free(vector3);
  
    return 0; // Return 0 to indicate successful execution
}
