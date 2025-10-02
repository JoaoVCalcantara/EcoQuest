#include "cenario.h"
#include "entidades.h"
#include "allegro_init.h"
#include <allegro5/allegro_primitives.h>


// desenha os quatro quadrados que representam a entrada das cavernas

void quadrado(AllegroContext *ctx) {

    const float fator_tamanho = 0.125f;
        const float tamanho = ctx->width * fator_tamanho;

<<<<<<< HEAD
=======
     
        
>>>>>>> d9fdfa19086c7d0ddddcd8badce85e5e32dc0a15
        al_draw_filled_rectangle(0, 0, tamanho, tamanho,al_map_rgb(255,255,255));

        al_draw_filled_rectangle(ctx->width - tamanho, 0, ctx->width, tamanho, al_map_rgb(255, 255, 255));

        al_draw_filled_rectangle(0, ctx->height - tamanho, tamanho, ctx->height, al_map_rgb(255, 255, 255));

        al_draw_filled_rectangle(ctx->width - tamanho , ctx->height - tamanho, ctx->width, ctx->height, al_map_rgb(255, 255, 255));
    }

<<<<<<< HEAD
//desenha os cenarios, como cavernas e o mundo

void cenarios(CenasDoJogo atual, AllegroContext *ctx) {
=======
void cenarios(CenasDoJogo atual, AllegroContext *ctx,entidade jogador) {
>>>>>>> d9fdfa19086c7d0ddddcd8badce85e5e32dc0a15

    static float frame = 0.f;
    static float current_frame_y = 161 * 3.0;


    switch (atual) {
    case mundo:

        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_draw_bitmap(ctx->mundo, 0, 0, 0);
        quadrado(ctx);
        break;

<<<<<<< HEAD
    case caverna1:
=======
    case CAVERNA_1:

        frame += 0.3f;
        if (frame > 3) {
            frame -= 3;
        }
        

            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_bitmap(ctx->batalha, 0, 0, 0);
            al_draw_bitmap_region(ctx->dragao, 191 * (int)frame, current_frame_y, 191, 161 * 3, jogador.y, jogador.x, 0);
            al_draw_filled_rectangle(ctx->width / 2.0f, ctx->height / 2.0f, ctx->width / 2.0f - ctx->tamanho / 2.0f, ctx->height / 2.0f - ctx->tamanho / 2.0f,al_map_rgb(0, 0, 0));
           
            break;
    case CAVERNA_2:
>>>>>>> d9fdfa19086c7d0ddddcd8badce85e5e32dc0a15

        al_clear_to_color(al_map_rgb(0, 0, 0));
       

<<<<<<< HEAD
        break;
    case caverna2:

        al_clear_to_color(al_map_rgb(0, 0, 0));
        
        break;
    case caverna3:

        al_clear_to_color(al_map_rgb(0, 0, 0));
        

        break;
    case caverna4:

        al_clear_to_color(al_map_rgb(0, 0, 0));
        
=======

        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_draw_bitmap(ctx->batalha, 0, 0, 0);
        al_draw_bitmap_region(ctx->dragao, 191 * (int)frame, current_frame_y, 191, 161 * 3, jogador.x, jogador.y, 0);
        al_draw_filled_rectangle(ctx->width / 2.0f, ctx->height / 2.0f, ctx->width / 2.0f - ctx->tamanho / 2.0f, ctx->height / 2.0f - ctx->tamanho / 2.0f, al_map_rgb(0, 0, 0));

        break;
    case CAVERNA_3:

        frame += 0.3f;
        if (frame > 3) {
            frame -= 3;
        }


        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_draw_bitmap(ctx->batalha, 0, 0, 0);
        al_draw_bitmap_region(ctx->dragao, 191 * (int)frame, current_frame_y, 191, 161 * 3, ctx->width / 2, ctx->height / 2, 0);
        al_draw_filled_rectangle(ctx->width / 2.0f, ctx->height / 2.0f, ctx->width / 2.0f - ctx->tamanho / 2.0f, ctx->height / 2.0f - ctx->tamanho / 2.0f, al_map_rgb(0, 0, 0));

        break;
    case CAVERNA_4:

        frame += 0.3f;
        if (frame > 3) {
            frame -= 3;
        }


        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_draw_bitmap(ctx->batalha, 0, 0, 0);
        al_draw_bitmap_region(ctx->dragao, 191 * (int)frame, current_frame_y, 191, 161 * 3, ctx->width / 2, ctx->height / 2, 0);
        al_draw_filled_rectangle(ctx->width / 2.0f, ctx->height / 2.0f, ctx->width / 2.0f - ctx->tamanho / 2.0f, ctx->height / 2.0f - ctx->tamanho / 2.0f, al_map_rgb(0, 0, 0));
>>>>>>> d9fdfa19086c7d0ddddcd8badce85e5e32dc0a15

        break;
    }
}
