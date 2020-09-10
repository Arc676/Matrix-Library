//Copyright (C) 2018-20 Arc676/Alessandro Vinciguerra <alesvinciguerra@gmail.com>

//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation (version 3).

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//GNU General Public License for more details.

//You should have received a copy of the GNU General Public License
//along with this program. If not, see <http://www.gnu.org/licenses/>.

#ifdef __cplusplus
extern "C" {
#endif

#ifndef MATRIX_H
#define MATRIX_H

#include <stdlib.h>
#include <math.h>
#include <stdarg.h>
#include <string.h>

typedef struct Matrix {
	int rows;
	int cols;
	double** matrix;
} Matrix;

/**
 * Creates and returns a pointer to an uninitialized matrix
 * @param rows Desired number of rows in the matrix
 * @param cols Desired number of columns in the matrix
 * @return Pointer to the newly constructed matrix
 */
Matrix* matrix_createMatrix(int rows, int cols);

/**
 * Creates and returns a pointer to a new zero matrix
 * @param rows Desired number of rows in the matrix
 * @param cols Desired number of columns in the matrix
 * @return Pointer to a newly constructed zero matrix with the given size
 */
Matrix* matrix_createZeroMatrix(int rows, int cols);

/**
 * Creates and returns a pointer to a new identity matrix
 * @param size Size (row and column count) of the matrix
 * @return Pointer to a newly constructed identity matrix with the given size
 */
Matrix* matrix_createIdentityMatrix(int size);

/**
 * Creates a new matrix and populates it with the given elements
 * @param rows Desired number of rows in the matrix
 * @param cols Desired number of columns in the matrix
 * @param ... Elements in the matrix (rows*cols elements required)
 * @return A newly constructed matrix with the given size and elements
 */
Matrix* matrix_createMatrixWithElements(int rows, int cols, ...);

/**
 * Creates a new matrix with the entries in a given 1D array
 * @param rows Desired number of rows in the matrix
 * @param cols Desired number of columns in the matrix
 * @param elements A list of rows*cols elements
 * @return A newly constructed matrix with the given size and elements
 */
Matrix* matrix_createMatrixWithElementsFrom1D(int rows, int cols, double* elements);

/**
 * Creates a new matrix with the entries in a given 2D array
 * @param rows Desired number of rows in the matrix
 * @param cols Desired number of columns in the matrix
 * @param elements An array containing 'row' arrays each containing 'col' elements
 * @return A newly constructed matrix with the given size and elements
 */
Matrix* matrix_createMatrixWithElementsFrom2D(int rows, int cols, double** elements);

/**
 * Copies a matrix
 * @param matrix Matrix to copy
 * @return A newly constructed matrix with the same entries
 */
Matrix* matrix_copyMatrix(const Matrix* matrix);

/**
 * Copies the entries of a matrix into another one; if they are of unequal sizes,
 * this method does nothing
 * @param dst The destination matrix
 * @param src The source matrix
 */
void matrix_copyEntries(Matrix* dst, const Matrix* src);

/**
 * Deallocates the memory for a matrix
 * @param matrix Matrix to destroy
 */
void matrix_destroyMatrix(Matrix* matrix);

/**
 * Transposes a matrix. If the destination matrix doesn't have dimensions equal
 * to the transpose of the source matrix, the arguments are left unchanged.
 * @param dst Destination matrix (can be the source matrix if it's a square matrix)
 * @param matrix Matrix to transpose
 */
void matrix_transpose(Matrix* dst, const Matrix* matrix);

/**
 * Sets all the entries in a matrix to zero
 * @param matrix Matrix to turn into a zero matrix
 */
void matrix_zeroMatrix(Matrix* matrix);

/**
 * Determines whether a matrix is a zero matrix
 * @param matrix Matrix to check
 * @return Whether the given matrix is a zero matrix
 */
int matrix_isZero(const Matrix* matrix);

/**
 * Sets the entries in a square matrix to make it an identity matrix. If the
 * given matrix is not a square matrix, it is left unchanged.
 * @param matrix Matrix to turn into an identity matrix
 */
void matrix_makeIdentity(Matrix* matrix);

/**
 * Determines whether a matrix is an identity matrix
 * @param matrix Matrix to check
 * @return Whether the matrix is an identity matrix
 */
int matrix_isIdentity(const Matrix* matrix);

/**
 * Determines whether two matrices are equal
 * @param m1 First matrix
 * @param m2 Second matrix
 * @param tolerance Maximum deviation between entries before they are considered unequal
 * @return Whether all the entries in the given matrices are equal
 */
int matrix_areEqual(const Matrix* m1, const Matrix* m2, double tolerance);

/**
 * Determines whether a matrix is a square matrix
 * @param m Matrix to check
 * @return Whether the matrix has the same number of rows and columns
 */
int matrix_isSquare(const Matrix* m);

#endif

#ifdef __cplusplus
}
#endif
