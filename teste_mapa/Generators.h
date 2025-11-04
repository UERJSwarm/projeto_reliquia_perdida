#include "Global_Headers.h"


typedef struct Point Point;

Point *allocate_item (int x0, int xf, int y0, int yf, int **map, int **regions, int largest_id, int type_item);
int how_many_itens (int level, int tam_map);
int pick_random_item (int level);