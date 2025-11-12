#include "allegro_init.h"
#include <stdio.h>

int inicializar_allegro(AllegroContext* ctx) {
    if (!al_init()) {
        fprintf(stderr, "Erro: falha ao inicializar Allegro\n");
        return 0;
    }
    
    // Inicializa componentes do Allegro
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_image_addon();
    al_init_primitives_addon();
    al_install_keyboard();

    al_set_new_bitmap_flags(0);

    // Cria o display com as dimensões definidas
    ctx->display = al_create_display(LARGURA_TELA, ALTURA_TELA);
    if (!ctx->display) {
        fprintf(stderr, "Erro: falha ao criar display\n");
        return 0;
    }

    // Cria o timer para controle de FPS
    ctx->timer = al_create_timer(1.0 / 60.0);
    if (!ctx->timer) {
        al_destroy_display(ctx->display);
        fprintf(stderr, "Erro: falha ao criar timer\n");
        return 0;
    }

    // Carrega o mapa do jogo
    ctx->mapa = al_load_bitmap("./assets/img/estruturas/mapa.png");
    if (!ctx->mapa) {
        fprintf(stderr, "Aviso: falha ao carregar mapa.png\n");
        ctx->altura = ctx->largura = 0;
        ctx->metadealtura = ctx->metadelargura = 0;
    } else {
        ctx->altura = al_get_bitmap_height(ctx->mapa);
        ctx->largura = al_get_bitmap_width(ctx->mapa);
        ctx->metadealtura = ctx->altura / 2;
        ctx->metadelargura = ctx->largura / 2;
        
        // Cria sub-bitmaps para cada quadrante do mapa
        ctx->regioes[0] = al_create_sub_bitmap(ctx->mapa, 0, 0, ctx->metadelargura, ctx->metadealtura);
        ctx->regioes[1] = al_create_sub_bitmap(ctx->mapa, ctx->metadelargura, 0, ctx->metadelargura, ctx->metadealtura);
        ctx->regioes[2] = al_create_sub_bitmap(ctx->mapa, 0, ctx->metadealtura, ctx->metadelargura, ctx->metadealtura);
        ctx->regioes[3] = al_create_sub_bitmap(ctx->mapa, ctx->metadelargura, ctx->metadealtura, ctx->metadelargura, ctx->metadealtura);
    }
    
    ctx->font = NULL;  // Sem fonte por enquanto

    // Cria a fila de eventos
    ctx->event_queue = al_create_event_queue();
    if (!ctx->event_queue) {
        if (ctx->mapa) al_destroy_bitmap(ctx->mapa);
        if (ctx->timer) al_destroy_timer(ctx->timer);
        al_destroy_display(ctx->display);
        fprintf(stderr, "Erro: falha ao criar fila de eventos\n");
        return 0;
    }

    // Registra as fontes de eventos
    al_register_event_source(ctx->event_queue, al_get_display_event_source(ctx->display));
    al_register_event_source(ctx->event_queue, al_get_timer_event_source(ctx->timer));
    al_register_event_source(ctx->event_queue, al_get_keyboard_event_source());

    al_start_timer(ctx->timer);

    // Guarda as dimensões da tela
    ctx->width = al_get_display_width(ctx->display);
    ctx->height = al_get_display_height(ctx->display);

    // Define as cores de fundo para cada cenário
    ctx->CoresFundo[0] = al_map_rgb(0, 0, 0);       // Preto
    ctx->CoresFundo[1] = al_map_rgb(139, 0, 0);     // Vermelho escuro
    ctx->CoresFundo[2] = al_map_rgb(0, 100, 0);     // Verde escuro
    ctx->CoresFundo[3] = al_map_rgb(0, 0, 139);     // Azul escuro
    ctx->CoresFundo[4] = al_map_rgb(255, 255, 255); // Branco

    return 1;
}

void destruir_allegro(AllegroContext* ctx) {
    if (!ctx) return;
    
    // Destrói recursos na ordem inversa da criação
    for (int i = 0; i < 4; i++) {
        if (ctx->regioes[i]) al_destroy_bitmap(ctx->regioes[i]);
    }
    
    if (ctx->mapa) al_destroy_bitmap(ctx->mapa);
    if (ctx->font) al_destroy_font(ctx->font);
    if (ctx->timer) al_destroy_timer(ctx->timer);
    if (ctx->event_queue) al_destroy_event_queue(ctx->event_queue);
    if (ctx->display) al_destroy_display(ctx->display);
}