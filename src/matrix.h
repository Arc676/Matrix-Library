//Copyright (C) 2018  Arc676/Alessandro Vinciguerra <alesvinciguerra@gmail.com>

//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation (version 3).

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.

//You should have received a copy of the GNU General Public License
//along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef MATRIX_H
#define MATRIX_H

typedef struct Matrix {
	int rows;
	int cols;
	double** matrix;
} Matrix;

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
 * Copies a matrix
 * @param matrix Matrix to copy
 * @return A newly constructed matrix with the same entries
 */
Matrix* matrix_copyMatrix(Matrix* matrix);

/**
 * Transposes a matrix
 * @param dst Destination matrix (can be the original matrix)
 * @param matrix Matrix to transpose
 */
void matrix_transpose(Matrix* dst, Matrix* matrix);

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
int matrix_isZero(Matrix* matrix);

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
int matrix_isIdentity(Matrix* matrix);

/**
 * Determines whether two matrices are equal
 * @param m1 First matrix
 * @param m2 Second matrix
 * @param tolerance Maximum deviation between entries before they are considered unequal
 * @return Whether all the entries in the given matrices are equal
 */
int matrix_areEqual(Matrix* m1, Matrix* m2, double tolerance);

#endif
