#include <stdio.h>
#include <stdlib.h>
#include "maze.h"

maze * create_maze(FILE *in) {
   maze * mazeIn = malloc( sizeof(maze));
  
  //start line
  char * line = malloc(sizeof(char)*100);   
  // start line string size at 100

  
  int num_rows;
  int num_cols;
  int start_row;
  int start_col;
  int end_row;
  int end_col;
  ssize_t num_read;
  num_read = getline(&line, &num_read, in);
  if(sscanf(line, "%d %d %d %d %d %d", &num_rows, &num_cols, &start_row, &start_col, &end_row, &end_col) != 6) {
    fprintf(stderr, "Maze file format incorrect. Top line must include 6 numbers.\n");
    return NULL;
  } 
  //check for invalid maze parameters
  if (num_rows <= 0) {
    fprintf(stderr, "Invalid row size. Cannot create maze\n");
    return NULL;
  }
  if (num_cols <= 0) {
    fprintf(stderr, "Invlaid column size. Cannot create maze\n");
    return NULL;
  }
  if (start_row < 0 || start_row >= num_rows) {
    fprintf(stderr, "Invalid row start location. Cannot create maze\n");
    return NULL;
  }
  if (start_col < 0 || start_col >= num_cols) {
    fprintf(stderr, "Invalid column start location. Cannot create maze\n");
    return NULL;
  }
  if (end_row < 0 || end_row > num_rows) {
    fprintf(stderr, "Invalid row exit location. Cannot create maze\n");
    return NULL;
  }
  if (end_col < 0 || end_col > num_cols) {
    fprintf(stderr, "Invalid col exit location. Cannot create maze\n");
    return NULL;
  }
 
  //valid tests passed. Assign maze values
  mazeIn->width = num_cols;
  mazeIn->height = num_rows;
  mazeIn->entry.row = start_row;
  mazeIn->entry.col = start_col;
  mazeIn->exit.row = end_row;
  mazeIn->exit.col = end_col;

  //create heap of "data" for mazeIn, 2D array
  int i;
  int j;
  mazeIn->data = (char **)calloc(num_rows, sizeof(char *));
  if(mazeIn->data != NULL) {
    /* allocate memory to store data for each row */ 
    for(i = 0; i < num_rows; i++) {
      mazeIn->data[i] = (char *)calloc(num_cols, sizeof(char));
    }
  }
  
//get each line. Error check the following before storing in data
// 1) That actual rows count matches given maze height
// 2) That actual columns count match given maze width
// 3) That there are no illegal characters in the maze: non ' ' or '*'
int c; //for EOF value storage
for(j = 0; j < num_rows ; j++) {
     num_read = getline(&line, &num_read, in); //read new line
     
     if(num_read != (num_cols +1) ) { //check that this line is valid length (valid # of cols)
       fprintf(stderr, "Error reading maze. Given num_cols is not equal to actual value. Cannot create maze\n");
       return NULL;
     }
  
     for(i = 0; i < num_cols; i++) { //put line inside ith line of mazeIn.data

       if(line[i] != ' ' && line[i] != '*') { //check for illegal character in maze
	fprintf(stderr, "Illegal character that is not space or *. Cannot create maze\n");
	return NULL;
       }

       //debugging code here to print out stored maze in case something is broken
       mazeIn->data[j][i] = line[i];
       //printf("%c",mazeIn->data[j][i]);
    }
    //printf("\n");
}

if( (c = fgetc(in) ) != EOF) {
   fprintf(stderr, "Actual number of rows doesn't equal given maze height. Cannot create maze\n");
   return NULL;
}

printf("success in storing maze!\n\n");
return mazeIn;

}
int solve(int row, int col, maze * the_maze) { //recursive function of 4 cases.
if( (row < 0) || (row >= the_maze->height) || (col < 0) || (col >= the_maze->width)) {//going out of bounds. No solution
   //printf("case 1\n");
   return 0;
}
else if( (row == the_maze->exit.row) && (col == the_maze->exit.col) && (the_maze->data[row][col] == ' ') ) {//found the exit
   //printf("case 2\n");
   return 1;
} 
else if( (the_maze->data[row][col] == '*') ) {//ran into a wall
   //printf("case 3\n");
   return 0;
}
else {
   //printf("case 4\n"); //if there is an empty space, then we can go there! Use recursive function to find possible solutions from there
   the_maze->data[row][col] = '*';
   return (solve(row, col-1, the_maze)) || (solve(row-1, col, the_maze)) || (solve(row, col+1, the_maze)) || (solve(row+1, col, the_maze));
}


}

void free_maze(maze * the_maze) {

//create heap of "data" for mazeIn, 2D array
int i;
   for(i = 0; i < the_maze->height; i++) {
     free(the_maze->data[i]);
   }
free(the_maze->data);
free(the_maze);
}
