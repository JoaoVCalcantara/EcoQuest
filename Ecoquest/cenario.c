#include "cenario.h"
#include "entidades.h"
#include "allegro_init.h"


#include <allegro5/allegro_primitives.h>





void desenhar_quadrados(int width, int height) {
    int tamanho = width * 0.125;

    // canto superior esquerdo
    al_draw_filled_rectangle(0, 0, tamanho, tamanho, al_map_rgb(255, 255, 255));

    // canto superior direito
    al_draw_filled_rectangle(width - tamanho, 0, width, tamanho, al_map_rgb(255, 255, 255));

    // canto inferior esquerdo
    al_draw_filled_rectangle(0, height - tamanho, tamanho, height, al_map_rgb(255, 255, 255));

    // canto inferior direito
    al_draw_filled_rectangle(width - tamanho, height - tamanho, width, height, al_map_rgb(255, 255, 255));
}

void cenarios(CenasDoJogo atual, entidade *jogador, AllegroContext *ctx) {

    float frame = 0.f;
    float current_frame_y = 161 * 3.0;

    switch (atual) {
    case MUNDO:
        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_draw_bitmap(ctx->mundo, 0, 0, 0);
        desenhar_quadrados(ctx->width, ctx->height);
        desenharjogador(&jogador);
        break;

    case ANIMAL:

        frame += 0.2f;
        if (frame > 3) {
            frame -= 3;

            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_bitmap(ctx->batalha, 0, 0, 0);
            al_draw_bitmap_region(ctx->dragao, 191 * (int)frame, current_frame_y, 191, 161 * 3, ctx->width - 161, ctx->height - 161, 0);
            desenharjogador(&jogador);
            break;
        }
    }
}