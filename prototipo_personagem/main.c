#include <assert.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>

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
    PULANDO
} EstadoMovimento;

typedef enum {
    DANCANDO,
    HIPNOTIZANDO,
    ATORDOADA,
    PARADA
} EstadoDancarina;

typedef struct {
    EstadoDancarina estadoAtual;
    int x, y;
    int w, h;
    int vida;
    int alcanceVisaoQuadrado;  
    int raioHipnoseQuadrado;  
    int raioHipnoseSairQuadrado;
    Uint32 tempoEstado;
} Dancarina;

int main(int argc, char* args[]) {
    SDL_Init(SDL_INIT_EVERYTHING);

    const int WINDOW_WIDTH = 600;
    const int WINDOW_HEIGHT = 400;

    SDL_Window* win = SDL_CreateWindow("Protótipo do Personagem",
                                     SDL_WINDOWPOS_CENTERED,
                                     SDL_WINDOWPOS_CENTERED,
                                     WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    SDL_Texture* img = IMG_LoadTexture(ren, "anim2.png");
    assert(img != NULL);

    // --- Personagem ---
    const int PLAYER_WIDTH = 70;
    const int PLAYER_HEIGHT = 70;
    const int PLAYER_SCREEN_X = (WINDOW_WIDTH - PLAYER_WIDTH) / 2;
    const int PLAYER_SCREEN_Y = (WINDOW_HEIGHT - PLAYER_HEIGHT) / 2;

    SDL_Rect r = {PLAYER_SCREEN_X, PLAYER_SCREEN_Y, PLAYER_WIDTH, PLAYER_HEIGHT};
    SDL_Rect c = {0, 0, 100, 80}; 

    int player_world_x = 180;
    int player_world_y = 130;

    int vel = 1;
    bool noChao = true;
    int chao = player_world_y;
    bool subindo = true;
    int aux = 2;
    int k = 0;
    EstadoMovimento estado = PARADO;

    bool hipnotizado = false;
    Uint32 tempoHipnoseInicio = 0; 

    // --- Inimigo ---
    Dancarina danca = {
        .estadoAtual = PARADA,
        .x = 400,
        .y = 100,
        .w = 50,
        .h = 50,
        .vida = 3,
        .alcanceVisaoQuadrado = 200 * 200, 
        .raioHipnoseQuadrado = 70 * 70, 
        .raioHipnoseSairQuadrado = 90 * 90, 
        .tempoEstado = 0
    };

    bool rodando = true;
    SDL_Event evt;
    Uint32 espera = 10;

    while (rodando) {
        int isevt = AUX_WaitEventTimeout(&evt, &espera);
        const Uint8* teclas = SDL_GetKeyboardState(NULL);

        if (isevt) {
            if (evt.type == SDL_QUIT) rodando = false;
            else if (evt.type == SDL_KEYDOWN) {
                if(!hipnotizado) { 
                    switch (evt.key.keysym.sym) {
                        case SDLK_LSHIFT: if(noChao) estado = CORRENDO; break;
                        case SDLK_SPACE: 
                            if(noChao) {
                                chao = player_world_y; 
                                estado = PULANDO;
                                noChao = false;
                            }
                            break;
                        case SDLK_UP: case SDLK_DOWN: case SDLK_LEFT: case SDLK_RIGHT:
                            if(estado != CORRENDO && estado != PULANDO && noChao)
                                estado = ANDANDO;
                            break;
                    }
                }
            } else if (evt.type == SDL_KEYUP) {
                if(!hipnotizado && noChao) { 
                    if(evt.key.keysym.sym == SDLK_LSHIFT) {
                        if(teclas[SDL_SCANCODE_LEFT] || teclas[SDL_SCANCODE_RIGHT] ||
                           teclas[SDL_SCANCODE_UP] || teclas[SDL_SCANCODE_DOWN])
                            estado = ANDANDO;
                        else
                            estado = PARADO;
                    }
                    if(evt.key.keysym.sym == SDLK_UP || evt.key.keysym.sym == SDLK_DOWN ||
                       evt.key.keysym.sym == SDLK_LEFT || evt.key.keysym.sym == SDLK_RIGHT) {
                        if(teclas[SDL_SCANCODE_LEFT] || teclas[SDL_SCANCODE_RIGHT] ||
                           teclas[SDL_SCANCODE_UP] || teclas[SDL_SCANCODE_DOWN]) {
                            if(teclas[SDL_SCANCODE_LSHIFT])
                                estado = CORRENDO;
                            else
                                estado = ANDANDO;
                        } else
                            estado = PARADO;
                    }
                }
            }
        }

        // --- Lógica da dançarina ---
        // calcula distancia no mundo
        int dx = player_world_x - danca.x;
        int dy = player_world_y - danca.y;
        
        int distQuadrada = (dx*dx) + (dy*dy);
        const int STUN_DIST_QUADRADO = 50 * 50; 

        switch(danca.estadoAtual) {
            case DANCANDO:
                if(distQuadrada < danca.raioHipnoseQuadrado) {
                    danca.estadoAtual = HIPNOTIZANDO;
                    hipnotizado = true;
                    tempoHipnoseInicio = SDL_GetTicks();
                    estado = PARADO; 
                    printf("Jogador hipnotizado!\n");
                }
                break;
            case HIPNOTIZANDO:
                if(teclas[SDL_SCANCODE_E] && distQuadrada < STUN_DIST_QUADRADO) {
                    danca.estadoAtual = ATORDOADA;
                    danca.tempoEstado = SDL_GetTicks();
                    hipnotizado = false;
                    estado = PARADO; 
                    printf("Dançarina atordoada!\n");
                }
                
                if(distQuadrada > danca.raioHipnoseSairQuadrado) {
                    danca.estadoAtual = DANCANDO;
                    hipnotizado = false;
                    estado = PARADO; 
                    printf("Jogador escapou da hipnose.\n");
                }
                break;
            case ATORDOADA:
                if(SDL_GetTicks() - danca.tempoEstado > 3000)
                    danca.estadoAtual = PARADA;
                break;
            case PARADA:
                if(distQuadrada < danca.alcanceVisaoQuadrado)
                    danca.estadoAtual = DANCANDO;
                break;
        }

        // Lógica de expiração da hipnose
        if (hipnotizado) {
            if (SDL_GetTicks() - tempoHipnoseInicio > 5000) {
                hipnotizado = false;
                estado = PARADO; 
                danca.estadoAtual = ATORDOADA;       
                danca.tempoEstado = SDL_GetTicks(); 
                printf("Jogador resistiu à hipnose!\n");
            }
        }

        // --- Lógica do personagem ---
        if(!hipnotizado) {
            switch(estado) {
                case ANDANDO:
                    vel = 1;
                    if(teclas[SDL_SCANCODE_UP]) { player_world_y -= vel; c = (aux==1)? (SDL_Rect){200,0,100,80} : (SDL_Rect){100,0,100,80}; }
                    if(teclas[SDL_SCANCODE_DOWN]) { player_world_y += vel; c = (aux==1)? (SDL_Rect){200,0,100,80} : (SDL_Rect){100,0,100,80}; }
                    if(teclas[SDL_SCANCODE_LEFT]) { player_world_x -= vel; c = (SDL_Rect){200,0,100,80}; aux=1; }
                    if(teclas[SDL_SCANCODE_RIGHT]) { player_world_x += vel; c = (SDL_Rect){100,0,100,80}; aux=2; }
                    break;

                case CORRENDO:
                    vel = 2;
                    if(teclas[SDL_SCANCODE_UP]) { player_world_y -= vel; c = (aux==1)? (SDL_Rect){590,0,100,80} : (SDL_Rect){490,0,100,80}; }
                    if(teclas[SDL_SCANCODE_DOWN]) { player_world_y += vel; c = (aux==1)? (SDL_Rect){590,0,100,80} : (SDL_Rect){490,0,100,80}; }
                    if(teclas[SDL_SCANCODE_LEFT]) { player_world_x -= vel; c = (SDL_Rect){590,0,100,80}; aux=1; }
                    if(teclas[SDL_SCANCODE_RIGHT]) { player_world_x += vel; c = (SDL_Rect){490,0,100,80}; aux=2; }
                    break;

                case PULANDO:
                    if(k==0) { c=(SDL_Rect){300,0,100,80}; k=1; }
                    if(teclas[SDL_SCANCODE_LEFT]) { player_world_x -= vel; c=(SDL_Rect){400,0,100,80}; }
                    if(teclas[SDL_SCANCODE_RIGHT]) { player_world_x += vel; c=(SDL_Rect){300,0,100,80}; }
                    if(subindo) { player_world_y -= vel; if(player_world_y <= chao-50) subindo=false; }
                    else { player_world_y += vel; if(player_world_y >= chao) { player_world_y=chao; k=0; estado=(teclas[SDL_SCANCODE_LEFT]||teclas[SDL_SCANCODE_RIGHT])? ANDANDO : PARADO; noChao=true; subindo=true; } }
                    break;

                case PARADO:
                    c = (SDL_Rect){0,0,100,80};
                    break;
                default: break;
            }
        } else {
            c = (SDL_Rect){680,0,100,80};
        }

        SDL_SetRenderDrawColor(ren, 255,255,255,255);
        SDL_RenderClear(ren);
        SDL_RenderCopy(ren, img, &c, &r);

        // Dançarina
        // Calcula o deslocamento na tela
        int camera_offset_x = player_world_x - PLAYER_SCREEN_X;
        int camera_offset_y = player_world_y - PLAYER_SCREEN_Y;
        
        // Calcula a posição da dançarina na tela
        SDL_Rect rDanca = { 
            danca.x - camera_offset_x,
            danca.y - camera_offset_y,
            danca.w, 
            danca.h 
        };
        
        SDL_Color corDanca;
        switch(danca.estadoAtual) {
            case DANCANDO: corDanca=(SDL_Color){128,0,128,255}; break;     // roxo
            case HIPNOTIZANDO: corDanca=(SDL_Color){128,0,128,255}; break; // roxo
            case ATORDOADA: corDanca=(SDL_Color){128,128,128,255}; break;  // cinza
            case PARADA: corDanca=(SDL_Color){0,255,0,255}; break;         // verde
        }
        SDL_SetRenderDrawColor(ren, corDanca.r, corDanca.g, corDanca.b, 255);
        SDL_RenderFillRect(ren, &rDanca);

        SDL_RenderPresent(ren);

        espera = 10;
    }

    SDL_DestroyTexture(img);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
