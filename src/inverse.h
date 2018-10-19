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

#ifndef INVERSE_H
#define INVERSE_H

/**
 * Determine the matrix of minors for a given matrix
 * @param dst Destination matrix in which to store the matrix of minors (cannot be the operand)
 * @param matrix Matrix whose matrix of minors to find
 */
void matrix_minors(Matrix* dst, Matrix* matrix);

/**
 * Determine the matrix of cofactors for a given matrix
 * @param dst Destination matrix in which to store the matrix of cofactors (can be the operand)
 * @param matrix Matrix whose matrix of cofactors to find
 * @param minors Matrix of minors for the given matrix. If NULL, this will be calculated.
 */
void matrix_cofactors(Matrix* dst, Matrix* matrix, Matrix* minors);

/**
 * Determine the determinant of a matrix
 * @param matrix Matrix whose determinant to find
 * @param cofactors Matrix of cofactors for the given matrix. If NULL, this will be calculated.
 * @return Determinant of the matrix
 */
double matrix_determinant(Matrix* matrix, Matrix* cofactors);

/**
 * Determine the inverse of a matrix
 * @param dst Destination matrix in which to store the inverse matrix (can be the operand)
 * @param matrix Matrix whose inverse to find
 * @param minors Matrix of minors for the given matrix. If NULL, this will be calculated.
 * @param cofactors Matrix of cofactors for the given matrix. If NULL, this will be calculated.
 * @return The determinant of the matrix
 */
double matrix_invert(Matrix* dst, Matrix* matrix, Matrix* minors, Matrix* cofactors);

#endif