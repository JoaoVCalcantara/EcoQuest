#include <stdio.h>
#include <stdbool.h>

#include "allegro_init.h"
#include "cenario.h"
#include "entidades.h"

int main(void) {
    AllegroContext ctx;
    if (!inicializar_allegro(&ctx)) {
        fprintf(stderr, "Falha ao inicializar Allegro\n");
        return -1;
    }

    entidade jogador;
    iniciar_entidade(&jogador, (float)ctx.width, (float)ctx.height);
    

    JogoCenas cena_atual = MUNDO;
    CavernaState portas;
    init_portas(&portas, (float)ctx.width, (float)ctx.height);
    

    bool rodando = true;
    bool redesenhar = true;

    //Loop principal usando o padrão mais eficiente de Allegro.
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

            
            cenarios(cena_atual, &ctx, &portas);
            desenhar_jogador(&jogador,ctx.CoresFundo[4]);

            al_flip_display();
        }
    }

    destruir_allegro(&ctx);
    return 0;
}