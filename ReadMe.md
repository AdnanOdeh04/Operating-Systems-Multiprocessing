# Word Frequency Analysis Project

## Overview
This project implements different approaches to word frequency analysis using C programming techniques, focusing on multi-processing and multi-threading strategies for efficient word counting and sorting.

## Project Files
- [MultiProcess.c](MultiProcess.c): Multi-process implementation of word frequency analysis
- [mainThread.c](mainThread.c): Multi-threading implementation of word frequency analysis
- [naive.c](naive.c): A basic implementation of word frequency counting
- [OS+Report.pdf](OS+Report.pdf): Detailed project report

## Common Features
All implementations share several key characteristics:
- Uses a custom `struct Data` to store words and their frequencies
- Implements merge sort algorithms for sorting words
- Supports large-scale word processing (up to 260,000 unique words)
- Utilizes binary search and linear search techniques

### Data Structure
```c
struct Data {
    char word[MAX_WORDS_LENGTH];  // Word storage
    int frequency;                 // Word occurrence count
    int noElementsFilled;          // Tracking filled elements
};
```

## Key Algorithms

### 1. Merge Sort Implementations
- Alphabetical sorting (`merge()`)
- Frequency-based sorting (`mergeN()`)

### 2. Search Techniques
- Binary Search: Efficient word lookup
- Linear Search: Alternative word finding method

## Parallel Processing Approaches

### Multi-Process Approach (MultiProcess.c)
- Uses `fork()` for creating child processes
- Implements process-level parallelism
- Synchronizes using semaphores

### Multi-Threading Approach (mainThread.c)
- Uses POSIX threads (`pthread`)
- Enables concurrent word processing
- Synchronizes thread operations

### Naive Approach (naive.c)
- Sequential processing
- Basic word frequency counting
- Serves as a baseline implementation

## Compilation and Execution
Compile each file separately using a C compiler with thread and process support:
```bash
gcc -o multiprocess MultiProcess.c -lpthread -lrt
gcc -o mainthread mainThread.c -lpthread
gcc -o naive naive.c
```

## Performance Considerations
- Handles large text files efficiently
- Supports case-insensitive word matching
- Optimized memory management using dynamic allocation

## Limitations
- Fixed maximum word length (100 characters)
- Limited to processing around 260,000 unique words

## Future Improvements
- Dynamic memory allocation
- Support for larger text files
- Enhanced error handling
- More sophisticated synchronization mechanisms

## Contributors
Adnan Odeh - Operating Systems Course Project
