/*
mazeio.cpp
*/

#include <iostream>
#include <fstream>
#include "mazeio.h"

using namespace std;

/*************************************************
 * read_maze:
 * Read the maze from the given filename into a
 *  2D dynamically  allocated array.
 *
 * Return the pointer to that array.
 * Return NULL (a special address) if there is a problem,
 * opening the file or reading in the integer sizes.
 *************************************************/
char** read_maze(char* filename, int* rows, int* cols)
{
  // open file
  ifstream mazeFile(filename);
  
  // if there is a problem with opening file
  if (!mazeFile.is_open()) {
    return NULL;
  }
  // if there is a problem with reading integer sizes
  if (!(mazeFile >> *rows >> *cols)) {
    return NULL;
  }

  // allocate memory for maze
  char** maze = new char*[*rows];
  for (int i = 0; i < *rows; i++) { 
    maze[i] = new char[*cols];
  }
  // read maze from mazeFile
  int intRows = *rows; // change type for the for loop
  int intCols = *cols; // change type for the for loop
  for (int i = 0; i < intRows; i++){
    for (int j = 0; j < intCols; j++) {
      mazeFile >> maze[i][j];
    }
  }

  mazeFile.close();
  return maze;
}

/*************************************************
 * Print the maze contents to the screen in the
 * same format as the input (rows and columns, then
 * the maze character grid).
 *************************************************/
void print_maze(char** maze, int rows, int cols)
{
  // print maze
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      cout << maze[i][j];
    }
    // seperates rows
    cout << endl;
  }
}

