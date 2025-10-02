#ifndef ALLEGRO_INIT_H
#define ALLEGRO_INIT_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>


//struct que centraliza todos as criações que vão ser chamadas da allegro_init.c

typedef struct {
    ALLEGRO_DISPLAY* display;
    ALLEGRO_TIMER* timer;
    ALLEGRO_FONT* font;
    ALLEGRO_BITMAP* mundo;
    ALLEGRO_BITMAP* dragao;
    ALLEGRO_BITMAP* batalha;
    ALLEGRO_EVENT_QUEUE* event_queue;
    int width;
    int height;
    float tamanho;
} AllegroContext;

int inicializar_allegro(AllegroContext* ctx);
void destruir_allegro(AllegroContext* ctx);


#endif
