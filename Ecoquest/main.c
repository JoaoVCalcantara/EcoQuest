#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#include "allegro_init.h"
#include "cenario.h"
#include "entidades.h"
#include "menu.h"
#include "batalha.h"
#include "inimigo.h"
#include "sprites.h"

#define MAX_BOTS 4

int main(void) {

    AllegroContext ctx;
    if (!inicializar_allegro(&ctx)) {
        fprintf(stderr, "Falha ao inicializar Allegro\n");
        return -1;
    }

    // -------- MENU DE INICIO (FORA DA MAIN) --------
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

    // -------- INICIALIZA O JOGO --------
    entidade jogador;
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

    // Ativa sprites
    jogador.usar_sprite = sprite_animado_frames_valido(jogador.sprite_baixo) &&
        sprite_animado_frames_valido(jogador.sprite_cima);

    if (!jogador.usar_sprite) {
		printf("Aviso: Sprites do jogador nao carregaram corretamente. Usando circulo simples.\n");
    }



    
    // Cria bots em diferentes cenarios
    Bot bots[MAX_BOTS];
    iniciar_bot_com_sprite(&bots[0], 150.0f, 100.0f, "Raposa", "Carnivoro", CENARIO1, "assets/img/animais/raposa.png");
    iniciar_bot_com_sprite(&bots[1], 400.0f, 500.0f, "Jacare", "Carnivoro", CENARIO2, "assets/img/animais/jacare.png");
    iniciar_bot_com_sprite(&bots[2], 900.0f, 150.0f, "Boto", "Carnivoro", CENARIO3, "assets/img/animais/boto.png");
    iniciar_bot_com_sprite(&bots[3], 1000.0f, 600.0f, "Onca", "Carnivoro", CENARIO4, "assets/img/animais/onca.png");

    JogoCenas cena_atual = CENARIO1;
    
    bool rodando = true;
    bool redesenhar = true;
    float delta_time = 1.0f / 60.0f;

    ALLEGRO_FONT* fonte = al_create_builtin_font();
    
    const char* nomes_cenas[] = {
        "CENARIO1",
        "CENARIO2",
        "CENARIO3",
        "CENARIO4"
    };
    
    while (rodando) {
        ALLEGRO_EVENT event;
        // Espera por um evento
        al_wait_for_event(ctx.event_queue, &event);

        // --- Processamento de Eventos ---
        if (event.type == ALLEGRO_EVENT_TIMER) {
            ALLEGRO_KEYBOARD_STATE estado_teclado;
            al_get_keyboard_state(&estado_teclado);
            // processar_teclado só trata movimento, não interação.

            
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
                    case DIRECAO_CIMA:          // ← ADICIONAR
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
            
            // Atualiza todos os bots
            for (int i = 0; i < MAX_BOTS; i++) {
                atualizar_bot(&bots[i], 1280.0f, 720.0f, delta_time);
            }
            
            // Verifica colisao com bots ativos
            for (int i = 0; i < MAX_BOTS; i++) {
                if (bots[i].ativo && verificar_colisao_bot(&jogador, &bots[i])) {
                    printf("Colisao com %s!\n", bots[i].animal_data.nome);
                    
                    // Inicia batalha
                    iniciar_batalha(fonte, &bots[i].animal_data, ctx.event_queue, ctx.display);
                    
                    // Se o animal atingiu XP maximo, desativa o bot
                    if (bots[i].animal_data.experiencia >= 100) {
                        bots[i].ativo = false;
                        printf("%s foi totalmente educado e nao aparecera mais!\n", bots[i].animal_data.nome);
                    }
                    
                    // Recua o jogador para evitar colisao continua
                    jogador.y += 20.0f;
                }
            }
            
            redesenhar = true;
        }
        else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            rodando = false;
        }
        else if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
            rodando = false;
        }
        //Interação (pressionar 'E') é verificada apenas no evento de KEY_DOWN.
        else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (event.keyboard.keycode == ALLEGRO_KEY_E) {
                // Se a cena mudar (jogador entrou/saiu), força um redesenho imediato.
                if (checar_interacao_porta(&jogador, &cena_atual, &portas)) {
                    redesenhar = true;
                }
            }
        }

        

        // --- Seção de Desenho ---
        if (redesenhar && al_is_event_queue_empty(ctx.event_queue)) {
            redesenhar = false;

            
            // Desenha bots do cenario atual
            for (int i = 0; i < MAX_BOTS; i++) {
                if (bots[i].cenario == cena_atual) {
                    // Cor vermelha para bots
                    desenhar_bot(&bots[i], al_map_rgb(255, 50, 50), camera_x, camera_y);
                }
            }
            
            // Desenha o jogador
            desenhar_jogador(&jogador, ctx.CoresFundo[4]);
            
            // Info de debug
            al_draw_textf(fonte, al_map_rgb(255, 255, 255), 10, 10, 0, 
                         "Pos: (%.0f, %.0f) | Cenario: %s", 
                         jogador.x, jogador.y, nomes_cenas[cena_atual]);
            
            al_flip_display();
        }
    }
	destruir_entidade(&jogador);
    for(int i = 0; i < MAX_BOTS; i++) {
        destruir_bot(&bots[i]);
	}
    al_destroy_font(fonte);
    destruir_allegro(&ctx);
    return 0;
}