#include <iostream>
#include "SudokuSolver.h"

using namespace std;

void SudokuSolver::solve(int grid[][9]) {
	int guessReplace[2] = { 0, 0 };
	int emptyReplace[2] = { 0, 0 };
	Guess origGrid(grid, guessReplace, emptyReplace, grid[0][0]); //This Guess object serves as the original grid. Makes the following logic easier
	guesses.push_back(origGrid);

	while (true) {
		fill_loop(guesses.back().grid);
		if (is_solved(guesses.back().grid)) {
			for (int r = 0; r < 9; r++) {
				for (int c = 0; c < 9; c++) {
					grid[r][c] = guesses.back().grid[r][c];
				}
			}
			return;
		}

		make_guess(guesses.back().grid);
		fill_loop(guesses.back().grid);
		if (contains_errors(guesses.back().grid)) {
			guesses.at(guesses.size() - 2).grid[guesses.back().emptyCoordinate[0]][guesses.back().emptyCoordinate[1]] = guesses.back().guessDigit;
			guesses.pop_back();
		}
	}
}

void SudokuSolver::print(const int grid[][9]) const {
	for (int r = 0; r < 9; r++) {
		for (int c = 0; c < 9; c++) {
			cout << grid[r][c] << " ";
			if ((c + 1) % 3 == 0 && c != 8) {
				cout << "| ";
			}
		}
		cout << endl;
		if ((r + 1) % 3 == 0 && r != 8) {
			cout << "------ ------- ------" << endl;
		}
	}
	cout << endl;
}

bool SudokuSolver::fill(int grid[][9]) const {
	bool changed = false;

	for (int digit = 1; digit <= 9; digit++) { //Loops through all the digits 1-9
		int boolbox[9][9]; //Make a grid of true or false values. If a value is true, it's possible a digit might be there
		for (int r = 0; r < 9; r++) { //Set the grid to all true values, except for the spots where digits are already located
			for (int c = 0; c < 9; c++) {
				if (grid[r][c] == 0) {
					boolbox[r][c] = true;
				}
				else {
					boolbox[r][c] = false;
				}
			}
		}
		for (int r = 0; r < 9; r++) { //Set true values to false if there is a digit on the same row/column/box
			for (int c = 0; c < 9; c++) {
				if (grid[r][c] == digit) {
					for (int i = 0; i < 9; i++) {
						boolbox[r][i] = false;
					}
					for (int i = 0; i < 9; i++) {
						boolbox[i][c] = false;
					}
					for (int row = (r / 3) * 3; row < (r / 3) * 3 + 3; row++) {
						for (int column = (c / 3) * 3; column < (c / 3) * 3 + 3; column++) {
							boolbox[row][column] = false;
						}
					}
				}
			}
		}

		int boolsum;
		int trueCoord[2];

		for (int r = 0; r < 9; r++) { //Search the rows for one true value
			boolsum = 0;
			for (int c = 0; c < 9; c++) {
				boolsum += boolbox[r][c];
				if (boolbox[r][c] == true) {
					trueCoord[0] = r;
					trueCoord[1] = c;
				}
			}
			if (boolsum == 1) {
				changed = true;
				grid[trueCoord[0]][trueCoord[1]] = digit;
			}
		}

		for (int c = 0; c < 9; c++) { //Search the columns for one true value
			boolsum = 0;
			for (int r = 0; r < 9; r++) {
				boolsum += boolbox[r][c];
				if (boolbox[r][c] == true) {
					trueCoord[0] = r;
					trueCoord[1] = c;
				}
			}
			if (boolsum == 1) {
				changed = true;
				grid[trueCoord[0]][trueCoord[1]] = digit;
			}
		}

		for (int box = 0; box < 9; box++) { //Search the boxes for one true value
			boolsum = 0;
			for (int r = 0 + ((box * 3) % 9); r < 3 + ((box * 3) % 9); r++) {
				for (int c = 0 + (box / 3) * 3; c < 3 + (box / 3) * 3; c++) {
					boolsum += boolbox[r][c];
					if (boolbox[r][c] == true) {
						trueCoord[0] = r;
						trueCoord[1] = c;
					}
				}
			}
			if (boolsum == 1) {
				changed = true;
				grid[trueCoord[0]][trueCoord[1]] = digit;
			}
		}
	}

	return changed;
}

void SudokuSolver::fill_loop(int grid[][9]) const {
	while (fill(grid)) {}
}

bool SudokuSolver::is_solved(const int grid[][9]) const {

	for (int r = 0; r < 9; r++) { //Check if all rows have numbers 1-9
		for (int digit = 1; digit <= 9; digit++) {
			bool found = false;
			for (int c = 0; c < 9; c++) {
				if (grid[r][c] == digit) {
					found = true;
					break;
				}
			}
			if (found == false) {
				return false;
			}
		}
	}

	for (int c = 0; c < 9; c++) { //Check if all columns have numbers 1-9
		for (int digit = 1; digit <= 9; digit++) {
			bool found = false;
			for (int r = 0; r < 9; r++) {
				if (grid[r][c] == digit) {
					found = true;
					break;
				}
			}
			if (found == false) {
				return false;
			}
		}
	}

	for (int box = 0; box < 9; box++) { //Check if all boxes have numbers 1-9
		for (int digit = 1; digit <= 9; digit++) {
			bool found = false;
			for (int r = 0 + ((box * 3) % 9); r < 3 + ((box * 3) % 9); r++) {
				for (int c = 0 + (box / 3) * 3; c < 3 + (box / 3) * 3; c++) {
					if (grid[r][c] == digit) {
						found = true;
						break;
					}
				}
			}
			if (found == false) {
				return false;
			}
		}
	}

	return true;
}

void SudokuSolver::make_guess(const int grid[][9]) {
	for (int digit = 1; digit <= 9; digit++) { //Loops through all the digits 1-9
		int boolbox[9][9]; //Make a grid of true or false values. If a value is true, it's possible a digit might be there
		for (int r = 0; r < 9; r++) { //Set the grid to all true values, except for the spots where digits are already located
			for (int c = 0; c < 9; c++) {
				if (grid[r][c] == 0) {
					boolbox[r][c] = true;
				}
				else {
					boolbox[r][c] = false;
				}
			}
		}
		for (int r = 0; r < 9; r++) { //Set true values to false if there is a digit on the same row/column/box
			for (int c = 0; c < 9; c++) {
				if (grid[r][c] == digit) {
					for (int i = 0; i < 9; i++) {
						boolbox[r][i] = false;
					}
					for (int i = 0; i < 9; i++) {
						boolbox[i][c] = false;
					}
					for (int row = (r / 3) * 3; row < (r / 3) * 3 + 3; row++) {
						for (int column = (c / 3) * 3; column < (c / 3) * 3 + 3; column++) {
							boolbox[row][column] = false;
						}
					}
				}
			}
		}

		int boolsum;
		int guessCoord[2] = { -1, -1 };
		int emptyCoord[2] = { -1, -1 };

		for (int r = 0; r < 9; r++) { //Search the rows for two true values
			boolsum = 0;
			for (int c = 0; c < 9; c++) {
				boolsum += boolbox[r][c];
				if (boolbox[r][c] == true && boolsum == 1) {
					guessCoord[0] = r;
					guessCoord[1] = c;
				}
				else if (boolbox[r][c] == true && boolsum == 2) {
					emptyCoord[0] = r;
					emptyCoord[1] = c;
				}
			}
			if (boolsum == 2) {
				Guess guess(grid, guessCoord, emptyCoord, digit);
				guesses.push_back(guess);
				return;
			}
		}

		for (int c = 0; c < 9; c++) { //Search the columns for two true values
			boolsum = 0;
			for (int r = 0; r < 9; r++) {
				boolsum += boolbox[r][c];
				if (boolbox[r][c] == true && boolsum == 1) {
					guessCoord[0] = r;
					guessCoord[1] = c;
				}
				else if (boolbox[r][c] == true && boolsum == 2) {
					emptyCoord[0] = r;
					emptyCoord[1] = c;
				}
			}
			if (boolsum == 2) {
				Guess guess(grid, guessCoord, emptyCoord, digit);
				guesses.push_back(guess);
				return;
			}
		}

		for (int box = 0; box < 9; box++) { //Search the boxes for two true values
			boolsum = 0;
			for (int r = 0 + ((box * 3) % 9); r < 3 + ((box * 3) % 9); r++) {
				for (int c = 0 + (box / 3) * 3; c < 3 + (box / 3) * 3; c++) {
					boolsum += boolbox[r][c];
					if (boolbox[r][c] == true && boolsum == 1) {
						guessCoord[0] = r;
						guessCoord[1] = c;
					}
					else if (boolbox[r][c] == true && boolsum == 2) {
						emptyCoord[0] = r;
						emptyCoord[1] = c;
					}
				}
			}
			if (boolsum == 2) {
				Guess guess(grid, guessCoord, emptyCoord, digit);
				guesses.push_back(guess);
				return;
			}
		}
	}
}

bool SudokuSolver::contains_errors(const int grid[][9]) const {

	//Check for a single missing value in a row.
	//If there is one, check along its respective box and column. If the box or column already contains the missing digit, there's an error.
	int zeroColumnIndex;
	for (int r = 0; r < 9; r++) {
		int numOfZeroes = 0;
		int rowSum = 0;
		for (int c = 0; c < 9; c++) {
			rowSum += grid[r][c];
			if (grid[r][c] == 0) {
				numOfZeroes++;
				zeroColumnIndex = c;
			}
		}
		if (numOfZeroes == 1) { //There is a single missing value in a row
			int missingDigit = 45 - rowSum;

			for (int row = 0; row < 9; row++) {
				if (grid[row][zeroColumnIndex] == missingDigit) { //There is a value in the column that matches the row's missing digit
					return true;
				}
			}

			for (int row = (r / 3) * 3; row < (r / 3) * 3 + 3; row++) {
				for (int column = (zeroColumnIndex / 3) * 3; column < (zeroColumnIndex / 3) * 3 + 3; column++) {
					if (grid[row][column] == missingDigit) { //There is a value in the box that matches the row's missing digit
						return true;
					}
				}
			}
		}
		else if (numOfZeroes == 0 && rowSum != 45) { //There is a duplicate value in the row
			return true;
		}
	}

	//Check for a single missing value in a column.
	//If there is one, check along its respective box (no need to check row because rows were already checked). If the box already contains the missing digit, there's an error.
	int zeroRowIndex;
	for (int c = 0; c < 9; c++) {
		int numOfZeroes = 0;
		int columnSum = 0;
		for (int r = 0; r < 9; r++) {
			columnSum += grid[r][c];
			if (grid[r][c] == 0) {
				numOfZeroes++;
				zeroRowIndex = r;
			}
		}
		if (numOfZeroes == 1) { //There is a single missing value in a column
			int missingDigit = 45 - columnSum;

			for (int row = (zeroRowIndex / 3) * 3; row < (zeroRowIndex / 3) * 3 + 3; row++) {
				for (int column = (c / 3) * 3; column < (c / 3) * 3 + 3; column++) {
					if (grid[row][column] == missingDigit) { //There is a value in the box that matches the row's missing digit
						return true;
					}
				}
			}
		}
		else if (numOfZeroes == 0 && columnSum != 45) { //There is a duplicate value in the column
			return true;
		}
	}

	return false;
}