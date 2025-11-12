#include "cenario.h"
#include <allegro5/allegro_primitives.h>

float camera_x = 0;
float camera_y = 0;

JogoCenas verificar_area_atual(const entidade* jogador) {
    if (jogador->x < 640) return CENARIO1;
    if (jogador->x < 1280) return CENARIO2;
    if (jogador->x < 1920) return CENARIO3;
    return CENARIO4;
}

JogoCenas cenarios(JogoCenas atual, const AllegroContext* ctx, const entidade* jogador) {
    al_clear_to_color(al_map_rgb(20, 20, 30));
    atualizar_camera(jogador, &camera_x, &camera_y, ctx->width, ctx->height, 1280, 720);

    // Bot fixo
    al_draw_filled_rectangle(600 - camera_x, 300 - camera_y, 650 - camera_x, 350 - camera_y, al_map_rgb(255, 50, 50));

    return verificar_area_atual(jogador);
}

void atualizar_camera(const entidade* jogador, float* camera_x, float* camera_y,
    float largura_tela, float altura_tela,
    float largura_mapa, float altura_mapa) {
    *camera_x = jogador->x - largura_tela / 2.0f;
    *camera_y = jogador->y - altura_tela / 2.0f;

    if (*camera_x < 0) *camera_x = 0;
    if (*camera_y < 0) *camera_y = 0;
    if (*camera_x > largura_mapa - largura_tela) *camera_x = largura_mapa - largura_tela;
    if (*camera_y > altura_mapa - altura_tela) *camera_y = altura_mapa - altura_tela;
}
