# Road Construction Optimization – A Pathfinding Project in ANSI C

This project implements an efficient algorithm to compute a minimum-cost path across a geographical grid, following the official specifications of the Laboratorio di Algoritmi e Strutture Dati course (University of Bologna, A.Y. 2023/2024).
The program is fully written in ANSI C (C89/C90) and adheres to the provided formatting, memory-management, and execution rules.

---

## 📌 Problem Description

> A geographic area is modeled as a grid of n rows and m columns.
> Each cell `(i, j)` contains an integer value representing its height above sea level, which > may also be negative.

The goal is to build a road from the top-left cell `(0, 0)` to the bottom-right cell `(n−1, m−1)`.
The road:
- must move only through adjacent cells that share a side (no diagonals),
- must never visit the same cell twice,
- must stay inside the grid boundaries.

---

## Cost Model

The total cost of a path is composed of:

Cell cost
- A fixed cost Ccell is added for each visited cell.
- Height-difference cost

For every pair of consecutive visited cells `(xi, yi) → (xi+1, yi+1)` an additional cost is paid:

$$ Cheight × (H[x_i, y_i] − H[x_i+1, y_i+1])^2 $$


The algorithm must compute a valid path with minimum total cost, outputting one optimal solution (even when multiple exist).

---

## 📥 Input Specification

The program receives exactly one command-line argument:
a text file containing:
- Ccell – non-negative integer
- Cheight – non-negative integer
- n – number of rows (5 ≤ n ≤ 250)
- m – number of columns (5 ≤ m ≤ 250)
- n lines, each containing m integers: the height matrix H

---

## 📤 Output Specification
The program prints:
1. The coordinates of each cell in the optimal path (one per line):
```
x y
```

2. A termination line:
```
-1 -1
```

3. The total cost of the path, printed as an integer.

The output format must match the specification exactly.

---

## 🛠️ Compilation

The program must compile using GCC with:
```
gcc -std=c90 -Wall -Wpedantic file.c -o file
```

- No warnings should be present.
- Only ANSI C89/C90 features are allowed.
- All memory allocated with `malloc()` must be freed.
- `fclose()` must be called for every opened file.

---

## ✔️ Correctness & Validation

The implementation is tested using `valgrind` to ensure:
- no memory leaks,
- no invalid memory accesses.

> The optimal path may differ from provided reference outputs,
> but its total cost must match the minimum expected value.

---

## 📦 File Structure

A single C source file must be submitted, named after the student’s ID, e.g.: `1234567890.c`

The repository may be organized as follows:

```
/project-root
│── src/
│   └── <your_source_file>.c
│── input/     # sample input files
│── output/    # sample expected outputs
│── README.md
```

---

## 🚀 Running the Program
`./program_name inputfile.in`

The output is printed to `stdout`.