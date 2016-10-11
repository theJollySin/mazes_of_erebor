
#include <functional>
#include <random>
#include <ncurses.h>
#include <string.h>
#include <vector>
#include "data.h"
#include "maze.h"
#include "menu.h"
#include "splash.h"
#include "windows.h"
#include "maze_window.h"

using namespace std;

// forward declarations
void maze_loop_update(const int c, bool *needs_update, maze_data *maze, player_data *player);

/**
 *  Master UPDATE for game loop - in maze
 */
void maze_loop_update(const int c, bool *needs_update, maze_data *maze, player_data *player) {
    // UPDATE 1: direct updates
    switch (c) {
        case KEY_UP:
            if (maze_valid_move(*maze, player->loc[0] - 1, player->loc[1])) {
                player->loc[0] -= 1;
                *needs_update = true;
            }
            break;
        case KEY_DOWN:
            if (maze_valid_move(*maze, player->loc[0] + 1, player->loc[1])) {
                player->loc[0] += 1;
                *needs_update = true;
            }
            break;
        case KEY_LEFT:
            if (maze_valid_move(*maze, player->loc[0], player->loc[1] - 1)) {
                player->loc[1] -= 1;
                *needs_update = true;
            }
            break;
        case KEY_RIGHT:
            if (maze_valid_move(*maze, player->loc[0], player->loc[1] + 1)) {
                player->loc[1] += 1;
                *needs_update = true;
            }
            break;
        // no default actions to be taken
    }

    // UPDATE 2: indirect updates
    if (needs_update) {
        // if you reach the end, start over in a new maze
        if (player->loc[0] == maze->finish[0] && player->loc[1] == maze->finish[1]) {
            gen_new_maze(maze);
            reset_player(player, maze);
            maze->level += 1;
        }

        player->visited[player->loc[0] * maze->ncols + player->loc[1]] = true;
    }
}


/**
 *   The game maze GUI.
 *
 *   Use arrow keys to navigate the maze or type "q" to quit.
 */
menu_state game_ui(WINDOW *win, game_data *d, menu_state state)
{
    maze_data *maze = &d->maze;
    player_data *player = &d->player;
    int win_y(15);
    int win_x(15);
    int last_win_y, last_win_x;

    // init window at current resolution
    if (maze->level == -1) {
        intro_splash(win);
    }
    init_maze_window(win);
    getmaxyx(stdscr, win_y, win_x);
    last_win_y = win_y;
    last_win_x = win_x;

    // select the appropriate print function
    function<void(WINDOW*, const maze_data, const player_data)> maze_print = \
        state == game_easy ? maze_print_easy : (state == game_medium ? maze_print_medium : maze_print_hard);

    // generate a new maze, if necessary
    if (maze->level == -1) {
        maze->level = 0;
        gen_new_maze(maze);
        reset_player(player, maze);
        maze->difficulty = state;
    }

    int c;
    int level(0);
    bool needs_update(true);

    // GAME LOOP
    while (true) {
        // RENDER
        if (level != maze->level) {
            level = maze->level;
            success_splash(win, level + 2);
        }
        if (needs_update) {
            maze_print(win, *maze, *player);
            needs_update = false;
        }

        // INPUT
        c = wgetch(win);
        switch (c) {
            case 113:  // q
                return menu_cont;
            case KEY_RESIZE:
                getmaxyx(stdscr, win_y, win_x);
                if (last_win_x != win_x || last_win_y != win_y) {
                    last_win_y = win_y;
                    last_win_x = win_x;
                    full_box_resize(win, win_y, win_x);
                }
                needs_update = true;
                c = -999;
                break;
            // no default actions to be taken
        }

        // UPDATE
        maze_loop_update(c, &needs_update, maze, player);
    }
}
