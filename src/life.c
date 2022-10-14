#include "life.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "utils.h"
#include <stdlib.h>

// initialize all values inside the struct to -1 or 0
void reset_rules(Rules *rules)
{
    for (int i = 0; i < 8; i++)
    {
        rules->s_rules[i] = -1;
        rules->b_rules[i] = -1;
    }
}

// Still unused : Copy a world to another
void copy(int height, int width, bool to[height][width], bool from[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            to[i][j] = from[i][j];
        }
    }
}

// Still unused : Return true if the two world are the same
bool compare(int height, int width, bool world1[height][width], bool world2[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (world1[i][j] != world2[i][j])
            {
                return false;
            }
        }
    }
    return true;
}

// Fill randomly the world with dead or living cell
void populate_world(int height, int width, bool world[height][width], int one_chance_over)
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            world[i][j] = random_int(1, one_chance_over) == 1 ? true : false;
        }
    }
}

// Clear the terminal and prints the world
void display_world(int height, int width, bool world[height][width])
{
    clear_terminal();
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (world[i][j])
                printf("\x1b[47m  \x1b[0m");
            else
                printf("\x1b[40m  \x1b[0m");
        }
        printf("\n");
    }
    fflush(stdout);
}

// Unused : Kill all cells in a world
void reset_world(int height, int width, bool world[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            world[i][j] = false;
        }
        printf("\n");
    }
}

// Allow the user to edit the world from the terminal by entering x and y coordinates
void user_edit_world(int height, int width, bool world[height][width])
{
    bool ok = false;
    printf("Press enter to start\n");
    while (!ok)
    {
        display_world(height, width, world);

        char res[10];
        do
        {
            scanf("%*[^\n][\n]{0,}");
            printf("Enter OK when you are done, press Ctrl+C to exit\n");
            printf("Insert a pixel with its coordinates (x:y) -> ");
        } while (scanf("%9s", res) != 1);
        if (strncmp(res, "OK", strlen(res)) == 0)
            return;
        char *e = strchr(res, ':');
        if (e == NULL)
        {
            continue;
        }

        int separator_pos = e - res;
        res[separator_pos] = '\0';
        int x = atoi(res);
        int y = atoi(&res[separator_pos + 1]);

        world[y][x] = true;
    }
}

// Returns the number of living cells in a rayon of 1 cell around the cell at row:col parameters
int get_neighbors(int height, int width, bool world[height][width], int row, int col)
{
    int neighbors = 0;

    for (int i = -1; i < 2; i++)
    {
        for (int j = -1; j < 2; j++)
        {
            if (row + i < 0 || row + i > height - 1 || col + j < 0 || col + j > width - 1 || (i == 0 && j == 0))
            {
                continue;
            }
            if (world[row + i][col + j])
            {
                neighbors++;
            }
        }
    }

    return neighbors;
}

// Update the world with the next logic generation
void next_generation(int height, int width, bool world[height][width], Rules rules)
{
    bool temp[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            temp[i][j] = false;
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int neighbors = get_neighbors(height, width, world, i, j);
            if (world[i][j])
            {
                // If the cell is alive
                if (!in_array(neighbors, rules.s_rules))
                {
                    // Kill the cell
                    temp[i][j] = false;
                }
                else
                {
                    temp[i][j] = true;
                }
            }
            else
            {
                if (in_array(neighbors, rules.b_rules))
                {
                    // A cell is born!
                    temp[i][j] = true;
                }
            }
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            world[i][j] = temp[i][j];
        }
    }
}

// add a square of random cells at the center
void add_rsc(int height, int width, bool world[height][width], int rs_width, int rs_height, int rsc_one_chance_over)
{
    int center_x = width / 2;
    int center_y = height / 2;
    int start_x = center_x - rs_width / 2;
    int start_y = center_y - rs_height / 2;

    for (int y = 0; y < rs_height; y++)
    {
        for (int x = 0; x < rs_width; x++)
        {
            int pos_x = start_x + x;
            int pos_y = start_y + y;
            if (pos_x < width && pos_x >= 0 && pos_y < height && pos_y >= 0 && random_int(1, rsc_one_chance_over) == 1)
            {
                world[pos_y][pos_x] = true;
            }
        }
    }
}