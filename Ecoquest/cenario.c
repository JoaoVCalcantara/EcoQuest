#include "cenario.h"
#include "entidades.h"
#include "allegro_init.h"
#include "cavernas.h"
#include <allegro5/allegro_primitives.h>
// desenha os quatro quadrados que representam a entrada das cavernas


void quadrado(float entradaX1[], float entradaY1[], float entradaX2[], float entradaY2[], ALLEGRO_COLOR cor) {
 
        al_draw_filled_rectangle(entradaX1[0], entradaY1[0], entradaX2[0], entradaY2[0], cor);
        al_draw_filled_rectangle(entradaX1[1], entradaY1[1], entradaX2[1], entradaY2[1], cor);
        al_draw_filled_rectangle(entradaX1[2], entradaY1[2], entradaX2[2], entradaY2[2], cor);
        al_draw_filled_rectangle(entradaX1[3], entradaY1[3], entradaX2[3], entradaY2[3], cor);
       
    }

//desenha os cenarios, como cavernas e o mundo

void cenarios(CenasDoJogo atual, AllegroContext *ctx) {
    
    static float frame = 0.f; 
    static int current_frame_y = 161;
    int spriteX = 0;
   
       

    switch (atual) {
    case mundo:

        al_clear_to_color(al_map_rgb(0, 0, 0));
        quadrado(entradaX1, entradaY1, entradaX2, entradaY2, ctx->CoresFundo[4]);


        break;

    case caverna1:
       
        
       

        al_clear_to_color(ctx->CoresFundo[0]);
        al_draw_bitmap_region(ctx->dragao,(int)frame,current_frame_y,191,161,spriteX,ctx->height/2,0);
        al_draw_filled_rectangle(saidaX, saidaY, comprimentoPorta, alturaPorta, ctx->CoresFundo[4]);

        break;
    case caverna2:
        al_clear_to_color(ctx->CoresFundo[1]);
        al_draw_filled_rectangle(saidaX, saidaY, comprimentoPorta, alturaPorta, ctx->CoresFundo[4]);

        break;
    case caverna3:
        al_clear_to_color(ctx->CoresFundo[2]);
        al_draw_filled_rectangle(saidaX, saidaY, comprimentoPorta, alturaPorta, ctx->CoresFundo[4]);

        break;
    case caverna4:
        al_clear_to_color(ctx->CoresFundo[3]);
        al_draw_filled_rectangle(saidaX, saidaY, comprimentoPorta, alturaPorta, ctx->CoresFundo[4]);

        break;
    }
}
