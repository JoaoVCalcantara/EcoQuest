#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>

#include "menu.h"
#include "allegro_init.h"

void desenhar_tela_inicio(AllegroContext* ctx, ALLEGRO_FONT* fonte, ALLEGRO_BITMAP* fundo, bool mostrar_texto) {
    if (fundo) {
        al_draw_scaled_bitmap(
            fundo,
            0, 0,
            (float)al_get_bitmap_width(fundo), (float)al_get_bitmap_height(fundo),
            0, 0,
            (float)ctx->width, (float)ctx->height,
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
            (float)ctx->width / 2.0f,
            (float)ctx->height - 100.0f,
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
            tempo_texto += 1.0f / 60.0f;
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

void mostrar_tutorial(ALLEGRO_FONT* fonte, ALLEGRO_DISPLAY* display, ALLEGRO_EVENT_QUEUE* event_queue) {
    if (!display || !fonte) return;

    int largura = al_get_display_width(display);
    int altura  = al_get_display_height(display);

    const char* linhas[] = {
        "TUTORIAL",
        "",
        "1 - Estude todos os animais para completar o bestiario.",
        "2 - Explore todos os terrenos.",
        "3 - Derrote os cacadores e, apos estudar os animais, o chefe sera liberado.",
        "",
        "W, A, S, D, voce se movimenta",
        "B Abre o Bestiario",
        "Pressione ENTER, ESPACO, ESC ou clique para fechar."
    };
    const int total_linhas = (int)(sizeof(linhas) / sizeof(linhas[0]));

    // Desenha overlay escuro
    al_draw_filled_rectangle(0, 0, (float)largura, (float)altura, al_map_rgba(0, 0, 0, 180));

    // Caixa central
    float box_w = largura * 0.75f;
    float box_h = altura * 0.5f;
    float box_x = ((float)largura - box_w) / 2.0f;
    float box_y = ((float)altura  - box_h) / 2.0f;

    al_draw_filled_rectangle(box_x, box_y, box_x + box_w, box_y + box_h, al_map_rgb(240, 240, 230));
    al_draw_rectangle(box_x, box_y, box_x + box_w, box_y + box_h, al_map_rgb(100, 70, 30), 2.0f);

    // Texto dentro da caixa
    float text_x = box_x + 20.0f;
    float text_y = box_y + 20.0f;
    float line_h = al_get_font_line_height(fonte) + 6.0f;

    for (int i = 0; i < total_linhas; i++) {
        ALLEGRO_COLOR cor = (i == 0) ? al_map_rgb(40, 20, 0) : al_map_rgb(30, 30, 30);
        al_draw_text(fonte, cor, text_x, text_y + i * line_h, ALLEGRO_ALIGN_LEFT, linhas[i]);
    }

    al_flip_display();

    // Espera por ENTER / SPACE / ESC / clique / fechar display
    bool esperando = true;
    while (esperando && event_queue) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            int k = ev.keyboard.keycode;
            if (k == ALLEGRO_KEY_ENTER || k == ALLEGRO_KEY_SPACE || k == ALLEGRO_KEY_ESCAPE) esperando = false;
        }
        else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
            esperando = false;
        }
        else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            esperando = false;
        }
    }

    // Redesenha a tela do menu após fechar (o chamador normalmente redesenha)
}