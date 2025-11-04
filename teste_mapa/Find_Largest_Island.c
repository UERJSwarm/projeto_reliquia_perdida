#ifndef FIND_LARGEST_ISLAND_H
#define FIND_LARGEST_ISLAND_H
#include "Find_Largest_Island.h"

Element* new_Element (int id, int x0, int xf, int y0, int yf, int tam, int type){
	Element* element = (Element*) malloc(sizeof(Element));
	element->id = id;
	element->x0 = x0;
	element->xf = xf;
	element->y0 = y0;
	element->yf = yf;
	element->tam = tam;
	element->type = type;
	element->next = NULL;
	
	return element;
}

void insert_Element (Element** head, Element* new_element){
	new_element ->next = *head;
	*head = new_element;
}

Element* find_Element (Element** head, int id){
	Element* wanted = *head;
	while (wanted != NULL){
		if(wanted->id == id)
			return wanted;
		wanted = wanted->next;
	}
	return NULL;
}

Point *new_point (int x, int y){
	Point *point = malloc(sizeof(Point));
	if (point == NULL) return NULL;
	point->x = x;
	point->y = y;
	point->next = NULL;
	
	return point;
}

void enqueue_Point (Point *point, queueSearch *q){
	if (point == NULL) return;
	point ->next = NULL;
	if(q->head == NULL) q->head = point;
	else q->tail->next = point;
	q->tail = point;
	q->tam++;
}

Point* dequeue_Point (queueSearch *q){
	Point *last_point;
	last_point = q->head;
	if (last_point == NULL) return NULL; //fila vazia
	q->head = last_point->next;
	q->tam--;
	return last_point;
}

queueSearch *new_queueSearch (){
	queueSearch *new;
	new = malloc (sizeof(queueSearch));
	if (new == NULL) return NULL;
	new->head = NULL;
	new->tail = NULL;
	new->tam = 0;
	return new;
}

void free_queueSearch (queueSearch *q){
	Point *aux, *next;
	aux = q->head;
	while (aux != NULL){
		next = aux->next;
		free(aux);
		aux = next;
	}
	free(q);
}

int queue_is_empty( queueSearch *q) {
   return q->head == NULL;
}
		
int greater_than (int num1, int num2){
	if (num1>=num2) return num1;
	else return num2;
}

int less_than (int num1, int num2){
	if (num1<=num2) return num1;
	else return num2;
}

Element* BFS_mapping (int **grid, int **visited, int id, int x, int y, int rows, int cols){
	int type = grid[x][y], vizinho_x, vizinho_y;
	Point *point = new_point (x, y);
	queueSearch *queue = new_queueSearch();
	enqueue_Point(point, queue);
	visited[x][y]=id;
	
	//int h[8] = {-1, -1, -1,  0, 0, 1, 1, 1};
    //int v[8] = {-1,  0,  1, -1, 1,-1, 0, 1};
	int h[4] = {-1,1,0,0};
	int v[4] = {0,0,-1,1};
	
	int i, j, counter = 1, x0 = x, xf = x, y0 = y, yf = y;
	Point *aux;
	while(!queue_is_empty(queue)){
		aux = dequeue_Point(queue);
		i = aux->x;
		j = aux->y;
		
		for (int k = 0; k<4; k++){
			vizinho_x = i+h[k];
			vizinho_y = j+v[k];
			if (vizinho_x>=0 && (vizinho_x) < rows && vizinho_y >= 0 && vizinho_y < cols
			&& grid[vizinho_x][vizinho_y] == type && visited[vizinho_x][vizinho_y] == 0){
				visited[vizinho_x][vizinho_y] = id;
				counter++;
				point = new_point (vizinho_x, vizinho_y);
				enqueue_Point(point, queue);
				
				x0 = less_than(x0, vizinho_x);
				xf = greater_than(xf, vizinho_x);
				y0 = less_than(y0, vizinho_y);
				yf = greater_than(yf, vizinho_y);
			}
		}
		free(aux);
	}
	
	Element* new_elem = new_Element (id, x0, xf, y0, yf, counter, type);
	free_queueSearch(queue);
	
	return new_elem;
}

int** mapping_the_map (int **grid, Element** list, int *counter, int rows, int cols){
	int id=0;
	int **visited = allocate_grid_int (rows, cols);
	reset_grid_int (visited, rows, cols); //zera matriz
		
	for(int i=0; i<rows; i++){
		for (int j=0; j<cols; j++){
			if (visited[i][j] == 0){
				id++;
				Element* new_region = BFS_mapping (grid, visited, id, i, j, rows, cols);
				insert_Element(list, new_region);
			}
		}
	}
	*counter = id;
	return visited; 
}

int find_largest_island (Element* head){
	int largest_island_id = -1;
    int largest_island_size = 0; 
	Element* aux = head;

	while (aux != NULL){
		if(aux->type == 0){
			if (aux->tam > largest_island_size){
				largest_island_size = aux->tam;
                largest_island_id = aux->id;
			}
		}
		aux = aux->next;
	}
    
	return largest_island_id;
}

#endif