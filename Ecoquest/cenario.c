#include "entidades.h"
#include "cenario.h"
#include "allegro_init.h"
#include <math.h>
#include <allegro5/allegro_primitives.h>

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

    if (*camera_x_ptr < 0) *camera_x_ptr = 0;
    if (*camera_y_ptr < 0) *camera_y_ptr = 0;
    if (*camera_x_ptr > max_x) *camera_x_ptr = max_x;
    if (*camera_y_ptr > max_y) *camera_y_ptr = max_y;
}

JogoCenas verificar_area_atual(const entidade* jogador) {
    if (jogador->x < 640.0f) {
        if (jogador->y < 360.0f) {
            return CENARIO1; // Superior esquerdo
        } else {
            return CENARIO2; // Inferior esquerdo
        }
    } else {
        if (jogador->y < 360.0f) {
            return CENARIO3; // Superior direito
        } else {
            return CENARIO4; // Inferior direito
        }
    }
}

JogoCenas cenarios(JogoCenas atual, const AllegroContext* ctx, const entidade *jogador) {
    float largura_mapa = al_get_bitmap_width(ctx->mapa);
    float altura_mapa = al_get_bitmap_height(ctx->mapa);

    JogoCenas area_atual = verificar_area_atual(jogador);
    
    if (area_atual != atual) {
        atual = area_atual;
    }

    atualizar_camera(jogador, &camera_x, &camera_y,
                    (float)ctx->width, (float)ctx->height,
                    largura_mapa, altura_mapa);

    al_clear_to_color(ctx->CoresFundo[atual]); // Usa o índice do cenário como índice da cor
    
    // Desenha o mapa com zoom aplicado
    al_draw_scaled_bitmap(
        ctx->mapa, 
        camera_x, 
        camera_y, 
        ctx->width / ZOOM_FACTOR, 
        ctx->height / ZOOM_FACTOR, 
        0, 
        0, 
        ctx->width, 
        ctx->height, 
        0
    );
    
    return atual;
}