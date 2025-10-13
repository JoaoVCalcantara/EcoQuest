#ifndef ALLEGRO_INIT_H
#define ALLEGRO_INIT_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_color.h>

// MELHORIA: Definir dimensões da tela como constantes
#define LARGURA_TELA 1280
#define ALTURA_TELA 720

typedef struct AllegroContext {
    ALLEGRO_DISPLAY* display;
    ALLEGRO_TIMER* timer;
    ALLEGRO_EVENT_QUEUE* event_queue;
    ALLEGRO_BITMAP* mapa;
    ALLEGRO_FONT* font; // <-- atualmente isso aqui não faz nada
    int width;
    int height;
    ALLEGRO_COLOR CoresFundo[5];
} AllegroContext;

int inicializar_allegro(AllegroContext* ctx);
void destruir_allegro(AllegroContext* ctx);

#endif // ALLEGRO_INIT_H