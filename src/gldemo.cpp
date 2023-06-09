//
// Array Visualisation for CIS6007 WRIT1
//

#include <iostream>
#include <string>
#include <random>
#include "gl_stuff.h"
#include "Array2D.h"
#include <tbb/tbb.h>
#include <chrono>

using namespace std;
using namespace tbb;

#pragma region Function Prototypes

void initPattern(Array2D<int>& array); // Initialise grid with "alive" and "dead" cell patterns
void update(void); // Main update function - apply Game of Life rules here

#pragma endregion


//
// My Array
//
Array2D<int> myArray = Array2D<int>(32, 32);


// Starting point for our program...
int main(int argc, char* argv[])
{
	// Initialise OpenGL window...
	gl_init(argc, argv, update, "CIS6007 WRIT1 - Game of Life Demo", true);

	// Initialise our Game of Life starting pattern...
	initPattern(myArray);

	// Start the main loop - this calls our update function (below) to update our game of life generations.
	// Note: We never return from glutMainLoop until we quit the application
	glutMainLoop();

	return 0;

}


// Initialise the grid with a random pattern of "alive" and "dead" cells
void initPattern(Array2D<int>& array) {
	// 1. First clear the array (set all cells to "dead" ie. 0)
	for (int i = 0; i < array.w * array.h; i++) {
		int x = i % array.w;
		int y = i / array.h;
		array(x, y) = 0;
	}

	// 2. Randomly set select cells to be "alive"
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, 1);
	for (int i = 0; i < array.w * array.h; i++) {
		int x = i % array.w;
		int y = i / array.h;
		if (dis(gen) == 1) {
			array(x, y) = 1;
		}
	}
}


// Main update function - apply Game of Life rules here
// Note: This is called by FreeGLUT's main event loop - YOU DON'T NEED TO CALL THIS - IT'S CALLED FOR YOU!!!//


// This is the count for the alive neighbours 

// Count the number of alive neighbors for the current cell based on teh rules of Conway's Game of Life 
int count_live_neighbour_cell(int x, int y, Array2D<int>& array) {
	int count = 0;
	int row = array.h;
	int col = array.w;

	// Check the neighboring cells
	for (int i = x - 1; i <= x + 1; i++) {
		for (int j = y - 1; j <= y + 1; j++) {
			// Ignore the current cell
			if (i == x && j == y) continue;

			// Check if the neighboring cell is within the bounds of the array
			if (i >= 0 && i < row && j >= 0 && j < col) {
				// Count alive neighboring cells
				if (array(i, j) == 1 || array(i, j) == 2) {
					// Update count for if the cell is alive or was alive in the previous generation
					count++;
				}
			}
		}
	}
	return count;
}

	// *** ADD SEQUENTIAL OR PARALLEL CODE HERE TO UPDATE GAME OF LIFE *** //
void update(void) {
	Array2D<int> nextGeneration = Array2D<int>(myArray.w, myArray.h);
	auto start_time = std::chrono::high_resolution_clock::now(); // Start the measuring time

	// Parallel loop through all the cells and apply the rules of Conway's Game of Life
	parallel_for(blocked_range<int>(0, myArray.w * myArray.h), [&](const blocked_range<int>& range) {
		for (int i = range.begin(); i < range.end(); i++) {
			int x = i % myArray.w;
			int y = i / myArray.w;
			// Count the number of live neighbors for the current cell
			int liveNeighbors = count_live_neighbour_cell(x, y, myArray);

			// Apply the rules of the Game of Life
			if (myArray(x, y) == 1) {
				// Cell is alive
				if (liveNeighbors < 2 || liveNeighbors > 3) {
					// Cell dies
					nextGeneration(x, y) = 0;
				}
				else {
					// Cell remains alive
					nextGeneration(x, y) = 1;
				}
			}
			else {
				// Cell is dead
				if (liveNeighbors == 3) {
					// Cell becomes alive
					nextGeneration(x, y) = 1;
				}
				else {
					// Cell remains dead
					nextGeneration(x, y) = 0;
				}
			}
		}
		});

	// Copy the next generation to the current generation
	myArray = nextGeneration;

	// End measuring time and calculate duration
	auto end_time = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

	std::cout << "Update function took " << duration.count() << " microseconds." << std::endl;

	// Redraw the screen to show latest generation
	renderLatestGeneration(myArray);


	//HERE IS MY SEQUENTIAL VERSION - COMMENTED OUT

	//void update(void) {
		//Array2D<int> nextGeneration = Array2D<int>(myArray.w, myArray.h);
		//auto start_time = std::chrono::high_resolution_clock::now(); // Start the measuring time


		// Loop through all the cells and apply the rules of Conway's Game of Life
		//for (int i = 0; i < myArray.w; i++) {
			//for (int j = 0; j < myArray.h; j++) {
				// Count the number of live neighbors for the current cell
				//int liveNeighbors = count_live_neighbour_cell(i, j, myArray);

				// Apply the rules of the Game of Life
				//if (myArray(i, j) == 1) {
					// Cell is alive
					//if (liveNeighbors < 2 || liveNeighbors > 3) {
						// Cell dies
						//nextGeneration(i, j) = 0;
					//}
					//else {
						// Cell remains alive
						//nextGeneration(i, j) = 1;
					//}
				//}
				//else {
					// Cell is dead
					//if (liveNeighbors == 3) {
						// Cell becomes alive
						//nextGeneration(i, j) = 1;
					//}
					//else {
						// Cell remains dead
						//nextGeneration(i, j) = 0;

						// End measuring time and calculate duration
						//auto end_time = std::chrono::high_resolution_clock::now();
						//auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

						//std::cout << "Update function took " << duration.count() << " microseconds." << std::endl;
					//}
				//}
			//}
		//}

		// Copy the next generation to the current generation
		//myArray = nextGeneration;

		// Redraw the screen to show latest generation
		//renderLatestGeneration(myArray);

	//}

}




	





	



	




