#include <stdio.h>
#include <stdbool.h>
#include "allegro_init.h"
#include "entidades.h"
#include "cenario.h"

void desenhar_tela_inicio(AllegroContext* ctx, ALLEGRO_FONT* fonte, ALLEGRO_BITMAP* fundo, bool mostrar_texto) {
    // Desenha o fundo
    if (fundo) {
        al_draw_scaled_bitmap(
            fundo,
            0, 0,
            al_get_bitmap_width(fundo), al_get_bitmap_height(fundo),
            0, 0,
            ctx->width, ctx->height,
            0
        );
    }
    else {
        al_clear_to_color(al_map_rgb(0, 0, 0));
    }

    // Texto piscando "Pressione ENTER"
    if (mostrar_texto) {
        al_draw_text(
            fonte,
            al_map_rgb(255, 255, 255),
            ctx->width / 2,
            ctx->height - 100,
            ALLEGRO_ALIGN_CENTRE,
            "Pressione ENTER"
        );
    }

    al_flip_display();
}

int main(void) {
    AllegroContext ctx;
    if (!inicializar_allegro(&ctx)) {
        fprintf(stderr, "Falha ao inicializar Allegro\n");
        return -1;
    }

    ALLEGRO_FONT* fonte = al_create_builtin_font();
    ALLEGRO_BITMAP* fundo_menu = al_load_bitmap("assets/img/Estruturas/ecoquest.png");
    if (!fundo_menu) {
        fprintf(stderr, "❌ Erro ao carregar ecoquest.png! Verifique o caminho.\n");
    }

    // -------- MENU DE INÍCIO --------
    bool no_menu = true;
    bool mostrar_texto = true;
    float tempo_texto = 0.0f;

    while (no_menu) {
        ALLEGRO_EVENT event;
        al_wait_for_event(ctx.event_queue, &event);

        if (event.type == ALLEGRO_EVENT_TIMER) {
            tempo_texto += 1.0 / 60.0; // timer de ~60 FPS
            if (tempo_texto >= 0.7f) { // pisca a cada 0.7 segundos
                mostrar_texto = !mostrar_texto;
                tempo_texto = 0.0f;
            }
        }
        else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (event.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                no_menu = false; // começa o jogo
            }
            else if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                destruir_allegro(&ctx);
                return 0; // sai
            }
        }
        else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            destruir_allegro(&ctx);
            return 0;
        }

        desenhar_tela_inicio(&ctx, fonte, fundo_menu, mostrar_texto);
    }

    if (fundo_menu) al_destroy_bitmap(fundo_menu);

    // -------- JOGO COMEÇA AQUI --------
    entidade jogador;
    iniciar_entidade(&jogador, (float)ctx.width, (float)ctx.height);

    JogoCenas cena_atual = MUNDO;
    CavernaState portas;
    init_portas(&portas, (float)ctx.width, (float)ctx.height);

    bool rodando = true;
    bool redesenhar = true;

    while (rodando) {
        ALLEGRO_EVENT event;
        al_wait_for_event(ctx.event_queue, &event);

        if (event.type == ALLEGRO_EVENT_TIMER) {
            ALLEGRO_KEYBOARD_STATE estado_teclado;
            al_get_keyboard_state(&estado_teclado);
            processar_teclado(&estado_teclado, &jogador);
            limitar_jogador(&jogador, (float)ctx.width, (float)ctx.height);
            redesenhar = true;
        }
        else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            rodando = false;
        }
        else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                rodando = false;
            else if (event.keyboard.keycode == ALLEGRO_KEY_E)
                if (checar_interacao_porta(&jogador, &cena_atual, &portas))
                    redesenhar = true;
        }

        if (redesenhar && al_is_event_queue_empty(ctx.event_queue)) {
            redesenhar = false;
            cenarios(cena_atual, &ctx, &portas);
            desenhar_jogador(&jogador, ctx.CoresFundo[4]);
            al_flip_display();
        }
    }

    destruir_allegro(&ctx);
    return 0;
}
