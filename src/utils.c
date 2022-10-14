//
// Created by skwal on 8/10/22.
//

#include "utils.h"
#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include <stdlib.h>

#if defined(_WIN32)
#include <windows.h>
#elif defined(__linux__)
#include <sys/ioctl.h>
#include <unistd.h>

#endif // Windows/Linux

void get_terminal_size(int *width, int *height)
{
#if defined(_WIN32)
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    *width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    *height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
#elif defined(__linux__)
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    *width = w.ws_col;
    *height = w.ws_row;

#endif // Windows/Linux
}

void sleep_ms(int ms)
{
#if defined(_WIN32)
    Sleep(ms);
#elif defined(__linux__)
    struct timespec ts;
    ts.tv_sec = ms / 1000;
    ts.tv_nsec = (ms % 1000) * 1000000;
    nanosleep(&ts, NULL);
#endif
}

void clear_terminal(void)
{
    printf("\e[1;1H\e[2J");
}

int random_int(int min, int max)
{
    // 0 : uninitialized
    static int initialized = 0;
    if (initialized == 0)
    {
        initialized = 1;
        // Initialize the rand function seed with the current timestamp
        srand((int)clock() + (int)time(NULL));
    }
    return rand() % (max - min + 1) + min;
}

bool in_array(char val, char arr[8])
{
    for (int i = 0; i < 8; i++)
    {
        if (arr[i] == val)
        {
            return true;
        }
    }
    return false;
}