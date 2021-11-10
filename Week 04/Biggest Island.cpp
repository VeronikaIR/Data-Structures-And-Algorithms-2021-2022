#include <iostream>
#include "vector.h"

const unsigned int rows = 3;
const unsigned int cols = 10;

struct Island {
	char nameOfIsland;
	int sizeOfIsland;
}; 

int findIsland(char matrix[rows][cols],bool visitedCells[rows][cols], int i, int j, char prevSymbol) {

	if (i < 0 || i >= rows || j < 0 || j >= cols) {
		return 0;
	}

	char currentSymbol = matrix[i][j];

	if (currentSymbol == 'x' || visitedCells[i][j] || currentSymbol != prevSymbol) {
		return 0;
	}

	visitedCells[i][j] = true;
	return 1 +
		findIsland(matrix, visitedCells, i - 1, j, currentSymbol) + //visit up
		findIsland(matrix, visitedCells, i + 1, j, currentSymbol) + //visit down
		findIsland(matrix, visitedCells, i, j - 1, currentSymbol) + // visit left
		findIsland(matrix, visitedCells, i, j + 1, currentSymbol);  //visit right
}

Vector<Island> treaverse(char matrix[rows][cols]) {

	Vector<Island> Islands;

	bool visitedCells[rows][cols] = { false };

	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {

			if (matrix[i][j] != 'x' && !visitedCells[i][j]) {
				int sizeOfIsland = findIsland(matrix,visitedCells, i, j, matrix[i][j]);
				Island currIsland = { matrix[i][j], sizeOfIsland };
				Islands.addElement(currIsland);
			}
		}
	}
	return Islands;
}



int main(){

    char matrix[rows][cols] = { {'A', 'A', 'A', 'x', 'B', 'B', 'x', 'C', 'C', 'C'},
								{'x', 'A', 'x', 'B', 'B', 'x', 'x', 'x', 'x', 'x'},
								{'A', 'A', 'x', 'x', 'x', 'x', 'x', 'x', 'D', 'D'} };


	Vector<Island> Islands = treaverse(matrix);

	if (Islands.get_curSize() > 0) {
		Island biggestIsland = Islands[0];
		for (int i = 0; i < Islands.get_curSize(); ++i) {

			std::cout << "Island '" << Islands[i].nameOfIsland << "' has size:" << Islands[i].sizeOfIsland << std::endl;

			if (biggestIsland.sizeOfIsland < Islands[i].sizeOfIsland) {
				biggestIsland = Islands[i];
			}
		}


		std::cout << "==============" << std::endl;
		std::cout << "Total islands: " << Islands.get_curSize() << std::endl;
		std::cout << "Biggest island is '" << biggestIsland.nameOfIsland << "' with length: " << biggestIsland.sizeOfIsland << std::endl;

	}
	else {
		std::cout << "No islands were found" << std::endl;
	}

	return 0;
}
