#ifndef ENTIDADES_H
#define ENTIDADES_H

#include <allegro5/allegro_color.h>
#include <allegro5/keyboard.h>
#include <stdbool.h>
#include "sprites.h"

/* forward declarations */
struct CavernaState;
struct AllegroContext; /* necessário porque alguns protótipos usam AllegroContext* */

/* Constants */
#define PLAYER_RADIUS 17.0f
#define PLAYER_SPEED 2.0f

typedef enum {
    DIRECAO_CIMA,
	DIRECAO_BAIXO,
    DIRECAO_ESQ,
	DIRECAO_DIR
} Direcao;


/* Estrutura completa das entidades */
typedef struct {
    float x;
    float y;
    float raio;
    float velocidade;

	SpriteAnimadoFrameAFrame* sprite_cima;
    SpriteAnimadoFrameAFrame* sprite_baixo;
    SpriteAnimadoFrameAFrame* sprite_direita;
    SpriteAnimadoFrameAFrame* sprite_esquerda;

    SpriteAnimadoFrameAFrame* sprite_idle_cima;
    SpriteAnimadoFrameAFrame* sprite_idle_baixo;
    SpriteAnimadoFrameAFrame* sprite_idle_esquerda;
    SpriteAnimadoFrameAFrame* sprite_idle_direita;

    Direcao Direcao_atual;
    bool movendo;
    bool usar_sprite;
} entidade;

/* Dar tag à struct camera para permitir forward declarations */
typedef struct camera {
    float x, y;
    float zoom;
} camera;

/* protótipos */

void iniciar_entidade(entidade* p, float width, float height);
void desenhar_jogador(const entidade* p, ALLEGRO_COLOR cor);
void limitar_jogador(entidade* jogador, float largura_mapa, float altura_mapa);
void processar_teclado(ALLEGRO_KEYBOARD_STATE* estado, entidade* jogador);
bool colisao(float cx, float cy, float r, float rx, float ry, float rw, float rh);
void destruir_entidade(entidade* p);

#endif // ENTIDADES_H
