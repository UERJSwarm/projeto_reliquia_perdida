#include <assert.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <SDL2/SDL_image.h>

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

typedef enum {
    PARADO,
    ANDANDO,
    CORRENDO,
    PULANDO,

} EstadoMovimento;

// --- Funções Auxiliares ---

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

// --- Lógica do Jogador ---

void EventosJogador(SDL_Event* evt, EstadoMovimento* estado, bool* noChao, int* chao, const Uint8* teclas, SDL_Rect* r) {
    if (*noChao) {
        if (evt->type == SDL_KEYDOWN) {
            switch (evt->key.keysym.sym) {
                case SDLK_LSHIFT: if (noChao) *estado = CORRENDO; 
                    break;
                case SDLK_SPACE:
                    if (noChao) {
                      *chao = r->y;
                      *estado = PULANDO;
                      *noChao = false;
                    }
                    break;
                case SDLK_UP:
                case SDLK_DOWN:
                case SDLK_LEFT:
                case SDLK_RIGHT:
                    if (*estado != CORRENDO && *estado != PULANDO) *estado = ANDANDO;
                    break;
            }
        } else if (evt->type == SDL_KEYUP) {
            if (evt->key.keysym.sym == SDLK_LSHIFT) {
                if (teclas[SDL_SCANCODE_LEFT] || teclas[SDL_SCANCODE_RIGHT] ||
                    teclas[SDL_SCANCODE_UP] || teclas[SDL_SCANCODE_DOWN]) {
                    *estado = ANDANDO;
                } else {
                    *estado = PARADO;
                }
            } else if (evt->key.keysym.sym == SDLK_UP || evt->key.keysym.sym == SDLK_DOWN ||
                       evt->key.keysym.sym == SDLK_LEFT || evt->key.keysym.sym == SDLK_RIGHT) {
                if (teclas[SDL_SCANCODE_LEFT] || teclas[SDL_SCANCODE_RIGHT] ||
                    teclas[SDL_SCANCODE_UP] || teclas[SDL_SCANCODE_DOWN]) {
                    if (teclas[SDL_SCANCODE_LSHIFT])
                        *estado = CORRENDO;
                    else
                        *estado = ANDANDO;
                } else {
                    *estado = PARADO;
                }
            }
        }
    }
 
    if (evt->type == SDL_KEYDOWN) {
        if (evt->key.keysym.sym == SDLK_s) (*estado = PARADO);
        if (evt->key.keysym.sym == SDLK_d) (*estado = PARADO);
    }
    else if (!*noChao) *estado = PULANDO;
}

void AtualizarJogador(EstadoMovimento* estado, SDL_Rect* r, SDL_Rect* c, int* vel, bool* noChao, int* chao, bool* subindo, int* k, int* aux, const Uint8* teclas) {
    switch (*estado) {
        case ANDANDO:
            *vel = 1;
            if (teclas[SDL_SCANCODE_UP]) {
               *c = (*aux == 1) ? (SDL_Rect) {200, 0, 100, 80} : (SDL_Rect) {110, 0, 100, 80};
            }
            if (teclas[SDL_SCANCODE_DOWN]) {
               *c = (*aux == 1) ? (SDL_Rect) {200, 0, 100, 80} : (SDL_Rect) {110, 0, 100, 80};
            }
            if (teclas[SDL_SCANCODE_LEFT]) {
               r->x -= *vel;
               *c = (SDL_Rect) {200, 0, 100, 80};
               *aux = 1;
            }
            if (teclas[SDL_SCANCODE_RIGHT]) {
               r->x += *vel;
               *c = (SDL_Rect) {110, 0, 100, 80};
               *aux = 2;
            }
            break;

        case CORRENDO:
            *vel = 2;
            if (teclas[SDL_SCANCODE_UP]) {
               *c = (*aux == 1) ? (SDL_Rect) {590, 0, 100, 80} : (SDL_Rect) {490, 0, 100, 80};
            }
            if (teclas[SDL_SCANCODE_DOWN]) {
               *c = (*aux == 1) ? (SDL_Rect) {590, 0, 100, 80} : (SDL_Rect) {490, 0, 100, 80};
            }
            if (teclas[SDL_SCANCODE_LEFT]) {
               r->x -= *vel;
               *c = (SDL_Rect) {590, 0, 100, 80};
               *aux = 1;
            }
            if (teclas[SDL_SCANCODE_RIGHT]) {
               r->x += *vel;
               *c = (SDL_Rect) {490, 0, 100, 80};
               *aux = 2;
            }
            break;

        case PULANDO:
        {
           if (*k == 0) {
              *c = (SDL_Rect) {300, 0, 100, 80};
              *k = 1;
           }
           if (teclas[SDL_SCANCODE_LEFT]) {
              r->x -= *vel;
              *c = (SDL_Rect) {400, 0, 100, 80}; 
           }
           if (teclas[SDL_SCANCODE_RIGHT]) {
              r->x += *vel;
              *c = (SDL_Rect) {300, 0, 100, 80}; 
           }

           if (*subindo) {
              r->y -= 1;
              if (r->y <= *chao - 25) 
                   *subindo = false;
           } else {
               r->y += 1;
               if (r->y >= *chao) { 
                   r->y = *chao;
                   *k = 0;
                   if (teclas[SDL_SCANCODE_LEFT] || teclas[SDL_SCANCODE_RIGHT] ||
                            teclas[SDL_SCANCODE_UP] || teclas[SDL_SCANCODE_DOWN]) {
                        if (teclas[SDL_SCANCODE_LSHIFT])
                            *estado = CORRENDO;
                        else
                            *estado = ANDANDO;
                    } else {
                        *estado = PARADO;
                    }
                   *noChao = true;
                   *subindo = true;
               }
            }
        }
        break;
        case PARADO:
            *c = (SDL_Rect) {0, 0, 100, 80};
            break;

        default:
            break;
    }
}



int main(int argc, char* args[])
{
    /* INICIALIZACAO */
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG);

    SDL_Window* win = SDL_CreateWindow("A Relíquia Perdida",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         LARGURA_TELA, ALTURA_TELA, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);
    SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);

    /* Texturas da Interface */
    SDL_Texture* background = IMG_LoadTexture(ren, "background_menu.png"); assert(background != NULL);
    SDL_Texture* logo_menu = IMG_LoadTexture(ren, "logo_menu.png"); assert(logo_menu != NULL);
    SDL_Texture* button_play = IMG_LoadTexture(ren, "button_play.png"); assert(button_play != NULL);
    SDL_Texture* button_options = IMG_LoadTexture(ren, "button_options.png"); assert(button_options != NULL);
    SDL_Texture* button_quit = IMG_LoadTexture(ren, "button_quit.png"); assert(button_quit != NULL);
    SDL_Texture* button_pause = IMG_LoadTexture(ren, "button_pause.png"); assert(button_pause != NULL);
    SDL_Texture* button_invent = IMG_LoadTexture(ren, "state_inventory.png"); assert(button_invent != NULL);
    SDL_Texture* screen_map = IMG_LoadTexture(ren, "screen_map.png"); assert(screen_map != NULL);
    SDL_Texture* background_inicial = IMG_LoadTexture(ren, "background_inicial.png"); assert(background_inicial != NULL);
    SDL_Texture* button_start = IMG_LoadTexture(ren, "button_start.png"); assert(button_start != NULL);
    SDL_Texture* logo_jogo = IMG_LoadTexture(ren, "logo_jogo.png"); assert(logo_jogo != NULL);

    /* Textura do Jogador e Cenário */
    SDL_Texture* player_img = IMG_LoadTexture(ren, "anim2.png"); assert(player_img != NULL);
    SDL_Texture* fundo = IMG_LoadTexture(ren, "fundo.png"); assert(fundo != NULL);
    /* Variáveis da Interface */
    int running = 1;
    int y0_button = 300, gap_button = 30;
    enum State state_ = STATE_INICIAL;
    SDL_Rect buttons[NUM_BUTTONS];
    buttons[0] = (SDL_Rect){ (LARGURA_TELA - BUTTON_WIDTH)/2, y0_button, BUTTON_WIDTH, BUTTON_HEIGHT };
    buttons[1] = (SDL_Rect){ (LARGURA_TELA - BUTTON_WIDTH)/2, y0_button + (BUTTON_HEIGHT + gap_button), BUTTON_WIDTH, BUTTON_HEIGHT };
    buttons[2] = (SDL_Rect){ (LARGURA_TELA - BUTTON_WIDTH)/2, y0_button + 2*(BUTTON_HEIGHT + gap_button), BUTTON_WIDTH, BUTTON_HEIGHT };
    
    SDL_Rect back_rect = {0,0,LARGURA_TELA, ALTURA_TELA};
    SDL_Rect logo_rect = { (LARGURA_TELA - LOGO_WIDTH)/2, y0_button - 2*BUTTON_HEIGHT - gap_button, LOGO_WIDTH, LOGO_HEIGHT };
    SDL_Rect pause_rect = {(LARGURA_TELA - 350)/2,(ALTURA_TELA - 150)/2,350, 150}; 
    SDL_Rect invent_rect = {(LARGURA_TELA - 740)/2,(ALTURA_TELA - 498)/2,740, 498}; 
    SDL_Rect map_rect = {0,0,LARGURA_TELA, ALTURA_TELA}; 
    SDL_Rect title_rect = { (LARGURA_TELA - LOGO_WIDTH)/2, 100, LOGO_WIDTH, LOGO_HEIGHT };
    SDL_Rect inicio_rect = { (LARGURA_TELA - BUTTON_WIDTH)/2, 600, BUTTON_WIDTH, BUTTON_HEIGHT };
    SDL_Rect fundo_rect = { 0, 0, LARGURA_TELA, ALTURA_TELA };

    Uint32 espera = TIMEOUT;
    int i=0;
    add_effect(&buttons[i]);
    int effect = 1;

    /* Variáveis do Jogador */
    EstadoMovimento estado_movimento = PARADO;
    SDL_Rect player_r = { 250, 620, 70, 70 }; 
    SDL_Rect player_c = { 0, 0, 100, 80 };  
    SDL_Rect fundo_r = { LARGURA_TELA, ALTURA_TELA };
    int vel = 1;
    bool noChao = true;
    int chao_y = player_r.y; 
    bool subindo = true;
    int k = 0; // Estado auxiliar do pulo
    int aux = 2; // Estado auxiliar para sprite (lado)
	
    /* EXECUÇÃO (Loop Principal) */
    while(running) {
        SDL_Event evt;
        int isevt = AUX_WaitEventTimeoutCount(&evt, &espera);
        const Uint8* teclas = SDL_GetKeyboardState(NULL);

        if(isevt) {		
			if (evt.type == SDL_QUIT) running = 0;
			
			switch (state_){
				
				case STATE_INICIAL:
					if(evt.type == SDL_KEYDOWN){
						if (evt.key.keysym.sym == SDLK_SPACE) state_ = STATE_MENU;
					}
					break;
				
				case STATE_MENU:
					if(evt.type == SDL_KEYDOWN){
						
						if (evt.key.keysym.sym == SDLK_a || evt.key.keysym.sym == SDLK_DOWN || evt.key.keysym.sym == SDLK_UP)
							remove_effect(&buttons[i]);
						
						if (evt.key.keysym.sym == SDLK_UP){
							i--;
							if (i<0) i=NUM_BUTTONS-1; // Correção: i=0
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
							add_effect(&buttons[i]);
						}
					}
					break;
						
				case STATE_PLAY:
					EventosJogador(&evt, &estado_movimento, &noChao, &chao_y, teclas, &player_r);
					
					if(evt.type == SDL_KEYDOWN){
						if (evt.key.keysym.sym == SDLK_s) state_ = STATE_INVENTORY;
						else if (evt.key.keysym.sym == SDLK_d) state_ = STATE_MAP;
						else if (evt.key.keysym.sym == SDLK_a) state_ = STATE_PAUSE; // Pause
						else if (evt.key.keysym.sym == SDLK_ESCAPE) state_ = STATE_MENU; // Voltar ao Menu
					}
					break;

				case STATE_INVENTORY:
					if(evt.type == SDL_KEYDOWN){
						if (evt.key.keysym.sym == SDLK_s) state_ = STATE_PLAY;
					}
					break;
				
				case STATE_PAUSE:
					if(evt.type == SDL_KEYDOWN){
						if (evt.key.keysym.sym == SDLK_a) state_ = STATE_PLAY;
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
			} else if (state_ == STATE_PLAY) {
                AtualizarJogador(&estado_movimento, &player_r, &player_c, &vel, &noChao, &chao_y, &subindo, &k, &aux, teclas);
            }
		}
		
		// --- Renderização ---
		SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
		SDL_RenderClear(ren);

        if (state_ == STATE_INICIAL){
            SDL_RenderCopy(ren, background_inicial, NULL, &back_rect);
            SDL_RenderCopy(ren, button_start, NULL, &inicio_rect);
            SDL_RenderCopy(ren, logo_jogo, NULL, &title_rect);
        }
        
        else if (state_ == STATE_MENU) {
            SDL_RenderCopy(ren, background, NULL, &back_rect);
            SDL_RenderCopy(ren, logo_menu, NULL, &logo_rect);
            SDL_RenderCopy(ren, button_play, NULL, &buttons[0]);
            SDL_RenderCopy(ren, button_options, NULL, &buttons[1]);
            SDL_RenderCopy(ren, button_quit, NULL, &buttons[2]);
        }
        else if (state_ == STATE_PLAY) {
            SDL_SetRenderDrawColor(ren, 100, 200, 100, 255);
            SDL_RenderFillRect(ren, &back_rect);
            SDL_RenderCopy(ren, fundo, NULL, &fundo_rect); 
            SDL_RenderCopy(ren, player_img, &player_c, &player_r);
        }
        else if (state_ == STATE_QUIT) {
            running = 0;
        }
        
        else if (state_ == STATE_PAUSE){
            SDL_SetRenderDrawColor(ren, 100, 200, 100, 255);
            SDL_RenderFillRect(ren, &back_rect);
            SDL_RenderCopy(ren, fundo, NULL, &fundo_rect);
            SDL_RenderCopy(ren, player_img, &player_c, &player_r);
            SDL_RenderCopy(ren, button_pause, NULL, &pause_rect);
        }
        
        else if (state_ == STATE_INVENTORY){
            SDL_SetRenderDrawColor(ren, 100, 200, 100, 255);
            SDL_RenderFillRect(ren, &back_rect);
            SDL_RenderCopy(ren, fundo, NULL, &fundo_rect);
            SDL_RenderCopy(ren, player_img, &player_c, &player_r);
            SDL_RenderCopy(ren, button_invent, NULL, &invent_rect);
        }
        
        else if (state_ == STATE_MAP){
            SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
            SDL_RenderFillRect(ren, &back_rect);
            SDL_RenderCopy(ren, fundo, NULL, &fundo_rect); 
            SDL_RenderCopy(ren, player_img, &player_c, &player_r);
            SDL_RenderCopy(ren, screen_map, NULL, &map_rect);
        }

        SDL_RenderPresent(ren);
        espera = 10;
    }
	
  SDL_DestroyTexture(player_img);
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
  SDL_DestroyTexture(fundo);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    IMG_Quit();
    SDL_Quit();
    return 0;
}
