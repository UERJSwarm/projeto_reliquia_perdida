#ifndef GENERATORS_H
#define GENERATORS_H
#include "Generators.h"
#include "Find_Largest_Island.h"

Point *allocate_item (int x0, int xf, int y0, int yf, int **map, int **regions, int largest_id, int type_item){
	Point *point = NULL;
	int x, y, aux=1, limit=100, counter_limit=0;
	
	while(aux && counter_limit<limit){
		x = rand() % (xf - x0 + 1) + x0;
		y = rand() % (yf - y0 + 1) + y0;
	
		if (map[x][y] == 0 && regions[x][y] == largest_id){
			map[x][y] = type_item;
			point = new_point(x,y);
			aux=0;
		}
		counter_limit++; //point retornará como NULL se passar limite de tentativas
	}
	return point;
}
	
int how_many_itens (int level, int tam_map){
	float tam_base = 0.02*tam_map;   // 2% do mapa
    float level_coeficient = 1.0f - (level * 0.1f); // diminui 10% por nível
    
    int qtd = (int)(tam_base*level_coeficient);
    
    return qtd;
}

//refatorar: generalizar a ideia de itens proibidos por nivel (matriz?)
int pick_random_item (int level){
	int w = rand() % 100 + 1;
	int weight_itens[8] = {20,15,10,15,10,5,5,20};
	int acc = 0, type;
	
	if (level == 1){ //nao pode Unfall e morte instantanea
		int weight_itens_ex[8] = {25,20,15,15,5,0,0,20};
		for(int i=0; i<7; i++){
			acc+=weight_itens_ex[i];
			if (w <= acc) type = i+1;
		}
	}
	else{
		for(int i=0; i<7; i++){
			acc+=weight_itens[i];
			if (w <= acc) type = i+1;
		}
	}
	return type;
}
#endif