#include <stdio.h>
#include <stdbool.h>
#include <time.h>

#include "allegro_init.h"
#include "cenario.h"
#include "entidades.h"
#include "animais.h"

int main(void) {

    AllegroContext ctx;
    if (!inicializar_allegro(&ctx)) {
        fprintf(stderr, "Falha ao inicializar Allegro\n");
        return -1;
    }

    srand((unsigned int)time(NULL));

    Animal animais[4];
    init_animais(animais, &ctx);

    entidade jogador;
    iniciar_entidade(&jogador, (float)ctx.width, (float)ctx.height);
    JogoCenas cena_atual = CENARIO1;

    bool rodando = true;
    bool redesenhar = true;

    const char* nomes_cenas[] = {
        "CENARIO1",
        "CENARIO2",
        "CENARIO3",
        "CENARIO4"
    };

    while (rodando) {
        ALLEGRO_EVENT event;
        al_wait_for_event(ctx.event_queue, &event);

        if (event.type == ALLEGRO_EVENT_TIMER) {
            ALLEGRO_KEYBOARD_STATE estado_teclado;
            al_get_keyboard_state(&estado_teclado);
            processar_teclado(&estado_teclado, &jogador);

            limitar_jogador(&jogador, (float)ctx.largura, (float)ctx.altura);
            update_animais(animais, &ctx);

            redesenhar = true;
        }
        else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE ||
            (event.type == ALLEGRO_EVENT_KEY_DOWN &&
                event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)) {
            rodando = false;
        }

        if (redesenhar && al_is_event_queue_empty(ctx.event_queue)) {
            redesenhar = false;

            cena_atual = cenarios(cena_atual, &ctx, &jogador);
            draw_animais(animais);
            desenhar_jogador(&jogador, &ctx);

            printf("Jogador: (%.1f, %.1f) | Camera: (%.1f,%.1f) | Cenario: %s\n",
                jogador.x, jogador.y, camera_x, camera_y, nomes_cenas[cena_atual]);

            al_flip_display();
    }
	}

    destruir_allegro(&ctx);
    return 0;
}