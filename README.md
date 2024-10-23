# File Compression using Huffman Implementation

# Huffman Coding Implementation (C++)

This implementation of Huffman Coding consists of `compress()` and `decompress()` functions.

## Features

- **Compress**: Compresses a given text file using Huffman Coding.
- **Decompress**: Decompresses the previously compressed file back to its original state.

## Running the Program

### Steps:

1. **Save / Clone the Repository**  
   Clone the repository or download the source files.

2. **Test the Program**  
   The repository includes a sample text file of size 715kB. You can use it to test the compression and decompression functionality.

3. **Run the Program**  
   To run the C++ program and perform compression and decompression, follow these steps:

   - Open your terminal.
   - Compile the program using the following command:

     ```bash
     g++ -o useHuffman useHuffman.cpp
     ```

   - Run the program to compress and decompress the sample file by executing:

     ```bash
     ./useHuffman
     ```

   This command will perform both compression and decompression on the `sample.txt` file. The compressed and decompressed files will be generated in the same directory.

4. **Compress Other Files**  
   To compress any other text file, modify the `filePath` variable in the `useHuffman.cpp` file to the path of your desired file.

### Note:

- The `decompress()` function must be called from the same object where `compress()` was executed, as the encoding information is stored in the object’s data members.

---

This version provides guidance for using a C++ implementation and specifies how to compile and run the program.
