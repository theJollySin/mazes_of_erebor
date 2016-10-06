
#ifndef WINDOWS_H
#define WINDOWS_H

// forward declaration
WINDOW* init_window();
void init_maze_window(WINDOW *win);
void full_box_clear(WINDOW *win);
void full_box_resize(WINDOW *win, int win_y, int win_x);

#endif
