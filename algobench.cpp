#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>
#include <functional>
#include <iomanip>
#include <fstream>
#include <windows.h>
#include <psapi.h>

class MemoryTracker {
private:
    size_t initial_memory;
    
public:
    MemoryTracker() : initial_memory(getCurrentMemoryUsage()) {}
    
    size_t getCurrentMemoryUsage() {
        PROCESS_MEMORY_COUNTERS_EX pmc;
        GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
        return pmc.WorkingSetSize;
    }
    
    size_t getMemoryIncrease() {
        return getCurrentMemoryUsage() - initial_memory;
    }
    
    void reset() {
        initial_memory = getCurrentMemoryUsage();
    }
};

class PerformanceAnalyzer {
private:
    MemoryTracker memTracker;
    // Removed size 1 and kept the rest
    std::vector<int> test_sizes = {100, 10000, 1000000, 10000000};
    
    std::vector<int> generateRandomVector(int n) {
        std::vector<int> vec;
        vec.reserve(n); // Reserve memory to avoid reallocations
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1, n);
        
        for (int i = 0; i < n; i++) {
            vec.push_back(dis(gen));
        }
        return vec;
    }
    
    std::vector<int> generateSortedVector(int n) {
        std::vector<int> vec;
        vec.reserve(n); // Reserve memory to avoid reallocations
        for (int i = 0; i < n; i++) {
            vec.push_back(i + 1);
        }
        return vec;
    }
    
    void measureAlgorithm(const std::string& name, std::function<void()> algorithm, int n) {
        memTracker.reset();
        auto start = std::chrono::high_resolution_clock::now();
        algorithm();
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        size_t memory_used = memTracker.getMemoryIncrease();

        // Print to console
        std::cout << std::setw(18) << name 
                  << std::setw(12) << n 
                  << std::setw(15) << duration.count() << " us"
                  << std::setw(15) << memory_used << " bytes" << std::endl;

        // Write to CSV
        std::ofstream csvFile("results.csv", std::ios::app); // Append mode
        if (csvFile.is_open()) {
            csvFile << name << "," << n << "," << duration.count() << "," << memory_used << "\n";
            csvFile.close();
        } else {
            std::cerr << "Failed to open CSV file!" << std::endl;
        }
    }
    
public:
    void bubbleSort(std::vector<int> vec) {
        int n = vec.size();
        for (int i = 0; i < n - 1; i++) {
            for (int j = 0; j < n - i - 1; j++) {
                if (vec[j] > vec[j + 1]) {
                    std::swap(vec[j], vec[j + 1]);
                }
            }
        }
    }
    
    void selectionSort(std::vector<int> vec) {
        int n = vec.size();
        for (int i = 0; i < n - 1; i++) {
            int min_idx = i;
            for (int j = i + 1; j < n; j++) {
                if (vec[j] < vec[min_idx]) {
                    min_idx = j;
                }
            }
            std::swap(vec[min_idx], vec[i]);
        }
    }
    
    void insertionSort(std::vector<int> vec) {
        int n = vec.size();
        for (int i = 1; i < n; i++) {
            int key = vec[i];
            int j = i - 1;
            while (j >= 0 && vec[j] > key) {
                vec[j + 1] = vec[j];
                j = j - 1;
            }
            vec[j + 1] = key;
        }
    }
    
    // Iterative quicksort to avoid stack overflow
    void quickSort(std::vector<int>& vec) {
        if (vec.size() <= 1) return;
        
        std::vector<std::pair<int, int>> stack;
        stack.push_back(std::make_pair(0, static_cast<int>(vec.size() - 1)));
        
        while (!stack.empty()) {
            std::pair<int, int> range = stack.back();
            stack.pop_back();
            int low = range.first;
            int high = range.second;
            
            if (low < high) {
                int pi = partition(vec, low, high);
                stack.push_back(std::make_pair(low, pi - 1));
                stack.push_back(std::make_pair(pi + 1, high));
            }
        }
    }
    
    int partition(std::vector<int>& vec, int low, int high) {
        int pivot = vec[high];
        int i = (low - 1);
        
        for (int j = low; j <= high - 1; j++) {
            if (vec[j] < pivot) {
                i++;
                std::swap(vec[i], vec[j]);
            }
        }
        std::swap(vec[i + 1], vec[high]);
        return (i + 1);
    }
    
    // Iterative merge sort to avoid stack overflow
    void mergeSort(std::vector<int>& vec) {
        int n = vec.size();
        if (n <= 1) return;
        
        for (int curr_size = 1; curr_size < n; curr_size = 2 * curr_size) {
            for (int left_start = 0; left_start < n - 1; left_start += 2 * curr_size) {
                int mid = std::min(left_start + curr_size - 1, n - 1);
                int right_end = std::min(left_start + 2 * curr_size - 1, n - 1);
                
                if (mid < right_end) {
                    merge(vec, left_start, mid, right_end);
                }
            }
        }
    }
    
    void merge(std::vector<int>& vec, int left, int mid, int right) {
        int n1 = mid - left + 1;
        int n2 = right - mid;
        
        std::vector<int> L(n1), R(n2);
        
        for (int i = 0; i < n1; i++)
            L[i] = vec[left + i];
        for (int j = 0; j < n2; j++)
            R[j] = vec[mid + 1 + j];
        
        int i = 0, j = 0, k = left;
        
        while (i < n1 && j < n2) {
            if (L[i] <= R[j]) {
                vec[k] = L[i];
                i++;
            } else {
                vec[k] = R[j];
                j++;
            }
            k++;
        }
        
        while (i < n1) {
            vec[k] = L[i];
            i++;
            k++;
        }
        
        while (j < n2) {
            vec[k] = R[j];
            j++;
            k++;
        }
    }
    
    bool linearSearch(const std::vector<int>& vec, int target) {
        for (int i = 0; i < vec.size(); i++) {
            if (vec[i] == target) return true;
        }
        return false;
    }
    
    bool binarySearch(const std::vector<int>& vec, int target) {
        int left = 0, right = vec.size() - 1;
        while (left <= right) {
            int mid = left + (right - left) / 2;
            if (vec[mid] == target) return true;
            if (vec[mid] < target) left = mid + 1;
            else right = mid - 1;
        }
        return false;
    }
    
    void runAnalysis() {
        std::cout << "Algorithm Performance Analysis" << std::endl;
        std::cout << std::string(70, '=') << std::endl;
        
        std::ofstream csvFile("results.csv");
        csvFile << "Algorithm,Size,Time_us,Memory_bytes" << std::endl;
        csvFile.close();
        
        for (int n : test_sizes) {
            std::cout << "\nInput Size: " << n << std::endl;
            std::cout << std::setw(18) << "Algorithm" 
                      << std::setw(12) << "Size" 
                      << std::setw(15) << "Time" 
                      << std::setw(15) << "Memory" << std::endl;
            std::cout << std::string(60, '-') << std::endl;
            
            // SEARCH ALGORITHMS FIRST
            std::cout << "SEARCH ALGORITHMS:" << std::endl;
            
            auto vec_search = generateRandomVector(n);
            // Ensure target is within valid range (1 to n)
            int target = std::max(1, n / 2);  // Changed to ensure target is at least 1
            measureAlgorithm("Linear Search", [&]() { 
                linearSearch(vec_search, target); 
            }, n);
            
            auto vec_sorted = generateSortedVector(n);
            measureAlgorithm("Binary Search", [&]() { 
                binarySearch(vec_sorted, target); 
            }, n);
            
            std::cout << "\nSORTING ALGORITHMS:" << std::endl;
            
            // Skip slow sorting algorithms for large inputs
            if (n <= 10000) {
                auto vec = generateRandomVector(n);
                measureAlgorithm("Bubble Sort", [&]() { bubbleSort(vec); }, n);
                
                vec = generateRandomVector(n);
                measureAlgorithm("Selection Sort", [&]() { selectionSort(vec); }, n);
                
                vec = generateRandomVector(n);
                measureAlgorithm("Insertion Sort", [&]() { insertionSort(vec); }, n);
            }
            
            auto vec1 = generateRandomVector(n);
            measureAlgorithm("Quick Sort", [&]() { 
                quickSort(vec1); 
            }, n);
            
            auto vec2 = generateRandomVector(n);
            measureAlgorithm("Merge Sort", [&]() { 
                mergeSort(vec2); 
            }, n);
            
            auto vec3 = generateRandomVector(n);
            measureAlgorithm("STL Sort", [&]() { 
                std::sort(vec3.begin(), vec3.end()); 
            }, n);
        }
        
        std::cout << "\nAnalysis complete. Results saved to results.csv" << std::endl;
    }
};

int main() {
    std::cout << "Starting algorithm analysis..." << std::endl;
    
    PerformanceAnalyzer analyzer;
    analyzer.runAnalysis();
    
    return 0;
}