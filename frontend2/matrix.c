//Copyright (C) 2019 Arc676/Alessandro Vinciguerra <alesvinciguerra@gmail.com>

//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation (version 3).

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.

//You should have received a copy of the GNU General Public License
//along with this program. If not, see <http://www.gnu.org/licenses/>.

#define MATRIX_MEMORY_SIZE 50

#include <stdio.h>
#include <string.h>
#include "libmatrix.h"

void printMatrix(Matrix* m) {
	for (int r = 0; r < m->rows; r++) {
		for (int c = 0; c < m->cols; c++) {
			printf("%lf ", m->matrix[r][c]);
		}
		printf("\n");
	}
}

Matrix* inputMatrix() {
	int rows, cols;
	printf("Row, Col: ");
	scanf("%d %d", &rows, &cols);
	Matrix* m = matrix_createMatrix(rows, cols);
	for (int r = 0; r < rows; r++) {
		for (int c = 0; c < cols; c++) {
			scanf("%lf", &(m->matrix[r][c]));
		}
	}
	getc(stdin);
	return m;
}

Matrix* eval(char* expr, Matrix** matrices, char** progress) {
	Matrix* res = 0;
	char* saveptr;
	if (progress) {
		saveptr = *progress;
	}
	char* token = progress ? strtok_r(NULL, " ", &saveptr) : strtok_r(expr, " ", &saveptr);
	switch (token[0]) {
		case '+':
		case '-':
		case '*':
		{
			Matrix* left = eval(expr, matrices, &saveptr);
			Matrix* right = eval(expr, matrices, &saveptr);
			if (token[0] == '-') {
				matrix_multiplyScalar(right, right, -1);
			}

			res = matrix_createMatrix(left->rows, right->cols);
			if (token[0] == '*') {
				matrix_multiplyMatrix(res, left, right);
			} else {
				matrix_add(res, left, right);
			}

			matrix_destroyMatrix(left);
			matrix_destroyMatrix(right);
			break;
		}
		case '^':
		{
			Matrix* m1 = eval(expr, matrices, &saveptr);
			token = strtok_r(NULL, " ", &saveptr);
			int power = (int)strtol(token, (char**)NULL, 0);
			if (power == 0) {
				res = matrix_createIdentityMatrix(m1->rows);
				matrix_destroyMatrix(m1);
				break;
			} else if (power < 2) {
				res = m1;
				break;
			}
			Matrix* m2 = matrix_copyMatrix(m1);
			Matrix* mp = matrix_copyMatrix(m1);
			for (int i = 2; i <= power; i++) {
				matrix_multiplyMatrix(mp, m1, m2);
				matrix_copyEntries(m1, mp);
			}
			res = mp;
			matrix_destroyMatrix(m1);
			matrix_destroyMatrix(m2);
			break;
		}
		case 'i':
		{
			Matrix* m1 = eval(expr, matrices, &saveptr);
			matrix_invert(m1, m1, NULL, NULL);
			res = m1;
			break;
		}
		case 't':
		{
			Matrix* m1 = eval(expr, matrices, &saveptr);
			res = matrix_createMatrix(m1->cols, m1->rows);
			matrix_transpose(res, m1);
			matrix_destroyMatrix(m1);
			break;
		}
		default:
		{
			int idx = (int)strtol(token + 1, (char**)NULL, 0);
			res = matrix_copyMatrix(matrices[idx]);
			break;
		}
	}
	if (progress) {
		*progress = saveptr;
	}
	return res;
}

int main(int argc, char* argv[]) {
	printf("Matrix Calculator\nAvailable under GPLv3. See LICENSE for more details.\n");
	char input[200];
	Matrix** memory = malloc(MATRIX_MEMORY_SIZE * sizeof(Matrix*));
	memset(memory, 0, MATRIX_MEMORY_SIZE * sizeof(Matrix*));
	while (1) {
		printf("\n> ");
		fgets(input, sizeof(input), stdin);
		if (!strncmp(input, "set m", 5)) {
			int idx = (int)strtol(input + 5, (char**)NULL, 0);
			if (idx >= 0 && idx < MATRIX_MEMORY_SIZE) {
				if (memory[idx]) {
					matrix_destroyMatrix(memory[idx]);
				}
				memory[idx] = inputMatrix();
			} else {
				printf("Can only save %d matrices\n", MATRIX_MEMORY_SIZE);
			}
		} else if (!strncmp(input, "calc m", 6)) {
			int idx = (int)strtol(input + 6, (char**)NULL, 0);
			if (idx >= 0 && idx < MATRIX_MEMORY_SIZE) {
				if (memory[idx]) {
					matrix_destroyMatrix(memory[idx]);
				}
				fgets(input, sizeof(input), stdin);
				memory[idx] = eval(input, memory, NULL);
				printMatrix(memory[idx]);
			} else {
				printf("Can only save %d matrices\n", MATRIX_MEMORY_SIZE);
			}
		} else if (!strncmp(input, "exit", 4)) {
			printf("Exiting...\n");
			break;
		} else {
			Matrix* matrix = eval(input, memory, NULL);
			if (matrix) {
				printMatrix(matrix);
				matrix_destroyMatrix(matrix);
			} else {
				printf("No result\n");
			}
		}
		memset(input, 0, sizeof(input));
	}
	for (int i = 0; i < MATRIX_MEMORY_SIZE; i++) {
		if (memory[i]) {
			matrix_destroyMatrix(memory[i]);
		}
	}
	free(memory);
	return 0;
}
