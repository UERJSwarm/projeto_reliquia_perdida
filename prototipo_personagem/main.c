#include <SDL2/SDL.h>
#include <stdbool.h>

int AUX_WaitEventTimeout(SDL_Event* evt, Uint32* ms) {
    Uint32 antes = SDL_GetTicks();
    int ret = SDL_WaitEventTimeout(evt, *ms);
    Uint32 depois = SDL_GetTicks();
    Uint32 d = depois - antes;

    if (d >= *ms)
        *ms = 0;
    else
        *ms -= d;

    return ret;
}

typedef enum {
    PARADO,
    ANDANDO,
    CORRENDO,
    PULANDO,
    PEGARITEM,
    DROPARITEM
} EstadoMovimento;

int main (int argc, char* args[]) {
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("Protótipo do Personagem",
                         SDL_WINDOWPOS_CENTERED,
                         SDL_WINDOWPOS_CENTERED,
                         400, 300, SDL_WINDOW_SHOWN);
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

    SDL_Rect r = { 180, 130, 20, 20 };
    bool rodando = true;
    Uint32 espera = 100;
    SDL_Event evt;

    EstadoMovimento estado = PARADO;
    int vel = 1;
    bool noChao = true;
    int chao = r.y;
    bool subindo = true;
     
    while (rodando) {
        int isevt = AUX_WaitEventTimeout(&evt, &espera);

        const Uint8* teclas = SDL_GetKeyboardState(NULL);

            if (isevt) { 
               if (evt.type == SDL_QUIT) rodando = false; 
               else if (evt.type == SDL_KEYDOWN) { 
                  switch (evt.key.keysym.sym) { 
                     case SDLK_LSHIFT: estado = CORRENDO; 
                        break; 
                     case SDLK_SPACE: 
                         if (noChao) {
 
                            chao = r.y;
                            estado = PULANDO;
                            noChao = false;
                        }
                        break; 
                     case SDLK_UP: 
                     case SDLK_DOWN: 
                     case SDLK_LEFT: 
                     case SDLK_RIGHT: 
                         if (estado != CORRENDO && estado != PULANDO) estado = ANDANDO; 
                      break; 
                   } 
                }

            else if (evt.type == SDL_KEYUP) {
                if (evt.key.keysym.sym == SDLK_LSHIFT) {
                    if (teclas[SDL_SCANCODE_LEFT] || teclas[SDL_SCANCODE_RIGHT] ||
                        teclas[SDL_SCANCODE_UP]   || teclas[SDL_SCANCODE_DOWN]) {
                        estado = ANDANDO;
                    } else {
                        estado = PARADO;
                    }
                }

                if (evt.key.keysym.sym == SDLK_UP ||
                    evt.key.keysym.sym == SDLK_DOWN ||
                    evt.key.keysym.sym == SDLK_LEFT ||
                    evt.key.keysym.sym == SDLK_RIGHT) {

                    if (teclas[SDL_SCANCODE_LEFT] || teclas[SDL_SCANCODE_RIGHT] ||
                        teclas[SDL_SCANCODE_UP]   || teclas[SDL_SCANCODE_DOWN]) {
                        if (teclas[SDL_SCANCODE_LSHIFT])
                            estado = CORRENDO;
                        else
                            estado = ANDANDO;
                    } else {
                        estado = PARADO;
                    }
                }
            }
         }

        switch (estado) {
            case ANDANDO:
                vel = 1;
                if (teclas[SDL_SCANCODE_UP])    r.y -= vel;
                if (teclas[SDL_SCANCODE_DOWN])  r.y += vel;
                if (teclas[SDL_SCANCODE_LEFT])  r.x -= vel;
                if (teclas[SDL_SCANCODE_RIGHT]) r.x += vel;
                break;

            case CORRENDO:
                vel = 2;
                if (teclas[SDL_SCANCODE_UP])    r.y -= vel;
                if (teclas[SDL_SCANCODE_DOWN])  r.y += vel;
                if (teclas[SDL_SCANCODE_LEFT])  r.x -= vel;
                if (teclas[SDL_SCANCODE_RIGHT]) r.x += vel;
                break;

            case PULANDO:
                {
               if (teclas[SDL_SCANCODE_LEFT])  r.x -= vel;
               if (teclas[SDL_SCANCODE_RIGHT]) r.x += vel;

               if (subindo) {
                  r.y -= vel;
                  if (r.y <= chao - 50)
                       subindo = false;
               } else {
               r.y += vel;
               if (r.y >= chao) { 
               r.y = chao;
               if (teclas[SDL_SCANCODE_LEFT] || teclas[SDL_SCANCODE_RIGHT] ||
                        teclas[SDL_SCANCODE_UP]   || teclas[SDL_SCANCODE_DOWN]) {
                        estado = ANDANDO;
                    } else {
                        estado = PARADO;
                    }
               noChao = true;
               subindo = true;
                    }
                  }
                 }
            break;

            default:
                break;
        }

        SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
        SDL_RenderClear(ren);

        SDL_SetRenderDrawColor(ren, 0, 0, 255, 255);
        SDL_RenderFillRect(ren, &r);

        SDL_RenderPresent(ren);
    }

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
