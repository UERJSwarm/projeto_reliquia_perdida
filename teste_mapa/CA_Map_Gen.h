#include"Global_Headers.h"
#include "Grid_Utils.h"

int adjacent_to (int ** grid, int rows, int cols, int i, int j);
void play_rules (int ** grid, int ** new_grid, int rows, int cols, int deathLimit, int birthLimit);
void CA_initialise_grid (int ** grid, int rows, int cols, float chanceToStartAlive);
void CA_cookies_shape (int ** grid, int rows, int cols, int radius);
int ** CA_build_map (int rows, int cols, int steps, float chance);
