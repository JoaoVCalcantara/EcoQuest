#include "allegro_init.h"
#include <stdio.h>

int inicializar_allegro(AllegroContext* ctx) {
    if (!al_init()) {
        fprintf(stderr, "Erro: al_init\n");
        return 0;
    }

    al_init_font_addon();
    al_init_ttf_addon();
    al_init_image_addon();
    al_init_primitives_addon();
    al_install_keyboard();

    al_set_new_bitmap_flags(0);

    //Usando as constantes definidas no .h
    ctx->display = al_create_display(LARGURA_TELA, ALTURA_TELA);
    if (!ctx->display) {
        fprintf(stderr, "Erro: display\n");
        return 0;
    }

    ctx->timer = al_create_timer(1.0 / 60.0);
    if (!ctx->timer) {
        al_destroy_display(ctx->display);
        fprintf(stderr, "Erro: timer\n");
        return 0;
    }

    ctx->mapa = al_load_bitmap("./assets/img/estruturas/mapa.png");
    if (!ctx->mapa) {
        fprintf(stderr, "Aviso: Nao foi possivel carregar o mapa.png\n");
    }
    if (ctx->mapa) {
        ctx->altura = al_get_bitmap_height(ctx->mapa);
        ctx->largura = al_get_bitmap_width(ctx->mapa);
        ctx->metadealtura = ctx->altura / 2;
        ctx->metadelargura = ctx->largura / 2;
        /* criar regioes ... */
    }
    else {
        ctx->altura = ctx->largura = 0;
        ctx->metadealtura = ctx->metadelargura = 0;
        /* lidar com regioes = NULL ou criar bitmaps fallback */
    }

    ctx->regioes[0] = al_create_sub_bitmap(ctx->mapa, 0, 0, ctx->metadelargura, ctx->metadealtura);
    ctx->regioes[1] = al_create_sub_bitmap(ctx->mapa, ctx->metadelargura, 0, ctx->metadelargura, ctx->metadealtura);
    ctx->regioes[2] = al_create_sub_bitmap(ctx->mapa, 0, ctx->metadealtura, ctx->metadelargura, ctx->metadealtura);
    ctx->regioes[3] = al_create_sub_bitmap(ctx->mapa, ctx->metadelargura, ctx->metadealtura, ctx->metadelargura, ctx->metadealtura);
   
    // Inicializar 'font' como NULL.
    ctx->font = NULL;

    ctx->event_queue = al_create_event_queue();
    if (!ctx->event_queue) {
        if (ctx->mapa) al_destroy_bitmap(ctx->mapa);
        if (ctx->font) al_destroy_font(ctx->font);
        al_destroy_timer(ctx->timer);
        al_destroy_display(ctx->display);
        fprintf(stderr, "Erro: event_queue\n");
        return 0;
    }

    al_register_event_source(ctx->event_queue, al_get_display_event_source(ctx->display));
    al_register_event_source(ctx->event_queue, al_get_timer_event_source(ctx->timer));
    al_register_event_source(ctx->event_queue, al_get_keyboard_event_source());

    al_start_timer(ctx->timer);

    ctx->width = al_get_display_width(ctx->display);
    ctx->height = al_get_display_height(ctx->display);

    ctx->CoresFundo[0] = al_map_rgb(0, 0, 0);       // Preto
    ctx->CoresFundo[1] = al_map_rgb(139, 0, 0);     // Vermelho escuro
    ctx->CoresFundo[2] = al_map_rgb(0, 100, 0);     // Verde escuro
    ctx->CoresFundo[3] = al_map_rgb(0, 0, 139);     // Azul escuro
    ctx->CoresFundo[4] = al_map_rgb(255, 255, 255); // Branco

    return 1;
}

void destruir_allegro(AllegroContext* ctx) {
    if (!ctx) return;
    if (ctx->mapa) al_destroy_bitmap(ctx->mapa);
    if (ctx->font) al_destroy_font(ctx->font);
    if (ctx->timer) al_destroy_timer(ctx->timer);
    if (ctx->event_queue) al_destroy_event_queue(ctx->event_queue);
    if (ctx->display) al_destroy_display(ctx->display);
}