#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>

#include "allegro_init.h"
#include "entidades.h"
#include "cenario.h"
#include "batalha.h" // 🔹 ADICIONA ESTA LINHA

#define TAM_QUADRADO_CENTRO 80

void desenhar_tela_inicio(AllegroContext* ctx, ALLEGRO_FONT* fonte, ALLEGRO_BITMAP* fundo, bool mostrar_texto) {
    // Desenha o fundo
    if (fundo) {
        al_draw_scaled_bitmap(
            fundo,
            0, 0,
            al_get_bitmap_width(fundo), al_get_bitmap_height(fundo),
            0, 0,
            ctx->width, ctx->height,
            0
        );
    }
    else {
        al_clear_to_color(al_map_rgb(0, 0, 0));
    }

    // Texto piscando "Pressione ENTER"
    if (mostrar_texto) {
        al_draw_text(
            fonte,
            al_map_rgb(255, 255, 255),
            ctx->width / 2,
            ctx->height - 100,
            ALLEGRO_ALIGN_CENTRE,
            "Pressione ENTER"
        );
    }

    al_flip_display();
}

entidade jogador;
AllegroContext ctx;
JogoCenas cena_atual;

bool checar_quadrado_centro(entidade* jogador, const AllegroContext* ctx);
void desenhar_quadrado_centro(const AllegroContext* ctx);
bool batalha_finalizada = false;
int main(void) {
    if (!inicializar_allegro(&ctx)) {
        fprintf(stderr, "Falha ao inicializar Allegro\n");
        return -1;
    }

    ALLEGRO_FONT* fonte = al_create_builtin_font();
    ALLEGRO_BITMAP* fundo_menu = al_load_bitmap("assets/img/Estruturas/ecoquest.png");
    if (!fundo_menu) {
        fprintf(stderr, "Erro ao carregar ecoquest.png! Verifique o caminho.\n");
    }

    // -------- MENU DE INÍCIO --------
    bool no_menu = true;
    bool mostrar_texto = true;
    float tempo_texto = 0.0f;

    while (no_menu) {
        ALLEGRO_EVENT event;
        al_wait_for_event(ctx.event_queue, &event);

        if (event.type == ALLEGRO_EVENT_TIMER) {
            tempo_texto += 1.0 / 60.0;
            if (tempo_texto >= 0.7f) {
                mostrar_texto = !mostrar_texto;
                tempo_texto = 0.0f;
            }
        }
        else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (event.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                no_menu = false;
            }
            else if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                destruir_allegro(&ctx);
                return 0;
            }
        }
        else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            destruir_allegro(&ctx);
            return 0;
        }

        desenhar_tela_inicio(&ctx, fonte, fundo_menu, mostrar_texto);
    }

    if (fundo_menu) al_destroy_bitmap(fundo_menu);

    // -------- JOGO COMEÇA AQUI --------
    iniciar_entidade(&jogador, (float)ctx.width, (float)ctx.height);

    cena_atual = MUNDO;
    CavernaState portas;
    init_portas(&portas, (float)ctx.width, (float)ctx.height);

    bool rodando = true;
    bool redesenhar = true;

    while (rodando) {
        ALLEGRO_EVENT event;
        al_wait_for_event(ctx.event_queue, &event);

        if (event.type == ALLEGRO_EVENT_TIMER) {
            ALLEGRO_KEYBOARD_STATE estado_teclado;
            al_get_keyboard_state(&estado_teclado);
            processar_teclado(&estado_teclado, &jogador);
            limitar_jogador(&jogador, (float)ctx.width, (float)ctx.height);

            // Se o jogador entrar no quadrado central → inicia batalha
            if (cena_atual == MUNDO && !batalha_finalizada && checar_quadrado_centro(&jogador, &ctx)) {
                Animal tigre = { 0, 1, false, "Tigre", "Carnívoro" };

                // muda de cena para batalha
                cena_atual = BATALHA;
                iniciar_batalha(fonte, &tigre, ctx.event_queue, ctx.display);

                // Se o animal chegou ao nível máximo, some o bloco preto
                if (tigre.experiencia >= 100) {
                    batalha_finalizada = true; // desativa o ponto de batalha
                    printf("🐯 O tigre atingiu 100 de XP! Bloco preto removido.\n");
                }

                // 🔹 Ao terminar a batalha, ele recua dentro do mapa
                float recuo = 50;
                jogador.y += recuo;
                cena_atual = MUNDO;
            }

            redesenhar = true;
        }
        else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            rodando = false;
        }
        else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                rodando = false;
            else if (event.keyboard.keycode == ALLEGRO_KEY_E)
                if (checar_interacao_porta(&jogador, &cena_atual, &portas))
                    redesenhar = true;
        }

        if (redesenhar && al_is_event_queue_empty(ctx.event_queue)) {
            redesenhar = false;
            cenarios(cena_atual, &ctx, &portas, fonte);
            desenhar_jogador(&jogador, ctx.CoresFundo[4]);
            al_flip_display();
        }
    }

    destruir_allegro(&ctx);
    return 0;
}
