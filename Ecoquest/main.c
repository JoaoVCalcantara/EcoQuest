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

#define MAX_BOTS 4

int main(void) {
    // Inicializa gerador de numeros aleatorios para IA dos bots
    srand((unsigned int)time(NULL));

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

    // -------- INICIALIZA O JOGO --------
    entidade jogador;
    iniciar_entidade(&jogador, (float)ctx.width, (float)ctx.height);
    
    // Cria bots em diferentes cenarios
    Bot bots[MAX_BOTS];
    iniciar_bot(&bots[0], 150.0f, 100.0f, "Tigre", "Carnivoro", CENARIO1);
    iniciar_bot(&bots[1], 400.0f, 500.0f, "Panda", "Herbivoro", CENARIO2);
    iniciar_bot(&bots[2], 900.0f, 150.0f, "Aguia", "Carnivoro", CENARIO3);
    iniciar_bot(&bots[3], 1000.0f, 600.0f, "Urso", "Onivoro", CENARIO4);

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
        al_wait_for_event(ctx.event_queue, &event);

        if (event.type == ALLEGRO_EVENT_TIMER) {
            ALLEGRO_KEYBOARD_STATE estado_teclado;
            al_get_keyboard_state(&estado_teclado);
            processar_teclado(&estado_teclado, &jogador);
            limitar_jogador(&jogador, 1280.0f, 720.0f);
            
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
        else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE || 
                (event.type == ALLEGRO_EVENT_KEY_DOWN && 
                 event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)) {
            rodando = false;
        }
        
        if (redesenhar && al_is_event_queue_empty(ctx.event_queue)) {
            redesenhar = false;

            cena_atual = cenarios(cena_atual, &ctx, &jogador);
            
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

    al_destroy_font(fonte);
    destruir_allegro(&ctx);
    return 0;
}