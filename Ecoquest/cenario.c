#include "entidades.h" // CORREÇÃO: Necessário para a definição COMPLETA de 'entidade'
#include "cenario.h"   // CORREÇÃO: Necessário para a definição COMPLETA de 'JogoCenas'
#include "allegro_init.h"
#include <math.h>

void init_portas(CavernaState* state, float width, float height) {
    state->tamanho = width * ENTRADA_RATIO;
    float size = state->tamanho;

    // Posições das 4 entradas nos cantos da tela
    float positions[NUM_ENTRADAS][4] = {
        {0.0f, 0.0f, size, size},
        {width - size, 0.0f, width, size},
        {0.0f, height - size, size, height},
        {width - size, height - size, width, height}
    };

    for (int i = 0; i < NUM_ENTRADAS; i++) {
        state->entradaX1[i] = positions[i][0];
        state->entradaY1[i] = positions[i][1];
        state->entradaX2[i] = positions[i][2];
        state->entradaY2[i] = positions[i][3];
    }

    state->saidaX = 0.0f;
    state->saidaY = 0.0f;
    state->comprimentoPorta = size;
    state->alturaPorta = size;
}

bool checar_interacao_porta(entidade* jogador, JogoCenas* atual, CavernaState* state) {
    if (*atual == MUNDO) {
        for (int i = 0; i < NUM_ENTRADAS; i++) {
            float rw = state->entradaX2[i] - state->entradaX1[i];
            float rh = state->entradaY2[i] - state->entradaY1[i];

            if (colisao(jogador->x, jogador->y, jogador->raio, state->entradaX1[i], state->entradaY1[i], rw, rh)) {
                *atual = (JogoCenas)(CAVERNA1 + i);

                state->saidaX = state->entradaX1[i];
                state->saidaY = state->entradaY1[i];
                return true;
            }
        }
    }
    else { // Se estiver dentro de uma caverna
        if (colisao(jogador->x, jogador->y, jogador->raio, state->saidaX, state->saidaY, state->comprimentoPorta, state->alturaPorta)) {
            *atual = MUNDO;
            // MELHORIA: Reposiciona o jogador fora da porta ao retornar para o mundo.
            reposicionar_jogador_saida(jogador, state);
            return true;
        }
    }
    return false;
}

/* desenha entradas (squares) */
void desenhar_entradas(const CavernaState* state, ALLEGRO_COLOR cor) {
    for (int i = 0; i < NUM_ENTRADAS; i++) {
        al_draw_filled_rectangle(
            state->entradaX1[i], state->entradaY1[i],
            state->entradaX2[i], state->entradaY2[i],
            cor
        );
    }
}

/* helper para desenhar a saída da caverna */
static void draw_exit(const CavernaState* state) {
    float exit_x2 = state->saidaX + state->comprimentoPorta;
    float exit_y2 = state->saidaY + state->alturaPorta;
    // MELHORIA: A saída agora é sempre um quadrado branco para se destacar.
    al_draw_filled_rectangle(state->saidaX, state->saidaY, exit_x2, exit_y2, al_map_rgb(255, 255, 255));
}

void cenarios(JogoCenas atual, const AllegroContext* ctx, const CavernaState* state) {
    switch (atual) {
    case MUNDO:
        al_clear_to_color(al_map_rgb(173, 216, 230)); // Fundo azul claro (melhor que branco puro)
        // MELHORIA: Desenha o mapa se ele foi carregado com sucesso.
        if (ctx->mapa) {
            al_draw_bitmap(ctx->mapa, 0, 0, 0);
        }
        desenhar_entradas(state, al_map_rgb(0, 0, 0));
        if (ctx->logo) {
            float logo_x = (ctx->width - al_get_bitmap_width(ctx->logo)) / 2;
            float logo_y = (ctx->height - al_get_bitmap_height(ctx->logo)) / 2;
            al_draw_bitmap(ctx->logo, logo_x, logo_y, 0);
        }

        break;
    case CAVERNA1:
        al_clear_to_color(ctx->CoresFundo[0]); // Preto
        draw_exit(state);
        break;
    case CAVERNA2:
        al_clear_to_color(ctx->CoresFundo[1]); // Vermelho escuro
        draw_exit(state);
        break;
    case CAVERNA3:
        al_clear_to_color(ctx->CoresFundo[2]); // Verde escuro
        draw_exit(state);
        break;
    case CAVERNA4:
        al_clear_to_color(ctx->CoresFundo[3]); // Azul escuro
        draw_exit(state);
        break;
    }
}
bool colisao_logo(const entidade* jogador, const AllegroContext* ctx) {
    float logo_x = (ctx->width - al_get_bitmap_width(ctx->logo)) / 2;
    float logo_y = (ctx->height - al_get_bitmap_height(ctx->logo)) / 2;
    float logo_w = al_get_bitmap_width(ctx->logo);
    float logo_h = al_get_bitmap_height(ctx->logo);

    return colisao(jogador->x, jogador->y, jogador->raio, logo_x, logo_y, logo_w, logo_h);
}
