#ifndef ENTIDADES_H
#define ENTIDADES_H

#include <stdbool.h>
#include <allegro5/keyboard.h>
#include <allegro5/allegro_color.h>
#include "allegro_init.h" // definir AllegroContext

#define PLAYER_RADIUS 6.0f
#define PLAYER_SPEED 2.0f

typedef struct {
    float x;
    float y;
    float raio;
    float velocidade;

    // Controle de animação
    int direcao;      // 0=frente, 1=costas, 2=esquerda, 3=direita
    bool andando;
} entidade;

/* protótipos */

void iniciar_entidade(entidade* p, float width, float height);
void desenhar_jogador(const entidade* p, const AllegroContext* ctx);
void limitar_jogador(entidade* jogador, float largura_mapa, float altura_mapa);
void processar_teclado(ALLEGRO_KEYBOARD_STATE* estado, entidade* jogador);
bool colisao(float cx, float cy, float r, float rx, float ry, float rw, float rh);

#endif // ENTIDADES_H