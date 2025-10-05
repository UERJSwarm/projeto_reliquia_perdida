#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <assert.h>

#define LARGURA_TELA 800
#define ALTURA_TELA 800
#define BUTTON_WIDTH 250
#define BUTTON_HEIGHT 100
#define LOGO_WIDTH 500
#define LOGO_HEIGHT 200
#define NUM_BUTTONS 3
#define TIMEOUT 500

enum State {
	STATE_INICIAL,
  STATE_MENU,
  STATE_PLAY,
  STATE_QUIT,
	STATE_PAUSE,
	STATE_MAP,
	STATE_INVENTORY
};

int AUX_WaitEventTimeoutCount(SDL_Event* evt, Uint32* ms);

void add_effect (SDL_Rect* button){
	int num = 30;
	
	button->y -= num/2;
	button->x -= num/2;
	button->w += num;
	button->h += num;
}

void remove_effect (SDL_Rect *button){
	int num = 30;
	
	button->y += num/2;
	button->x += num/2;
	button->w -= num;
	button->h -= num;
}

int main(int argc, char* args[])
{
    /* INICIALIZACAO */
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("A Relíquia Perdida",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         LARGURA_TELA, ALTURA_TELA, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);
    SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);

    /* EXECUÇÃO */
    int running = 1;
    int y0_button = 300, gap_button = 30;
    enum State state_ = STATE_INICIAL;

    SDL_Rect buttons[NUM_BUTTONS];
    buttons[0] = (SDL_Rect){ (LARGURA_TELA - BUTTON_WIDTH)/2, y0_button, BUTTON_WIDTH, BUTTON_HEIGHT };
    buttons[1] = (SDL_Rect){ (LARGURA_TELA - BUTTON_WIDTH)/2, y0_button + (BUTTON_HEIGHT + gap_button), BUTTON_WIDTH, BUTTON_HEIGHT };
    buttons[2] = (SDL_Rect){ (LARGURA_TELA - BUTTON_WIDTH)/2, y0_button + 2*(BUTTON_HEIGHT + gap_button), BUTTON_WIDTH, BUTTON_HEIGHT };
	
	SDL_Rect back_rect = {0,0,LARGURA_TELA, ALTURA_TELA};
	SDL_Rect logo_rect = { (LARGURA_TELA - LOGO_WIDTH)/2, y0_button - 2*BUTTON_HEIGHT - gap_button, LOGO_WIDTH, LOGO_HEIGHT };
	
	SDL_Texture* background = IMG_LoadTexture(ren, "background_menu.png");
    assert(background != NULL);
	SDL_Texture* logo_menu = IMG_LoadTexture(ren, "logo_menu.png");
    assert(logo_menu != NULL);
	SDL_Texture* button_play = IMG_LoadTexture(ren, "button_play.png");
    assert(button_play != NULL);
	SDL_Texture* button_options = IMG_LoadTexture(ren, "button_options.png");
    assert(button_options != NULL);
	SDL_Texture* button_quit = IMG_LoadTexture(ren, "button_quit.png");
    assert(button_quit != NULL);
	SDL_Texture* button_pause = IMG_LoadTexture(ren, "button_pause.png");
    assert(button_pause != NULL);
	SDL_Texture* button_invent = IMG_LoadTexture(ren, "state_inventory.png");
    assert(button_invent != NULL);
	SDL_Texture* screen_map = IMG_LoadTexture(ren, "screen_map.png");
    assert(screen_map != NULL);
	SDL_Texture* background_inicial = IMG_LoadTexture(ren, "background_inicial.png");
    assert(background_inicial != NULL);
	SDL_Texture* button_start = IMG_LoadTexture(ren, "button_start.png");
    assert(button_start != NULL);
	SDL_Texture* logo_jogo = IMG_LoadTexture(ren, "logo_jogo.png");
    assert(button_start != NULL);

    Uint32 espera = TIMEOUT;
	
	int i=0; //inicializa o efeito no primeiro botao
	add_effect(&buttons[i]);
	
	int effect =1;
	
	// STATE: PLAY (exemplo)
	SDL_Rect r = {0,0,200, 200};
	
	//STATE: PAUSE (exemplo)
	SDL_Rect pause_rect = {(LARGURA_TELA - 350)/2,(ALTURA_TELA - 150)/2,350, 150}; 
	
	//STATE: INVENTORY (exemplo)
	SDL_Rect invent_rect = {(LARGURA_TELA - 740)/2,(ALTURA_TELA - 498)/2,740, 498}; 
	
	//STATE: MAP (exemplo)
	SDL_Rect map_rect = {0,0,LARGURA_TELA, ALTURA_TELA}; 
	
	//STATE: INICIAL (exemplo)
	SDL_Rect title_rect = { (LARGURA_TELA - LOGO_WIDTH)/2, 100, LOGO_WIDTH, LOGO_HEIGHT };
	SDL_Rect inicio_rect = { (LARGURA_TELA - BUTTON_WIDTH)/2, 600, BUTTON_WIDTH, BUTTON_HEIGHT };
	
    while(running) {
        SDL_Event evt;
        int isevt = AUX_WaitEventTimeoutCount(&evt, &espera);

        if(isevt) {		
			if (evt.type == SDL_QUIT) running = 0;
			
			switch (state_){
				
				case STATE_INICIAL:
					if(evt.type == SDL_KEYDOWN){
						if(evt.type == SDL_KEYDOWN){
							if (evt.key.keysym.sym == SDLK_SPACE) state_ = STATE_MENU;
						}
					}
					break;
				
				case STATE_MENU:
					if(evt.type == SDL_KEYDOWN){
						
						if (evt.key.keysym.sym == SDLK_a || evt.key.keysym.sym == SDLK_DOWN || evt.key.keysym.sym == SDLK_UP)
							remove_effect(&buttons[i]);
						
						if (evt.key.keysym.sym == SDLK_UP){
							i--;
							if (i<0) i=0;
							add_effect(&buttons[i]);
						}
								
						else if (evt.key.keysym.sym == SDLK_DOWN){
							i++;
							if (i == NUM_BUTTONS) i = 0;
							add_effect(&buttons[i]);
						}
								
						else if (evt.key.keysym.sym == SDLK_a){
							if (i == 0) state_ = STATE_PLAY;
							else if (i == 1) state_ = STATE_MENU;
							else running = 0;
							add_effect(&buttons[i]); //retirar depois de pronto
						}
					}
					break;
						
				case STATE_PLAY:
					if(evt.type == SDL_KEYDOWN){
						if (evt.key.keysym.sym == SDLK_UP) r.y -= 5;
						else if (evt.key.keysym.sym == SDLK_DOWN) r.y += 5;
						else if (evt.key.keysym.sym == SDLK_LEFT) r.x -= 5;
						else if (evt.key.keysym.sym == SDLK_RIGHT) r.x += 5;
						else if (evt.key.keysym.sym == SDLK_s) state_ = STATE_INVENTORY;
						else if (evt.key.keysym.sym == SDLK_d) state_ = STATE_MAP;
						else if (evt.key.keysym.sym == SDLK_SPACE) state_ = STATE_PAUSE;
						else if (evt.key.keysym.sym == SDLK_ESCAPE) state_ = STATE_MENU;
					}
					break;

				case STATE_INVENTORY:
					if(evt.type == SDL_KEYDOWN){
						if (evt.key.keysym.sym == SDLK_s) state_ = STATE_PLAY;
					}
					break;
				
				case STATE_PAUSE:
					if(evt.type == SDL_KEYDOWN){
						if (evt.key.keysym.sym == SDLK_SPACE) state_ = STATE_PLAY;
					}
					break;
						
				case STATE_MAP:
					if(evt.type == SDL_KEYDOWN){
						if (evt.key.keysym.sym == SDLK_d) state_ = STATE_PLAY;
					}
					break;
			}
			
        }
		else{
			if (state_ == STATE_INICIAL){
				if (effect) {
					add_effect (&inicio_rect);
				}
				else {
					remove_effect (&inicio_rect);
				}
				effect = !effect;
			}
		}
		
		SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);

    if (state_ == STATE_INICIAL){
		SDL_RenderClear(ren);
        SDL_RenderCopy(ren, background_inicial, NULL, &back_rect);
		SDL_RenderCopy(ren, button_start, NULL, &inicio_rect);
		SDL_RenderCopy(ren, logo_jogo, NULL, &title_rect);
	}
	
	if (state_ == STATE_MENU) {
        SDL_RenderClear(ren);
        SDL_RenderCopy(ren, background, NULL, &back_rect);
        SDL_RenderCopy(ren, logo_menu, NULL, &logo_rect);
        SDL_RenderCopy(ren, button_play, NULL, &buttons[0]);
        SDL_RenderCopy(ren, button_options, NULL, &buttons[1]);
        SDL_RenderCopy(ren, button_quit, NULL, &buttons[2]);
    }
    else if (state_ == STATE_PLAY) {
		SDL_RenderClear(ren);
        SDL_SetRenderDrawColor(ren, 0, 0, 255, 255);
        SDL_RenderFillRect(ren, &r);
    }
    else if (state_ == STATE_QUIT) {
        running = 0;
    }
	
	else if (state_ == STATE_PAUSE){
		SDL_RenderCopy(ren, button_pause, NULL, &pause_rect);
	}
	
	else if (state_ == STATE_INVENTORY){
		SDL_RenderCopy(ren, button_invent, NULL, &invent_rect);
	}
	
	else if (state_ == STATE_MAP){
		SDL_RenderCopy(ren, screen_map, NULL, &map_rect);
	}

    SDL_RenderPresent(ren);
    }
	
	SDL_DestroyTexture(logo_jogo);
	SDL_DestroyTexture(button_start);
	SDL_DestroyTexture(background_inicial);
	SDL_DestroyTexture(screen_map);
	SDL_DestroyTexture(button_invent);
	SDL_DestroyTexture(button_pause);
	SDL_DestroyTexture(button_quit);
	SDL_DestroyTexture(button_options);
	SDL_DestroyTexture(button_play);
	SDL_DestroyTexture(logo_menu);
	SDL_DestroyTexture(background);
  SDL_DestroyRenderer(ren);
  SDL_DestroyWindow(win);
  SDL_Quit();
  return 0;
}

int AUX_WaitEventTimeoutCount(SDL_Event* evt, Uint32* ms) {
    Uint32 antes = SDL_GetTicks();
    int isevt = SDL_WaitEventTimeout(evt, *ms);

    if (isevt) {
        Uint32 espera = SDL_GetTicks() - antes;
        if (espera >= *ms) *ms = 0;
        else *ms -= espera;
    }
    else *ms = TIMEOUT;

    return isevt;
}
