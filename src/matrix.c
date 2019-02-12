//Copyright (C) 2018-9 Arc676/Alessandro Vinciguerra <alesvinciguerra@gmail.com>

//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation (version 3).

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.

//You should have received a copy of the GNU General Public License
//along with this program. If not, see <http://www.gnu.org/licenses/>.

#include "matrix.h"

Matrix* matrix_createMatrix(int rows, int cols) {
	Matrix* matrix = malloc(sizeof(Matrix));
	matrix->rows = rows;
	matrix->cols = cols;
	matrix->matrix = malloc(rows * sizeof(double*));
	for (int r = 0; r < rows; r++) {
		matrix->matrix[r] = malloc(cols * sizeof(double));
	}
	return matrix;
}

Matrix* matrix_createZeroMatrix(int rows, int cols) {
	Matrix* matrix = matrix_createMatrix(rows, cols);
	matrix_zeroMatrix(matrix);
	return matrix;
}

Matrix* matrix_createIdentityMatrix(int size) {
	Matrix* matrix = matrix_createMatrix(size, size);
	matrix_makeIdentity(matrix);
	return matrix;
}

Matrix* matrix_createMatrixWithElements(int rows, int cols, ...) {
	Matrix* matrix = matrix_createMatrix(rows, cols);

	int r = 0, c = 0;
	int count = rows * cols;

	va_list arglist;
	va_start(arglist, count);

	for (int i = 0; i < count; i++) {
		matrix->matrix[r][c] = va_arg(arglist, double);
		if (++c >= cols) {
			c = 0;
			r++;
		}
	}

	va_end(arglist);

	return matrix;
}

Matrix* matrix_copyMatrix(Matrix* matrix) {
	Matrix* copy = matrix_createMatrix(matrix->rows, matrix->cols);
	for (int r = 0; r < matrix->rows; r++) {
		memcpy(copy->matrix[r], matrix->matrix[r], matrix->cols * sizeof(double));
	}
	return copy;
}

void matrix_destroyMatrix(Matrix* matrix) {
	for (int r = 0; r < matrix->rows; r++) {
		free(matrix->matrix[r]);
	}
	free(matrix->matrix);
	free(matrix);
}

void matrix_transpose(Matrix* dst, Matrix* matrix) {
	if (dst->rows != matrix->rows || dst->cols != matrix->cols) {
		return;
	}
	for (int r = 0; r < matrix->rows; r++) {
		dst->matrix[r][r] = matrix->matrix[r][r];
		for (int c = r + 1; c < matrix->cols; c++) {
			double toSwap = matrix->matrix[c][r];
			dst->matrix[c][r] = matrix->matrix[r][c];
			dst->matrix[r][c] = toSwap;
		}
	}
}

void matrix_zeroMatrix(Matrix* matrix) {
	for (int r = 0; r < matrix->rows; r++) {
		for (int c = 0; c < matrix->cols; c++) {
			matrix->matrix[r][c] = 0;
		}
	}
}

int matrix_isZero(Matrix* matrix) {
	for (int r = 0; r < matrix->rows; r++) {
		for (int c = 0; c < matrix->cols; c++) {
			if (matrix->matrix[r][c] != 0) {
				return 0;
			}
		}
	}
	return 1;
}

void matrix_makeIdentity(Matrix* matrix) {
	if (matrix->rows != matrix->cols) {
		return;
	}
	for (int r = 0; r < matrix->rows; r++) {
		for (int c = 0; c < matrix->cols; c++) {
			matrix->matrix[r][c] = r == c;
		}
	}
}

int matrix_isIdentity(Matrix* matrix) {
	if (matrix->rows != matrix->cols) {
		return 0;
	}
	for (int r = 0; r < matrix->rows; r++) {
		for (int c = 0; c < matrix->cols; c++) {
			if (matrix->matrix[r][c] != (r == c)) {
				return 0;
			}
		}
	}
	return 1;
}

int matrix_areEqual(Matrix* m1, Matrix* m2, double tolerance) {
	if (m1->rows != m2->rows || m1->cols != m2->cols) {
		return 0;
	}
	for (int r = 0; r < m1->rows; r++) {
		for (int c = 0; c < m1->cols; c++) {
			if (fabs(m1->matrix[r][c] - m2->matrix[r][c]) >= tolerance) {
				return 0;
			}
		}
	}
	return 1;
}
