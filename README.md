# Algorithm Benchmarking Tool

A comprehensive C++ performance analysis tool for measuring and comparing the execution time and memory usage of fundamental algorithms including sorting and searching implementations.

## Features

- **Comprehensive Algorithm Coverage**: Benchmarks 7 different algorithms across two categories
- **Intelligent Test Sizing**: Uses different input sizes based on algorithm complexity (O(n²) vs O(n log n))
- **Performance Metrics**: Measures execution time in nanoseconds and theoretical memory usage
- **CSV Export**: Automatically generates results.csv for data analysis and visualization
- **Adaptive Iterations**: Runs multiple iterations for small inputs to get accurate measurements
- **Warm-up Cycles**: Includes warm-up runs to minimize cold-start effects

## Algorithms Tested

### Searching Algorithms
- **Linear Search** - O(n) time complexity
- **Binary Search** - O(log n) time complexity (on sorted data)

### Sorting Algorithms
- **Bubble Sort** - O(n²) time complexity
- **Selection Sort** - O(n²) time complexity  
- **Insertion Sort** - O(n²) time complexity
- **Quick Sort** - O(n log n) average case
- **Merge Sort** - O(n log n) guaranteed
- **STL Sort** - O(n log n) (typically introsort)

## Test Data Sizes

The tool uses different input sizes optimized for each algorithm's complexity:

- **Quadratic Sorts (O(n²))**: 1K, 10K, 100K elements
- **Efficient Sorts (O(n log n))**: 1K, 10K, 100K, 1M elements
- **Linear Search (O(n))**: 1K, 10K, 100K, 1M, 10M elements
- **Binary Search (O(log n))**: 1K, 10K, 100K, 1M, 10M, 100M elements

## Requirements

- C++11 compatible compiler (g++, clang++, MSVC)
- Standard Template Library (STL)
- Minimum 1GB RAM (for largest test cases)

## Compilation

```bash
# Using g++
g++ -std=c++11 -O2 -o algobench algobench.cpp

# Using clang++
clang++ -std=c++11 -O2 -o algobench algobench.cpp

# For debug builds (slower but more detailed)
g++ -std=c++11 -g -o algobench algobench.cpp
```

## Usage

```bash
# Run the benchmark
./algobench

# The program will display progress and results in real-time
# Results are automatically saved to results.csv
```

## Output

### Console Output
The tool provides real-time feedback showing:
- Current test size being processed
- Algorithm name, input size, execution time, and memory usage
- Organized by algorithm category (Searching/Sorting)

### CSV Output
Results are saved to `results.csv` with the following columns:
- `Algorithm`: Name of the algorithm tested
- `Size`: Input size (number of elements)
- `Time_ns`: Average execution time in nanoseconds
- `Memory_bytes`: Theoretical memory usage in bytes

## Performance Characteristics

### Execution Time
- **Small inputs (≤1K)**: Multiple iterations (up to 100K) for accurate timing
- **Large inputs**: Single iterations to avoid excessive runtime
- **Warm-up**: 10 iterations before actual measurement

### Memory Calculation
The tool calculates theoretical memory usage based on:
- Input data size
- Algorithm-specific additional memory requirements
- Temporary arrays and data structures used

## Example Output

```
Algorithm Performance Analysis
======================================================================

======================================================================
SIZE: 1000
======================================================================

  Searching Algorithms:
------------------------------------------------------------
         Algorithm        Size       Time (ns)        Memory
------------------------------------------------------------
    Linear Search        1000         245.50 ns        4000 bytes
    Binary Search        1000          45.20 ns        4000 bytes

  Sorting Algorithms:
------------------------------------------------------------
         Algorithm        Size       Time (ns)        Memory
------------------------------------------------------------
       Bubble Sort        1000    2458390.75 ns        4000 bytes
    Selection Sort        1000    1234567.25 ns        4000 bytes
    Insertion Sort        1000     567890.50 ns        4000 bytes
       Quick Sort        1000      12345.80 ns        8000 bytes
       Merge Sort        1000      15678.90 ns        8000 bytes
         STL Sort        1000       8901.25 ns        4000 bytes
```

## Data Analysis

The generated CSV file can be imported into:
- **Excel/Google Sheets**: For basic analysis and charts
- **Python (pandas/matplotlib)**: For advanced visualization
- **R**: For statistical analysis
- **Jupyter Notebooks**: For interactive exploration

## Implementation Notes

### Algorithm Implementations
- **Iterative approaches**: Used where possible to avoid stack overflow
- **In-place sorting**: Bubble, Selection, Insertion, and STL Sort
- **Additional memory**: Quick Sort (stack simulation), Merge Sort (temporary arrays)

### Timing Methodology
- High-resolution clock for nanosecond precision
- Multiple iterations averaged for consistent results
- Warm-up cycles to minimize measurement variance

### Memory Calculations
- Theoretical rather than actual memory measurement
- Includes primary data structures and algorithm-specific overhead
- Based on typical implementations and standard data type sizes

## Limitations

- **Platform dependent**: Results may vary across different hardware/OS
- **Compiler optimizations**: Different optimization levels affect results
- **Random data only**: Uses uniformly distributed random integers
- **Single-threaded**: No parallel algorithm implementations
- **Memory estimation**: Theoretical calculations, not actual memory profiling

## Future Enhancements

- [ ] Additional algorithms (Heap Sort, Radix Sort, etc.)
- [ ] Different data patterns (sorted, reverse sorted, partially sorted)
- [ ] Actual memory profiling integration
- [ ] Benchmark comparison across different machines

## License

This project is provided as-is for educational and benchmarking purposes. Feel free to modify and distribute according to your needs.

## Contributing

Contributions are welcome! Please feel free to submit pull requests for:
- Additional algorithms
- Performance improvements
- Bug fixes
- Enhanced documentation
- New features

## Contact

For questions, suggestions, or issues, please open an issue in the project repository.