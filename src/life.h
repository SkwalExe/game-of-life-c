#ifndef LIFE
#define LIFE
#include <stdbool.h>

typedef struct
{
    char b_rules[8];
    char s_rules[8];
} Rules;
void reset_rules(Rules *rules);
void copy(int height, int width, bool to[height][width], bool from[height][width]);
bool compare(int height, int width, bool world1[height][width], bool world2[height][width]);
void populate_world(int height, int width, bool world[height][width], int one_chance_over);
void display_world(int height, int width, bool world[height][width]);
void reset_world(int height, int width, bool world[height][width]);
void user_edit_world(int height, int width, bool world[height][width]);
int get_neighbors(int height, int width, bool world[height][width], int row, int col);
void next_generation(int height, int width, bool world[height][width], Rules rules);
void add_rsc(int height, int width, bool world[height][width], int rs_width, int rs_height, int rsc_one_chance_over);

#endif