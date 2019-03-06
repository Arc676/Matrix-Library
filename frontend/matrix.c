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

#include <stdio.h>
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

int main(int argc, char* argv[]) {
	printf("Matrix Calculator\nAvailable under GPLv3. See LICENSE for more details.\n");
	char cmd[200];
	while (1) {
		printf("> ");
		fgets(cmd, sizeof(cmd), stdin);
		cmd[strlen(cmd) - 1] = 0;
		if (!strcmp(cmd, "add")) {
			Matrix* m1 = inputMatrix();
			Matrix* m2 = inputMatrix();
			matrix_add(m1, m1, m2);
			printMatrix(m1);
			matrix_destroyMatrix(m1);
			matrix_destroyMatrix(m2);
		} else if (!strcmp(cmd, "inverse")) {
			Matrix* m = inputMatrix();
			Matrix *min = matrix_createMatrix(m->rows, m->cols), *cof = matrix_createMatrix(m->rows, m->cols);
			double det = matrix_invert(m, m, min, cof);
			printf("Minors:\n");
			printMatrix(min);
			printf("Cofactors:\n");
			printMatrix(cof);
			printf("Inverse:\n");
			printMatrix(m);
			printf("Determinant: %lf\n", det);
			matrix_destroyMatrix(m);
			matrix_destroyMatrix(min);
			matrix_destroyMatrix(cof);
		} else if (!strcmp(cmd, "multiply")) {
			Matrix* m1 = inputMatrix();
			Matrix* m2 = inputMatrix();
			Matrix* md = matrix_createMatrix(m1->rows, m2->cols);
			matrix_multiplyMatrix(md, m1, m2);
			printMatrix(md);
			matrix_destroyMatrix(m1);
			matrix_destroyMatrix(m2);
			matrix_destroyMatrix(md);
		} else if (!strcmp(cmd, "power")) {
			Matrix* m1 = inputMatrix();
			int power;
			printf("Power: ");
			scanf("%d", &power);
			getc(stdin);
			if (power < 2) {
				printf("Doing nothing\n");
				matrix_destroyMatrix(m1);
				continue;
			}
			Matrix* m2 = matrix_copyMatrix(m1);
			Matrix* mp = matrix_copyMatrix(m1);
			for (int i = 2; i <= power; i++) {
				matrix_multiplyMatrix(mp, m1, m2);
				matrix_copyEntries(m1, mp);
			}
			printMatrix(mp);
			matrix_destroyMatrix(m1);
			matrix_destroyMatrix(m2);
			matrix_destroyMatrix(mp);
		} else if (!strcmp(cmd, "similar")) {
			printf("Enter U: ");
			Matrix* U = inputMatrix();
			printf("Enter A: ");
			Matrix* A = inputMatrix();
			Matrix* res = matrix_createMatrix(A->rows, A->cols);
			matrix_multiplyMatrix(res, U, A);
			matrix_invert(U, U, NULL, NULL);
			matrix_copyEntries(A, res);
			matrix_multiplyMatrix(res, A, U);
			printf("UAU^-1:\n");
			printMatrix(res);
			matrix_destroyMatrix(U);
			matrix_destroyMatrix(A);
			matrix_destroyMatrix(res);
		}
		else if (!strcmp(cmd, "exit")) {
			break;
		} else {
			printf("Invalid command\n");
		}
		memset(cmd, 0, sizeof(cmd));
	}
	return 0;
}
