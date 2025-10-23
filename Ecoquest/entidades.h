#ifndef ENTIDADES_H
#define ENTIDADES_H

#include <stdbool.h>
#include <allegro5/keyboard.h>
#include <allegro5/allegro_color.h>

/* forward declarations */
struct CavernaState;
struct AllegroContext; /* necessário porque alguns protótipos usam AllegroContext* */

/* Constants */
#define PLAYER_RADIUS 50.0f
#define PLAYER_SPEED 8.0f

/* Estrutura completa das entidades */
typedef struct {
    float x;
    float y;
    float raio;
    float velocidade;
} entidade;

/* Dar tag à struct camera para permitir forward declarations */
typedef struct camera {
    float x, y;
    float zoom;
} camera;

/* protótipos */

void iniciar_entidade(entidade* p, float width, float height);
void desenhar_jogador(const entidade* p, ALLEGRO_COLOR cor);
void limitar_jogador(entidade* p, float width, float height);
void processar_teclado(ALLEGRO_KEYBOARD_STATE* estado, entidade* jogador);
bool colisao(float cx, float cy, float r, float rx, float ry, float rw, float rh);
void reposicionar_jogador_saida(entidade* jogador, const struct CavernaState* portas);

#endif // ENTIDADES_H
