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

#ifdef THREADSAFE
#define PARSE_TOKEN(expr, ptr) strtok_r(expr, " ", ptr)
#else
#define PARSE_TOKEN(expr, ptr) strtok(expr, " ")
#endif

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

int evalFailed = 0;

Matrix* eval(char* expr, Matrix** matrices, char** progress) {
	evalFailed = 0;
	Matrix* res = 0;
	char* saveptr;
	#ifdef THREADSAFE
	if (progress) {
		saveptr = *progress;
	}
	#endif
	char* token = progress ? PARSE_TOKEN(NULL, &saveptr) : PARSE_TOKEN(expr, &saveptr);
	switch (token[0]) {
		case '+':
		case '-':
		case '*':
		{
			Matrix* left = eval(expr, matrices, &saveptr);
			if (evalFailed) return NULL;

			Matrix* right = eval(expr, matrices, &saveptr);
			if (evalFailed) {
				matrix_destroyMatrix(left);
				return NULL;
			}

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
		case '.':
		{
			token = PARSE_TOKEN(NULL, &saveptr);
			double scalar = (double)strtod(token, (char**)NULL);

			Matrix* m1 = eval(expr, matrices, &saveptr);
			if (evalFailed) return NULL;

			res = matrix_createMatrix(m1->rows, m1->cols);
			matrix_multiplyScalar(res, m1, scalar);
			matrix_destroyMatrix(m1);
			break;
		}
		case '^':
		{
			Matrix* m1 = eval(expr, matrices, &saveptr);
			if (evalFailed) return NULL;

			token = PARSE_TOKEN(NULL, &saveptr);
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
		case 'm':
		{
			// if NOT computing the minors, instead copy the matrix with the given index
			if (strcmp(token, "min")) {
				int idx = (int)strtol(token + 1, (char**)NULL, 0);
				if (!matrices[idx]) {
					evalFailed = 1;
					printf("No matrix saved at index %d\n", idx);
					return NULL;
				}
				res = matrix_copyMatrix(matrices[idx]);
				break;
			}
		}
		case 'i':
		case 'd':
		case 'c':
		{
			if (!strcmp(token, "id")) {
				token = PARSE_TOKEN(NULL, &saveptr);
				int size = (int)strtol(token, (char**)NULL, 0);
				res = matrix_createIdentityMatrix(size);
			} else {
				Matrix* m1 = eval(expr, matrices, &saveptr);
				if (evalFailed) return NULL;

				Matrix* minors = matrix_createMatrix(m1->rows, m1->cols);
				Matrix* cofactors = matrix_createMatrix(m1->rows, m1->cols);
				double det = matrix_invert(m1, m1, minors, cofactors);
				switch (token[0]) {
					case 'd':
						res = matrix_createMatrixWithElements(1, 1, det);
						break;
					case 'i':
					default:
						res = matrix_copyMatrix(m1);
						break;
					case 'c':
						res = matrix_copyMatrix(cofactors);
						break;
					case 'm':
						res = matrix_copyMatrix(minors);
						break;
				}
				matrix_destroyMatrix(m1);
				matrix_destroyMatrix(minors);
				matrix_destroyMatrix(cofactors);
			}
			break;
		}
		case 't':
		{
			Matrix* m1 = eval(expr, matrices, &saveptr);
			if (evalFailed) return NULL;

			res = matrix_createMatrix(m1->cols, m1->rows);
			matrix_transpose(res, m1);
			matrix_destroyMatrix(m1);
			break;
		}
		case '?':
		{
			res = inputMatrix();
			break;
		}
		case '=':
		{
			token = PARSE_TOKEN(NULL, &saveptr);
			int idx = (int)strtol(token + 1, (char**)NULL, 0);
			res = eval(expr, matrices, &saveptr);
			if (evalFailed) return NULL;

			if (matrices[idx]) {
				matrix_destroyMatrix(matrices[idx]);
			}
			matrices[idx] = matrix_copyMatrix(res);
			break;
		}
		default:
			printf("Failed to interpret token %s", token);
			evalFailed = 1;
			break;
	}
	#ifdef THREADSAFE
	if (progress) {
		*progress = saveptr;
	}
	#endif
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
		if (!strncmp(input, "exit", 4)) {
			printf("Exiting...\n");
			break;
		} else if (!strncmp(input, "help", 4)) {
			printf("Commands: exit, help\nEBNF:\n\
expression = operator argument [argument]\n\
argument = expression | matrix\n\
matrix = 'm'index | '?'\n\
index = (a number between 0 and 49, inclusive)\n\n\
Available operators:\n\
| Operator | Arguments | Description |\n\
| --- | --- | --- |\n\
| + | 2 matrices | Adds the two matrices together |\n\
| - | 2 matrices | Subtracts the second matrix from the first |\n\
| * | 2 matrices | Multiplies the two matrices in their given order |\n\
| . | 1 real number, then 1 matrix | Multiplies the matrix by the scalar |\n\
| ^ | 1 square matrix, then 1 integer | Computes the matrix to the given power |\n\
| i | 1 square matrix | Computes the inverse of the matrix |\n\
| d | 1 matrix | Computes the determinant of the given matrix |\n\
| min | 1 matrix | Computes the matrix of minors of the given matrix |\n\
| c | 1 matrix | Computes the matrix of cofactors of the given matrix |\n\
| t | 1 matrix | Computes the transpose of the matrix |\n\
| id | 1 integer | Creates an identity matrix of the given size |\n");
			continue;
		}
		Matrix* matrix = eval(input, memory, NULL);
		if (matrix) {
			printMatrix(matrix);
			matrix_destroyMatrix(matrix);
		} else {
			printf("No result\n");
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
