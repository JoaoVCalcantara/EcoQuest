#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

#include "entidades.h"
#include "allegro_init.h"
#include "cenario.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>





int main() {


    AllegroContext ctx;
    if (!inicializar_allegro(&ctx)) {
        printf("Falha ao inicializar Allegro\n");
        return -1;
    }

    entidade jogador;
    iniciarentidade(&jogador, ctx.width, ctx.height);

    CenasDoJogo atual = MUNDO;

    bool rodando = true;
    bool desenhar = true;

    while (rodando) {
        ALLEGRO_EVENT event;
        al_wait_for_event(ctx.event_queue, &event);

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            rodando = false;
        }
        else if (event.type == ALLEGRO_EVENT_TIMER) {
            ALLEGRO_KEYBOARD_STATE estado;
            al_get_keyboard_state(&estado);

            if (al_key_down(&estado, ALLEGRO_KEY_W)) jogador.y -= jogador.velocidade;
            if (al_key_down(&estado, ALLEGRO_KEY_S)) jogador.y += jogador.velocidade;
            if (al_key_down(&estado, ALLEGRO_KEY_A)) jogador.x -= jogador.velocidade;
            if (al_key_down(&estado, ALLEGRO_KEY_D)) jogador.x += jogador.velocidade;

            limitar_jogador(&jogador, ctx.width, ctx.height);

            desenhar = true;

            bool square1 = colisao(jogador.x, jogador.y, jogador.raio, 0, 0, ctx.width, ctx.height);

            if (al_key_down(&estado, ALLEGRO_KEY_E)){
                if (square1) {
                    atual = ANIMAL;
                    jogador.x = jogador.raio;
                    jogador.y = ctx.height; 
                }
            }
        } 

        if (desenhar && al_is_event_queue_empty(ctx.event_queue)) {
            desenhar = false;

            desenhar_quadrados(ctx.height, ctx.height);
            cenarios(atual, &jogador, &ctx);
           
            al_flip_display();
        }
    }

    destruir_allegro(&ctx);
    return 0;
}
