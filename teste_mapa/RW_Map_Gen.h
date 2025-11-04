#include"Global_Headers.h"
#include "Grid_Utils.h"

void RW_initialise_grid (int ** grid, int rows, int cols, int level);
void fill_water (int ** grid, int rows, int cols);
int ** RW_build_map (int rows, int cols, int level);
