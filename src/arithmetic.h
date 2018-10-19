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

#ifndef ARITHMETIC_H
#define ARITHMETIC_H

/**
 * Adds two matrices. If the matrices are not of equal size,
 * the arguments are left unchanged.
 * @param dst Destination matrix in which to store the result (can be one of the operands)
 * @param m1 First matrix
 * @param m2 Second matrix
 */
void matrix_add(Matrix* dst, Matrix* m1, Matrix* m2);

/**
 * Multiplies a matrix by a scalar quantity
 * @param dst Destination matrix in which to store the result (can be the operand)
 * @param matrix Matrix to multiply
 * @param scale Scalar value
 */
void matrix_multiplyScalar(Matrix* dst, Matrix* matrix, double scale);

/**
 * Multiplies two matrices. If the matrices do not have compatible dimensions,
 * the arguments are left unchanged.
 * @param dst Destination matrix in which to store the result (cannot be either of the operands)
 * @param m1 First matrix
 * @param m2 Second matrix
 */
void matrix_multiplyMatrix(Matrix* dst, Matrix* m1, Matrix* m2);

#endif
