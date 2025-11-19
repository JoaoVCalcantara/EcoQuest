#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#include "allegro_init.h"
#include "debug.h"
#include "cenario.h"
#include "entidades.h"
#include "menu.h"
#include "batalha.h"
#include "inimigo.h"
#include "sprites.h"
#include "bestiario.h"

#define MAX_BOTS 4

int main(void) {

    srand((unsigned int)time(NULL));

    AllegroContext ctx = { 0 };

    if (!inicializar_allegro(&ctx)) {
        fprintf(stderr, "Falha ao inicializar Allegro\n");
        return -1;
    }

    if (!executar_menu_inicial(&ctx)) {
        destruir_allegro(&ctx);
        return 0;
    }

    const char* frames_jogador_cima[] = {
        "assets/img/Heroi/rear_0.png",
        "assets/img/Heroi/rear_1.png"
    };
    const char* frames_jogador_baixo[] = {
        "assets/img/Heroi/front_0.png",
        "assets/img/Heroi/front_1.png"
    };
    const char* frames_jogador_direita[] = {
        "assets/img/heroi/player_right_0.png",
        "assets/img/heroi/player_right_1.png"
    };
    const char* frames_jogador_esquerda[] = {
        "assets/img/heroi/player_left_0.png",
        "assets/img/heroi/player_left_1.png"
    };
    const char* arr_idle_baixo[] = { "assets/img/Heroi/idle_down.png" };
    const char* arr_idle_cima[] = { "assets/img/Heroi/idle_up.png" };
    const char* arr_idle_direita[] = { "assets/img/Heroi/idle_right.png" };
    const char* arr_idle_esquerda[] = { "assets/img/Heroi/idle_left.png" };

    entidade jogador = { 0 };
    iniciar_entidade(&jogador, (float)ctx.width, (float)ctx.height);

    jogador.sprite_cima = criar_sprite_animado_array(frames_jogador_cima, 2, 0.2f);
    jogador.sprite_baixo = criar_sprite_animado_array(frames_jogador_baixo, 2, 0.2f);
    jogador.sprite_esquerda = criar_sprite_animado_array(frames_jogador_esquerda, 2, 0.2f);
    jogador.sprite_direita = criar_sprite_animado_array(frames_jogador_direita, 2, 0.2f);

    jogador.sprite_idle_baixo = criar_sprite_animado_array(arr_idle_baixo, 1, 1.0f);
    jogador.sprite_idle_direita = criar_sprite_animado_array(arr_idle_direita, 1, 1.0f);
    jogador.sprite_idle_esquerda = criar_sprite_animado_array(arr_idle_esquerda, 1, 1.0f);

    const char* arr_idle_cima_fallback[] = { "assets/img/Heroi/rear_0.png" };
    jogador.sprite_idle_cima = criar_sprite_animado_array(arr_idle_cima_fallback, 1, 1.0f);

    jogador.usar_sprite = sprite_animado_frames_valido(jogador.sprite_baixo) &&
        sprite_animado_frames_valido(jogador.sprite_cima);

    if (!jogador.usar_sprite) {
        printf("Aviso: Sprites do jogador nao carregaram corretamente. Usando circulo simples.\n");
    }

    Bot bots[MAX_BOTS];
    iniciar_bot_com_sprite_e_fundo(&bots[0], 150.0f, 100.0f, "Raposa", "Carnivoro", CENARIO1,
        "assets/img/animais/raposa.png", "assets/img/estruturas/selva.png");
    iniciar_bot_com_sprite_e_fundo(&bots[1], 400.0f, 500.0f, "Jacare", "Carnivoro", CENARIO2,
        "assets/img/animais/jacare.png", "assets/img/estruturas/pantano.png");
    iniciar_bot_com_area_eliptica(&bots[2], 1000.0f, 150.0f, "Boto", "Carnivoro", CENARIO3,
        "assets/img/animais/boto.png",
        "assets/img/estruturas/lago.png",
        950.0f, 205.0f,
        165.0f, 105.0f);
    iniciar_bot_com_sprite_e_fundo(&bots[3], 1000.0f, 600.0f, "Onca", "Carnivoro", CENARIO4,
        "assets/img/animais/onca.png", "assets/img/estruturas/cerrado.png");

    Bestiario* bestiario = criar_bestiario();
    if (!bestiario) {
        fprintf(stderr, "ERRO CRÍTICO: Falha ao criar bestiário!\n");
    }
    else {
        for (int i = 0; i < MAX_BOTS; i++) {
            adicionar_especie_bestiario(bestiario, &bots[i].animal_data);
        }
        printf("[MAIN] Bestiário inicializado com %d espécies\n", bestiario->total_especies);
    }

    JogoCenas cena_atual = CENARIO1;

    bool rodando = true;
    bool redesenhar = true;

    double tempo_anterior = al_get_time();
    double tempo_atual = 0.0;
    float delta_time = 0.0f;

    ALLEGRO_FONT* fonte = al_create_builtin_font();

    DebugContext debug_ctx = { 0 };
    inicializar_debug(&debug_ctx, fonte);

    while (rodando) {
        ALLEGRO_EVENT event;
        al_wait_for_event(ctx.event_queue, &event);

        if (event.type == ALLEGRO_EVENT_TIMER) {
            tempo_atual = al_get_time();
            delta_time = (float)(tempo_atual - tempo_anterior);
            tempo_anterior = tempo_atual;

            ALLEGRO_KEYBOARD_STATE estado_teclado;
            al_get_keyboard_state(&estado_teclado);
            processar_teclado(&estado_teclado, &jogador);
            limitar_jogador(&jogador, 1280.0f, 720.0f);

            if (jogador.usar_sprite) {
                if (jogador.movendo) {
                    switch (jogador.Direcao_atual) {
                    case DIRECAO_CIMA:
                        atualizar_sprite_animado_frames(jogador.sprite_cima, delta_time);
                        break;
                    case DIRECAO_BAIXO:
                        atualizar_sprite_animado_frames(jogador.sprite_baixo, delta_time);
                        break;
                    case DIRECAO_ESQ:
                        atualizar_sprite_animado_frames(jogador.sprite_esquerda, delta_time);
                        break;
                    case DIRECAO_DIR:
                        atualizar_sprite_animado_frames(jogador.sprite_direita, delta_time);
                        break;
                    }
                }
                else {
                    switch (jogador.Direcao_atual) {
                    case DIRECAO_CIMA:
                        atualizar_sprite_animado_frames(jogador.sprite_idle_cima, delta_time);
                        break;
                    case DIRECAO_BAIXO:
                        atualizar_sprite_animado_frames(jogador.sprite_idle_baixo, delta_time);
                        break;
                    case DIRECAO_ESQ:
                        atualizar_sprite_animado_frames(jogador.sprite_idle_esquerda, delta_time);
                        break;
                    case DIRECAO_DIR:
                        atualizar_sprite_animado_frames(jogador.sprite_idle_direita, delta_time);
                        break;
                    }
                }
            }

            for (int i = 0; i < MAX_BOTS; i++) {
                atualizar_bot(&bots[i], 1280.0f, 720.0f, delta_time);
            }

            for (int i = 0; i < MAX_BOTS; i++) {
                if (bots[i].ativo && verificar_colisao_bot(&jogador, &bots[i])) {
                    printf("Colisao com %s!\n", bots[i].animal_data.nome);

                    iniciar_batalha_com_bestiario(fonte, &bots[i].animal_data, ctx.event_queue, ctx.display, bestiario);

                    tempo_anterior = al_get_time();

                    bots[i].cooldown_colisao = 6.0f;

                    if (bots[i].animal_data.estudado) {
                        bots[i].ativo = false;
                        printf("%s foi totalmente estudado e nao aparecera mais!\n", bots[i].animal_data.nome);
                    }
                    else if (bots[i].animal_data.domado) {
                        printf("%s esta domado e permanece no mapa para estudio!\n", bots[i].animal_data.nome);
                    }

                    jogador.y += 20.0f;
                }
            }

            debug_atualizar_fps(&debug_ctx, delta_time);

            redesenhar = true;
        }
        else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE ||
            (event.type == ALLEGRO_EVENT_KEY_DOWN &&
                event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)) {
            rodando = false;
        }
        else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            debug_processar_teclas(&debug_ctx, &event);

            if (event.keyboard.keycode == ALLEGRO_KEY_B) {
                printf("[DEBUG] Abrindo bestiario...\n");
                if (bestiario) {
                    printf("[DEBUG] %d especies, %d desbloqueadas\n",
                        bestiario->total_especies, bestiario->especies_desbloqueadas);
                    mostrar_bestiario_popup(bestiario, fonte, ctx.display, ctx.event_queue);
                    tempo_anterior = al_get_time();
                }
                else {
                    printf("[ERRO] Bestiario NULL!\n");
                }
            }
        }

        if (redesenhar && al_is_event_queue_empty(ctx.event_queue)) {
            redesenhar = false;

            cena_atual = cenarios(cena_atual, &ctx, &jogador);

            for (int i = 0; i < MAX_BOTS; i++) {
                if (bots[i].ativo && bots[i].cenario == cena_atual) {
                    desenhar_bot(&bots[i], al_map_rgb(255, 50, 50), camera_x, camera_y);
                }
            }

            desenhar_jogador(&jogador, ctx.CoresFundo[4]);

            if (debug_ctx.mostrar_grid) {
                debug_desenhar_grid(1280, 720, 64, al_map_rgba(255, 255, 255, 50),
                    camera_x, camera_y, ZOOM_FACTOR);
            }

            if (debug_ctx.mostrar_hitboxes) {
                debug_desenhar_hitbox_circulo(jogador.x, jogador.y, jogador.raio,
                    al_map_rgb(0, 255, 0), camera_x, camera_y, ZOOM_FACTOR);

                for (int i = 0; i < MAX_BOTS; i++) {
                    if (bots[i].ativo && bots[i].cenario == cena_atual) {
                        debug_desenhar_hitbox_circulo(bots[i].x, bots[i].y, bots[i].raio,
                            al_map_rgb(255, 0, 0), camera_x, camera_y, ZOOM_FACTOR);
                    }
                }
            }

            if (debug_ctx.mostrar_info_bots) {
                for (int i = 0; i < MAX_BOTS; i++) {
                    if (bots[i].ativo && bots[i].cenario == cena_atual) {
                        debug_desenhar_info_entidade(&debug_ctx, bots[i].animal_data.nome,
                            bots[i].x, bots[i].y, bots[i].direcao_x, bots[i].direcao_y,
                            camera_x, camera_y, ZOOM_FACTOR);
                    }
                }
            }

            if (debug_ctx.mostrar_areas_restritas) {
                for (int i = 0; i < MAX_BOTS; i++) {
                    if (bots[i].cenario == cena_atual) {
                        if (bots[i].area_restrita.tipo == AREA_ELIPTICA) {
                            if (bots[i].area_restrita.raio_horizontal > 0 &&
                                bots[i].area_restrita.raio_vertical > 0) {
                                debug_desenhar_area_eliptica(
                                    bots[i].area_restrita.centro_elipse_x,
                                    bots[i].area_restrita.centro_elipse_y,
                                    bots[i].area_restrita.raio_horizontal,
                                    bots[i].area_restrita.raio_vertical,
                                    al_map_rgb(0, 200, 255),
                                    camera_x, camera_y, ZOOM_FACTOR
                                );
                            }
                        }
                        else if (bots[i].area_restrita.tipo == AREA_CIRCULAR) {
                            float cx = (bots[i].area_restrita.centro_x - camera_x) * ZOOM_FACTOR;
                            float cy = (bots[i].area_restrita.centro_y - camera_y) * ZOOM_FACTOR;
                            al_draw_circle(cx, cy, bots[i].area_restrita.raio_area * ZOOM_FACTOR,
                                al_map_rgb(255, 200, 0), 2.0f);
                        }
                    }
                }
            }

            debug_desenhar_painel_info(&debug_ctx, jogador.x, jogador.y, cena_atual, MAX_BOTS);

            debug_desenhar_fps(&debug_ctx);

            al_flip_display();
        }
    }

    destruir_entidade(&jogador);
    for (int i = 0; i < MAX_BOTS; i++) {
        destruir_bot(&bots[i]);
    }
    al_destroy_font(fonte);
    destruir_debug(&debug_ctx);
    destruir_allegro(&ctx);
    destruir_bestiario(bestiario);

    return 0;
}