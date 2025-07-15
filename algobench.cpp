#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>
#include <functional>
#include <iomanip>
#include <fstream>

class PerformanceAnalyzer {
private:
    // Different test sizes based on algorithm complexity
    std::vector<int> search_linear_sizes = {1000, 10000, 100000, 1000000, 10000000}; // O(n) - up to 10M
    std::vector<int> search_binary_sizes = {1000, 10000, 100000, 1000000, 10000000, 100000000}; // O(log n) - up to 100M
    std::vector<int> sort_quadratic_sizes = {1000, 10000, 100000}; // O(n²) - up to 100k
    std::vector<int> sort_nlogn_sizes = {1000, 10000, 100000, 1000000}; // O(n log n) - up to 1M
    
    std::vector<int> generateRandomVector(int n) {
        std::vector<int> vec;
        vec.reserve(n);
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
        vec.reserve(n);
        for (int i = 0; i < n; i++) {
            vec.push_back(i + 1);
        }
        return vec;
    }
    
    void measureAlgorithm(const std::string& name, std::function<void()> algorithm, int n) {
        int iterations = 1;
        // For n^2 sorts, always use 1 iteration
        if (!(name == "Bubble Sort" || name == "Selection Sort" || name == "Insertion Sort")) {
            if (n <= 1000) {
                if (name == "Quick Sort" || name == "Merge Sort" || name == "STL Sort") {
                    iterations = 100; // 1,00 for nlogn sorts at small sizes
                } else {
                    iterations = 100000; // 100,000 for others at small sizes
                }
            } else if (name.find("Binary Search") != std::string::npos) {
                iterations = 100000;
            } else if (name.find("Linear Search") != std::string::npos) {
                iterations = 1000;
            }
        }
        // Warm up
        if (iterations > 1) {
            for (int i = 0; i < 10; i++) {
                algorithm();
            }
        }
        // Measure time in nanoseconds
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < iterations; i++) {
            algorithm();
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        // Average the time if multiple iterations
        double avg_time = static_cast<double>(duration.count()) / iterations;
        // Calculate theoretical memory usage instead of process memory
        size_t theoretical_memory = calculateTheoreticalMemory(name, n);
        // Print to console
        std::cout << std::setw(18) << name 
                  << std::setw(12) << n 
                  << std::setw(15) << std::fixed << std::setprecision(2) << avg_time << " ns"
                  << std::setw(15) << theoretical_memory << " bytes" << std::endl;
        // Write to CSV
        std::ofstream csvFile("results.csv", std::ios::app);
        if (csvFile.is_open()) {
            csvFile << name << "," << n << "," << avg_time << "," << theoretical_memory << "\n";
            csvFile.close();
        } else {
            std::cerr << "Failed to open CSV file!" << std::endl;
        }
    }
    
    size_t calculateTheoreticalMemory(const std::string& algorithm, int n) {
        size_t int_size = sizeof(int);
        
        if (algorithm == "Linear Search" || algorithm == "Binary Search") {
            return n * int_size; // Input vector only
        }
        else if (algorithm == "Bubble Sort" || algorithm == "Selection Sort" || algorithm == "Insertion Sort") {
            return n * int_size; // In-place sorting, just the input vector
        }
        else if (algorithm == "Quick Sort") {
            return n * int_size + (n * sizeof(std::pair<int, int>)); // Vector + stack for iterative approach
        }
        else if (algorithm == "Merge Sort") {
            return n * int_size * 2; // Original vector + temporary arrays during merge
        }
        else if (algorithm == "STL Sort") {
            return n * int_size; // Typically in-place (introsort)
        }
        
        return n * int_size; // Default
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
        csvFile << "Algorithm,Size,Time_ns,Memory_bytes" << std::endl;
        csvFile.close();

        // Collect all unique sizes from all algorithm groups
        std::vector<int> all_sizes;
        all_sizes.insert(all_sizes.end(), search_linear_sizes.begin(), search_linear_sizes.end());
        all_sizes.insert(all_sizes.end(), search_binary_sizes.begin(), search_binary_sizes.end());
        all_sizes.insert(all_sizes.end(), sort_quadratic_sizes.begin(), sort_quadratic_sizes.end());
        all_sizes.insert(all_sizes.end(), sort_nlogn_sizes.begin(), sort_nlogn_sizes.end());
        std::sort(all_sizes.begin(), all_sizes.end());
        all_sizes.erase(std::unique(all_sizes.begin(), all_sizes.end()), all_sizes.end());

        for (int n : all_sizes) {
            std::cout << "\n" << std::string(70, '=') << std::endl;
            std::cout << "SIZE: " << n << std::endl;
            std::cout << std::string(70, '=') << std::endl;

            // SEARCHING GROUP
            bool has_search = (std::find(search_linear_sizes.begin(), search_linear_sizes.end(), n) != search_linear_sizes.end()) ||
                              (std::find(search_binary_sizes.begin(), search_binary_sizes.end(), n) != search_binary_sizes.end());
            if (has_search) {
                std::cout << "\n  Searching Algorithms:" << std::endl;
                std::cout << std::string(60, '-') << std::endl;
                std::cout << std::setw(18) << "Algorithm"
                          << std::setw(12) << "Size"
                          << std::setw(15) << "Time (ns)"
                          << std::setw(15) << "Memory" << std::endl;
                std::cout << std::string(60, '-') << std::endl;
                // Linear Search
                if (std::find(search_linear_sizes.begin(), search_linear_sizes.end(), n) != search_linear_sizes.end()) {
                    auto vec_search = generateRandomVector(n);
                    int target = std::max(1, n / 2);
                    measureAlgorithm("Linear Search", [&]() {
                        linearSearch(vec_search, target);
                    }, n);
                }
                // Binary Search
                if (std::find(search_binary_sizes.begin(), search_binary_sizes.end(), n) != search_binary_sizes.end()) {
                    auto vec_sorted = generateSortedVector(n);
                    int target = std::max(1, n / 2);
                    measureAlgorithm("Binary Search", [&]() {
                        binarySearch(vec_sorted, target);
                    }, n);
                }
            }

            // SORTING GROUP
            bool has_sort = (std::find(sort_quadratic_sizes.begin(), sort_quadratic_sizes.end(), n) != sort_quadratic_sizes.end()) ||
                            (std::find(sort_nlogn_sizes.begin(), sort_nlogn_sizes.end(), n) != sort_nlogn_sizes.end());
            if (has_sort) {
                std::cout << "\n  Sorting Algorithms:" << std::endl;
                std::cout << std::string(60, '-') << std::endl;
                std::cout << std::setw(18) << "Algorithm"
                          << std::setw(12) << "Size"
                          << std::setw(15) << "Time (ns)"
                          << std::setw(15) << "Memory" << std::endl;
                std::cout << std::string(60, '-') << std::endl;
                // Quadratic sorts
                if (std::find(sort_quadratic_sizes.begin(), sort_quadratic_sizes.end(), n) != sort_quadratic_sizes.end()) {
                    auto vec = generateRandomVector(n);
                    measureAlgorithm("Bubble Sort", [&]() { bubbleSort(vec); }, n);
                    vec = generateRandomVector(n);
                    measureAlgorithm("Selection Sort", [&]() { selectionSort(vec); }, n);
                    vec = generateRandomVector(n);
                    measureAlgorithm("Insertion Sort", [&]() { insertionSort(vec); }, n);
                }
                // nlogn sorts
                if (std::find(sort_nlogn_sizes.begin(), sort_nlogn_sizes.end(), n) != sort_nlogn_sizes.end()) {
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
            }
        }
        std::cout << "\nAnalysis complete. Results saved to results.csv" << std::endl;
    }
};

int main() {
    std::cout << "Starting comprehensive algorithm analysis..." << std::endl;
    std::cout << "This may take several minutes to complete..." << std::endl;
    
    PerformanceAnalyzer analyzer;
    analyzer.runAnalysis();
    
    return 0;
}
