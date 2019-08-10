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

#include <map>
#include <string>

#include "memory.h"

std::map<std::string, Matrix*> matrixMemory;

void initMemory() {
	matrixMemory = std::map<std::string, Matrix*>();
}

void clearMemory() {
	for (auto it = matrixMemory.cbegin(); it != matrixMemory.cend();) {
		matrix_destroyMatrix(it->second);
		matrixMemory.erase(it++);
	}
}

Matrix* getMatrixWithName(char* name) {
	std::string cppname(name);
	if (matrixMemory.find(cppname) != matrixMemory.end()) {
		return matrixMemory[cppname];
	}
	return 0;
}

void saveMatrixWithName(char* name, Matrix* m) {
	std::string cppname(name);
	if (matrixMemory.find(cppname) != matrixMemory.end()) {
		matrix_destroyMatrix(matrixMemory[cppname]);
	}
	matrixMemory[cppname] = m;
}
