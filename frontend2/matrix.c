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
#include <ctype.h>

#include "exprfix.h"
#include "libmatrix.h"

#include "memory.h"

int isUn(char* str) {
	char c = str[0];
	return c == '~' || c == 'i' || c == 'd' || c == 'm' || c == 'c' || c == 't';
}

int isBin(char* str) {
	char c = str[0];
	return c == '+' || c == '-' || c == '*' || c == '.' || c == '^' || c == '=';
}

void getOpProps(char* str, int* prec, int* left) {
	switch (str[0]) {
		case '+':
			*prec = 10;
			*left = 1;
			break;
		case '-':
			*prec = 10;
			*left = 1;
			break;
		case '*':
			*prec = 20;
			*left = 1;
			break;
		case '^':
			*prec = 30;
			*left = 0;
			break;
		case '.':
			*prec = 20;
			*left = 1;
			break;
		case '=':
			*prec = 40;
			*left = 0;
			break;
	}
	*prec = 0;
	*left = 0;
}

int isValidMatrixName(char* name) {
	char c = name[0];
	if (isalpha(c)) {
		if (c != 'i' && c != 'm' && c != 't'&& c != 'd' && c != 'c') {
			return 1;
		}
	}
	return 0;
}

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

Matrix* eval(char* expr, char** progress) {
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
			Matrix* left = eval(expr, &saveptr);
			if (evalFailed) return NULL;

			Matrix* right = eval(expr, &saveptr);
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

			Matrix* m1 = eval(expr, &saveptr);
			if (evalFailed) return NULL;

			res = matrix_createMatrix(m1->rows, m1->cols);
			matrix_multiplyScalar(res, m1, scalar);
			matrix_destroyMatrix(m1);
			break;
		}
		case '^':
		{
			Matrix* m1 = eval(expr, &saveptr);
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
		case 'i':
		case 'd':
		case 'c':
		{
			if (!strcmp(token, "id")) {
				token = PARSE_TOKEN(NULL, &saveptr);
				int size = (int)strtol(token, (char**)NULL, 0);
				res = matrix_createIdentityMatrix(size);
			} else {
				Matrix* m1 = eval(expr, &saveptr);
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
			Matrix* m1 = eval(expr, &saveptr);
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
			if (!isValidMatrixName(token)) {
				evalFailed = 1;
				printf("Cannot save matrix with name %s\n", token);
				return NULL;
			}
			res = eval(expr, &saveptr);
			if (evalFailed) return NULL;

			saveMatrixWithName(token, matrix_copyMatrix(res));
			break;
		}
		default:
			if (token[strlen(token) - 1] == '\n') {
				token[strlen(token) - 1] = '\0';
			}
			Matrix* stored = getMatrixWithName(token);
			if (!stored) {
				evalFailed = 1;
				printf("Failed to interpret token %s", token);
				return NULL;
			}
			res = matrix_copyMatrix(stored);
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
	int infixMode = 1;
	printf("Matrix Calculator\nAvailable under GPLv3. See LICENSE for more details.\n");
	char input[200];
	memset(input, 0, sizeof(input));
	initMemory();
	while (1) {
		printf("\n> ");
		fgets(input, sizeof(input), stdin);
		input[strlen(input) - 1] = 0;
		if (!strcmp(input, "exit")) {
			printf("Exiting...\n");
			break;
		} else if (!strcmp(input, "help")) {
			printf("Commands: exit, help\nEBNF:\n\
expression = operator argument [argument]\n\
argument = expression | matrix\n\
matrix = (name) | '?'\n\n\
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
| m | 1 matrix | Computes the matrix of minors of the given matrix |\n\
| c | 1 matrix | Computes the matrix of cofactors of the given matrix |\n\
| t | 1 matrix | Computes the transpose of the matrix |\n\
| id | 1 integer | Creates an identity matrix of the given size |\n");
			continue;
		} else if (!strcmp(input, "mode")) {
			infixMode = !infixMode;
			printf("Input mode: %s\n", infixMode ? "infix" : "postfix");
			continue;
		}
		char* postfix = infixMode ? infixToPrefix(input, isBin, isUn, getOpProps) : NULL;
		if (postfix) printf("Converted: %s\n", postfix);
		Matrix* matrix = eval(infixMode ? postfix : input, NULL);
		if (postfix) {
			free(postfix);
		}
		if (matrix) {
			printMatrix(matrix);
			matrix_destroyMatrix(matrix);
		} else {
			printf("No result\n");
		}
		memset(input, 0, sizeof(input));
	}
	clearMemory();
	return 0;
}
