//
//Author: Rimsha Rizvi
//


/** #include statements... **/

#include <stdio.h>

#include <stdlib.h>

#include <time.h>

#include <ctype.h>

#include "bpgame.h"



/** TYPEDEFS / STRUCTs HERE ***/

typedef struct bpgame{



  char mal[MAX_ROWS][MAX_COLS];

  char pos;  // position to pop balloon

  int numrows;

  int numcols;

  int score;

  int undo_val;



} BPGame;





/*** IMPLEMENTATION OF bp_XXXX FUNCTIONS HERE  ****/

#define None '.'    // "air"

#define Red  '^'

#define Blue '='

#define Green 'o'

#define Yellow '+'





/**

* Function:  bp_create

* Parameters: number of rows and columns

* Description: returns pointer to newly created

*              board with given number of rows

*              and columns.

*

*              Populates board with random balloons.

*

*              If nrows or ncols is out of range,

*              an error message is printed to stderr

*              and NULL is returned.

*

*/

extern BPGame * bp_create(int numrows, int numcols){



  if ((numrows < 1) || (numrows > MAX_ROWS) || (numcols < 1) || (numcols > MAX_COLS)){

    fprintf (stderr, "\n**ERROR\n");

    return NULL;

  }



    BPGame* bp = (BPGame*) malloc(sizeof(BPGame));

    bp->numrows = numrows;

    bp->numcols = numcols;

    bp->score = 0;

    bp->undo_val = 0;



  srand(time(NULL));



  for(int j = 0; j < bp->numrows; j++){

    for(int c = 0; c < bp->numcols; c++){

      int rn = rand();

    if (rn % 4 == 0) bp->mal[j][c] = Red;

      else if (rn % 4 == 1) bp->mal[j][c] = Blue;

      else if (rn % 4 == 2) bp->mal[j][c] = Green;

      else if (rn % 4 == 3) bp->mal[j][c] = Yellow;

    }

  }

  return bp;

}



extern BPGame * bp_create_from_mtx(char mtx[][MAX_COLS], int numrows, int numcols){



  if ((numrows < 1) || (numrows > MAX_ROWS) || (numcols < 1) || (numcols > MAX_COLS)){

    fprintf (stderr, "\n**ERROR\n");

    return NULL;

  }



  for(int j = 0; j < numrows; j++){

    for(int c = 0; c < numcols; c++){

      if((mtx[j][c] != Red ) && (mtx[j][c] != Blue ) && (mtx[j][c] != Green ) && (mtx[j][c] != Yellow)){

          fprintf (stderr, "\n**ERROR\n");

          return NULL;

      }

    }

  }

  BPGame* bp = (BPGame*) malloc(sizeof(BPGame));

  bp->numrows = numrows;

  bp->numcols = numcols;

  bp->score = 0;

  for(int j = 0; j < bp->numrows; j++){

    for(int c = 0; c < bp->numcols; c++){

      bp->mal[j][c] = mtx[j][c];

      bp->undo_val++;

    }

  }

  return bp;

}



extern void bp_destroy(BPGame * b){

  free(b);

}





extern void bp_display(BPGame * b){

  for(int j = 0; j < b->numrows; j++ ){

    for(int i = 0; i < b->numcols; i++ ){

      printf("%c", b->mal[i][j]);

    }

    printf("\n");

  }

}



extern void bp_display_STD(BPGame * b){

  for(int i = 0; i < b->numrows; i++ ){

    for(int j = 0; j < b->numcols; j++ ){

      printf("%c", b->mal[i][j]);

    }

    printf("\n");

  }

}



void validPop(BPGame * b, int r, int c, char pos, int *baloon_count) {

  b->mal[r][c] = '.';

  *baloon_count += 1;



  if (r+1 < b->numrows && (pos == b->mal[r+1][c])) {

    validPop(b,r+1,c,pos,baloon_count);

  }

  if (c+1 < b->numcols && (pos == b->mal[r][c+1])) {

    validPop(b,r,c+1,pos,baloon_count);

  }

  if (r-1 >= 0 && (pos == b->mal[r-1][c])) {

    validPop(b,r-1,c,pos,baloon_count);

  }

  if (c-1 >= 0 && (pos == b->mal[r][c-1])) {

    validPop(b,r,c-1,pos,baloon_count);

  }

}





extern int bp_pop(BPGame * b, int r, int c) {

  int baloon_count = 0;

  b->pos = b->mal[r][c];



  if (b->mal[r][c] == '.') {

    return 0;

  }



  if ((b->pos == b->mal[r+1][c]) || (b->pos == b->mal[r][c+1]) || (b->pos == b->mal[r][c-1]) || (b->pos == b->mal[r-1][c])) {

    validPop(b,r,c,b->pos,&baloon_count);  // recursive loop to pop

    b->score += baloon_count * (baloon_count - 1);

    return baloon_count;

  }

  return 0;

}











extern int bp_is_compact(BPGame * b){



  for(int j = 0; j < b->numrows-1; j++ ){

    for(int k = 0; k < b->numcols; k++ ){

      if (b->mal[j][k] == None) {

        if (b->mal[j+1][k] != None) {

          return 0;

        }

      }

    }

  }

  return 1;

}









extern void bp_float_one_step(BPGame * b){



  for (int i = 0; i < b->numrows-1; i++) {

    for (int j = 0; j < b->numcols; j++) {

      if (b->mal[i][j] == None) {

        if (b->mal[i+1][j] != None) {

          b->mal[i][j] = b->mal[i+1][j];

          b->mal[i+1][j] = None;

        }

      }

    }

  }

}





extern int bp_score(BPGame * b){

  return b->score;

}





extern int bp_get_balloon(BPGame * b, int r, int c){



 if ((r < 0) || (r >= MAX_ROWS) || (c < 0) || (c >= MAX_COLS)){

  fprintf (stderr, "\n**ERROR\n");

  return -1;

}

 return b->mal[r][c];

}





extern int bp_can_pop(BPGame * b){

  int valid = 0;

  for (int x = 0; x < b->numrows; x++) {

    for (int y = 0; y < b->numcols; y++) {

      if(b->mal[x][y] != None) {

        // going to compare x-value baloon positions

        if (x-1 >= 0){

          if (b->mal[x][y] == b->mal[x-1][y]){

            valid = 1;

            return valid;

          }

        }

        if (x+1 < b->numrows){

          if (b->mal[x][y] == b->mal[x+1][y]){

            valid = 1;

            return valid;

          }

        }

        // going to compare y-value baloon positions

        if (y-1 >= 0) {

          if (b->mal[x][y] == b->mal[x][y-1]) {

            valid = 1;

            return valid;

          }

        }

        if (y+1 < b->numcols) {

          if (b->mal[x][y] == b->mal[x][y+1]) {

            valid = 1;

            return valid;

          }

        }

      }

    }

  }

  return valid;

}





extern int bp_undo(BPGame * b){

  if (b->undo_val >= 1) {

    b->undo_val--;

    return 1;

  }

  return 0;

}
