#include <vector>

using namespace std;

struct Guess {
	int grid[9][9];
	int guessCoordinate[2];
	int emptyCoordinate[2];
	int guessDigit;

	Guess(const int grid[][9], const int guessCoordinate[], const int emptyCoordinate[], int guessDigit) : guessDigit(guessDigit) {
		for (int r = 0; r < 9; r++) {
			for (int c = 0; c < 9; c++) {
				this->grid[r][c] = grid[r][c];
			}
		}
		this->grid[guessCoordinate[0]][guessCoordinate[1]] = guessDigit;
		this->guessCoordinate[0] = guessCoordinate[0];
		this->guessCoordinate[1] = guessCoordinate[1];
		this->emptyCoordinate[0] = emptyCoordinate[0];
		this->emptyCoordinate[1] = emptyCoordinate[1];
	}
};

class SudokuSolver {

public:
	void solve(int grid[][9]);
	void print(const int grid[][9]) const;

private:
	bool fill(int grid[][9]) const; //Fills in missing numbers. Returns true if it changes the grid, false if it doesn't
	void fill_loop(int grid[][9]) const; //Keeps filling the grid until it can't anymore
	bool is_solved(const int grid[][9]) const; //Checks if the grid has any zeroes
	void make_guess(const int grid[][9]); //Looks for two missing values and makes a new Guess object, filling one value with a missing digit
	bool contains_errors(const int grid[][9]) const; //Checks if the grid contains duplicate values in the rows, columns, and boxes

	vector<Guess> guesses;

};