
#ifndef MAZE_H
#define MAZE_H

#include "data.h"

bool maze_get(const maze_data maze, const int row, const int col);
void maze_set(maze_data *maze, const int row, const int col, const bool value);
int* find_neighbor(const maze_data maze, const int row, const int col, int result[]);
bool maze_valid_move(const maze_data maze, const int r, const int c);
void maze_print_easy(WINDOW *win, const maze_data maze, const player_data player);
void maze_print_medium(WINDOW *win, const maze_data maze, const player_data player);
void maze_print_hard(WINDOW *win, const maze_data maze, const player_data player);
void gen_new_maze(maze_data *maze);

#endif
