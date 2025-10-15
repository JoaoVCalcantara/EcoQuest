#ifndef CENARIO_H
#define CENARIO_H

#include <allegro5/allegro_color.h>
#include <allegro5/allegro_font.h> // <-- Adicionado para ALLEGRO_FONT
#include <stdbool.h>


#define NUM_ENTRADAS 4
#define ENTRADA_RATIO 0.125f

// CORREÇÃO: Forward declarations para evitar includes cíclicos
struct AllegroContext;
typedef struct AllegroContext AllegroContext; // <-- Adicionado

    struct CavernaState;
enum JogoCenas;

typedef enum {
    MUNDO,
    CAVERNA1,
    CAVERNA2,
    CAVERNA3,
    CAVERNA4,
    BATALHA // nova cena
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

void desenhar_entradas(const struct CavernaState* state, ALLEGRO_COLOR cor);
void cenarios(JogoCenas atual, const struct AllegroContext* ctx, const struct CavernaState* state, ALLEGRO_FONT* fonte);
void init_portas(CavernaState* state, float width, float height);
bool checar_interacao_porta(entidade* jogador,JogoCenas* atual, CavernaState* state);
bool colisao_logo(const entidade* jogador, const AllegroContext* ctx);
bool checar_quadrado_centro(entidade* jogador, const AllegroContext* ctx);

struct entidade; // Forward declaration suficiente

#endif // CENARIO_H