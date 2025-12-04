#include "entidades.h"
#include "cenario.h"
#include "allegro_init.h"
#include "config_jogo.h"  // ADICIONAR

float camera_x = 0.0f;
float camera_y = 0.0f;

void atualizar_camera(const entidade* jogador, float* camera_x_ptr, float* camera_y_ptr,
    float largura_tela, float altura_tela,
    float largura_mapa, float altura_mapa) {
    if (!jogador || !camera_x_ptr || !camera_y_ptr) return;

    *camera_x_ptr = jogador->x - (largura_tela / 2.0f / ZOOM_FACTOR);
    *camera_y_ptr = jogador->y - (altura_tela / 2.0f / ZOOM_FACTOR);

    float max_x = largura_mapa - largura_tela / ZOOM_FACTOR;
    float max_y = altura_mapa - altura_tela / ZOOM_FACTOR;

    if (*camera_x_ptr < 0.0f) *camera_x_ptr = 0.0f;
    if (*camera_y_ptr < 0.0f) *camera_y_ptr = 0.0f;
    if (*camera_x_ptr > max_x) *camera_x_ptr = max_x;
    if (*camera_y_ptr > max_y) *camera_y_ptr = max_y;
}

JogoCenas verificar_area_atual(const entidade* jogador) {
    // SUBSTITUIR 640.0f e 360.0f por constantes
    if (jogador->x < LIMITE_HORIZONTAL_MAPA) {
        if (jogador->y < LIMITE_VERTICAL_MAPA) {
            return CENARIO1;
        }
        else {
            return CENARIO2;
        }
    }
    else {
        if (jogador->y < LIMITE_VERTICAL_MAPA) {
            return CENARIO3;
        }
        else {
            return CENARIO4;
        }
    }
}

JogoCenas cenarios(JogoCenas atual, const AllegroContext* ctx, const entidade* jogador) {
    float largura_mapa = (float)al_get_bitmap_width(ctx->mapa);
    float altura_mapa = (float)al_get_bitmap_height(ctx->mapa);

    JogoCenas area_atual = verificar_area_atual(jogador);

    if (area_atual != atual) {
        atual = area_atual;
    }

    atualizar_camera(jogador, &camera_x, &camera_y,
        (float)ctx->width, (float)ctx->height,
        largura_mapa, altura_mapa);

    al_clear_to_color(ctx->CoresFundo[atual]);

    al_draw_scaled_bitmap(
        ctx->mapa,
        camera_x,
        camera_y,
        (float)ctx->width / ZOOM_FACTOR,
        (float)ctx->height / ZOOM_FACTOR,
        0.0f,
        0.0f,
        (float)ctx->width,
        (float)ctx->height,
        0
    );

    return atual;
}