#include"global_headers.h"
#include "Grid_Utils.h"

typedef struct Element{
	int id;
	int x0;
	int xf;
	int y0;
	int yf;
	int tam;
	int type;
	struct Element *next;
} Element;

typedef struct Point{
	int x,y;
	struct Point *next;
} Point;	

typedef struct{
	Point *head;
	Point *tail;
	int tam;
} queueSearch;

Element* new_Element (int id, int x0, int xf, int y0, int yf, int tam, int type);
void insert_Element (Element** head, Element* new_element);
Element* find_Element (Element** head, int id);
Point *new_point (int x, int y);
void enqueue_Point (Point *point, queueSearch *q);
Point* dequeue_Point (queueSearch *q);
queueSearch *new_queueSearch();
void free_queueSearch (queueSearch *q);
int queue_is_empty( queueSearch *q);
int greater_than (int num1, int num2);
int less_than (int num1, int num2);
Element* BFS_mapping (int **grid, int **visited, int id, int x, int y, int rows, int cols);
int** mapping_the_map (int **grid, Element** list, int *counter, int rows, int cols);
int find_largest_island (Element* head);
