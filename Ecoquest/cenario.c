#include "cenario.h"
#include "entidades.h"
#include "allegro_init.h"
#include <allegro5/allegro_primitives.h>


// desenha os quatro quadrados que representam a entrada das cavernas

void quadrado(AllegroContext *ctx) {

    const float fator_tamanho = 0.125f;
        const float tamanho = ctx->width * fator_tamanho;

        al_draw_filled_rectangle(0, 0, tamanho, tamanho,al_map_rgb(255,255,255));

        al_draw_filled_rectangle(ctx->width - tamanho, 0, ctx->width, tamanho, al_map_rgb(255, 255, 255));

        al_draw_filled_rectangle(0, ctx->height - tamanho, tamanho, ctx->height, al_map_rgb(255, 255, 255));

        al_draw_filled_rectangle(ctx->width - tamanho , ctx->height - tamanho, ctx->width, ctx->height, al_map_rgb(255, 255, 255));
    }

//desenha os cenarios, como cavernas e o mundo

void cenarios(CenasDoJogo atual, AllegroContext *ctx) {

    static float frame = 0.f;
    static float current_frame_y = 161 * 3.0;

    switch (atual) {
    case mundo:

        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_draw_bitmap(ctx->mundo, 0, 0, 0);
        quadrado(ctx);
        
        
        break;

    case caverna1:

        al_clear_to_color(al_map_rgb(0, 0, 0));
       

        break;
    case caverna2:

        al_clear_to_color(al_map_rgb(0, 0, 0));
        
        break;
    case caverna3:

        al_clear_to_color(al_map_rgb(0, 0, 0));
        

        break;
    case caverna4:

        al_clear_to_color(al_map_rgb(0, 0, 0));
        

        break;
    }
}
