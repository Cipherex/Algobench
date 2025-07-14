# Algobench

Algobench is a lightweight C++ benchmarking tool designed to analyze the performance of classic sorting and searching algorithms. It measures execution time and estimates memory usage across a range of input sizes, exporting results to CSV for easy analysis and reporting.

**Made for college reports and algorithm analysis projects.**

---

## Features
- Benchmarks both searching and sorting algorithms
- Measures execution time (nanoseconds) and theoretical memory usage (bytes)
- Automatically tests each algorithm with input sizes appropriate to its time complexity
- Outputs results in a clean CSV file for further analysis or inclusion in reports
- Console output formatted for easy reading

---

## Algorithms & Input Sizes

| Algorithm Type      | Algorithms                              | Input Sizes Tested           |
|--------------------|-----------------------------------------|-----------------------------|
| Quadratic Sorts    | Bubble, Selection, Insertion Sort        | 1,000; 10,000; 100,000      |
| Linearithmic Sorts | Quick Sort, Merge Sort, STL std::sort    | 1,000; 10,000; 100,000; 1M  |
| Linear Search      | Linear Search                            | 1,000; 10,000; 100,000; 1M; 10M |
| Binary Search      | Binary Search                            | 1,000; 10,000; 100,000; 1M; 10M; 100M |

Input sizes are chosen to reflect the practical limits of each algorithm's time complexity.

---

## Usage

1. **Build the program:**
   
   ```sh
   g++ -O2 -std=c++11 algobench.cpp -o algobench
   ```

2. **Run the benchmark:**
   
   ```sh
   ./algobench
   ```
   
   The program will print progress and results to the console. It may take several minutes, especially for large input sizes.

---

## Output
- Results are saved to `results.csv` in the same directory.
- Each row contains: `Algorithm,Size,Time_ns,Memory_bytes`
- Example:

  ```csv
  Algorithm,Size,Time_ns,Memory_bytes
  Linear Search,1000,12345,4000
  Binary Search,1000,234,4000
  Bubble Sort,1000,56789,4000
  ...
  ```

---

## Example Results Table

| Algorithm      | Size    | Time (ns) | Memory (bytes) |
|---------------|---------|-----------|---------------|
| Linear Search | 10,000  | 120,000   | 40,000        |
| Binary Search | 1,000,000 | 300     | 4,000,000     |
| Quick Sort    | 100,000 | 2,000,000 | 400,000       |
| Bubble Sort   | 10,000  | 5,000,000 | 40,000        |

---

## Notes
- For very large input sizes, the program may use significant memory and CPU.
- Results may vary depending on your hardware and system load.
- The tool is ideal for generating tables and charts for college reports or algorithm analysis assignments.

---

## License
This project is open source and free to use for educational purposes.
