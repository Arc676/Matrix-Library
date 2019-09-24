# Matrix Library

*One cannot be given the Matrix library. The only way is to compile it for oneself.* Just kidding.

The Matrix C library provides a representation of matrices and functions for manipulating them. View the header files for a list of available functions and descriptions of how they work.

## Frontends

The repository includes two frontends i.e. matrix calculators to the Matrix library.

When entering a matrix, you will be prompted `Row, Col:`. Enter the number of rows and then the number of columns, separated by any whitespace. The program will then read in the appropriate number of entries, again separated by any whitespace.

### `frontend/` (C)

The initial frontend and the simpler of the two. Type `help` at the prompt to see a list of available commands. You can only perform preset operations on pairs of matrices. There is no way to store the result of any calculations. Reusing matrices requires re-typing their contents.

### `frontend2/` (C++)

The more user-friendly and flexible of the two frontends, the second matrix calculator recognizes both normal infix notation and [Polish notation](https://en.wikipedia.org/wiki/Polish_notation) to read and evaluate expressions. Additionally, the user may store any number of matrices in memory under any name that doesn't start with a reserved operator. These are stored in a `std::map<std::string, Matrix*>`.

Type `mode` at the prompt to switch between infix and postfix mode.

Expression tokens must be separated by *spaces*. Expressions must adhere to the following syntax:

```
expression = operator argument [argument]
argument = expression | matrix
matrix = (name of a matrix) | '?'
```

Any time a matrix is required, if `?` is encountered, the user will be prompted to manually enter a matrix.

The following operators are available:

| Operator | Arguments | Description |
| --- | --- | --- |
| + | 2 matrices | Adds the two matrices together |
| - | 2 matrices | Subtracts the second matrix from the first |
| * | 2 matrices | Multiplies the two matrices in their given order |
| . | 1 real number, then 1 matrix | Multiplies the matrix by the scalar |
| ^ | 1 square matrix, then 1 integer | Computes the matrix to the given power |
| i | 1 square matrix | Computes the inverse of the matrix |
| d | 1 matrix | Computes the determinant of the given matrix |
| m | 1 matrix | Computes the matrix of minors of the given matrix |
| c | 1 matrix | Computes the matrix of cofactors of the given matrix |
| t | 1 matrix | Computes the transpose of the matrix |
| id | 1 integer | Creates an identity matrix of the given size |

To save a matrix in memory, use `= (name) expression`. The result of the given expression will be stored in memory with the given name. The first letter of the name of a matrix must be a letter and cannot be the first letter of an operator (`i, d, m, c, t`).

Example:

```
> = A ?
Row, Col: 2 2 1 2 3 4
1.000000 2.000000
3.000000 4.000000

> i A
-2.000000 1.000000
1.500000 -0.500000

> = B * i A t A
0.000000 -2.000000
0.500000 2.500000

> + B A
1.000000 0.000000
3.500000 6.500000

> * i A A
1.000000 0.000000
0.000000 1.000000

> * A i A
1.000000 0.000000
0.000000 1.000000
```

## Licensing

Project available under the terms of the GPLv3.
