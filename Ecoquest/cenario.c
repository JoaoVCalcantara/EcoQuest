#include "cenario.h"
#include "entidades.h"
#include "allegro_init.h"
#include <allegro5/allegro_primitives.h>

void quadrado(AllegroContext *ctx) {
   

     
    const float fator_tamanho = 0.125f;
        const float tamanho = ctx->width * fator_tamanho;

        

        
        al_draw_filled_rectangle(0, 0, tamanho, tamanho,al_map_rgb(255,255,255));

       
        al_draw_filled_rectangle(ctx->width - tamanho, 0, ctx->width, tamanho, al_map_rgb(255, 255, 255));

        
        al_draw_filled_rectangle(0, ctx->height - tamanho, tamanho, ctx->height, al_map_rgb(255, 255, 255));

        
        al_draw_filled_rectangle(ctx->width - tamanho , ctx->height - tamanho, ctx->width, ctx->height, al_map_rgb(255, 255, 255));
    }

void cenarios(CenasDoJogo atual, AllegroContext *ctx) {

    static float frame = 0.f;
    static float current_frame_y = 161 * 3.0;

    switch (atual) {
    case MUNDO:

        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_draw_bitmap(ctx->mundo, 0, 0, 0);
        quadrado(ctx);
        
        
        break;

    case ANIMAL:

        frame += 0.3f;
        if (frame > 3) {
            frame -= 3;
        }

            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_bitmap(ctx->batalha, 0, 0, 0);
            al_draw_bitmap_region(ctx->dragao, 191 * (int)frame, current_frame_y, 191, 161 * 3, ctx->width/2, ctx->height/2, 0);
            al_draw_filled_rectangle(ctx->width / 2.0f, ctx->height / 2.0f, ctx->width / 2.0f - ctx->tamanho / 2.0f, ctx->height / 2.0f - ctx->tamanho / 2.0f,al_map_rgb(0, 0, 0));
           
            break;
    }
}
