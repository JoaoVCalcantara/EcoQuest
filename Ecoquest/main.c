#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

#include "allegro_init.h"
#include "cenario.h"
#include "entidades.h"
#include "menu.h"
#include "batalha.h"
#include "inimigo.h"
#include "cacador.h"
#include "sprites.h"
#include "bestiario.h"

#define MAX_BOTS 4
#define MAX_CACADORES 4

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

    // ========== INICIALIZAÇÃO DO JOGADOR ==========
    const char* frames_jogador_cima[2] = {
        "assets/img/Heroi/rear_0.png",
        "assets/img/Heroi/rear_1.png"
    };
    const char* frames_jogador_baixo[2] = {
        "assets/img/Heroi/front_0.png",
        "assets/img/Heroi/front_1.png"
    };
    const char* frames_jogador_direita[2] = {
        "assets/img/heroi/player_right_0.png",
        "assets/img/heroi/player_right_1.png"
    };
    const char* frames_jogador_esquerda[2] = {
        "assets/img/heroi/player_left_0.png",
        "assets/img/heroi/player_left_1.png"
    };
    const char* arr_idle_baixo[1] = { "assets/img/Heroi/idle_down.png" };
    const char* arr_idle_cima[1] = { "assets/img/Heroi/idle_up.png" };
    const char* arr_idle_direita[1] = { "assets/img/Heroi/idle_right.png" };
    const char* arr_idle_esquerda[1] = { "assets/img/Heroi/idle_left.png" };

    entidade jogador = { 0 };
    iniciar_entidade(&jogador, (float)ctx.width, (float)ctx.height);

    jogador.sprite_cima = criar_sprite_animado_array(frames_jogador_cima, 2, 0.2f);
    jogador.sprite_baixo = criar_sprite_animado_array(frames_jogador_baixo, 2, 0.2f);
    jogador.sprite_esquerda = criar_sprite_animado_array(frames_jogador_esquerda, 2, 0.2f);
    jogador.sprite_direita = criar_sprite_animado_array(frames_jogador_direita, 2, 0.2f);

    jogador.sprite_idle_baixo = criar_sprite_animado_array(arr_idle_baixo, 1, 1.0f);
    jogador.sprite_idle_direita = criar_sprite_animado_array(arr_idle_direita, 1, 1.0f);
    jogador.sprite_idle_esquerda = criar_sprite_animado_array(arr_idle_esquerda, 1, 1.0f);
    jogador.sprite_idle_cima = criar_sprite_animado_array(arr_idle_cima, 1, 1.0f);
        
    jogador.usar_sprite = sprite_animado_frames_valido(jogador.sprite_baixo) &&
        sprite_animado_frames_valido(jogador.sprite_cima);

    if (!jogador.usar_sprite) {
        printf("Aviso: Sprites do jogador nao carregaram corretamente. Usando circulo simples.\n");
    }

    // ========== INICIALIZAÇÃO DOS ANIMAIS ==========
    Bot bots[MAX_BOTS];
    iniciar_bot_com_sprite_e_fundo(&bots[0], 150.0f, 100.0f, "onca", "Carnivoro", CENARIO1,
        "assets/img/animais/onca.png", "assets/img/estruturas/selva.png");
    
    iniciar_bot_com_sprite_e_fundo(&bots[1], 400.0f, 500.0f, "Jacare", "Carnivoro", CENARIO2,
        "assets/img/animais/jacare.png", "assets/img/estruturas/pantano.png");
    
    iniciar_bot_com_area_eliptica(&bots[2], 1000.0f, 150.0f, "Boto", "Carnivoro", CENARIO3,
        "assets/img/animais/boto.png",
        "assets/img/estruturas/lago.png",
        950.0f, 205.0f,
        165.0f, 105.0f);
    
    iniciar_bot_com_sprite_e_fundo(&bots[3], 1000.0f, 600.0f, "Lobo guara", "Onivoro", CENARIO4,
        "assets/img/animais/lobo_guara.png", "assets/img/estruturas/cerrado.png");

    // Inicializar fuga para todos os animais
    for (int i = 0; i < MAX_BOTS; i++) {
        bots[i].fugindo = false;
        bots[i].fuga_timer = 0.0f;
        bots[i].fuga_velocidade_multiplicador = 2.5f;
    }

    // ========== INICIALIZAÇÃO DOS CAÇADORES ==========
    Cacador cacadores[MAX_CACADORES];
    
    // Caçador 1 - CENARIO1
    iniciar_cacador_com_alvo(&cacadores[0], 200.0f, 200.0f, CENARIO1, &bots[0], 
        "Cacador da Selva", "assets/img/cacador/cacador.png");
    
    // Caçador 2 - CENARIO2
    iniciar_cacador_com_alvo(&cacadores[1], 300.0f, 600.0f, CENARIO2, &bots[1], 
        "Cacador do Pantano", "assets/img/cacador/cacador.png");
    
    // Caçador 3 - CENARIO3
    iniciar_cacador_com_alvo(&cacadores[2], 900.0f, 100.0f, CENARIO3, &bots[2], 
        "Cacador do Lago", "assets/img/cacador/cacador.png");
    
    // Caçador 4 - CENARIO4
    iniciar_cacador_com_alvo(&cacadores[3], 1100.0f, 500.0f, CENARIO4, &bots[3], 
        "Cacador do Cerrado", "assets/img/cacador/cacador.png");

    // ========== INICIALIZAÇÃO DO BOSS ==========
    CacadorChefe boss;
    iniciar_cacador_chefe(&boss, 640.0f, 360.0f, CENARIO1,
        "assets/img/cacador/boss_cacador.png",
        "assets/img/estruturas/selva.png");

    int cacadores_derrotados = 0;
    bool boss_foi_derrotado = false;

    // ========== BESTIÁRIO ==========
    Bestiario* bestiario = criar_bestiario();
    if (!bestiario) {
        fprintf(stderr, "ERRO CRITICO: Falha ao criar bestiario!\n");
    }
    else {
        for (int i = 0; i < MAX_BOTS; i++) {
            adicionar_especie_bestiario(bestiario, &bots[i].animal_data);
        }
        printf("[MAIN] Bestiario inicializado com %d especies\n", bestiario->total_especies);
    }

    // ========== VARIÁVEIS DE CONTROLE ==========
    JogoCenas cena_atual = CENARIO1;

    bool rodando = true;
    bool redesenhar = true;

    double tempo_anterior = al_get_time();
    double tempo_atual = 0.0;
    float delta_time = 0.0f;

    ALLEGRO_FONT* fonte = al_create_builtin_font();

    // ========== LOOP PRINCIPAL DO JOGO ==========
    while (rodando) {
        ALLEGRO_EVENT event;
        al_wait_for_event(ctx.event_queue, &event);

        if (event.type == ALLEGRO_EVENT_TIMER) {
            tempo_atual = al_get_time();
            delta_time = (float)(tempo_atual - tempo_anterior);
            tempo_anterior = tempo_atual;

            // ========== PROCESSAMENTO DO JOGADOR ==========
            ALLEGRO_KEYBOARD_STATE estado_teclado;
            al_get_keyboard_state(&estado_teclado);
            processar_teclado(&estado_teclado, &jogador);
            limitar_jogador_com_progresso(&jogador, 1280.0f, 720.0f, &jogador.progresso);

            // Atualizar animações do jogador
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

            // ========== ATUALIZAÇÃO DOS ANIMAIS COM CAÇADORES ==========
            for (int i = 0; i < MAX_BOTS; i++) {
                if (bots[i].ativo) {
                    atualizar_bot_com_cacador(&bots[i], &cacadores[i], delta_time, 1280.0f, 720.0f);
                }
            }

            // ========== ATUALIZAÇÃO DOS CAÇADORES ==========
            for (int i = 0; i < MAX_CACADORES; i++) {
                if (cacadores[i].ativo && !cacadores[i].derrotado && cacadores[i].alvo) {
                    atualizar_cacador(&cacadores[i],
                        cacadores[i].alvo->x,
                        cacadores[i].alvo->y,
                        cacadores[i].alvo->ativo,
                        delta_time,
                        1280.0f, 720.0f);
                    
                    // Aplicar dano ao animal se estiver em contato
                    aplicar_dano_cacador_em_animal(&cacadores[i], cacadores[i].alvo, delta_time);
                }
            }

            // ========== ATUALIZAÇÃO DO BOSS ==========
            if (boss.liberado && !boss.derrotado) {
                atualizar_cacador_chefe(&boss, &jogador, 1280.0f, 720.0f, delta_time);
            }

            // ========== COLISÃO JOGADOR x ANIMAIS ==========
            for (int i = 0; i < MAX_BOTS; i++) {
                if (bots[i].ativo && verificar_colisao_bot(&jogador, &bots[i])) {
                    printf("Colisao com %s!\n", bots[i].animal_data.nome);

                    // VERIFICAR SE O CAÇADOR DO CENÁRIO FOI DERROTADO
                    if (!cacadores[i].derrotado) {
                        // Mostrar mensagem de alerta
                        printf("[BLOQUEADO] Derrote o cacador deste cenario primeiro!\n");
                        
                        // Empurrar jogador para trás
                        jogador.y += 20.0f;
                        
                        // Pular para próxima iteração
                        continue;
                    }

                    // Se caçador foi derrotado, permitir interação
                    iniciar_batalha_com_bestiario(fonte, &bots[i].animal_data, ctx.event_queue, ctx.display, bestiario);

                    tempo_anterior = al_get_time();
                    bots[i].cooldown_colisao = 6.0f;

                    if (bots[i].animal_data.estudado) {
                        bots[i].ativo = false;
                        printf("%s foi totalmente estudado e nao aparecera mais!\n", bots[i].animal_data.nome);
                        
                        // Marcar cenário como completo
                        switch (bots[i].cenario) {
                            case CENARIO1:
                                jogador.progresso.cenario1_completo = true;
                                printf("[PROGRESSO] Cenario 1 completo! Novos areas desbloqueadas!\n");
                                break;
                            case CENARIO2:
                                jogador.progresso.cenario2_completo = true;
                                printf("[PROGRESSO] Cenario 2 completo! Novos areas desbloqueadas!\n");
                                break;
                            case CENARIO3:
                                jogador.progresso.cenario3_completo = true;
                                printf("[PROGRESSO] Cenario 3 completo! Novos areas desbloqueadas!\n");
                                break;
                            case CENARIO4:
                                jogador.progresso.cenario4_completo = true;
                                printf("[PROGRESSO] Todos os cenarios completados! Parabens!\n");
                                break;
                        }
                    }
                    else if (bots[i].animal_data.domado) {
                        printf("%s esta domado e permanece no mapa para estudio!\n", bots[i].animal_data.nome);
                    }

                    jogador.y += 20.0f;
                }
            }

            // ========== COLISÃO JOGADOR x CAÇADORES ==========
            for (int i = 0; i < MAX_CACADORES; i++) {
                if (cacadores[i].ativo && !cacadores[i].derrotado && 
                    verificar_colisao_cacador_jogador(&jogador, &cacadores[i])) {
                    
                    // VERIFICAR COOLDOWN DE BATALHA
                    if (cacadores[i].cooldown_batalha > 0.0f) {
                        printf("[COOLDOWN] Aguarde %.1f segundos para batalhar novamente.\n", 
                               cacadores[i].cooldown_batalha);
                        jogador.y += 25.0f;
                        continue;
                    }
                    
                    printf("Colisao com %s!\n", cacadores[i].nome);

                    iniciar_batalha_cacador_visual(fonte, &cacadores[i], ctx.event_queue, ctx.display, &jogador);

                    tempo_anterior = al_get_time();

                    if (cacadores[i].derrotado) {
                        cacadores_derrotados++;
                        printf("[PROGRESSO] Cacadores derrotados: %d/4\n", cacadores_derrotados);
                        
                        // Verificar se deve liberar o boss
                        if (verificar_liberacao_boss(cacadores_derrotados) && !boss.liberado) {
                            boss.liberado = true;
                            printf("\n============================================\n");
                            printf("   ATENCAO! O CACADOR CHEFE APARECEU!\n");
                            printf("   Procure por ele no mapa!\n");
                            printf("============================================\n");
                        }
                    }

                    // Empurrar jogador para trás
                    jogador.y += 25.0f;
                }
            }

            // ========== COLISÃO JOGADOR x BOSS ==========
            if (boss.liberado && !boss.derrotado && 
                verificar_colisao_cacador_jogador(&jogador, &boss.base)) {
                
                printf("\n============================================\n");
                printf("   BATALHA FINAL CONTRA O CACADOR CHEFE!\n");
                printf("============================================\n");

                iniciar_batalha_cacador_visual(fonte, &boss.base, ctx.event_queue, ctx.display, &jogador);

                tempo_anterior = al_get_time();

                if (boss.base.derrotado) {
                    boss.derrotado = true;
                    boss_foi_derrotado = true;
                    
                    printf("\n============================================\n");
                    printf("   PARABENS! VOCE VENCEU O JOGO!\n");
                    printf("   Todos os animais estao protegidos!\n");
                    printf("============================================\n");
                }

                jogador.y += 30.0f;
            }

            redesenhar = true;
        }
        else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE ||
            (event.type == ALLEGRO_EVENT_KEY_DOWN &&
                event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)) {
            rodando = false;
        }
        else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (event.keyboard.keycode == ALLEGRO_KEY_B) {
                if (bestiario) {
                    printf("%d especies, %d desbloqueadas\n",
                        bestiario->total_especies, bestiario->especies_desbloqueadas);
                    mostrar_bestiario_popup(bestiario, fonte, ctx.display, ctx.event_queue);
                    tempo_anterior = al_get_time();
                }
                else {
                    printf("[ERRO] Bestiario NULL!\n");
                } 
            }
        }

        // ========== RENDERIZAÇÃO ==========
        if (redesenhar && al_is_event_queue_empty(ctx.event_queue)) {
            redesenhar = false;

            cena_atual = cenarios(cena_atual, &ctx, &jogador);

            // Desenhar animais
            for (int i = 0; i < MAX_BOTS; i++) {
                if (bots[i].ativo && bots[i].cenario == cena_atual) {
                    desenhar_bot(&bots[i], al_map_rgb(255, 50, 50), camera_x, camera_y);
                }
            }

            // Desenhar caçadores
            for (int i = 0; i < MAX_CACADORES; i++) {
                if (cacadores[i].ativo && !cacadores[i].derrotado && cacadores[i].cenario == cena_atual) {
                    desenhar_cacador(&cacadores[i], al_map_rgb(150, 0, 0), camera_x, camera_y);
                }
            }

            // Desenhar boss
            if (boss.liberado && !boss.derrotado && boss.base.cenario == cena_atual) {
                desenhar_cacador(&boss.base, al_map_rgb(200, 0, 200), camera_x, camera_y);
                
                // Indicador de boss
                float boss_x_tela = (boss.base.x - camera_x) * ZOOM_FACTOR;
                float boss_y_tela = (boss.base.y - camera_y) * ZOOM_FACTOR;
                al_draw_text(fonte, al_map_rgb(255, 0, 255), boss_x_tela, boss_y_tela - 30.0f * ZOOM_FACTOR, 
                            ALLEGRO_ALIGN_CENTRE, "BOSS");
            }

            desenhar_jogador(&jogador, ctx.CoresFundo[4]);
            
            // ========== MENSAGENS NA TELA ==========
            if (fonte) {
                // Mensagens de área bloqueada
                if (!jogador.progresso.cenario1_completo && (jogador.y > 340.0f || jogador.x > 620.0f)) {
                    al_draw_text(fonte, al_map_rgb(255, 255, 0), 640, 10, ALLEGRO_ALIGN_CENTRE, 
                                 "AREA BLOQUEADA - Estude a Onca na selva primeiro!");
                }
                else if (jogador.progresso.cenario1_completo && !jogador.progresso.cenario2_completo && jogador.x > 620.0f) {
                    al_draw_text(fonte, al_map_rgb(255, 255, 0), 640, 10, ALLEGRO_ALIGN_CENTRE, 
                                 "AREA BLOQUEADA - Estude o Jacare no pantano primeiro!");
                }
                else if (jogador.progresso.cenario2_completo && !jogador.progresso.cenario3_completo && 
                         jogador.x > 620.0f && jogador.y > 340.0f) {
                    al_draw_text(fonte, al_map_rgb(255, 255, 0), 640, 10, ALLEGRO_ALIGN_CENTRE, 
                                 "AREA BLOQUEADA - Estude o Boto no lago primeiro!");
                }

                // Contador de caçadores derrotados
                char texto_cacadores[64];
                snprintf(texto_cacadores, sizeof(texto_cacadores), "Cacadores Derrotados: %d/4", cacadores_derrotados);
                al_draw_text(fonte, al_map_rgb(255, 200, 0), 10, 10, ALLEGRO_ALIGN_LEFT, texto_cacadores);

                // Alerta de boss liberado
                if (boss.liberado && !boss.derrotado) {
                    al_draw_text(fonte, al_map_rgb(255, 0, 255), 640, 700, ALLEGRO_ALIGN_CENTRE, 
                                "!!! CACADOR CHEFE NO MAPA !!!");
                }

                // Mensagem de vitória
                if (boss_foi_derrotado) {
                    al_draw_text(fonte, al_map_rgb(0, 255, 0), 640, 350, ALLEGRO_ALIGN_CENTRE, 
                                "JOGO COMPLETO! PARABENS!");
                }

                // NOVA: Mensagem de bloqueio de estudo
                for (int i = 0; i < MAX_BOTS; i++) {
                    if (bots[i].ativo && !cacadores[i].derrotado && bots[i].cenario == cena_atual) {
                        float dx = jogador.x - bots[i].x;
                        float dy = jogador.y - bots[i].y;
                        float dist = sqrtf(dx * dx + dy * dy);
                        
                        if (dist < 50.0f) {  // Próximo do animal
                            al_draw_text(fonte, al_map_rgb(255, 100, 100), 640, 680, 
                                        ALLEGRO_ALIGN_CENTRE, 
                                        "! Derrote o cacador para estudar este animal !");
                        }
                    }
                }
            }

            al_flip_display();
        }
    }

    // ========== LIMPEZA DE RECURSOS ==========
    destruir_entidade(&jogador);
    
    for (int i = 0; i < MAX_BOTS; i++) {
        destruir_bot(&bots[i]);
    }
    
    for (int i = 0; i < MAX_CACADORES; i++) {
        destruir_cacador(&cacadores[i]);
    }
    
    destruir_cacador(&boss.base);
    
    al_destroy_font(fonte);
    destruir_bestiario(bestiario);
    destruir_allegro(&ctx);

    return 0;
}