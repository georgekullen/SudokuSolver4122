/*
Author: George Kullen
Class: ECE4122
Last Date Modified: 9/26/22
Description:
https://stackoverflow.com/questions/12611465/reading-from-a-file-using-an-overloaded-operator
https://www.tutorialspoint.com/sudoku-solver-in-cplusplus
References for sudoku algorithm and filestream operators above.
SudokuGrid class file.
contains member variables and methods to solve sudoku puzzles.
Also contains overloaded operators for simple file i/o.
*/
#include <string>
#include <fstream>

/*
* SudokuGrid Class
* Manages GridName and gridElement private variables.
* Contains helper functions and sudoku solving function.
*/
class SudokuGrid {
private:
    std::string m_strGridName;
    unsigned char gridElement[9][9];
public:
    bool findEmptyPlace(int& row, int& col);
    bool isValidPlace(int row, int col, char num);
    bool isPresentInCol(int col, char num);
    bool isPresentInRow(int row, char num);
    bool isPresentInBox(int boxStartRow, int boxStartCol, char num);

    SudokuGrid();

    friend std::fstream& operator>>(std::fstream& os, SudokuGrid& gridIn);
    friend std::fstream& operator<<(std::fstream& os, const SudokuGrid& gridOut);

    bool solve();
};

/*
* sudokuGrid
* Constructor. Initializes private member variables.
*
* input: none
* output: none
* return: none
*/
SudokuGrid::SudokuGrid() {
    m_strGridName = 'x'; //initalize m_strGridName
    for (int row = 0; row < 9; row++) { // loop through all rows
        for (int col = 0; col < 9; col++) { // loop through all columns
            gridElement[row][col] = '0'; //initalize all elements with '0'
        }
    }
}

/*
* SudokuGrid::findEmptyPlace
* finds first location with a '0' char and returns true or false if no '0' found.
*
* input: int& row, int& col
* output: none
* return: true or false if '0' is present in grid or not
*/
bool SudokuGrid::findEmptyPlace(int& row, int& col) {
    for (row = 0; row < 9; row++) //loop through all rows
        for (col = 0; col < 9; col++) //loop through all columns
            if (this->gridElement[row][col] == '0') //marked with 0 is empty
                return true; //return true if location is empty
    return false;
}

/*
* SudokuGrid::isValidPlace
* checks if the given row and col location is valid spot for given num.
*
* row: row location to check
* col: col location to check
* num: number to check for
* output: none
* return: true if spot is valid, false otherwise
*/
bool SudokuGrid::isValidPlace(int row, int col, char num) {
    //when item not found in col, row and current 3x3 box
    return !this->isPresentInRow(row, num) && !this->isPresentInCol(col, num) && !this->isPresentInBox(row - row % 3, col - col % 3, num);
}

/*
* SudokuGrid::isPresentInCol
* checks if given num is present already in a given col
* 
* col: col location to check
* num: number to check for
* output: none
* return: true if number is present in col, false if not
*/
bool SudokuGrid::isPresentInCol(int col, char num) {
    for (int row = 0; row < 9; row++) //loop through all rows in given column
        if (this->gridElement[row][col] == num) //check for num at location
            return true;
    return false;
}

/*
* SudokuGrid::isPresentInRow
* checks if given num is present already in a given row
*
* row: row location to check
* num: number to check for
* output: none
* return: true if number is present in row, false if not
*/
bool SudokuGrid::isPresentInRow(int row, char num) { //check whether num is present in row or not
    for (int col = 0; col < 9; col++) //loop through all columns in given row
        if (this->gridElement[row][col] == num) //check for num at location
            return true;
    return false;
}

/*
* SudokuGrid::isPresentInBox
* checks if given num is present already in its surrounding 3x3 box
* dimensions are managed to match sudoku rules when function is called
*
* boxStartRow: starting row location
* boxStartCol: starting column location
* num: number to check for
* output: none
* return: true if number is present in 3x3 box, false if not
*/
bool SudokuGrid::isPresentInBox(int boxStartRow, int boxStartCol, char num) {
    //check whether num is present in 3x3 box or not
    for (int row = 0; row < 3; row++) //loop through section of row
        for (int col = 0; col < 3; col++) //loop through section of col
            if (this->gridElement[row + boxStartRow][col + boxStartCol] == num) //check for num at location
                return true;
    return false;
}

/*
* std::fstream& operator>>
* overloads >> operator for sudokuGrid and fstream
*
* fstream& os: given filestream
* gridIn: sudokuGrid object
* output: puts contents of os into gridIn private member variables
* return: given filestream location
*/
std::fstream& operator>>(std::fstream& os, SudokuGrid& gridIn) {
    std::string whiteSpace; //default initalized string as whitespace
    std::getline(os, gridIn.m_strGridName); //get first line as Grid Name
    for (int row = 0; row < 9; row++) //loop through each row
        os >> gridIn.gridElement[row]; //save each row in GridElement
    std::getline(os, whiteSpace); //get whitespace
    return os; //return filestream
}

/*
* std::fstream& operator<<
* overloads >> operator for sudokuGrid and fstream
*
* fstream& os: given filestream
* gridIn: sudokuGrid object
* output: puts contents of private member variables into file
* return: given filestream location
*/
std::fstream& operator<<(std::fstream& os, const SudokuGrid& gridOut) {
    os << gridOut.m_strGridName << '\n'; // return Grid Name and newline
    for (int row = 0; row < 9; row++) { //loop through rows
        for (int col = 0; col < 9; col++) { //loop through columns
            os << gridOut.gridElement[row][col]; //output chars in Grid to fstream
        }
        os << '\n'; //newline
    }
    return os; //return filestream
}

/*
* SudokuGrid::solve
* recursively solves sudoku gridElement puzzle by checking for
* empty places and testing values and checking their validity
*
* input: none
* output: solve sudoku puzzle in gridElement
* return: true if puzzle is solved
*/
bool SudokuGrid::solve() {
    int row, col; //initalize row, col, and num
    char num;
    if (!this->findEmptyPlace(row, col))
        return true; //when no '0' remains
    for (int numInt = 1; numInt <= 9; numInt++) { //valid numbers are 1 - 9
        num = (char)(numInt + 48); //convert num to char for use in gridEleement
        if (this->isValidPlace(row, col, num)) { //check validation, if yes, put the number in the grid
            this->gridElement[row][col] = num;
            if (solve()) //recursively go for other rooms in the grid
                return true;
            this->gridElement[row][col] = '0'; //turn to unassigned space when conditions are not satisfied
        }
    }
    return false; //return unsolved to recursively loop
}

