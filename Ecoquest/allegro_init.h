#ifndef ALLEGRO_INIT_H
#define ALLEGRO_INIT_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

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
    
    // Dimensões
    int width, height;          // Dimensões da tela
    int largura, altura;        // Dimensões do mapa
    int metadelargura, metadealtura;  // Metades do mapa
    
    // Cores
    ALLEGRO_COLOR CoresFundo[5];
} AllegroContext;

int inicializar_allegro(AllegroContext* ctx);
void destruir_allegro(AllegroContext* ctx);

#endif // ALLEGRO_INIT_H