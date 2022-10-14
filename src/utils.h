#ifndef GAME_OF_LIFE_UTILS
#define GAME_OF_LIFE_UTILS

#include <stdbool.h>

void get_terminal_size(int *width, int *height);
void sleep_ms(int ms);
void clear_terminal(void);
int random_int(int min, int max);
bool in_array(char val, char arr[8]);

#endif