#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>

#include "allegro_init.h"
#include "cenario.h"
#include "entidades.h"
#include "batalha.h"

int main(void) {
    srand((unsigned int)time(NULL));

    AllegroContext ctx;
    if (!inicializar_allegro(&ctx)) {
        fprintf(stderr, "Falha ao inicializar Allegro\n");
        return -1;
    }

    entidade jogador;
    iniciar_entidade(&jogador, ctx.width, ctx.height);

    bool rodando = true;
    bool redesenhar = true;

    ALLEGRO_FONT* fonte = al_create_builtin_font();
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60);
    al_register_event_source(ctx.event_queue, al_get_timer_event_source(timer));
    al_start_timer(timer);

    JogoCenas cena_atual = CENARIO1;

    while (rodando) {
        ALLEGRO_EVENT event;
        al_wait_for_event(ctx.event_queue, &event);

        if (event.type == ALLEGRO_EVENT_TIMER) {
            ALLEGRO_KEYBOARD_STATE estado;
            al_get_keyboard_state(&estado);
            processar_teclado(&estado, &jogador);
            limitar_jogador(&jogador, 1280, 720);

            redesenhar = true;
        }
        else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            rodando = false;
        }

        if (redesenhar && al_is_event_queue_empty(ctx.event_queue)) {
            redesenhar = false;

            // Desenha o cenário e o bot parado
            cena_atual = cenarios(cena_atual, &ctx, &jogador);

            // Desenha o jogador
            desenhar_jogador(&jogador, al_map_rgb(0, 200, 255));

            al_flip_display();
        }
    }

    al_destroy_font(fonte);
    destruir_allegro(&ctx);
    return 0;
}
