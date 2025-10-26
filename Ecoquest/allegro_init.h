#ifndef ALLEGRO_INIT_H
#define ALLEGRO_INIT_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_color.h>

#define LARGURA_TELA 1280
#define ALTURA_TELA 720

typedef struct AllegroContext {
    // Recursos principais do Allegro
    ALLEGRO_DISPLAY* display;
    ALLEGRO_TIMER* timer;
    ALLEGRO_EVENT_QUEUE* event_queue;
    ALLEGRO_FONT* font;

    // Recursos de mapa e gráficos
    ALLEGRO_BITMAP* mapa;
    ALLEGRO_BITMAP* regioes[4];

    // Sprites dos animais
    ALLEGRO_BITMAP* onca;
    ALLEGRO_BITMAP* boto;
    ALLEGRO_BITMAP* jacare;
    ALLEGRO_BITMAP* raposa;

    // Sprites do herói
    ALLEGRO_BITMAP* heroi_frente;
    ALLEGRO_BITMAP* heroi_costas;
    ALLEGRO_BITMAP* heroi_esquerda;
    ALLEGRO_BITMAP* heroi_direita;
    ALLEGRO_BITMAP* heroi_andando_frente;
    ALLEGRO_BITMAP* heroi_andando_costas;
    ALLEGRO_BITMAP* heroi_andando_esquerda;
    ALLEGRO_BITMAP* heroi_andando_direita;

    // Dimensões
    int width, height;
    int largura, altura;
    int metadelargura, metadealtura;

    // Cores
    ALLEGRO_COLOR CoresFundo[5];
} AllegroContext;

int inicializar_allegro(AllegroContext* ctx);
void destruir_allegro(AllegroContext* ctx);

#endif // ALLEGRO_INIT_H