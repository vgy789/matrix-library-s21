[На русском 🇷🇺](README_RUS.md)

# Matrix Library

Implementation of a matrix library in C that provides basic operations for working with matrices.

## Features

### Matrix Operations
- Creating and removing
- Comparison
- Addition, subtraction
- Matrix multiplication by number/matrix
- Transposition
- Calculation of determinants
- Calculation of complements matrix
- Finding inverse matrix

## Implementation Details

### Determinant Calculation Algorithm

The project uses a recursive algorithm to calculate determinants using the first row expansion method (Laplace expansion):

1. For 1x1 matrices, returns the single element
2. For 2x2 matrices, uses the formula: ad - bc
3. For larger matrices:
   - Expansion is performed along the first row
   - For each element in the first row:
     * Calculate the corresponding minor (submatrix without current row and column)
     * Recursively calculate the determinant of the minor
     * Multiply the result by the element and corresponding sign (+/-)
   - Sum up the products

Expansion formula:
det(A) = a₁₁M₁₁ - a₁₂M₁₂ + a₁₃M₁₃ - ... ± a₁ₙM₁ₙ

where a₁ᵢ are elements of the first row, M₁ᵢ are corresponding minors


## Build and Usage

### Building the Library

```bash
make s21_matrix.a
```

### Using in Your Project

1. Include the header file:
```c
#include "s21_matrix.h"
```

2. Compile your project with the library:
```bash
gcc your_file.c s21_matrix.a -o your_program
```

## Testing

The project includes unit tests written using the check library.

```bash
make test
```

## License
This project is licensed under the Apache License, Version 2.0. See LICENSE file for details.
