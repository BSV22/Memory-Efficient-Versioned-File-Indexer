# Memory-Efficient Versioned File Indexer Using C++

## CS253 : Software Development and Operations 

### Problem Overview

This project implements a **memory-efficient versioned file indexer in C++** that handles large text files using a **fixed-size buffer** and builds a **word-level index**. The system supports multiple analytical queries while ensuring that the entire file is **never loaded into memory**.

By *word-level index*, we mean a data structure that reads the input file incrementally and builds a mapping from each unique word to the number of times it appears in the file.

---

## Supported Queries

- #### Word Count Query  
  Returns the frequency of a given word in a specified version.

- #### Difference Query  
  Computes the difference in frequency of a word between two versions.

- #### Top-K Query  
  Displays the top-K most frequent words in a specified version.

---

## C++ OOP Features

Implemented **inheritance**, **runtime polymorphism**, **function overloading**, **exception handling**, and **user-defined templates**.

---

## Inputs From Command Line

### Command Line Arguments

| Argument | Description |
|--------|-------------|
| `--file <path>` | Path to input file (single-version queries) |
| `--file1 <path>` | First input file (diff query) |
| `--file2 <path>` | Second input file (diff query) |
| `--version <name>` | Version identifier (single-version queries) |
| `--version1 <name>` | First version identifier (diff query) |
| `--version2 <name>` | Second version identifier (diff query) |
| `--buffer <kb>` | Buffer size in kilobytes (256 to 1024) |
| `--query <type>` | `word` \| `diff` \| `top` |
| `--word <token>` | Word for word/diff queries |
| `--top <k>` | Number of top results (top query) |

---

## Examples (Command-Line Usage)

### Compile the code to generate the executable

```bash
g++ -O2 main.cpp -o analyzer
```

After compiling, run any one of the three queries. (This is just an example)

---

### Word Query (Single File)

```bash
./analyzer --file dataset_v1.txt --version v1 \
--buffer 512 --query word --word error
```

---

### Top-K Query (Single File)

```bash
./analyzer --file dataset_v1.txt --version v1 \
--buffer 512 --query top --top 10
```

---

### Difference Query (Two Files)

```bash
./analyzer --file1 dataset_v1.txt --version1 v1 \
--file2 dataset_v2.txt --version2 v2 \
--buffer 512 --query diff --word error
```