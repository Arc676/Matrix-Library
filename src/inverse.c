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

#include "inverse.h"

void matrix_minors(Matrix* dst, Matrix* matrix) {
	// if destination matrix and input matrix are of unequal size, do nothing
	if (dst->rows != matrix->rows || dst->cols != matrix->cols) {
		return;
	}

	for (int r = 0; r < matrix->rows; r++) {
		for (int c = 0; c < matrix->cols; c++) {
			Matrix* submatrix = matrix_createMatrix(matrix->rows - 1, matrix->cols - 1);
			int dr = 0;
			for (int r1 = 0; r1 < submatrix->rows; r1++) {
				int dc = 0;
				if (r1 == r) {
					dr = 1;
				}
				for (int c1 = 0; c1 < submatrix->cols; c1++) {
					if (c1 == c) {
						dc = 1;
					}
					submatrix->matrix[r1][c1] = matrix->matrix[r1 + dr][c1 + dc];
				}
			}
			dst->matrix[r][c] = matrix_determinant(submatrix, NULL);
			matrix_destroyMatrix(submatrix);
		}
	}
}

void matrix_cofactors(Matrix* dst, Matrix* minors) {
	// if matrix of minors and destination matrix are of unequal size, do nothing
	if (dst->rows != minors->rows || dst->cols != minors->cols) {
		return;
	}

	for (int r = 0; r < minors->rows; r++) {
		for (int c = 0; c < minors->cols; c++) {
			dst->matrix[r][c] = minors->matrix[r][c] * ((r + c) % 2 == 0 ? 1 : -1);
		}
	}
}

double matrix_determinant(Matrix* matrix, Matrix* cofactors) {
	// determinant of a 1x1 matrix is its only element
	if (matrix->rows == 1 && matrix->cols == 1) {
		return matrix->matrix[0][0];
	}

	Matrix* mcofactors = cofactors;
	// determine cofactors if not given
	if (!mcofactors) {
		mcofactors = matrix_createMatrix(matrix->rows, matrix->cols);
		Matrix* minors = matrix_createMatrix(matrix->rows, matrix->cols);
		matrix_minors(minors, matrix);
		matrix_cofactors(mcofactors, minors);
		matrix_destroyMatrix(minors);
	} else {
		// if cofactors matrix and input matrix are of unequal size, do nothing
		if (matrix->rows != cofactors->rows || matrix->cols != cofactors->cols) {
			return 0;
		}
	}
	double det = 0;
	for (int c = 0; c < matrix->cols; c++) {
		det += matrix->matrix[0][c] * mcofactors->matrix[0][c];
	}
	// destroy temporary cofactors matrix if none was given
	if (!cofactors) {
		matrix_destroyMatrix(mcofactors);
	}
	return det;
}

double matrix_invert(Matrix* dst, Matrix* matrix, Matrix* minors, Matrix* cofactors) {
	// only square matrices allowed
	if (matrix->rows != matrix->cols) {
		return 0;
	}

	// determine matrix of minors
	Matrix* mminors = minors ? minors : matrix_createMatrix(matrix->rows, matrix->cols);
	matrix_minors(mminors, matrix);

	// determine matrix of cofactors
	Matrix* mcofactors = cofactors ? cofactors : matrix_createMatrix(matrix->rows, matrix->cols);
	matrix_cofactors(mcofactors, mminors);

	double det = matrix_determinant(matrix, mcofactors);

	// transpose cofactors to get adjugate
	matrix_transpose(mcofactors, mcofactors);

	matrix_multiplyScalar(dst, mcofactors, 1 / det);

	// transpose adjugate to get back cofactors
	matrix_transpose(mcofactors, mcofactors);

	// if the caller didn't provide destination matrices, destroy the created ones
	if (!minors) {
		matrix_destroyMatrix(mminors);
	}
	if (!cofactors) {
		matrix_destroyMatrix(mcofactors);
	}
	return det;
}
