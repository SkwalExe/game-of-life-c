#include <stdio.h>
#include "utils.h"
#include "life.h"
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

// Use the ESC char (ascii 27) to change the colors of the text in the terminal
#define RED "\033[91m"
#define YELLOW "\033[93m"
#define PURPLE "\033[95m"
#define WHITE "\033[97m"
#define RESET "\033[0m"

#define VERSION "0.1.0"

int get_index(char *string, char c)
{
    char *e = strchr(string, c);
    if (e == NULL)
    {
        return -1;
    }
    return (int)(e - string);
}

// Parameters
int height = 0;
int width = 0;
int delay = 75;
bool random_generation = false;
int one_chance_over = 3;
// random square center
bool rsc = false;
// one change over x to have a live cell for each cell of the rsc
int rsc_one_chance_over = 3;
// size of the rs
int rs_width = -1;
int rs_height = -1;
Rules rules = {
    .b_rules = {3, -1, -1, -1, -1, -1, -1, -1},
    .s_rules = {2, 3, -1, -1, -1, -1, -1, -1}};
// --------------

// Prints the help message
void help(void)
{
    printf("Game of life C\n");
    printf("%s━━━━━━━━━━━━━━━━━%s\n", PURPLE, RESET);
    printf("Author: %sSkwalExe%s\n", PURPLE, RESET);
    printf("Github: %shttps://github.com/SkwalExe/%s\n", PURPLE, RESET);
    printf("%s━━━━━━━━━━━━━━━━━%s\n", PURPLE, RESET);
    printf("C implementation of Conway's game of life algorithm\n");
    printf("%s━━━━━━━━━━━━━━━━━%s\n", PURPLE, RESET);
    printf("Options: \n");
    printf("\t%s-h, --help: %sPrints this message and exits%s\n", PURPLE, YELLOW, RESET);
    printf("\t%s-v, --version: %sPrints the program version and exits%s\n", PURPLE, YELLOW, RESET);
    printf("\t%s-d, --delay: %sSets the delay in ms between each generation step [D: 75]%s\n", PURPLE, YELLOW, RESET);
    printf("\t%s-r, --random: %sGenerate cells randomly%s\n", PURPLE, YELLOW, RESET);
    printf("\t%s-o, --over: %sWhen using -r, define the chances of a cell to be alive by one over [arg] [D: 3]%s\n", PURPLE, YELLOW, RESET);
    printf("\t%s-s, --size: %sIf the automatic size doesn't fit your terminal, specify it manually with this parameter (ex --size 100 50 -> width of 100 and height of 50)%s\n", PURPLE, YELLOW, RESET);
    printf("\t%s-ru, --rules: %sDefine new rules (D: B3/S23 -> Becomes alive with 3 neighbours and survives with 2 or three neighbors else, die)%s\n", PURPLE, YELLOW, RESET);
    printf("\t%s-l, --list: %sPrint a list of interesting variants of the life rules %s\n", PURPLE, YELLOW, RESET);
    printf("\t%s-rs, --random-square: %sAdd a square of width [arg1] and height [arg2] of random cells on the center%s\n", PURPLE, YELLOW, RESET);
    printf("\t%s-rsc, --random-square-chances: %sWhen using -rs, define the chances of a cell inside the square to be alive by one over [arg] [D: 3]%s\n", PURPLE, YELLOW, RESET);
}

// Prints the version
void version(void)
{
    printf("%sGame of life C => %s%s%s\n", PURPLE, YELLOW, VERSION, RESET);
}

void list(void)
{
    printf("%sRules variants%s\n", RED, RESET);
    char *names[] = {
        "Game of life (original)",
        "Replicator",
        "H-trees",
        "Gnarl",
        "Fredkin",
        "Seeds",
        "Live Free or Die",
        "Serviettes",
        "Iceballs"
        "Life without death",
        "Dotlife",
        "Mazertic",
        "Maze",
        "Snowlife",
        "Corrosion of Conformity",
        "Eight Life",
        "Coral",
        "3-4 Life",
        "Bacteria",
        "Assimilation",
        "Long life",
        "Gems",
        "Gems Minor",
        "Land Rush",
        "Bugs",
        "Holstein",
        "Diamoeba",
        "Amoeba",
        "Pseudo Life",
        "Geology",
        "2x2",
        "High Life",
        "Bilnker Life",
        "Logarithmic Replicator Rule",
        "Slow Blob",
        "Stains",
        "Day & Night",
        "LowDeath",
        "Morley",
        "DryLife Without Death",
        "Mazertic with Mice",
        "Maze with Mice",
        "DryLife",
        "Plow World",
        "Coagulation",
        "Pedestrian Without Death",
        "Pedestrian Life",
        "Honey Life",
        "Electrified Maze",
        "Osciallators Rules",
        "Walled Cities",
        "Majority",
        "Vote 4/5",
        "Vote"};

    char *rulestrings[] = {
        "B3/S23",
        "B1357/S1357",
        "B1/S012345678",
        "B1/S1",
        "B1357/S02468",
        "B2/S",
        "B2/S0",
        "B234/S",
        "B25678/S5678",
        "B3/S012345678",
        "B3/S023",
        "B3/S1234",
        "B3/S12345",
        "B3/S1237",
        "B3/S124",
        "B3/S238",
        "B3/S45678",
        "B34/S34",
        "B34/S456",
        "B345/S4567",
        "B345/S5",
        "B3457/S4568",
        "B34578/S456",
        "B35/S234578",
        "B3567/S15678",
        "B3567/S15678",
        "B35678/S5678",
        "B357/S1358",
        "B357/S238",
        "B3578/S24678",
        "B36/S125",
        "B36/S23",
        "B36/S235",
        "B367/S125678",
        "B3678/S235678",
        "B3678/S34678",
        "B368/S238",
        "B368/S245",
        "B37/S012345678",
        "B37/S1234",
        "B37/S12345",
        "B37/S23",
        "B378/S012345678",
        "B378/S235678",
        "B38/S012345678",
        "B38/S23",
        "B38/S238",
        "B45/S12345",
        "B45/S1235",
        "B45678/S2345",
        "B45678/S5678",
        "B4678/S35678",
        "B5678/S45678"};

    for (int i = 0, len = sizeof(names) / sizeof(names[0]); i < len; i++)
    {
        printf("%s%s => %s%s%s\n", YELLOW, names[i], PURPLE, rulestrings[i], RESET);
    }
}

int main(int argc, char *argv[])
{

    // Parse args --------------
    while (--argc > 0)
    {

        char *arg = *++argv;

        if (strcmp(arg, "--help") == 0 || strcmp(arg, "-h") == 0)
        {
            help();
            return 0;
        }
        else if (strcmp(arg, "--list") == 0 || strcmp(arg, "-l") == 0)
        {
            list();
            return 0;
        }
        else if (strcmp(arg, "--version") == 0 || strcmp(arg, "-v") == 0)
        {
            version();
            return 0;
        }
        else if (strcmp(arg, "--delay") == 0 || strcmp(arg, "-d") == 0)
        {
            if (argc <= 1)
            {
                printf("%s[ x ] : An argument is required after -d%s", RED, RESET);
                return 1;
            }

            delay = atoi(*++argv);
            argc--;

            if (delay <= 0)
            {
                printf("%s[ x ] : The delay must be a number higher than 0%s", RED, RESET);
                return 1;
            }
        }
        else if (strcmp(arg, "--random") == 0 || strcmp(arg, "-r") == 0)
        {
            random_generation = true;
        }
        else if (strcmp(arg, "--over") == 0 || strcmp(arg, "-o") == 0)
        {
            if (argc <= 1)
            {
                printf("%s[ x ] : An argument is required after -o%s", RED, RESET);
                return 1;
            }

            one_chance_over = atoi(*++argv);
            argc--;

            if (one_chance_over < 1)
            {
                printf("%s[ x ] : The argumentent after -o must be higher than 0 (and not too high!!)%s", RED, RESET);
                return 1;
            }
        }
        else if (strcmp(arg, "--random-square-chances") == 0 || strcmp(arg, "-rsc") == 0)
        {
            if (argc <= 1)
            {
                printf("%s[ x ] : An argument is required after -rsc%s", RED, RESET);
                return 1;
            }

            rsc_one_chance_over = atoi(*++argv);
            argc--;

            if (rsc_one_chance_over < 1)
            {
                printf("%s[ x ] : The argumentent after -rsc must be higher than 0 (and not too high!!)%s", RED, RESET);
                return 1;
            }
        }
        else if (strcmp(arg, "--size") == 0 || strcmp(arg, "-s") == 0)
        {
            if (argc <= 2)
            {
                printf("%s[ x ] : Two arguments are required after -s%s", RED, RESET);
                return 1;
            }

            width = atoi(*++argv);
            argc--;
            height = atoi(*++argv);
            argc--;

            if (height <= 1 || width <= 1)
            {
                printf("%s[ x ] : The height and the width must be higher than 1 (and not too high!!)%s", RED, RESET);
                return 1;
            }
        }
        else if (strcmp(arg, "--random-square") == 0 || strcmp(arg, "-rs") == 0)
        {
            if (argc <= 2)
            {
                printf("%s[ x ] : Two arguments are required after -rs [width and height of the square]%s", RED, RESET);
                return 1;
            }
            rsc = true;
            rs_width = atoi(*++argv);
            argc--;
            rs_height = atoi(*++argv);
            argc--;

            if (rs_height < 1 || rs_width < 1)
            {
                printf("%s[ x ] : The height and the width must be higher than 0 (and not too high!!)%s", RED, RESET);
                return 1;
            }
        }
        else if (strcmp(arg, "--rules") == 0 || strcmp(arg, "-ru") == 0)
        {
            if (argc <= 1)
            {
                printf("%s[ x ] : An argument is required after -ru%s", RED, RESET);
                return 1;
            }
            Rules new_rules;

            reset_rules(&new_rules);

            char *rules_text = *++argv;
            argc--;
            for (int i = 0; i < strlen(rules_text); i++)
            {
                rules_text[i] = toupper(rules_text[i]);
            }

            if (rules_text[0] != 'B')
            {
                printf("%s[ x ] : Invalid rules!! The rules format should start with a B (born)%s", RED, RESET);
                return 1;
            }
            rules_text++;

            int slash_index = get_index(rules_text, '/');

            if (slash_index == -1)
            {
                printf("%s[ x ] : Invalid rules!! The rules format should contain a '/'%s", RED, RESET);
                return 1;
            }

            // parse the B rules
            for (int i = 0; i < slash_index; i++)
            {

                if (!isdigit(rules_text[0]))
                {
                    printf("%s[ x ] : Invalid rules!! Expected digits in B rules but found another character%s", RED, RESET);
                    return 1;
                }
                char digit = (char)rules_text[0] - '0';
                new_rules.b_rules[i] = digit;
                rules_text++;
            }
            rules_text++;

            if (rules_text[0] != 'S')
            {
                printf("%s[ x ] : Invalid rules!! The rules format should contain a S (survive rules)%s", RED, RESET);
                return 1;
            }

            rules_text++;

            // S rules
            for (int i = 0, len = strlen(rules_text); i < len; i++)
            {
                if (!isdigit(rules_text[0]))
                {
                    printf("%s[ x ] : Invalid rules!! Expected digits in S rules but found another character%s", RED, RESET);
                    return 1;
                }
                char digit = (char)rules_text[0] - '0';

                new_rules.s_rules[i] = digit;
                rules_text++;
            }

            rules = new_rules;
        }
        else
        {
            printf("Unknown argument: %s\n", arg);
            return 1;
        }
    }

    // ----------- End of parsing args -------------
    // if the size has not been set manually by the user, determine it automatically
    if (height == 0 && width == 0)
    {
        get_terminal_size(&width, &height);
        width -= 4;
        height -= 4;
    }

    // Cut the width in half because a cell taxe two cols
    width /= 2;

    bool world[height][width];
    reset_world(height, width, world);
    if (rsc)
    {
        add_rsc(height, width, world, rs_width, rs_height, rsc_one_chance_over);
    }

    if (random_generation)
    {
        populate_world(height, width, world, one_chance_over);
    }
    else
    {
        user_edit_world(height, width, world);
    }

    while (true)
    {
        display_world(height, width, world);
        next_generation(height, width, world, rules);
        sleep_ms(delay);
    }
    return 0;
}
