#include <assert.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <SDL2/SDL_image.h>

int AUX_WaitEventTimeout(SDL_Event* evt, Uint32* ms) {
    Uint32 antes = SDL_GetTicks();
    int ret = SDL_WaitEventTimeout(evt, *ms);
    Uint32 depois = SDL_GetTicks();
    Uint32 d = depois - antes;
  
    if (ret) {
    if (d >= *ms)
        *ms = 0;
    else
        *ms -= d;
    }

    return ret;
}

typedef enum {
    PARADO,
    ANDANDO,
    CORRENDO,
    PULANDO,
} EstadoMovimento;

typedef Struct {
  EstadoMovimento EstadoAtual;
  int vida;
  int x;
  int y;

} Personagem

int main (int argc, char* args[]) {
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("Protótipo do Personagem",
                         SDL_WINDOWPOS_CENTERED,
                         SDL_WINDOWPOS_CENTERED,
                         400, 300, SDL_WINDOW_SHOWN);
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    SDL_Texture* img = IMG_LoadTexture(ren, "anim2.png");
    assert(img != NULL);
    SDL_Rect r = { 180, 130, 70, 70 };
    SDL_RenderClear(ren); SDL_Rect c; 
    bool rodando = true;
    Uint32 espera = 10;
    SDL_Event evt;

    EstadoMovimento estado = PARADO;
    c = (SDL_Rect) {0, 0, 100, 80};
    int vel = 1;
    bool noChao = true;
    int chao = r.y;
    bool subindo = true;
    int k = 0;
    int aux = 2;

    while (rodando) {
        int isevt = AUX_WaitEventTimeout(&evt, &espera);

        const Uint8* teclas = SDL_GetKeyboardState(NULL);

            if (isevt) { 
               if (evt.type == SDL_QUIT) rodando = false; 
               else if (evt.type == SDL_KEYDOWN) { 
                  switch (evt.key.keysym.sym) { 
                     case SDLK_LSHIFT: if (noChao) estado = CORRENDO; 
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
                         if (estado != CORRENDO && estado != PULANDO && noChao) estado = ANDANDO; 
                      break; 
                   } 
                }

            else if (evt.type == SDL_KEYUP) {
              if (noChao) {
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
         }

        switch (estado) {
            case ANDANDO:
                vel = 1;
                if (teclas[SDL_SCANCODE_UP]) {
                   if (aux == 1) c = (SDL_Rect) {200, 0, 100, 80};
                   if (aux == 2) c = (SDL_Rect) {100, 0, 100, 80};
                   r.y -= vel;
                }
                if (teclas[SDL_SCANCODE_DOWN]) {
                   if (aux == 1) c = (SDL_Rect) {200, 0, 100, 80};
                   if (aux == 2) c = (SDL_Rect) {100, 0, 100, 80};
                   r.y += vel;
                }
                if (teclas[SDL_SCANCODE_LEFT]) {
                   r.x -= vel;
                   c = (SDL_Rect) {200, 0, 100, 80};
                   aux = 1;
                }
                if (teclas[SDL_SCANCODE_RIGHT]) {
                   r.x += vel;
                   c = (SDL_Rect) {100, 0, 100, 80};
                   aux = 2;
                }
                break;

            case CORRENDO:
                vel = 2;
                if (teclas[SDL_SCANCODE_UP]) {
                   if (aux == 1) c = (SDL_Rect) {590, 0, 100, 80};
                   if (aux == 2) c = (SDL_Rect) {490, 0, 100, 80};
                   r.y -= vel;
                }
                if (teclas[SDL_SCANCODE_DOWN]) {
                   if (aux == 1) c = (SDL_Rect) {590, 0, 100, 80};
                   if (aux == 2) c = (SDL_Rect) {490, 0, 100, 80};
                   r.y += vel;
                }
                if (teclas[SDL_SCANCODE_LEFT]) {
                   r.x -= vel;
                   c = (SDL_Rect) {590, 0, 100, 80};
                   aux = 1;
                }
                if (teclas[SDL_SCANCODE_RIGHT]) {
                   r.x += vel;
                   c = (SDL_Rect) {490, 0, 100, 80};
                   aux = 2;
                }
                break;

            case PULANDO:
                {
               if (k == 0) {
                  c = (SDL_Rect) {300, 0, 100, 80};
                  k = 1;
               }
               if (teclas[SDL_SCANCODE_LEFT]) {
                  r.x -= vel;
                  c = (SDL_Rect) {400, 0, 100, 80};
               }
               if (teclas[SDL_SCANCODE_RIGHT]) {
                  r.x += vel;
                  c = (SDL_Rect) {300, 0, 100, 80};
               }

               if (subindo) {
                  r.y -= vel;
                  if (r.y <= chao - 50)
                       subindo = false;
               } else {
               r.y += vel;
               if (r.y >= chao) { 
               r.y = chao;
               k = 0;
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
            case PARADO:
               {
                c = (SDL_Rect) {0, 0, 100, 80};
               }
            break;

            default:
                break;
        }
        espera = 10;

        SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
        SDL_RenderClear(ren);

        SDL_SetRenderDrawColor(ren, 0, 0, 255, 255);
        SDL_RenderCopy(ren, img, &c, &r); 

        SDL_RenderPresent(ren);
    }

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
