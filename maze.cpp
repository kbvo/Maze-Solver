#include <iostream>
#include "mazeio.h"
#include "queue.h"
using namespace std;

// Prototype for maze_search
int maze_search(char**, int, int);

bool isValid_char(char**, int, int);
bool isValid_maze(char**, int, int);
bool isValid_path(char**, int, int);

// main function to read, solve maze, and print result
int main(int argc, char* argv[]) {
    int rows, cols, result;
    char** mymaze=NULL;
    const char* invalid_char_message = "Error, invalid character.";
    const char* invalid_maze_message = "Invalid maze.";
    const char* no_path_message = "No path could be found!";

    if(argc < 2)
    {
        cout << "Please provide a maze input file" << endl;
        return 1;
    }

    // reads maze into mymaze
    mymaze = read_maze(argv[1], &rows, &cols);

    // if maze has an invalid char
    if (!isValid_char(mymaze, rows, cols)) {
      cout << invalid_char_message << endl;
    }

    // complete maze
    result = maze_search(mymaze, rows, cols);
    
    // if invalid maze (no more or less than 1 start or finish)
    if (result == -1) {
      cout << invalid_maze_message << endl;
    }
    // if no path out of maze
    if (result == 0) {
      cout << no_path_message << endl;
    }
    // if path found
    if (result == 1) {
      cout << rows << " " << cols << endl;
      // print maze
      print_maze(mymaze, rows, cols);
    }

    // deallocate memory
    for (int i = 0; i < rows; i++) {
      delete [] mymaze[i];
    }
    delete [] mymaze;

    return 0;
}

/**************************************************
 * Attempt to find shortest path and return:
 *  1 if successful
 *  0 if no path exists
 *
 * If path is found fill it in with '*' characters
 *  but don't overwrite the 'S' and 'F' cells
 *************************************************/
int maze_search(char** maze, int rows, int cols)
{
  // location of start and finish
  Location startLoc, finishLoc;
  // keeps count of start and finish in maze
  int startCount = 0, finishCount = 0;
    
  // find start and finish cells in maze 
  // and count how many of each
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      if (maze[i][j] == 'S') {
        startLoc.row = i;
        startLoc.col = j;
        startCount++;
      }
      if (maze[i][j] == 'F') {
        finishLoc.row = i;
        finishLoc.col = j;
        finishCount++;
      }
    }
  }
    
  // if maze is not valid (more or less
  // than 1 start and 1 finish in maze)
  if (startCount != 1 || finishCount != 1) {
    return -1;
  }

  // create queue with appropriate size
  Queue myQueue(rows * cols);

  // allocate memory for predecessor and explored 2D arrays
  Location** predecessor = new Location*[rows];
  bool** explored = new bool*[rows];
  for (int i = 0; i < rows; i++) {
    predecessor[i] = new Location[cols];
    explored[i] = new bool[cols];
    for (int j = 0; j < cols; j++) {
      // initialize explored array to false
      explored[i][j] = false;
    }
  }

  // mark start location as explored
  explored[startLoc.row][startLoc.col] = true;
  // add start location to queue
  myQueue.add_to_back(startLoc);

  // BFS search
  while (!myQueue.is_empty()) {
    // item from front of queue is my location
    Location myLoc = myQueue.remove_from_front();
    int myRow = myLoc.row;
    int myCol = myLoc.col;
      
    // initialize array of neighbors with 
    // north, west, south, and east cells of my location
    Location neighborCell[] = {
      {myRow - 1, myCol}, // north
      {myRow, myCol - 1}, // west
      {myRow + 1, myCol}, // south
      {myRow, myCol + 1} // east
    };
      
    // for each neighbor of my location
    for (int i = 0; i < 4; i++) {
      // assign row and location of neighbor
      int neighborRow = neighborCell[i].row;
      int neighborCol = neighborCell[i].col;

      // if neighbor is valid, open, and not yet explored
      if (neighborRow >= 0 && neighborRow < rows && neighborCol >= 0 && neighborCol < cols
      && maze[neighborRow][neighborCol] != '#' && !explored[neighborRow][neighborCol]) {
        // mark neighbor as explored
        explored[neighborRow][neighborCol] = true;
        // add the neighbor to the back of queue
        myQueue.add_to_back(neighborCell[i]);
        // set my location to predecessor of the neighbor
        predecessor[neighborRow][neighborCol] = myLoc;
        // if finish was explored
        if (explored[finishLoc.row][finishLoc.col]) {
          break; // path is found
        }
      }
    }
  }
  
  // Backtracking path and marking it with '*'
  if (explored[finishLoc.row][finishLoc.col]) { // if finish location was explored
    // set current location to predecessor of finish location
    Location currLoc = predecessor[finishLoc.row][finishLoc.col];
    
    // while the current location is not the starting location
    while (currLoc.row != startLoc.row || currLoc.col != startLoc.col) {
      // mark location with '*'
      maze[currLoc.row][currLoc.col] = '*';
      // set current location to predecessor location
      currLoc = predecessor[currLoc.row][currLoc.col];
    }
    
    // deallocate memory
    for (int i = 0; i < rows; i++) {
      delete [] predecessor[i];
      delete [] explored[i];
    }
    delete [] predecessor;
    delete [] explored;
    
    // path found
    return 1;
  }
  else {
  // deallocate memory
  for (int i = 0; i < rows; i++) {
    delete [] predecessor[i];
    delete [] explored[i];
  }
  delete [] predecessor;
  delete [] explored;
    
  // path not found
  return 0;
  }
}

// checks if all chars in maze are valid (S, E, ., or #)
bool isValid_char(char** maze, int rows, int cols) {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      // assigns mazeCell to a char in maze
      char mazeCell = maze[i][j];
      // if char at mazeCell is not valid
      if (mazeCell != 'S' && mazeCell != 'F'
      && mazeCell != '.' && mazeCell != '#') {
        // there is an invalid char in maze
        return false;
      }
    }
  }
  return true; // no invalid char in maze
}
