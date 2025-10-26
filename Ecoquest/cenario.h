#ifndef CENARIO_H
#define CENARIO_H

#include <allegro5/allegro_color.h>
#include <stdbool.h>

#include "entidades.h"
#include "allegro_init.h"

#define ZOOM_FACTOR 4.0f

typedef enum {
    CENARIO1,
    CENARIO2,
    CENARIO3,
    CENARIO4
} JogoCenas;

extern float camera_x;
extern float camera_y;

JogoCenas verificar_area_atual(const entidade* jogador);
JogoCenas cenarios(JogoCenas atual, const AllegroContext* ctx, const entidade* jogador);
void atualizar_camera(const entidade* jogador, float* camera_x, float* camera_y,
    float largura_tela, float altura_tela,
    float largura_mapa, float altura_mapa);

#endif // CENARIO_H