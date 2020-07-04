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

#include "arithmetic.h"

void matrix_add(Matrix* dst, const Matrix* m1, const Matrix* m2) {
	if (m1->rows != m2->rows || m1->cols != m2->cols) {
		return;
	}
	for (int r = 0; r < m1->rows; r++) {
		for (int c = 0; c < m1->rows; c++) {
			dst->matrix[r][c] = m1->matrix[r][c] + m2->matrix[r][c];
		}
	}
}

void matrix_multiplyScalar(Matrix* dst, const Matrix* matrix, double scale) {
	for (int r = 0; r < matrix->rows; r++) {
		for (int c = 0; c < matrix->cols; c++) {
			dst->matrix[r][c] = matrix->matrix[r][c] * scale;
		}
	}
}

void matrix_multiplyMatrix(Matrix* dst, const Matrix* m1, const Matrix* m2) {
	// matrices cannot be multiplied
	if (m1->cols != m2->rows) {
		return;
	}
	// destination matrix is of the wrong size
	if (dst->rows != m1->rows || dst->cols != m2->cols) {
		return;
	}
	for (int r = 0; r < dst->rows; r++) {
		for (int c = 0; c < dst->cols; c++) {
			double sum = 0;
			for (int i = 0; i < m1->cols; i++) {
				sum += m1->matrix[r][i] * m2->matrix[i][c];
			}
			dst->matrix[r][c] = sum;
		}
	}
}
