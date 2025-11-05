#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>

#include "menu.h"
#include "allegro_init.h"

void desenhar_tela_inicio(AllegroContext* ctx, ALLEGRO_FONT* fonte, ALLEGRO_BITMAP* fundo, bool mostrar_texto) {
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

bool executar_menu_inicial(AllegroContext* ctx) {
    ALLEGRO_FONT* fonte = al_create_builtin_font();
    ALLEGRO_BITMAP* fundo_menu = al_load_bitmap("assets/img/Estruturas/ecoquest.png");
    
    if (!fundo_menu) {
        fprintf(stderr, "Aviso: Nao foi possivel carregar ecoquest.png\n");
    }

    bool no_menu = true;
    bool mostrar_texto = true;
    float tempo_texto = 0.0f;
    bool iniciar_jogo = false;

    while (no_menu) {
        ALLEGRO_EVENT event;
        al_wait_for_event(ctx->event_queue, &event);

        if (event.type == ALLEGRO_EVENT_TIMER) {
            tempo_texto += 1.0 / 60.0;
            if (tempo_texto >= 0.7f) {
                mostrar_texto = !mostrar_texto;
                tempo_texto = 0.0f;
            }
        }
        else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (event.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                no_menu = false;
                iniciar_jogo = true;
            }
            else if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                no_menu = false;
                iniciar_jogo = false;
            }
        }
        else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            no_menu = false;
            iniciar_jogo = false;
        }

        desenhar_tela_inicio(ctx, fonte, fundo_menu, mostrar_texto);
    }

    if (fundo_menu) al_destroy_bitmap(fundo_menu);
    al_destroy_font(fonte);

    return iniciar_jogo;
}