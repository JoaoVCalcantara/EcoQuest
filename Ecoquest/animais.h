#ifndef ANIMAIS_H
#define ANIMAIS_H

#include "allegro_init.h"
#include <allegro5/allegro.h>

typedef struct {
    const char* nome;
    ALLEGRO_BITMAP* sprite_raw;    // imagem original
    ALLEGRO_BITMAP* sprite_scaled; // redimensionada para o mapa
    float x, y;
    float vx, vy;
    float velocidade;
    float bx, by, bw, bh;          // limites do bioma
    int  cooldown_mudanca;
    int largura, altura;           // tamanho final
} Animal;

// Inicializa os animais nos biomas
void init_animais(Animal animais[4], AllegroContext* ctx);

// Atualiza o movimento dos animais
void update_animais(Animal animais[4], AllegroContext* ctx);

// Desenha os animais na tela
void draw_animais(const Animal animais[4]);

#endif // ANIMAIS_H