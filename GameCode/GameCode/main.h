#ifndef MAIN_H
#define MAIN_H

#include "gba.h"

// TODO: Create any necessary structs

/*
* For example, for a Snake game, one could be:
*
* struct snake {
*   int heading;
*   int length;
*   int row;
*   int col;
* };
*
* Example of a struct to hold state machine data:
*
* struct state {
*   int currentState;
*   int nextState;
* };
*
*/
#define N_CANDIES (2)
#define CHAR_HEIGHT (10)
#define CHAR_WIDTH (10)
#define BENCH_HEIGHT (10)
#define BENCH_WIDTH (10)
// TODO: Create any necessary structs

struct batman1 {
    int x;
    int y;
};

struct bench {
    int x;
    int y;
    int dx;
    int dy;
};

struct state {
    struct batman1 bat;
    int ncandies;
    struct bench candies_display[N_CANDIES];
};

struct state curr, prev;
struct bench *cc, *oc;


#endif
