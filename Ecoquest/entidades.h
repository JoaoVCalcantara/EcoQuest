#ifndef ENTIDADES_H
#define ENTIDADES_H

#include <stdbool.h>
// Incluindo o header necessário para ALLEGRO_KEYBOARD_STATE.
#include <allegro5/keyboard.h>
#include <allegro5/allegro_color.h>

// Forward declaration para o tipo usado em reposicionar_jogador_saida.
struct CavernaState;

// Constants
#define PLAYER_RADIUS 50.0f
#define PLAYER_SPEED 8.0f

// Estrutura completa das entidades
typedef struct {
    float x;
    float y;
    float raio;
    float velocidade;
} entidade;


void iniciar_entidade(entidade* p, float width, float height);
void desenhar_jogador(const entidade* p,ALLEGRO_COLOR cor);
void limitar_jogador(entidade* p, float width, float height);
// CORREÇÃO: processar_teclado usa o tipo completo ALLEGRO_KEYBOARD_STATE
void processar_teclado(ALLEGRO_KEYBOARD_STATE* estado, entidade* jogador);
bool colisao(float cx, float cy, float r, float rx, float ry, float rw, float rh);
// CORREÇÃO: Assinatura com forward declaration (struct CavernaState)
void reposicionar_jogador_saida(entidade* jogador, const struct CavernaState* portas);

#endif  // ENTIDADES_H