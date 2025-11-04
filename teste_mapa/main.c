#include "Global_Headers.h"
#include "CA_Map_Gen.h"
#include "VN_Map_Gen.h"
#include "RW_Map_Gen.h"
#include "Grid_Utils.h"
#include "Find_Largest_Island.h"
#include "Generators.h"

#define TILE_SIZE 10
#define ROWS 80
#define COLS 80
#define CA_STEPS 12
#define CA_CHANCE 0.45
#define LEVEL 0
#define SCREEN_WIDTH  (COLS * TILE_SIZE)
#define SCREEN_HEIGHT (ROWS * TILE_SIZE)
#define TIMEOUT 60

enum Types_item {
	P_INICIAL = 10,
    P_EXIT,
    P_POTION,
    P_ARTEFACT,
	P_TREASURE,
	P_THREAT,
	P_ITENS //testes
};

enum Types_map {
	M_CELLULAR_AUTOMATA,
	M_VALUE_NOISE,
	M_RANDOM_WALK
};

int AUX_WaitEventTimeoutCount (SDL_Event* evt, Uint32* ms);
void render_map(SDL_Renderer *renderer, int **map, int rows, int cols);
int **build_map(int rows, int cols, int type_map);
		
 int main (int argc, char* args[]) {
	/* INICIALIZACAO */
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("MapGen",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);
	
	/* EXECUÇÃO */
	srand(time(NULL));
	//srand(10);

	enum Types_map type_map = M_RANDOM_WALK;
	int **mapa = build_map(ROWS, COLS, type_map);
	
    int running = 1;
	Uint32 espera = TIMEOUT;
    while (running) {
		SDL_Event evt;
		int isevt = AUX_WaitEventTimeoutCount(&evt, &espera);
		
        if(isevt){
            if (evt.type == SDL_QUIT)
                running = 0;
			
			else if (evt.type == SDL_KEYDOWN && evt.key.keysym.sym == SDLK_SPACE){
				free_grid_int(mapa, ROWS);
				mapa = build_map(ROWS, COLS, type_map);
				
				Element *region_list = NULL;
				Element* largest;
				int total_regions = 0, largest_island = 0;

				// Mapeia todas as regiões do mapa
				int **visited_map = mapping_the_map(mapa, &region_list, &total_regions, ROWS, COLS);

				// Identifica a maior ilha
				largest_island = find_largest_island(region_list);
				largest = find_Element(&region_list, largest_island);

				// Aloca um item inicial dentro da maior ilha
				Point *item = allocate_item(largest->x0, largest->xf,
                            largest->y0, largest->yf,
                            mapa, visited_map,
							largest_island, P_INICIAL);
				item = allocate_item(largest->x0, largest->xf,
                            largest->y0, largest->yf,
                            mapa, visited_map,
							largest_island, P_EXIT);
				
				int qtd_itens = how_many_itens (1, largest->tam);
				printf("Ilha de tamanho %d gerou %d itens\n", largest->tam, qtd_itens);
				for (int i=0; i<qtd_itens; i++){
					item = allocate_item(largest->x0, largest->xf,
                            largest->y0, largest->yf,
                            mapa, visited_map,
							largest_island, P_ITENS);
				}
			}
			else if (evt.type == SDL_KEYDOWN && evt.key.keysym.sym == SDLK_1){
				type_map = M_CELLULAR_AUTOMATA;
			}
			else if (evt.type == SDL_KEYDOWN && evt.key.keysym.sym == SDLK_2){
				type_map = M_VALUE_NOISE;
			}
			else if (evt.type == SDL_KEYDOWN && evt.key.keysym.sym == SDLK_3){
				type_map = M_RANDOM_WALK;
			}
        }
		else{ //vazio
		}
		

        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
        SDL_RenderClear(ren);
        render_map(ren, mapa, ROWS, COLS);
        SDL_RenderPresent(ren);
    }
	
	//Teste da Find_Largest_Island
	/*
	Element* region_list = NULL;
    int total_regions = 0, largest_island = 0;
    int** visited_map = mapping_the_map(mapa, &region_list, &total_regions, ROWS, COLS);
    print_grid_int(visited_map, ROWS, COLS);
    printf("\n--- Regiões Encontradas (%d no total) ---\n", total_regions);
	printf("Maior ilha eh: %d", find_largest_island (region_list));*/

    free_grid_int(mapa, ROWS);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
} 

void render_map(SDL_Renderer *renderer, int **map, int rows, int cols) {
    SDL_Rect tile = {0,0,TILE_SIZE,TILE_SIZE};

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            tile.x = j * TILE_SIZE;
            tile.y = i * TILE_SIZE;

            if (map[i][j] == 0)
                SDL_SetRenderDrawColor(renderer, 168, 145, 50, 255);  // chão
			else if (map[i][j] == 2)
                SDL_SetRenderDrawColor(renderer, 112, 77, 49, 255);  // lama
            else if (map[i][j] == 1)
				SDL_SetRenderDrawColor(renderer, 50, 121, 168, 255); // agua
			else if (map[i][j] == P_INICIAL)
				SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // entrada
			else if(map[i][j] == P_EXIT)
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // saída
			else if(map[i][j] == P_ITENS)
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // itens
			
            SDL_RenderFillRect(renderer, &tile);
        }
    }
}

int AUX_WaitEventTimeoutCount (SDL_Event* evt, Uint32* ms){
	Uint32 antes, espera;
	antes = SDL_GetTicks();
	int isevt = SDL_WaitEventTimeout(evt, *ms);
	if (isevt){
		espera = (SDL_GetTicks() - antes);
		if (espera >= *ms) *ms = 0;
		else *ms -= espera;
	}
	else *ms = TIMEOUT;
	
	return isevt;
}

int **build_map(int rows, int cols, int type_map){
	int **map;
	if (type_map == M_CELLULAR_AUTOMATA) 
		map = CA_build_map(rows, cols, CA_STEPS, CA_CHANCE);
	else if (type_map == M_VALUE_NOISE)
		map = VN_build_map(rows, cols);
	else if (type_map == M_RANDOM_WALK)
		map = RW_build_map(rows, cols, LEVEL);
	return map;
}
