# SudokuSolver

A C++ program that solves Sudoku puzzles

## How To Use/How It Works

To use SudokuSolver, create a 9x9 array of integer values that represent an unsolved grid. Create a new SudokuSolver object and call .solve on the new object with the array as the parameter. After the call, the array will be changed into a solved state.

The algorithm for this Sudoku solver uses two key methods: filling and guessing. The "fill" algorithm tries to fill in the grid as much as possible based on a simple Sudoku algorithm. If the program can't use "fill" anymore and the Sudoku isn't solved, it makes a guess, meaning that the program looks for two missing digits in a row/column/box and randomly fills in one. The program then "fills" in the grid based on this guess, and if any errors arise then it corrects the guess. It repeats this process until the sudoku is solved.
