#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>


#include "allegro_init.h"
#include "entidades.h"

#include <stdio.h>
#include <stdbool.h>
#include <math.h>

//Inicia todos os addons usados do allegro

int inicializar_allegro(AllegroContext* ctx) {
    if (!al_init()) {
        fprintf(stderr, "Erro: falha ao inicializar Allegro.\n");
        return 0;
    }

    al_init_font_addon();
    al_init_ttf_addon();
    al_init_image_addon();
    al_init_primitives_addon();
    al_install_keyboard();

    ctx->display = al_create_display(1280, 720);
    if (!ctx->display) {
        fprintf(stderr, "Erro: falha ao criar display.\n");
        return 0;
    }

    ctx->timer = al_create_timer(1.0 / 60.0);
    if (!ctx->timer) {
        fprintf(stderr, "Erro: falha ao criar timer.\n");
        al_destroy_display(ctx->display);
        return 0;
    }

    ctx->font = al_load_font("./font.ttf", 50, 0);
    if (!ctx->font) {
        fprintf(stderr, "Erro: falha ao carregar fonte.\n");
        al_destroy_timer(ctx->timer);
        al_destroy_display(ctx->display);
        return 0;
    }

    ctx->mundo = al_load_bitmap("./bg.png");
    if (!ctx->mundo) {
        fprintf(stderr, "Erro: falha ao carregar mundo.png.\n");
        al_destroy_font(ctx->font);
        al_destroy_timer(ctx->timer);
        al_destroy_display(ctx->display);
        return 0;
    }
    ctx->batalha = al_load_bitmap("./campo_batalha.png");
    if (!ctx->batalha) {
        fprintf(stderr, "Erro: falha ao carregar campo_batalha.png.\n");
        al_destroy_bitmap(ctx->mundo);
        al_destroy_font(ctx->font);
        al_destroy_timer(ctx->timer);
        al_destroy_display(ctx->display);
        return 0;
    }


    ctx->dragao = al_load_bitmap("./dragon.png");
    if (!ctx->dragao) {
        fprintf(stderr, "Erro: falha ao carregar dragon.png.\n");
        al_destroy_bitmap(ctx->batalha);
        al_destroy_bitmap(ctx->mundo);
        al_destroy_font(ctx->font);
        al_destroy_timer(ctx->timer);
        al_destroy_display(ctx->display);
        return 0;
    }

    al_set_window_title(ctx->display, "Jogasso Triple AAA");

    ctx->event_queue = al_create_event_queue();
    if (!ctx->event_queue) {
        fprintf(stderr, "Erro: falha ao criar event_queue.\n");
        al_destroy_bitmap(ctx->dragao);
        al_destroy_bitmap(ctx->batalha);
        al_destroy_bitmap(ctx->mundo);
        al_destroy_font(ctx->font);
        al_destroy_timer(ctx->timer);
        al_destroy_display(ctx->display);
        return 0;
    }

    al_register_event_source(ctx->event_queue, al_get_display_event_source(ctx->display));
    al_register_event_source(ctx->event_queue, al_get_timer_event_source(ctx->timer));
    al_register_event_source(ctx->event_queue, al_get_keyboard_event_source());

    al_start_timer(ctx->timer);

    ctx->width = al_get_display_width(ctx->display);
    ctx->height = al_get_display_height(ctx->display);

    ctx->CoresFundo[0] = al_map_rgb(0, 0, 0);
    ctx->CoresFundo[1] = al_map_rgb(255, 0, 0);
    ctx->CoresFundo[2] = al_map_rgb(0, 255, 0);
    ctx->CoresFundo[3] = al_map_rgb(0, 0, 255);
    ctx->CoresFundo[4] = al_map_rgb(255, 255, 255);

    return 1; // sucesso
}

void destruir_allegro(AllegroContext* ctx) {
    if (ctx->dragao) al_destroy_bitmap(ctx->dragao);
    if (ctx->batalha)al_destroy_bitmap(ctx->batalha);
    if (ctx->mundo) al_destroy_bitmap(ctx->mundo);
    if (ctx->font) al_destroy_font(ctx->font);
    if (ctx->timer) al_destroy_timer(ctx->timer);
    if (ctx->event_queue) al_destroy_event_queue(ctx->event_queue);
    if (ctx->display) al_destroy_display(ctx->display);
}
