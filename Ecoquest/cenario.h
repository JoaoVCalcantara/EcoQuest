#ifndef CENARIO_H
#define CENARIO_H

#include <allegro5/allegro_color.h>
#include <stdbool.h>

#include "entidades.h" /* precisa de 'camera' e 'entidade' */

/* constantes */
#define NUM_ENTRADAS 4
#define ENTRADA_RATIO 0.125f

typedef enum {
    MUNDO,
    CAVERNA1,
    CAVERNA2,
    CAVERNA3,
    CAVERNA4
} JogoCenas;

typedef struct {
    float entradaX1[NUM_ENTRADAS];
    float entradaY1[NUM_ENTRADAS];
    float entradaX2[NUM_ENTRADAS];
    float entradaY2[NUM_ENTRADAS];
    float saidaX;
    float saidaY;
    float comprimentoPorta;
    float alturaPorta;
    float tamanho;
} CavernaState;

/* protótipos */
void desenhar_entradas(const CavernaState* state, ALLEGRO_COLOR cor);
void cenarios(JogoCenas atual, const struct AllegroContext* ctx, const CavernaState* state);
void init_portas(CavernaState* state, float width, float height);
bool checar_interacao_porta(entidade* jogador, JogoCenas* atual, CavernaState* state);

#endif // CENARIO_H
