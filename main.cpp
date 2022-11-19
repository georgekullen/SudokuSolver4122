/*
Author: George Kullen
Class: ECE4122
Last Date Modified: 9/26/22
Description:
https://stackoverflow.com/questions/30768216/c-stdvector-of-independent-stdthreads
reference used for multithreading above.
Accepts inputs in command line of file containing unsolved sudoku puzzles
solves them as fast as possible with multithreading and outputs solutions in a file
in the same format, unordered.
*/

#include <iostream>
#include <string>
#include <fstream>
#include "SudokuGrid.cpp"
#include <mutex>
#include <thread>
#include <vector>

using namespace std;

mutex outFileMutex;
mutex inFileMutex;
fstream outFile;
fstream inFile;

/*
* SolveSudokuPuzzles
* reads in input file, solves puzzle, and outputs to output file
* while managing mutex locks and unlocks.
*
* input: none
* output: solved sudoku puzzles in outfile
* return: none
*/
void SolveSudokuPuzzles() {
	SudokuGrid sudoku;
    do
    {
		// In file
		inFileMutex.lock(); // lock infile with infilemutex
		//check to see if end of file has been reached
		if (inFile.eof()) 
		{
			inFileMutex.unlock(); //unlock if end of file reached and break
			break;
		}
		else
		{
			inFile >> sudoku; //read inFile into sudoku object
			inFileMutex.unlock();
		}
		
		if (sudoku.solve()) //if solved output solved puzzle to outfile
		{
			//lock, output file, then unlock
			outFileMutex.lock();
			outFile << sudoku;
			outFileMutex.unlock();
		}
	}
	while (true); //infinite until end of file
}

int main(int argc, char *argv[])
{
	std::vector<std::thread> threadVector;
	unsigned int numThreads = thread::hardware_concurrency(); //check hardware for appropriate numThreads

	inFile.open(argv[1], ios::in); //open command arg inputfile
	outFile.open("output_sudoku.txt", ios::out); //create / open output_sudoku.txt

	for (int i = 0; i < numThreads - 1; i++) //initalize numThreads - 1;
	{
		threadVector.emplace_back([&]() {SolveSudokuPuzzles();});
	}
	for (auto& t : threadVector) //join threads
	{
		t.join();
	}
	//close input and output files
	inFile.close();
	outFile.close();
}