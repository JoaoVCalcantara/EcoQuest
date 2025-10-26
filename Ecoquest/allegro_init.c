#include "allegro_init.h"
#include <stdio.h>
#include <direct.h>  // para _getcwd
#include <string.h>

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

    al_set_new_bitmap_flags(ALLEGRO_VIDEO_BITMAP);

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

    // Cria a fila de eventos
    ctx->event_queue = al_create_event_queue();
    if (!ctx->event_queue) {
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

    // Define blending correto (remove transparência indevida)
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);

    // Mostra diretório de execução (útil para debug)
    char dir[512];
    _getcwd(dir, sizeof(dir));
    fprintf(stderr, "Diretório atual de execução: %s\n", dir);

    // Carrega o mapa do jogo
    ctx->mapa = al_load_bitmap("./assets/img/estruturas/mapa.png");
    if (!ctx->mapa) {
        fprintf(stderr, "Aviso: falha ao carregar mapa.png\n");
        ctx->altura = ctx->largura = 0;
        ctx->metadealtura = ctx->metadelargura = 0;
    }
    else {
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

    // Carregamento de sprites adicionais 
    ctx->onca = al_load_bitmap("./assets/img/animais/onca.png");
    ctx->boto = al_load_bitmap("./assets/img/animais/boto.png");
    ctx->jacare = al_load_bitmap("./assets/img/animais/jacare.png");
    ctx->raposa = al_load_bitmap("./assets/img/animais/raposa.png");

    // Sprites do Herói
    ctx->heroi_frente = al_load_bitmap("./assets/img/Heroi/parado_frente.png");
    ctx->heroi_costas = al_load_bitmap("./assets/img/Heroi/costas_direita.png");
    ctx->heroi_esquerda = al_load_bitmap("./assets/img/Heroi/parado_esquerda.png");
    ctx->heroi_direita = al_load_bitmap("./assets/img/Heroi/parado_direita.png");

    // Andando (a de baixo usa a mesma imagem do "parado_frente")
    ctx->heroi_andando_frente = ctx->heroi_frente;
    ctx->heroi_andando_costas = al_load_bitmap("./assets/img/Heroi/costas_direita.png");
    ctx->heroi_andando_esquerda = al_load_bitmap("./assets/img/Heroi/andando_esquerda.png");
    ctx->heroi_andando_direita = al_load_bitmap("./assets/img/Heroi/andando_direita.png");

    // Verificação de carregamento
    const char* nomes[] = {
        "parado_frente", "costas_direita", "parado_esquerda", "parado_direita",
        "andando_frente (usa parado_frente)", "costas_direita", "andando_esquerda", "andando_direita"
    };
    ALLEGRO_BITMAP* sprites[] = {
        ctx->heroi_frente, ctx->heroi_costas, ctx->heroi_esquerda, ctx->heroi_direita,
        ctx->heroi_andando_frente, ctx->heroi_andando_costas, ctx->heroi_andando_esquerda, ctx->heroi_andando_direita
    };

    for (int i = 0; i < 8; i++) {
        if (!sprites[i])
            fprintf(stderr, "? Falha ao carregar sprite: %s\n", nomes[i]);
        else {
            int w = al_get_bitmap_width(sprites[i]);
            int h = al_get_bitmap_height(sprites[i]);
            fprintf(stderr, "? Sprite '%s' carregado (%dx%d)\n", nomes[i], w, h);
        }
    }

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

    // Herói
    if (ctx->heroi_frente) al_destroy_bitmap(ctx->heroi_frente);
    if (ctx->heroi_costas) al_destroy_bitmap(ctx->heroi_costas);
    if (ctx->heroi_esquerda) al_destroy_bitmap(ctx->heroi_esquerda);
    if (ctx->heroi_direita) al_destroy_bitmap(ctx->heroi_direita);

    // Evita destruir o mesmo bitmap duas vezes
    if (ctx->heroi_andando_frente != ctx->heroi_frente && ctx->heroi_andando_frente)
        al_destroy_bitmap(ctx->heroi_andando_frente);

    if (ctx->heroi_andando_costas) al_destroy_bitmap(ctx->heroi_andando_costas);
    if (ctx->heroi_andando_esquerda) al_destroy_bitmap(ctx->heroi_andando_esquerda);
    if (ctx->heroi_andando_direita) al_destroy_bitmap(ctx->heroi_andando_direita);

    // Animais
    if (ctx->onca) al_destroy_bitmap(ctx->onca);
    if (ctx->boto) al_destroy_bitmap(ctx->boto);
    if (ctx->jacare) al_destroy_bitmap(ctx->jacare);
    if (ctx->raposa) al_destroy_bitmap(ctx->raposa);

    // Mapa
    if (ctx->mapa) al_destroy_bitmap(ctx->mapa);

    // Outros recursos
    if (ctx->font) al_destroy_font(ctx->font);
    if (ctx->timer) al_destroy_timer(ctx->timer);
    if (ctx->event_queue) al_destroy_event_queue(ctx->event_queue);
    if (ctx->display) al_destroy_display(ctx->display);
}
