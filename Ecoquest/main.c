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

            CenasDoJogo atual = mundo;

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

                    cenarios(atual, &ctx);
                    desenharjogador(&jogador);
                    al_flip_display();
                }
            }

            destruir_allegro(&ctx);
            return 0;
        }