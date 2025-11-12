#ifndef ENTIDADES_H
#define ENTIDADES_H

#include <stdbool.h>
#include <allegro5/keyboard.h>
#include <allegro5/allegro_color.h>

/* Constants */
#define PLAYER_RADIUS 35.0f
#define PLAYER_SPEED 2.0f

/* Estrutura da entidade (jogador ou bot) */
typedef struct {
    float x;
    float y;
    float raio;
    float velocidade;
} entidade;

/* Estrutura da câmera */
typedef struct camera {
    float x, y;
    float zoom;
} camera;

/* Protótipos */
void iniciar_entidade(entidade* p, float width, float height);
void desenhar_jogador(const entidade* p, ALLEGRO_COLOR cor);
void limitar_jogador(entidade* jogador, float largura_mapa, float altura_mapa);
void processar_teclado(ALLEGRO_KEYBOARD_STATE* estado, entidade* jogador);
bool colisao(float cx, float cy, float r, float rx, float ry, float rw, float rh);

#endif
