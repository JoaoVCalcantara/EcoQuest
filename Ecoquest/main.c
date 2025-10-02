#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

#include "entidades.h"
#include "allegro_init.h"
#include "cenario.h"
#include "cavernas.h"

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

            initPortas(ctx.width, ctx.height);

            while (rodando) {
                ALLEGRO_EVENT event;
                al_wait_for_event(ctx.event_queue, &event);

                if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                    rodando = false;
                }
                else if (event.type == ALLEGRO_EVENT_TIMER) {
                    ALLEGRO_KEYBOARD_STATE estado;
                    al_get_keyboard_state(&estado);

                    processar_teclado(&estado, &jogador, ctx.width, ctx.height);

                    desenhar = true;

                }

                if (event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == ALLEGRO_KEY_E) {
                    checarEntrada(&jogador,&atual);
                }

                if (desenhar && al_is_event_queue_empty(ctx.event_queue)) {
                    desenhar = false;

<<<<<<< HEAD
                    cenarios(atual, &ctx);
                    desenharjogador(&jogador);
                    al_flip_display();
                }
            }

            destruir_allegro(&ctx);
            return 0;
        }
=======
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

        } 
            

        if (event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == ALLEGRO_KEY_E) {
            if (atual == MUNDO) {
                for (int i = 0; i < NUM_ENTRADAS; i++) {
                    if (colisao(jogador.x, jogador.y, jogador.raio,
                        entradaX[i], entradaY[i], comprimentoPorta, alturaPorta)) {
                        switch (i) {
                        case 0:
                            atual = CAVERNA_1;
                            break;
                        case 1:
                            atual = CAVERNA_2;
                            break;
                        case 2:
                            atual = CAVERNA_3;
                            break;
                        case 3: 
                            atual = CAVERNA_4;
                            break;
                        }
                       
                        break;
                    }
                }
            }
            else if (atual != MUNDO) {
                if (colisao(jogador.x, jogador.y, jogador.raio,
                    saidaX, saidaY, comprimentoPorta, alturaPorta)) {
                    atual = MUNDO;
                    
                }
            }
        }


        if (desenhar && al_is_event_queue_empty(ctx.event_queue)) {
            desenhar = false;

            cenarios(atual,&ctx,jogador);
            desenharjogador(&jogador);
            al_flip_display();
        }
    }

    destruir_allegro(&ctx);
    return 0;
}
>>>>>>> d9fdfa19086c7d0ddddcd8badce85e5e32dc0a15
