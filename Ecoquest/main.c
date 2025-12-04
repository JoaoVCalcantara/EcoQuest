#define _CRT_SECURE_NO_WARNINGS


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
#include "config_jogo.h"       // NOVO: Configurações do jogo
#include "config_ui.h"         // NOVO: Configurações de UI
#include "config_cacador.h"    // JÁ EXISTENTE




// Mensagens de cooldown – precisam estar no escopo de todo o main para serem
// visíveis tanto durante a atualização quanto na renderização.
static float msg_cooldown_animal = 0.0f;
static float msg_cooldown_cacador = 0.0f;

static bool forcar_reinicio = false;
void marcar_reinicio_jogo(void) { forcar_reinicio = true; }

// ========== NOVAS FUNÇÕES PARA VERIFICAÇÃO DE PROGRESSÃO ==========

// Verifica se todos os caçadores de um cenário foram derrotados
static bool todos_cacadores_cenario_derrotados(Cacador cacadores[TOTAL_CACADORES], JogoCenas cenario) {
    for (int i = 0; i < TOTAL_CACADORES; i++) {
        if (cacadores[i].cenario == cenario && !cacadores[i].derrotado) {
            return false;
        }
    }
    return true;
}

// Conta quantos animais de um cenário foram estudados
static int contar_animais_estudados_cenario(Bot bots[TOTAL_ANIMAIS], JogoCenas cenario) {
    int count = 0;
    for (int i = 0; i < TOTAL_ANIMAIS; i++) {
        if (bots[i].cenario == cenario && bots[i].animal_data.estudado) {
            count++;
        }
    }
    return count;
}

// Verifica se todos os animais foram estudados
static bool todos_animais_estudados(Bot bots[TOTAL_ANIMAIS]) {
    for (int i = 0; i < TOTAL_ANIMAIS; i++) {
        if (!bots[i].animal_data.estudado) {
            return false;
        }
    }
    return true;
}

// Conta total de caçadores derrotados de um cenário específico
static int contar_cacadores_derrotados_cenario(Cacador cacadores[TOTAL_CACADORES], JogoCenas cenario) {
    int count = 0;
    for (int i = 0; i < TOTAL_CACADORES; i++) {
        if (cacadores[i].cenario == cenario && cacadores[i].derrotado) {
            count++;
        }
    }
    return count;
}

// Atualiza progresso de cenários baseado em animais estudados
static void atualizar_progresso_cenarios(entidade* jogador, Bot bots[TOTAL_ANIMAIS]) {
    // Cenário 1 completo: pelo menos 1 animal estudado
    if (contar_animais_estudados_cenario(bots, CENARIO1) >= 1) {
        jogador->progresso.cenario1_completo = true;
    }

    // Cenário 2 completo: pelo menos 1 animal estudado
    if (contar_animais_estudados_cenario(bots, CENARIO2) >= 1) {
        jogador->progresso.cenario2_completo = true;
    }

    // Cenário 3 completo: pelo menos 1 animal estudado
    if (contar_animais_estudados_cenario(bots, CENARIO3) >= 1) {
        jogador->progresso.cenario3_completo = true;
    }

    // Cenário 4 completo: pelo menos 1 animal estudado
    if (contar_animais_estudados_cenario(bots, CENARIO4) >= 1) {
        jogador->progresso.cenario4_completo = true;
    }
}

// ========== FIM DAS NOVAS FUNÇÕES ==========

static void inicializar_jogo(AllegroContext* ctx,
    entidade* jogador,
    Bot bots[TOTAL_ANIMAIS],
    Cacador cacadores[TOTAL_CACADORES],
    CacadorChefe* boss,
    Bestiario** bestiario,
    int* cacadores_derrotados,
    bool* boss_foi_derrotado,
    ALLEGRO_FONT* fonte) {

    /* ========== LIBERAR SPRITES ANTIGOS DO JOGADOR ========== */
    if (jogador->sprite_cima) destruir_sprite_animado_frames(jogador->sprite_cima);
    if (jogador->sprite_baixo) destruir_sprite_animado_frames(jogador->sprite_baixo);
    if (jogador->sprite_direita) destruir_sprite_animado_frames(jogador->sprite_direita);
    if (jogador->sprite_esquerda) destruir_sprite_animado_frames(jogador->sprite_esquerda);
    if (jogador->sprite_idle_baixo) destruir_sprite_animado_frames(jogador->sprite_idle_baixo);
    if (jogador->sprite_idle_cima) destruir_sprite_animado_frames(jogador->sprite_idle_cima);
    if (jogador->sprite_idle_direita) destruir_sprite_animado_frames(jogador->sprite_idle_direita);
    if (jogador->sprite_idle_esquerda) destruir_sprite_animado_frames(jogador->sprite_idle_esquerda);

    /* Reset jogador */
    *jogador = (entidade){ 0 };
    iniciar_entidade(jogador, (float)ctx->width, (float)ctx->height);

    /* Sprites jogador - USANDO CONFIGURAÇÕES */
    const char* frames_jogador_cima[2] = { PATH_JOGADOR_CIMA_0, PATH_JOGADOR_CIMA_1 };
    const char* frames_jogador_baixo[2] = { PATH_JOGADOR_BAIXO_0, PATH_JOGADOR_BAIXO_1 };
    const char* frames_jogador_dir[2] = { PATH_JOGADOR_DIR_0, PATH_JOGADOR_DIR_1 };
    const char* frames_jogador_esq[2] = { PATH_JOGADOR_ESQ_0, PATH_JOGADOR_ESQ_1 };
    const char* idle_baixo[1] = { PATH_JOGADOR_IDLE_BAIXO };
    const char* idle_cima[1] = { PATH_JOGADOR_IDLE_CIMA };
    const char* idle_dir[1] = { PATH_JOGADOR_IDLE_DIR };
    const char* idle_esq[1] = { PATH_JOGADOR_IDLE_ESQ };

    jogador->sprite_cima = criar_sprite_animado_array(frames_jogador_cima, 2, TEMPO_FRAME_MOVIMENTO);
    jogador->sprite_baixo = criar_sprite_animado_array(frames_jogador_baixo, 2, TEMPO_FRAME_MOVIMENTO);
    jogador->sprite_direita = criar_sprite_animado_array(frames_jogador_dir, 2, TEMPO_FRAME_MOVIMENTO);
    jogador->sprite_esquerda = criar_sprite_animado_array(frames_jogador_esq, 2, TEMPO_FRAME_MOVIMENTO);
    jogador->sprite_idle_baixo = criar_sprite_animado_array(idle_baixo, 1, TEMPO_FRAME_IDLE);
    jogador->sprite_idle_cima = criar_sprite_animado_array(idle_cima, 1, TEMPO_FRAME_IDLE);
    jogador->sprite_idle_direita = criar_sprite_animado_array(idle_dir, 1, TEMPO_FRAME_IDLE);
    jogador->sprite_idle_esquerda = criar_sprite_animado_array(idle_esq, 1, TEMPO_FRAME_IDLE);
    jogador->usar_sprite = sprite_animado_frames_valido(jogador->sprite_baixo) &&
        sprite_animado_frames_valido(jogador->sprite_cima);

    /* Bots - USANDO CONFIGURAÇÕES */
    iniciar_bot_com_sprite_e_fundo(&bots[0], POS_INICIAL_ONCA_X, POS_INICIAL_ONCA_Y,
        NOME_ANIMAL_ONCA, TIPO_CARNIVORO, CENARIO1,
        PATH_ANIMAL_ONCA, PATH_ESTRUTURA_SELVA);

    iniciar_bot_com_sprite_e_fundo(&bots[1], POS_INICIAL_JACARE_X, POS_INICIAL_JACARE_Y,
        NOME_ANIMAL_JACARE, TIPO_CARNIVORO, CENARIO2,
        PATH_ANIMAL_JACARE, PATH_ESTRUTURA_PANTANO);

    iniciar_bot_com_area_eliptica(&bots[2], POS_INICIAL_BOTO_X, POS_INICIAL_BOTO_Y,
        NOME_ANIMAL_BOTO, TIPO_CARNIVORO, CENARIO3,
        PATH_ANIMAL_BOTO, PATH_ESTRUTURA_LAGO,
        ELIPSE_BOTO_CENTRO_X, ELIPSE_BOTO_CENTRO_Y, ELIPSE_BOTO_RAIO_H, ELIPSE_BOTO_RAIO_V);

    iniciar_bot_com_sprite_e_fundo(&bots[3], POS_INICIAL_LOBO_X, POS_INICIAL_LOBO_Y,
        NOME_ANIMAL_LOBO, TIPO_ONIVORO, CENARIO4,
        PATH_ANIMAL_LOBO, PATH_ESTRUTURA_CERRADO);

    /* Novos Animais */
    /* SELVA - 2 novos */
    iniciar_bot_com_sprite_e_fundo(&bots[4], POS_INICIAL_HARPIA_X, POS_INICIAL_HARPIA_Y,
        NOME_ANIMAL_HARPIA, TIPO_CARNIVORO, CENARIO1,
        PATH_ANIMAL_HARPIA, PATH_ESTRUTURA_SELVA);

    iniciar_bot_com_sprite_e_fundo(&bots[5], POS_INICIAL_MICO_X, POS_INICIAL_MICO_Y,
        NOME_ANIMAL_MICO, TIPO_ONIVORO, CENARIO1,
        PATH_ANIMAL_MICO, PATH_ESTRUTURA_SELVA);

    /* PANTANO - 1 novo */
    iniciar_bot_com_sprite_e_fundo(&bots[6], POS_INICIAL_CAPIVARA_X, POS_INICIAL_CAPIVARA_Y,
        NOME_ANIMAL_CAPIVARA, TIPO_HERBIVORO, CENARIO2,
        PATH_ANIMAL_CAPIVARA, PATH_ESTRUTURA_PANTANO);

    /* LAGO - 1 novo */
    iniciar_bot_com_area_eliptica(&bots[7], POS_INICIAL_PIRARUCU_X, POS_INICIAL_PIRARUCU_Y,
        NOME_ANIMAL_PIRARUCU, TIPO_CARNIVORO, CENARIO3,
        PATH_ANIMAL_PIRARUCU, PATH_ESTRUTURA_LAGO,
        ELIPSE_BOTO_CENTRO_X, ELIPSE_BOTO_CENTRO_Y, ELIPSE_BOTO_RAIO_H, ELIPSE_BOTO_RAIO_V);

    /* CERRADO - 1 novo */
    iniciar_bot_com_sprite_e_fundo(&bots[8], POS_INICIAL_TAMANDUA_X, POS_INICIAL_TAMANDUA_Y,
        NOME_ANIMAL_TAMANDUA, TIPO_INSETIVORO, CENARIO4,
        PATH_ANIMAL_TAMANDUA, PATH_ESTRUTURA_CERRADO);

    /* Caçadores - USANDO CONFIGURAÇÕES */
    iniciar_cacador_com_alvo(&cacadores[0], POS_INICIAL_CACADOR1_X, POS_INICIAL_CACADOR1_Y,
        CENARIO1, &bots[0], NOME_CACADOR_SELVA, PATH_CACADOR_NORMAL);

    iniciar_cacador_com_alvo(&cacadores[1], POS_INICIAL_CACADOR2_X, POS_INICIAL_CACADOR2_Y,
        CENARIO2, &bots[1], NOME_CACADOR_PANTANO, PATH_CACADOR_NORMAL);

    iniciar_cacador_com_alvo(&cacadores[2], POS_INICIAL_CACADOR3_X, POS_INICIAL_CACADOR3_Y,
        CENARIO3, &bots[2], NOME_CACADOR_LAGO, PATH_CACADOR_NORMAL);

    iniciar_cacador_com_alvo(&cacadores[3], POS_INICIAL_CACADOR4_X, POS_INICIAL_CACADOR4_Y,
        CENARIO4, &bots[3], NOME_CACADOR_CERRADO, PATH_CACADOR_NORMAL);

    /* Novos Caçadores */
    iniciar_cacador_com_alvo(&cacadores[4], POS_INICIAL_CACADOR5_X, POS_INICIAL_CACADOR5_Y,
        CENARIO1, &bots[4], NOME_CACADOR_SELVA2, PATH_CACADOR_NORMAL);

    iniciar_cacador_com_alvo(&cacadores[5], POS_INICIAL_CACADOR6_X, POS_INICIAL_CACADOR6_Y,
        CENARIO2, &bots[6], NOME_CACADOR_PANTANO2, PATH_CACADOR_NORMAL);

    iniciar_cacador_com_alvo(&cacadores[6], POS_INICIAL_CACADOR7_X, POS_INICIAL_CACADOR7_Y,
        CENARIO3, &bots[7], NOME_CACADOR_LAGO2, PATH_CACADOR_NORMAL);

    iniciar_cacador_com_alvo(&cacadores[7], POS_INICIAL_CACADOR8_X, POS_INICIAL_CACADOR8_Y,
        CENARIO4, &bots[8], NOME_CACADOR_CERRADO2, PATH_CACADOR_NORMAL);

    for (int i = 0; i < TOTAL_CACADORES; i++) {
        cacadores[i].raio_deteccao = RAIO_DETECCAO_PADRAO;
        cacadores[i].raio_abandono = RAIO_ABANDONO_PADRAO;
        cacadores[i].velocidade = VELOCIDADE_PATRULHA;
        cacadores[i].velocidade_perseguicao = VELOCIDADE_PERSEGUICAO;
        cacadores[i].tempo_dano_interval = COOLDOWN_DANO_CACADOR;
        cacadores[i].ativo = false;

        // DEBUG: Verificar configuração de alvos
        if (cacadores[i].alvo) {
            printf("[INIT] Cacador %d (%s) -> Alvo: %s (Bot %d)\n",
                i, cacadores[i].nome,
                cacadores[i].alvo->animal_data.nome,
                (int)(cacadores[i].alvo - bots));  // Índice do bot
        }
        else {
            printf("[ERRO] Cacador %d (%s) SEM ALVO!\n", i, cacadores[i].nome);
        }
    }

    /* Boss - USANDO CONFIGURAÇÕES */
    iniciar_cacador_chefe(boss, POS_INICIAL_BOSS_X, POS_INICIAL_BOSS_Y, CENARIO1,
        PATH_CACADOR_BOSS, PATH_ESTRUTURA_SELVA);

    *cacadores_derrotados = 0;
    *boss_foi_derrotado = false;

    /* Reinicia bestiario */
    if (*bestiario) destruir_bestiario(*bestiario);
    *bestiario = criar_bestiario();
    if (*bestiario) {
        for (int i = 0; i < TOTAL_ANIMAIS; i++)
            adicionar_especie_bestiario(*bestiario, &bots[i].animal_data);
    }

    msg_cooldown_animal = 0.0f;
    msg_cooldown_cacador = 0.0f;
    forcar_reinicio = false;
}

static void mostrar_tela_derrota(ALLEGRO_FONT* fonte, ALLEGRO_DISPLAY* display, ALLEGRO_EVENT_QUEUE* queue) {
    al_clear_to_color(al_map_rgb(COR_FUNDO_DERROTA_TELA_R, COR_FUNDO_DERROTA_TELA_G, COR_FUNDO_DERROTA_TELA_B));

    al_draw_text(fonte, al_map_rgb(COR_TEXTO_VERMELHO_R, COR_TEXTO_VERMELHO_G, COR_TEXTO_VERMELHO_B),
        POS_TEXTO_CENTRO_X, POS_TEXTO_DERROTA_TITULO_Y, ALLEGRO_ALIGN_CENTRE, MSG_VOCE_PERDEU);

    al_draw_text(fonte, al_map_rgb(COR_TEXTO_CINZA_CLARO_R, COR_TEXTO_CINZA_CLARO_G, COR_TEXTO_CINZA_CLARO_B),
        POS_TEXTO_CENTRO_X, POS_TEXTO_DERROTA_DESC_Y, ALLEGRO_ALIGN_CENTRE, MSG_ANIMAL_CAPTURADO);

    al_draw_text(fonte, al_map_rgb(COR_TEXTO_AMARELO_R, COR_TEXTO_AMARELO_G, COR_TEXTO_AMARELO_B),
        POS_TEXTO_CENTRO_X, POS_TEXTO_DERROTA_ENTER_Y, ALLEGRO_ALIGN_CENTRE, MSG_PRESSIONE_ENTER_REINICIAR);

    al_flip_display();

    while (true) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(queue, &ev);
        if (ev.type == ALLEGRO_EVENT_KEY_DOWN && ev.keyboard.keycode == ALLEGRO_KEY_ENTER) break;
        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) break;
    }
}

int main() {
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


    if (ctx.font && ctx.display && ctx.event_queue) {
        mostrar_tutorial(ctx.font, ctx.display, ctx.event_queue);

        // ========== INICIALIZAÇÃO DO JOGADOR - CORREÇÃO DA ORDEM ==========
        entidade jogador = { 0 };

        // **IMPORTANTE**: Inicializa a estrutura ANTES de criar sprites
        iniciar_entidade(&jogador, (float)ctx.width, (float)ctx.height);

        // Agora SIM podemos criar os sprites
        const char* frames_jogador_cima[2] = { PATH_JOGADOR_CIMA_0, PATH_JOGADOR_CIMA_1 };
        const char* frames_jogador_baixo[2] = { PATH_JOGADOR_BAIXO_0, PATH_JOGADOR_BAIXO_1 };
        const char* frames_jogador_direita[2] = { PATH_JOGADOR_DIR_0, PATH_JOGADOR_DIR_1 };
        const char* frames_jogador_esquerda[2] = { PATH_JOGADOR_ESQ_0, PATH_JOGADOR_ESQ_1 };
        const char* arr_idle_baixo[1] = { PATH_JOGADOR_IDLE_BAIXO };
        const char* arr_idle_cima[1] = { PATH_JOGADOR_IDLE_CIMA };
        const char* arr_idle_direita[1] = { PATH_JOGADOR_IDLE_DIR };
        const char* arr_idle_esquerda[1] = { PATH_JOGADOR_IDLE_ESQ };

        jogador.sprite_cima = criar_sprite_animado_array(frames_jogador_cima, 2, TEMPO_FRAME_MOVIMENTO);
        jogador.sprite_baixo = criar_sprite_animado_array(frames_jogador_baixo, 2, TEMPO_FRAME_MOVIMENTO);
        jogador.sprite_esquerda = criar_sprite_animado_array(frames_jogador_esquerda, 2, TEMPO_FRAME_MOVIMENTO);
        jogador.sprite_direita = criar_sprite_animado_array(frames_jogador_direita, 2, TEMPO_FRAME_MOVIMENTO);

        jogador.sprite_idle_baixo = criar_sprite_animado_array(arr_idle_baixo, 1, TEMPO_FRAME_IDLE);
        jogador.sprite_idle_direita = criar_sprite_animado_array(arr_idle_direita, 1, TEMPO_FRAME_IDLE);
        jogador.sprite_idle_esquerda = criar_sprite_animado_array(arr_idle_esquerda, 1, TEMPO_FRAME_IDLE);
        jogador.sprite_idle_cima = criar_sprite_animado_array(arr_idle_cima, 1, TEMPO_FRAME_IDLE);

        jogador.usar_sprite = sprite_animado_frames_valido(jogador.sprite_baixo) &&
            sprite_animado_frames_valido(jogador.sprite_cima);

        if (!jogador.usar_sprite) {
            printf("Aviso: Sprites do jogador nao carregaram corretamente. Usando circulo simples.\n");
        }

        // ========== INICIALIZAÇÃO DOS ANIMAIS - USANDO CONFIGURAÇÕES ==========
        Bot bots[TOTAL_ANIMAIS] = { 0 };  // ? ZERA memória para evitar lixo!

        iniciar_bot_com_sprite_e_fundo(&bots[0], POS_INICIAL_ONCA_X, POS_INICIAL_ONCA_Y,
            NOME_ANIMAL_ONCA, TIPO_CARNIVORO, CENARIO1,
            PATH_ANIMAL_ONCA, PATH_ESTRUTURA_SELVA);

        iniciar_bot_com_sprite_e_fundo(&bots[1], POS_INICIAL_JACARE_X, POS_INICIAL_JACARE_Y,
            NOME_ANIMAL_JACARE, TIPO_CARNIVORO, CENARIO2,
            PATH_ANIMAL_JACARE, PATH_ESTRUTURA_PANTANO);

        iniciar_bot_com_area_eliptica(&bots[2], POS_INICIAL_BOTO_X, POS_INICIAL_BOTO_Y,
            NOME_ANIMAL_BOTO, TIPO_CARNIVORO, CENARIO3,
            PATH_ANIMAL_BOTO, PATH_ESTRUTURA_LAGO,
            ELIPSE_BOTO_CENTRO_X, ELIPSE_BOTO_CENTRO_Y, ELIPSE_BOTO_RAIO_H, ELIPSE_BOTO_RAIO_V);

        iniciar_bot_com_sprite_e_fundo(&bots[3], POS_INICIAL_LOBO_X, POS_INICIAL_LOBO_Y,
            NOME_ANIMAL_LOBO, TIPO_ONIVORO, CENARIO4,
            PATH_ANIMAL_LOBO, PATH_ESTRUTURA_CERRADO);

        /* Novos Animais */
        /* SELVA - 2 novos */
        iniciar_bot_com_sprite_e_fundo(&bots[4], POS_INICIAL_HARPIA_X, POS_INICIAL_HARPIA_Y,
            NOME_ANIMAL_HARPIA, TIPO_CARNIVORO, CENARIO1,
            PATH_ANIMAL_HARPIA, PATH_ESTRUTURA_SELVA);

        iniciar_bot_com_sprite_e_fundo(&bots[5], POS_INICIAL_MICO_X, POS_INICIAL_MICO_Y,
            NOME_ANIMAL_MICO, TIPO_ONIVORO, CENARIO1,
            PATH_ANIMAL_MICO, PATH_ESTRUTURA_SELVA);

        /* PANTANO - 1 novo */
        iniciar_bot_com_sprite_e_fundo(&bots[6], POS_INICIAL_CAPIVARA_X, POS_INICIAL_CAPIVARA_Y,
            NOME_ANIMAL_CAPIVARA, TIPO_HERBIVORO, CENARIO2,
            PATH_ANIMAL_CAPIVARA, PATH_ESTRUTURA_PANTANO);

        /* LAGO - 1 novo */
        iniciar_bot_com_area_eliptica(&bots[7], POS_INICIAL_PIRARUCU_X, POS_INICIAL_PIRARUCU_Y,
            NOME_ANIMAL_PIRARUCU, TIPO_CARNIVORO, CENARIO3,
            PATH_ANIMAL_PIRARUCU, PATH_ESTRUTURA_LAGO,
            ELIPSE_BOTO_CENTRO_X, ELIPSE_BOTO_CENTRO_Y, ELIPSE_BOTO_RAIO_H, ELIPSE_BOTO_RAIO_V);

        /* CERRADO - 1 novo */
        iniciar_bot_com_sprite_e_fundo(&bots[8], POS_INICIAL_TAMANDUA_X, POS_INICIAL_TAMANDUA_Y,
            NOME_ANIMAL_TAMANDUA, TIPO_INSETIVORO, CENARIO4,
            PATH_ANIMAL_TAMANDUA, PATH_ESTRUTURA_CERRADO);

        // ========== INICIALIZAÇÃO DOS CAÇADORES - USANDO CONFIGURAÇÕES ==========
        Cacador cacadores[TOTAL_CACADORES];

        iniciar_cacador_com_alvo(&cacadores[0], POS_INICIAL_CACADOR1_X, POS_INICIAL_CACADOR1_Y,
            CENARIO1, &bots[0], NOME_CACADOR_SELVA, PATH_CACADOR_NORMAL);

        iniciar_cacador_com_alvo(&cacadores[1], POS_INICIAL_CACADOR2_X, POS_INICIAL_CACADOR2_Y,
            CENARIO2, &bots[1], NOME_CACADOR_PANTANO, PATH_CACADOR_NORMAL);

        iniciar_cacador_com_alvo(&cacadores[2], POS_INICIAL_CACADOR3_X, POS_INICIAL_CACADOR3_Y,
            CENARIO3, &bots[2], NOME_CACADOR_LAGO, PATH_CACADOR_NORMAL);

        iniciar_cacador_com_alvo(&cacadores[3], POS_INICIAL_CACADOR4_X, POS_INICIAL_CACADOR4_Y,
            CENARIO4, &bots[3], NOME_CACADOR_CERRADO, PATH_CACADOR_NORMAL);

        /* Novos Caçadores */
        iniciar_cacador_com_alvo(&cacadores[4], POS_INICIAL_CACADOR5_X, POS_INICIAL_CACADOR5_Y,
            CENARIO1, &bots[4], NOME_CACADOR_SELVA2, PATH_CACADOR_NORMAL);

        iniciar_cacador_com_alvo(&cacadores[5], POS_INICIAL_CACADOR6_X, POS_INICIAL_CACADOR6_Y,
            CENARIO2, &bots[6], NOME_CACADOR_PANTANO2, PATH_CACADOR_NORMAL);

        iniciar_cacador_com_alvo(&cacadores[6], POS_INICIAL_CACADOR7_X, POS_INICIAL_CACADOR7_Y,
            CENARIO3, &bots[7], NOME_CACADOR_LAGO2, PATH_CACADOR_NORMAL);

        iniciar_cacador_com_alvo(&cacadores[7], POS_INICIAL_CACADOR8_X, POS_INICIAL_CACADOR8_Y,
            CENARIO4, &bots[8], NOME_CACADOR_CERRADO2, PATH_CACADOR_NORMAL);

        for (int i = 0; i < TOTAL_CACADORES; i++) {
            cacadores[i].raio_deteccao = RAIO_DETECCAO_PADRAO;
            cacadores[i].raio_abandono = RAIO_ABANDONO_PADRAO;
            cacadores[i].velocidade = VELOCIDADE_PATRULHA;
            cacadores[i].velocidade_perseguicao = VELOCIDADE_PERSEGUICAO;
            cacadores[i].tempo_dano_interval = COOLDOWN_DANO_CACADOR;
            cacadores[i].ativo = false;
        }

        // ========== INICIALIZAÇÃO DO BOSS - USANDO CONFIGURAÇÕES ==========
        CacadorChefe boss;
        iniciar_cacador_chefe(&boss, POS_INICIAL_BOSS_X, POS_INICIAL_BOSS_Y, CENARIO1,
            PATH_CACADOR_BOSS, PATH_ESTRUTURA_SELVA);

        int cacadores_derrotados = 0;
        bool boss_foi_derrotado = false;

        // ========== BESTIÁRIO ==========
        Bestiario* bestiario = criar_bestiario();
        if (!bestiario) {
            fprintf(stderr, "ERRO CRITICO: Falha ao criar bestiario!\n");
        }
        else {
            for (int i = 0; i < TOTAL_ANIMAIS; i++) {
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
                limitar_jogador_com_progresso(&jogador, LARGURA_MAPA, ALTURA_MAPA, &jogador.progresso);

                // Atualizar animações do jogador
                if (jogador.usar_sprite) {
                    if (jogador.movendo) {
                        switch (jogador.Direcao_atual) {
                        case DIRECAO_CIMA:
                            if (sprite_animado_frames_valido(jogador.sprite_cima))
                                atualizar_sprite_animado_frames(jogador.sprite_cima, delta_time);
                            break;
                        case DIRECAO_BAIXO:
                            if (sprite_animado_frames_valido(jogador.sprite_baixo))
                                atualizar_sprite_animado_frames(jogador.sprite_baixo, delta_time);
                            break;
                        case DIRECAO_ESQ:
                            if (sprite_animado_frames_valido(jogador.sprite_esquerda))
                                atualizar_sprite_animado_frames(jogador.sprite_esquerda, delta_time);
                            break;
                        case DIRECAO_DIR:
                            if (sprite_animado_frames_valido(jogador.sprite_direita))
                                atualizar_sprite_animado_frames(jogador.sprite_direita, delta_time);
                            break;
                        }
                    }
                    else {
                        switch (jogador.Direcao_atual) {
                        case DIRECAO_CIMA:
                            if (sprite_animado_frames_valido(jogador.sprite_idle_cima))
                                atualizar_sprite_animado_frames(jogador.sprite_idle_cima, delta_time);
                            else if (sprite_animado_frames_valido(jogador.sprite_cima))
                                atualizar_sprite_animado_frames(jogador.sprite_cima, delta_time);
                            break;
                        case DIRECAO_BAIXO:
                            if (sprite_animado_frames_valido(jogador.sprite_idle_baixo))
                                atualizar_sprite_animado_frames(jogador.sprite_idle_baixo, delta_time);
                            else if (sprite_animado_frames_valido(jogador.sprite_baixo))
                                atualizar_sprite_animado_frames(jogador.sprite_baixo, delta_time);
                            break;
                        case DIRECAO_ESQ:
                            if (sprite_animado_frames_valido(jogador.sprite_idle_esquerda))
                                atualizar_sprite_animado_frames(jogador.sprite_idle_esquerda, delta_time);
                            else if (sprite_animado_frames_valido(jogador.sprite_esquerda))
                                atualizar_sprite_animado_frames(jogador.sprite_esquerda, delta_time);
                            break;
                        case DIRECAO_DIR:
                            if (sprite_animado_frames_valido(jogador.sprite_idle_direita))
                                atualizar_sprite_animado_frames(jogador.sprite_idle_direita, delta_time);
                            else if (sprite_animado_frames_valido(jogador.sprite_direita))
                                atualizar_sprite_animado_frames(jogador.sprite_direita, delta_time);
                            break;
                        }
                    }
                }

                // Determinar cenário lógico atual
                JogoCenas cena_logic = verificar_area_atual(&jogador);

                // ========== SPAWN DOS CAÇADORES POR CENÁRIO ==========
                for (int i = 0; i < TOTAL_CACADORES; i++) {
                    // Só ativa caçador no cenário atual (e não-derrotado)
                    cacadores[i].ativo = (cacadores[i].cenario == cena_logic) && !cacadores[i].derrotado;
                }

                // ========== ATUALIZAÇÃO DOS ANIMAIS (continuação do movimento mesmo se o caçador morrer) ==========
                for (int i = 0; i < TOTAL_ANIMAIS; i++) {
                    if (!bots[i].ativo || bots[i].cenario != cena_logic) continue;

                    // Verificar se ALGUM caçador ativo está perseguindo ESTE animal
                    bool sendo_perseguido = false;
                    Cacador* cacador_perseguidor = NULL;

                    for (int j = 0; j < TOTAL_CACADORES; j++) {
                        if (cacadores[j].ativo &&
                            !cacadores[j].derrotado &&
                            cacadores[j].cenario == cena_logic &&
                            cacadores[j].alvo == &bots[i]) {  // Caçador tem ESTE animal como alvo
                            sendo_perseguido = true;
                            cacador_perseguidor = &cacadores[j];
                            break;  // Encontrou um caçador perseguindo
                        }
                    }

                    if (sendo_perseguido && cacador_perseguidor) {
                        // Animal sabe que está sendo perseguido e foge
                        atualizar_bot_com_cacador(&bots[i], cacador_perseguidor, LARGURA_MAPA, ALTURA_MAPA, delta_time);
                    }
                    else {
                        // Animal patrulha normalmente (sem caçador ativo perseguindo)
                        atualizar_bot(&bots[i], LARGURA_MAPA, ALTURA_MAPA, delta_time);
                    }
                }

                // ========== ATUALIZAÇÃO DOS CAÇADORES ==========
                for (int i = 0; i < TOTAL_CACADORES; i++) {
                    if (cacadores[i].ativo && !cacadores[i].derrotado &&
                        cacadores[i].alvo &&
                        cacadores[i].alvo->ativo &&
                        cacadores[i].alvo->cenario == cena_logic &&
                        cacadores[i].cenario == cena_logic) {

                        atualizar_cacador(&cacadores[i],
                            cacadores[i].alvo->x,
                            cacadores[i].alvo->y,
                            cacadores[i].alvo->ativo,
                            delta_time,
                            LARGURA_MAPA, ALTURA_MAPA);

                        aplicar_dano_cacador_em_animal(&cacadores[i], cacadores[i].alvo, delta_time);
                    }
                }

                // ========== ATUALIZAÇÃO DO BOSS ==========
                if (boss.liberado && !boss.derrotado) {
                    atualizar_cacador_chefe(&boss, &jogador, LARGURA_MAPA, ALTURA_MAPA, delta_time);
                }

                // Mensagens cooldown decaimento
                if (msg_cooldown_animal > 0.0f) msg_cooldown_animal -= delta_time;
                if (msg_cooldown_cacador > 0.0f) msg_cooldown_cacador -= delta_time;

                // ========== COLISÃO JOGADOR x ANIMAIS ==========
                for (int i = 0; i < TOTAL_ANIMAIS; i++) {
                    if (bots[i].ativo && bots[i].cenario == cena_logic &&
                        verificar_colisao_bot(&jogador, &bots[i])) {

                        if (bots[i].cooldown_colisao > 0.0f) {
                            msg_cooldown_animal = TEMPO_EXIBICAO_MENSAGEM;
                            continue;
                        }

                        printf("Colisao com %s!\n", bots[i].animal_data.nome);

                        // NOVA LÓGICA: Verificar se TODOS os caçadores do cenário foram derrotados
                        if (!todos_cacadores_cenario_derrotados(cacadores, bots[i].cenario)) {
                            printf("[BLOQUEADO] Derrote TODOS os cacadores do %s primeiro!\n",
                                bots[i].cenario == CENARIO1 ? "Cenario 1 (Selva)" :
                                bots[i].cenario == CENARIO2 ? "Cenario 2 (Pantano)" :
                                bots[i].cenario == CENARIO3 ? "Cenario 3 (Lago)" : "Cenario 4 (Cerrado)");

                            int derrotados = contar_cacadores_derrotados_cenario(cacadores, bots[i].cenario);
                            int total_cenario = 0;
                            for (int j = 0; j < TOTAL_CACADORES; j++) {
                                if (cacadores[j].cenario == bots[i].cenario) total_cenario++;
                            }
                            printf("Cacadores derrotados neste cenario: %d/%d\n", derrotados, total_cenario);
                            continue;
                        }

                        iniciar_batalha_com_bestiario(fonte, &bots[i].animal_data, ctx.event_queue, ctx.display, bestiario);

                        tempo_anterior = al_get_time();
                        bots[i].cooldown_colisao = COOLDOWN_COLISAO_ANIMAL;

                        if (bots[i].animal_data.estudado) {
                            bots[i].ativo = false;
                            printf("%s foi totalmente estudado e nao aparecera mais!\n", bots[i].animal_data.nome);

                            // Atualizar progresso de cenários
                            atualizar_progresso_cenarios(&jogador, bots);

                            // Verifica condição de liberação do boss: TODOS animais estudados E TODOS caçadores derrotados
                            if (!boss.liberado && cacadores_derrotados >= TOTAL_CACADORES) {
                                if (todos_animais_estudados(bots)) {
                                    // Marca liberação do boss
                                    boss.liberado = true;
                                    boss.derrotado = false;

                                    // Ativa a entidade base do boss para colisões/desenho
                                    boss.base.ativo = true;
                                    boss.base.derrotado = false;

                                    // Posiciona o boss próximo ao jogador (evita spawn em local inacessível)
                                    boss.base.x = jogador.x + 200.0f;
                                    if (boss.base.x > LARGURA_MAPA - 50.0f) boss.base.x = jogador.x - 200.0f;
                                    boss.base.y = jogador.y;

                                    // REMOVE limitação por cenário: amplia os raios para que o boss possa perseguir/andar por todo o mapa
                                    boss.base.cenario = CENARIO1;
                                    boss.base.raio_deteccao = fmaxf(LARGURA_MAPA, ALTURA_MAPA) * 2.0f;
                                    boss.base.raio_abandono = boss.base.raio_deteccao * 1.5f;
                                    boss.base.velocidade = VELOCIDADE_PERSEGUICAO;

                                    printf("\n============================================\n");
                                    printf("   ATENCAO! O CACADOR CHEFE APARECEU!\n");
                                    printf("   Todos os animais estudados!\n");
                                    printf("   Todos os cacadores derrotados!\n");
                                    printf("   Procure por ele no mapa!\n");
                                    printf("============================================\n");
                                }
                                else {
                                    printf("[INFO] Todos os caçadores derrotados, mas ainda faltam animais estudados para liberar o boss.\n");
                                }
                            }
                        }
                        else if (bots[i].animal_data.domado) {
                            printf("%s esta domado e permanece no mapa para estudo!\n", bots[i].animal_data.nome);
                        }
                    }
                }

                // ========== COLISÃO JOGADOR x CAÇADORES ==========
                for (int i = 0; i < TOTAL_CACADORES; i++) {
                    if (cacadores[i].ativo && !cacadores[i].derrotado &&
                        cacadores[i].cenario == cena_logic &&
                        verificar_colisao_cacador_jogador(&jogador, &cacadores[i])) {

                        if (cacadores[i].cooldown_batalha > 0.0f) {
                            msg_cooldown_cacador = TEMPO_EXIBICAO_MENSAGEM;
                            continue;
                        }

                        printf("Colisao com %s!\n", cacadores[i].nome);

                        iniciar_batalha_cacador_visual(fonte, &cacadores[i], ctx.event_queue, ctx.display, &jogador);

                        tempo_anterior = al_get_time();

                        if (cacadores[i].derrotado) {
                            cacadores_derrotados++;
                            printf("[PROGRESSO] Cacadores derrotados: %d/%d\n",
                                cacadores_derrotados, TOTAL_CACADORES);

                            // Verifica condição de liberação do boss: TODOS caçadores derrotados E TODOS animais estudados
                            if (!boss.liberado && cacadores_derrotados >= TOTAL_CACADORES) {
                                if (todos_animais_estudados(bots)) {
                                    // Marca liberação do boss
                                    boss.liberado = true;
                                    boss.derrotado = false;

                                    // Ativa a entidade base do boss para colisões/desenho
                                    boss.base.ativo = true;
                                    boss.base.derrotado = false;

                                    // Posiciona o boss próximo ao jogador (evita spawn em local inacessível)
                                    boss.base.x = jogador.x + 200.0f;
                                    if (boss.base.x > LARGURA_MAPA - 50.0f) boss.base.x = jogador.x - 200.0f;
                                    boss.base.y = jogador.y;

                                    // REMOVE limitação por cenário: amplia os raios para que o boss possa perseguir/andar por todo o mapa
                                    boss.base.cenario = CENARIO1;
                                    boss.base.raio_deteccao = fmaxf(LARGURA_MAPA, ALTURA_MAPA) * 2.0f;
                                    boss.base.raio_abandono = boss.base.raio_deteccao * 1.5f;
                                    boss.base.velocidade = VELOCIDADE_PERSEGUICAO;

                                    printf("\n============================================\n");
                                    printf("   ATENCAO! O CACADOR CHEFE APARECEU!\n");
                                    printf("   Todos os cacadores derrotados!\n");
                                    printf("   Todos os animais estudados!\n");
                                    printf("   Procure por ele no mapa!\n");
                                    printf("============================================\n");
                                }
                                else {
                                    printf("[INFO] Todos os cacadores derrotados, mas ainda faltam animais estudados para liberar o boss.\n");
                                }
                            }
                        }

                        jogador.y += EMPURRAO_JOGADOR_CACADOR;
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

                        // Desbloqueia humano no bestiário
                        if (bestiario) {
                            desbloquear_humano_bestiario(bestiario);
                        }

                        printf("\n============================================\n");
                        printf("   PARABENS! VOCE VENCEU O JOGO!\n");
                        printf("   Todos os animais estao protegidos!\n");
                        printf("   Humano adicionado ao bestiario!\n");
                        printf("============================================\n");
                    }

                    jogador.y += EMPURRAO_JOGADOR_BOSS;
                }

                // ========== REINÍCIO FORÇADO SE ALGUM ANIMAL FOI CAPTURADO ==========
                if (forcar_reinicio) {
                    mostrar_tela_derrota(fonte, ctx.display, ctx.event_queue);
                    printf("\n[AVISO] Reiniciando jogo apos derrota...\n");
                    for (int i = 0; i < TOTAL_ANIMAIS; i++) destruir_bot(&bots[i]);
                    for (int i = 0; i < TOTAL_CACADORES; i++) destruir_cacador(&cacadores[i]);
                    destruir_cacador(&boss.base);
                    inicializar_jogo(&ctx, &jogador, bots, cacadores, &boss, &bestiario,
                        &cacadores_derrotados, &boss_foi_derrotado, fonte);
                    tempo_anterior = al_get_time();
                    continue;
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

            // ========== RENDERIZAÇÃO - USANDO CONFIGURAÇÕES ==========
            if (redesenhar && al_is_event_queue_empty(ctx.event_queue)) {
                redesenhar = false;

                cena_atual = cenarios(cena_atual, &ctx, &jogador);

                // Desenhar animais
                for (int i = 0; i < TOTAL_ANIMAIS; i++) {
                    if (bots[i].ativo && bots[i].cenario == cena_atual) {
                        desenhar_bot(&bots[i], al_map_rgb(COR_ANIMAL_DESENHO_R, COR_ANIMAL_DESENHO_G, COR_ANIMAL_DESENHO_B), camera_x, camera_y);
                    }
                }

                // Desenhar caçadores
                for (int i = 0; i < TOTAL_CACADORES; i++) {
                    if (cacadores[i].ativo && !cacadores[i].derrotado && cacadores[i].cenario == cena_atual) {
                        desenhar_cacador(&cacadores[i], al_map_rgb(COR_CACADOR_DESENHO_R, COR_CACADOR_DESENHO_G, COR_CACADOR_DESENHO_B), camera_x, camera_y);
                    }
                }

                // Desenhar boss (AGORA: aparece no mapa independentemente do cenário)
                if (boss.liberado && !boss.derrotado) {
                    desenhar_cacador(&boss.base, al_map_rgb(COR_BOSS_DESENHO_R, COR_BOSS_DESENHO_G, COR_BOSS_DESENHO_B), camera_x, camera_y);

                    float boss_x_tela = (boss.base.x - camera_x) * ZOOM_FACTOR;
                    float boss_y_tela = (boss.base.y - camera_y) * ZOOM_FACTOR;
                    al_draw_text(fonte, al_map_rgb(COR_TEXTO_MAGENTA_R, COR_TEXTO_MAGENTA_G, COR_TEXTO_MAGENTA_B),
                        boss_x_tela, boss_y_tela - OFFSET_LABEL_BOSS_Y * ZOOM_FACTOR,  // ao invés de 30.0f
                        ALLEGRO_ALIGN_CENTRE, "BOSS");
                }

                desenhar_jogador(&jogador, ctx.CoresFundo[4]);

                // ========== MENSAGENS NA TELA - USANDO CONFIGURAÇÕES ==========
                if (fonte) {
                    // Mensagens de área bloqueada (progressão sequencial)
                    if (!jogador.progresso.cenario1_completo &&
                        (jogador.y > LIMITE_VERTICAL_MAPA - DISTANCIA_VERIFICACAO_BLOQUEIO ||
                            jogador.x > LIMITE_HORIZONTAL_MAPA - DISTANCIA_VERIFICACAO_BLOQUEIO)) {
                        al_draw_text(fonte, al_map_rgb(COR_TEXTO_AMARELO_R, COR_TEXTO_AMARELO_G, COR_TEXTO_AMARELO_B),
                            POS_TEXTO_CENTRO_X, POS_TEXTO_TOPO_Y, ALLEGRO_ALIGN_CENTRE, MSG_AREA_BLOQUEADA_1_NOVA);
                    }
                    else if (jogador.progresso.cenario1_completo && !jogador.progresso.cenario2_completo &&
                        jogador.x > LIMITE_HORIZONTAL_MAPA - 20.0f) {
                        al_draw_text(fonte, al_map_rgb(COR_TEXTO_AMARELO_R, COR_TEXTO_AMARELO_G, COR_TEXTO_AMARELO_B),
                            POS_TEXTO_CENTRO_X, POS_TEXTO_TOPO_Y, ALLEGRO_ALIGN_CENTRE, MSG_AREA_BLOQUEADA_2_NOVA);
                    }
                    else if (jogador.progresso.cenario2_completo && !jogador.progresso.cenario3_completo &&
                        jogador.x > LIMITE_HORIZONTAL_MAPA - 20.0f && jogador.y > LIMITE_VERTICAL_MAPA - 20.0f) {
                        al_draw_text(fonte, al_map_rgb(COR_TEXTO_AMARELO_R, COR_TEXTO_AMARELO_G, COR_TEXTO_AMARELO_B),
                            POS_TEXTO_CENTRO_X, POS_TEXTO_TOPO_Y, ALLEGRO_ALIGN_CENTRE, MSG_AREA_BLOQUEADA_3_NOVA);
                    }

                    // Mensagem de cooldown de animal
                    if (msg_cooldown_animal > 0.0f) {
                        al_draw_text(fonte, al_map_rgb(COR_TEXTO_LARANJA_R, COR_TEXTO_LARANJA_G, COR_TEXTO_LARANJA_B),
                            POS_TEXTO_CENTRO_X, POS_TEXTO_MSG_ANIMAL_Y, ALLEGRO_ALIGN_CENTRE, MSG_ANIMAL_DESCANSO);
                    }

                    // Mensagem de cooldown de caçador
                    if (msg_cooldown_cacador > 0.0f) {
                        al_draw_text(fonte, al_map_rgb(COR_TEXTO_ROSA_R, COR_TEXTO_ROSA_G, COR_TEXTO_ROSA_B),
                            POS_TEXTO_CENTRO_X, POS_TEXTO_MSG_CACADOR_Y, ALLEGRO_ALIGN_CENTRE, MSG_CACADOR_RECUPERACAO);
                    }

                    // Contador de caçadores derrotados
                    char texto_cacadores[64];
                    snprintf(texto_cacadores, sizeof(texto_cacadores), FMT_CACADORES_DERROTADOS, cacadores_derrotados);
                    al_draw_text(fonte, al_map_rgb(COR_TEXTO_OURO_R, COR_TEXTO_OURO_G, COR_TEXTO_OURO_B),
                        POS_TEXTO_CONTADOR_X, POS_TEXTO_CONTADOR_Y, ALLEGRO_ALIGN_LEFT, texto_cacadores);

                    // NOVA: Mostrar progresso de animais estudados
                    int animais_estudados = 0;
                    for (int i = 0; i < TOTAL_ANIMAIS; i++) {
                        if (bots[i].animal_data.estudado) animais_estudados++;
                    }
                    char texto_animais[64];
                    snprintf(texto_animais, sizeof(texto_animais), "Animais Estudados: %d/%d", animais_estudados, TOTAL_ANIMAIS);
                    al_draw_text(fonte, al_map_rgb(COR_TEXTO_VERDE_CLARO_R, COR_TEXTO_VERDE_CLARO_G, COR_TEXTO_VERDE_CLARO_B),
                        POS_TEXTO_CONTADOR_X, POS_TEXTO_CONTADOR_Y + 20, ALLEGRO_ALIGN_LEFT, texto_animais);

                    // NOVA: Mostrar requisitos para o boss se ainda não foi liberado
                    if (!boss.liberado && (cacadores_derrotados >= TOTAL_CACADORES || animais_estudados >= TOTAL_ANIMAIS - 2)) {
                        al_draw_text(fonte, al_map_rgb(COR_TEXTO_MAGENTA_R, COR_TEXTO_MAGENTA_G, COR_TEXTO_MAGENTA_B),
                            POS_TEXTO_CENTRO_X, 50, ALLEGRO_ALIGN_CENTRE, MSG_BOSS_REQUISITOS);
                    }

                    // Mensagem de vitória
                    if (boss_foi_derrotado) {
                        al_draw_text(fonte, al_map_rgb(COR_TEXTO_VERDE_R, COR_TEXTO_VERDE_G, COR_TEXTO_VERDE_B),
                            POS_TEXTO_CENTRO_X, POS_TEXTO_VITORIA_Y, ALLEGRO_ALIGN_CENTRE, MSG_JOGO_COMPLETO);
                    }

                    // Mensagem de bloqueio de estudo
                    for (int i = 0; i < TOTAL_ANIMAIS; i++) {
                        if (bots[i].ativo && bots[i].cenario == cena_atual) {
                            float dx = jogador.x - bots[i].x;
                            float dy = jogador.y - bots[i].y;
                            float dist = sqrtf(dx * dx + dy * dy);

                            if (dist < DISTANCIA_DETECCAO_MENSAGEM) {
                                // Verificar se todos os caçadores do cenário foram derrotados
                                if (!todos_cacadores_cenario_derrotados(cacadores, bots[i].cenario)) {
                                    int derrotados = contar_cacadores_derrotados_cenario(cacadores, bots[i].cenario);
                                    int total_cenario = 0;
                                    for (int j = 0; j < TOTAL_CACADORES; j++) {
                                        if (cacadores[j].cenario == bots[i].cenario) total_cenario++;
                                    }

                                    char mensagem_bloqueio[128];
                                    snprintf(mensagem_bloqueio, sizeof(mensagem_bloqueio),
                                        "Derrote todos os cacadores! (%d/%d)", derrotados, total_cenario);

                                    al_draw_text(fonte, al_map_rgb(COR_TEXTO_VERMELHO_R, COR_TEXTO_VERMELHO_G, COR_TEXTO_VERMELHO_B),
                                        POS_TEXTO_CENTRO_X, POS_TEXTO_BLOQUEIO_Y,
                                        ALLEGRO_ALIGN_CENTRE, mensagem_bloqueio);
                                }
                            }
                        }
                    }
                }

                al_flip_display();
            }
        }

        // ========== LIMPEZA DE RECURSOS ==========
        destruir_entidade(&jogador);

        for (int i = 0; i < TOTAL_ANIMAIS; i++) {
            destruir_bot(&bots[i]);
        }

        for (int i = 0; i < TOTAL_CACADORES; i++) {
            destruir_cacador(&cacadores[i]);
        }

        destruir_cacador(&boss.base);

        al_destroy_font(fonte);
        destruir_bestiario(bestiario);
        destruir_allegro(&ctx);

        return 0;
    }
}