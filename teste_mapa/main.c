#include"Global_Headers.h"
#include "CA_Map_Gen.h"
#include "VN_Map_Gen.h"
#include "Grid_Utils.h"

#define TILE_SIZE 5
#define ROWS 200
#define COLS 400
#define SCREEN_WIDTH  (COLS * TILE_SIZE)
#define SCREEN_HEIGHT (ROWS * TILE_SIZE)
#define TIMEOUT 60

int AUX_WaitEventTimeoutCount (SDL_Event* evt, Uint32* ms);
void render_map(SDL_Renderer *renderer, int **map, int rows, int cols);

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

	//int **mapa = CA_build_map(ROWS, COLS, 12, 0.45); //64 64 TILE 15
	int **mapa = VN_build_map(ROWS, COLS); //160 320 5
	
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
				mapa = VN_build_map(ROWS, COLS);
			}
        }
		else //vazio

        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
        SDL_RenderClear(ren);
        render_map(ren, mapa, ROWS, COLS);
        SDL_RenderPresent(ren);
    }

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
            else
				SDL_SetRenderDrawColor(renderer, 50, 121, 168, 255); // agua
                
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
