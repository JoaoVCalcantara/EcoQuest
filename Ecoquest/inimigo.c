#define _CRT_SECURE_NO_WARNINGS

#include "inimigo.h"
#include "cenario.h"
#include "sprites.h"
#include "entidades.h"
#include "cacador.h"
#include "config_jogo.h"      // NOVO: Configurações do jogo
#include "config_cacador.h"   // NOVO: Configurações do caçador
#include <allegro5/allegro_primitives.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Parada ocasional: parâmetros ajustáveis aqui */
static const int    PARAR_CHANCE_PORCENT = 40;          // % de chance ao escolher nova direção de ficar parado
static const float PARAR_DURACAO_MIN = 1.0f;       // segundos mínimo parado
static const float PARAR_DURACAO_VAR = 2.0f;       // variação adicionada (0..PARAR_DURACAO_VAR)

/* Limiar para considerar "parado" (em pixels por frame equivalente) */
static const float STOPPED_THRESHOLD = 1.0f;

/* ========== FUNÇÕES AUXILIARES ========== */

static bool posicao_valida(const Bot* bot, float x, float y) {
    if (bot->area_restrita.tipo == AREA_NENHUMA) return true;

    if (bot->area_restrita.tipo == AREA_CIRCULAR) {
        float dx = x - bot->area_restrita.centro_x;
        float dy = y - bot->area_restrita.centro_y;
        float distancia = sqrtf(dx * dx + dy * dy);
        return (distancia + bot->raio <= bot->area_restrita.raio_area);
    }

    if (bot->area_restrita.tipo == AREA_ELIPTICA) {
        float dx = x - bot->area_restrita.centro_elipse_x;
        float dy = y - bot->area_restrita.centro_elipse_y;
        float a = bot->area_restrita.raio_horizontal;
        float b = bot->area_restrita.raio_vertical;

        /* Guarda contra divisão por zero/valores inválidos */
        if (a <= EPSILON_ELIPSE || b <= EPSILON_ELIPSE) return true;

        float termo_x = (dx * dx) / (a * a);
        float termo_y = (dy * dy) / (b * b);
        float lim = ELIPSE_MARGEM_VALIDACAO * ELIPSE_MARGEM_VALIDACAO;
        return (termo_x + termo_y) <= lim;
    }
    return true;
}

static void corrigir_posicao_area_restrita(Bot* bot) {
    if (bot->area_restrita.tipo == AREA_ELIPTICA) {
        float dx = bot->x - bot->area_restrita.centro_elipse_x;
        float dy = bot->y - bot->area_restrita.centro_elipse_y;
        float a = bot->area_restrita.raio_horizontal;
        float b = bot->area_restrita.raio_vertical;

        if (a <= EPSILON_ELIPSE || b <= EPSILON_ELIPSE) return;

        float termo_x = (dx * dx) / (a * a);
        float termo_y = (dy * dy) / (b * b);

        if (termo_x + termo_y > ELIPSE_LIMITE_CORRECAO) {
            float dist_centro = sqrtf(dx * dx + dy * dy);
            if (dist_centro > 0.1f) {
                float dir_x = -dx / dist_centro;
                float dir_y = -dy / dist_centro;

                bot->x += dir_x * ELIPSE_FORCA_CORRECAO;
                bot->y += dir_y * ELIPSE_FORCA_CORRECAO;

                bot->direcao_x = dir_x;
                bot->direcao_y = dir_y;
                bot->tempo_mudanca = 1.0f;
            }
        }
    } else if (bot->area_restrita.tipo == AREA_CIRCULAR) {
        float dx = bot->x - bot->area_restrita.centro_x;
        float dy = bot->y - bot->area_restrita.centro_y;
        float distancia = sqrtf(dx * dx + dy * dy);

        if (distancia > bot->area_restrita.raio_area - bot->raio - 5.0f) {
            float ang = atan2f(dy, dx);
            bot->x = bot->area_restrita.centro_x + cosf(ang) * (bot->area_restrita.raio_area - bot->raio - 10.0f);
            bot->y = bot->area_restrita.centro_y + sinf(ang) * (bot->area_restrita.raio_area - bot->raio - 10.0f);
            bot->direcao_x = -cosf(ang);
            bot->direcao_y = -sinf(ang);
            bot->tempo_mudanca = 1.0f;
        }
    }
}

static void corrigir_posicao_borda(Bot* bot, float largura_mapa, float altura_mapa) {
    float min_x = 0.0f, max_x = largura_mapa;
    float min_y = 0.0f, max_y = altura_mapa;

    switch (bot->cenario) {
    case CENARIO1:
        max_x = LIMITE_HORIZONTAL_MAPA;
        max_y = LIMITE_VERTICAL_MAPA;
        break;
    case CENARIO2:
        min_y = LIMITE_VERTICAL_MAPA;
        max_x = LIMITE_HORIZONTAL_MAPA;
        max_y = altura_mapa;
        break;
    case CENARIO3:
        min_x = LIMITE_HORIZONTAL_MAPA;
        max_x = largura_mapa;
        max_y = LIMITE_VERTICAL_MAPA;
        break;
    case CENARIO4:
        min_x = LIMITE_HORIZONTAL_MAPA;
        max_x = largura_mapa;
        min_y = LIMITE_VERTICAL_MAPA;
        max_y = altura_mapa;
        break;
    }

    float margem = bot->raio * 2.0f;

    if (bot->x < min_x + margem) bot->x = min_x + margem;
    if (bot->x > max_x - margem) bot->x = max_x - margem;
    if (bot->y < min_y + margem) bot->y = min_y + margem;
    if (bot->y > max_y - margem) bot->y = max_y - margem;
}

// Função para calcular direção de fuga evitando bordas
static void calcular_direcao_fuga_segura(Bot* bot, float cacador_x, float cacador_y,
    float* fuga_x, float* fuga_y, float largura_mapa, float altura_mapa) {

    (void)altura_mapa;

    float min_x = 0.0f, max_x = largura_mapa;
    float min_y = 0.0f, max_y = altura_mapa;

    switch (bot->cenario) {
    case CENARIO1:
        max_x = LIMITE_HORIZONTAL_MAPA;
        max_y = LIMITE_VERTICAL_MAPA;
        break;
    case CENARIO2:
        min_y = LIMITE_VERTICAL_MAPA;
        max_x = LIMITE_HORIZONTAL_MAPA;
        break;
    case CENARIO3:
        min_x = LIMITE_HORIZONTAL_MAPA;
        max_y = LIMITE_VERTICAL_MAPA;
        break;
    case CENARIO4:
        min_x = LIMITE_HORIZONTAL_MAPA;
        min_y = LIMITE_VERTICAL_MAPA;
        break;
    }

    /* direção básica: fugir do caçador */
    float dx = bot->x - cacador_x;
    float dy = bot->y - cacador_y;
    float distancia = sqrtf(dx * dx + dy * dy);
    if (distancia > 0.1f) {
        *fuga_x = dx / distancia;
        *fuga_y = dy / distancia;
    } else {
        /* fallback aleatório se sobrepostos */
        float ang = ((float)rand() / RAND_MAX) * 6.283185f;
        *fuga_x = cosf(ang);
        *fuga_y = sinf(ang);
    }

    /* centro alvo (cenário ou área restrita) */
    float centro_x, centro_y;
    if (bot->area_restrita.tipo == AREA_ELIPTICA) {
        centro_x = bot->area_restrita.centro_elipse_x;
        centro_y = bot->area_restrita.centro_elipse_y;
    } else if (bot->area_restrita.tipo == AREA_CIRCULAR) {
        centro_x = bot->area_restrita.centro_x;
        centro_y = bot->area_restrita.centro_y;
    } else {
        centro_x = (min_x + max_x) / 2.0f;
        centro_y = (min_y + max_y) / 2.0f;
    }

    /* vetor do boto para o centro (direção interna) */
    float dx_centro = centro_x - bot->x;
    float dy_centro = centro_y - bot->y;
    float dist_centro = sqrtf(dx_centro * dx_centro + dy_centro * dy_centro);
    float centro_dir_x = 0.0f, centro_dir_y = 0.0f;
    if (dist_centro > 0.001f) {
        centro_dir_x = dx_centro / dist_centro;
        centro_dir_y = dy_centro / dist_centro;
    }

    /* (restante inalterado) */
    if (bot->area_restrita.tipo == AREA_ELIPTICA) {
        float a = bot->area_restrita.raio_horizontal;
        float b = bot->area_restrita.raio_vertical;
        if (a > EPSILON_ELIPSE && b > EPSILON_ELIPSE) {
            float dx_e = bot->x - bot->area_restrita.centro_elipse_x;
            float dy_e = bot->y - bot->area_restrita.centro_elipse_y;
            float termo = (dx_e * dx_e) / (a * a) + (dy_e * dy_e) / (b * b);

            float peso_centro = PESO_FUGA_CENTRO;
            if (termo > ELIPSE_PERTO_BORDA) peso_centro = fminf(0.95f, PESO_FUGA_CENTRO + 0.20f);

            if (dist_centro < 1.2f) {
                float tx = -centro_dir_y;
                float ty =  centro_dir_x;
                float s = 0.4f + ((float)rand() / RAND_MAX) * 0.4f;
                *fuga_x = centro_dir_x * (1.0f - s) + tx * s;
                *fuga_y = centro_dir_y * (1.0f - s) + ty * s;
            }
            else {
                *fuga_x = (*fuga_x * (1.0f - peso_centro)) + (centro_dir_x * peso_centro);
                *fuga_y = (*fuga_y * (1.0f - peso_centro)) + (centro_dir_y * peso_centro);
            }

            float mag = sqrtf((*fuga_x) * (*fuga_x) + (*fuga_y) * (*fuga_y));
            if (mag > 0.001f) {
                *fuga_x /= mag;
                *fuga_y /= mag;
            } else {
                if (dist_centro > 0.001f) {
                    *fuga_x = centro_dir_x;
                    *fuga_y = centro_dir_y;
                } else {
                    float ang = ((float)rand() / RAND_MAX) * 6.283185f;
                    *fuga_x = cosf(ang);
                    *fuga_y = sinf(ang);
                }
            }

            float passo_teste = bot->velocidade * bot->fuga_velocidade_multiplicador * 0.6f;
            float teste_x = bot->x + (*fuga_x) * passo_teste;
            float teste_y = bot->y + (*fuga_y) * passo_teste;
            if (!posicao_valida(bot, teste_x, teste_y)) {
                float tang_x = -(*fuga_y);
                float tang_y =  (*fuga_x);
                float try1_x = bot->x + tang_x * passo_teste * 0.6f;
                float try1_y = bot->y + tang_y * passo_teste * 0.6f;
                float try2_x = bot->x - tang_x * passo_teste * 0.6f;
                float try2_y = bot->y - tang_y * passo_teste * 0.6f;
                if (posicao_valida(bot, try1_x, try1_y)) {
                    *fuga_x = tang_x; *fuga_y = tang_y;
                } else if (posicao_valida(bot, try2_x, try2_y)) {
                    *fuga_x = -tang_x; *fuga_y = -tang_y;
                } else if (dist_centro > 0.001f) {
                    *fuga_x = centro_dir_x + (((float)rand() / RAND_MAX) - 0.5f) * 0.2f;
                    *fuga_y = centro_dir_y + (((float)rand() / RAND_MAX) - 0.5f) * 0.2f;
                    float mm = sqrtf((*fuga_x)*(*fuga_x) + (*fuga_y)*(*fuga_y));
                    if (mm > 0.001f) { *fuga_x /= mm; *fuga_y /= mm; }
                }
            }

            return;
        }
    }

    bool perto_borda_esquerda = bot->x < (min_x + MARGEM_SEGURANCA_BORDA);
    bool perto_borda_direita = bot->x > (max_x - MARGEM_SEGURANCA_BORDA);
    bool perto_borda_cima = bot->y < (min_y + MARGEM_SEGURANCA_BORDA);
    bool perto_borda_baixo = bot->y > (max_y - MARGEM_SEGURANCA_BORDA);

    if (perto_borda_esquerda || perto_borda_direita || perto_borda_cima || perto_borda_baixo) {
        if (dist_centro > 0.1f) {
            *fuga_x = (*fuga_x * (1.0f - PESO_FUGA_CENTRO)) + ((dx_centro / dist_centro) * PESO_FUGA_CENTRO);
            *fuga_y = (*fuga_y * (1.0f - PESO_FUGA_CENTRO)) + ((dy_centro / dist_centro) * PESO_FUGA_CENTRO);

            float mag = sqrtf((*fuga_x) * (*fuga_x) + (*fuga_y) * (*fuga_y));
            if (mag > 0.1f) {
                *fuga_x /= mag;
                *fuga_y /= mag;
            }
        }
    }
}

// ========== FUNÇÕES BOT ==========

void iniciar_bot(Bot* bot, float x, float y, const char* nome, const char* tipo, int cenario) {
    bot->x = x;
    bot->y = y;
    bot->raio = RAIO_ANIMAL;
    bot->velocidade = VELOCIDADE_ANIMAL;
    bot->velocidade_base = VELOCIDADE_ANIMAL;  // Salva velocidade original
    
    // Direção inicial aleatória
    float angulo_inicial = ((float)rand() / RAND_MAX) * 6.283185f;
    bot->direcao_x = cosf(angulo_inicial);
    bot->direcao_y = sinf(angulo_inicial);
    
    // Tempo de mudança inicial aleatória
    bot->tempo_mudanca = TEMPO_MUDANCA_MIN + ((float)rand() / RAND_MAX) * TEMPO_MUDANCA_VARIACAO;
    
    bot->ativo = true;
    bot->cenario = cenario;
    bot->cooldown_colisao = 0.0f;

    // Sistema de estado
    bot->estado_atual = ESTADO_PATRULHA;
    bot->tempo_no_estado = 0.0f;

    bot->fugindo = false;
    bot->fuga_timer = 0.0f;
    bot->fuga_velocidade_multiplicador = VELOCIDADE_FUGA_MULTIPLICADOR;

    bot->animal_data.experiencia = 0;
    bot->animal_data.nivel = 1;
    bot->animal_data.nivel_alimentacao = 0;
    bot->animal_data.domado = false;
    bot->animal_data.estudado = false;
    bot->animal_data.alimentado = false;
    bot->animal_data.nome = nome;
    bot->animal_data.tipo = tipo;
    bot->animal_data.caminho_fundo_batalha = NULL;

    bot->sprite_animal = NULL;
    bot->usar_sprite = false;
    bot->escala_sprite = ESCALA_ANIMAL;
    bot->caminho_sprite_batalha = NULL;

    bot->area_restrita.tipo = AREA_NENHUMA;

    bot->animal_data.vida_maxima = VIDA_MAXIMA_ANIMAL;
    bot->animal_data.vida_atual = VIDA_MAXIMA_ANIMAL;

    bot->vida_visual_maxima = VIDA_VISUAL_MAXIMA_ANIMAL;
    bot->vida_visual_atual = VIDA_MAXIMA_ANIMAL;

    // Inicializa estado de parada ocasional
    bot->parado = false;
    bot->parado_timer = 0.0f;
    
    // Inicializa sistema de visão
    inicializar_sistema_visao(&bot->visao, RAIO_VISAO_PADRAO, ANGULO_VISAO_PADRAO);
    
    // Salva posição inicial como segura
    bot->ultima_pos_segura_x = x;
    bot->ultima_pos_segura_y = y;
    bot->tentativas_fuga = 0;
}

void iniciar_bot_com_sprite(Bot* bot, float x, float y, const char* nome, const char* tipo, int cenario, const char* caminho_sprite) {
    iniciar_bot(bot, x, y, nome, tipo, cenario);
    bot->caminho_sprite_batalha = caminho_sprite;
    if (caminho_sprite) {
        bot->sprite_animal = criar_sprite(caminho_sprite);
        bot->usar_sprite = (bot->sprite_animal && bot->sprite_animal->carregado);
        if (!bot->usar_sprite) {
            printf("Aviso: Nao foi possivel carregar sprite do bot %s\n", nome);
        }
    }
}

void iniciar_bot_com_area_circular(Bot* bot, float x, float y, const char* nome, const char* tipo, int cenario, const char* caminho_sprite, float centro_x, float centro_y, float raio_area) {
    iniciar_bot_com_sprite(bot, x, y, nome, tipo, cenario, caminho_sprite);
    bot->area_restrita.tipo = AREA_CIRCULAR;
    bot->area_restrita.centro_x = centro_x;
    bot->area_restrita.centro_y = centro_y;
    bot->area_restrita.raio_area = raio_area;
}

void iniciar_bot_com_area_eliptica(Bot* bot, float x, float y, const char* nome, const char* tipo,
    JogoCenas cenario, const char* caminho_sprite,
    const char* fundo_batalha_path,
    float centro_x, float centro_y,
    float raio_h, float raio_v) {
    iniciar_bot_com_sprite_e_fundo(bot, x, y, nome, tipo, cenario, caminho_sprite, fundo_batalha_path);
    bot->area_restrita.tipo = AREA_ELIPTICA;
    bot->area_restrita.centro_elipse_x = centro_x;
    bot->area_restrita.centro_elipse_y = centro_y;
    bot->area_restrita.raio_horizontal = raio_h;
    bot->area_restrita.raio_vertical = raio_v;
}

void iniciar_bot_com_sprite_e_fundo(Bot* bot, float x, float y, const char* nome,
                                     const char* tipo, int cenario,
                                     const char* caminho_sprite, 
                                     const char* caminho_fundo_batalha) {
    iniciar_bot_com_sprite(bot, x, y, nome, tipo, cenario, caminho_sprite);
    bot->animal_data.caminho_fundo_batalha = caminho_fundo_batalha;
}

static void garantir_direcao_valida(Bot* bot) {
    // não força direção enquanto o bot estiver parado
    if (bot->parado) return;

    float mag = fabsf(bot->direcao_x) + fabsf(bot->direcao_y);
    if (mag < 0.001f) {
        float ang = ((float)rand() / RAND_MAX) * 6.283185f;
        bot->direcao_x = cosf(ang);
        bot->direcao_y = sinf(ang);
        bot->tempo_mudanca = TEMPO_MUDANCA_MIN;
    }
}

/* Ajustada assinatura para corresponder ao header: (largura_mapa, altura_mapa, delta_time) */
void atualizar_bot(Bot* bot, float largura_mapa, float altura_mapa, float delta_time) {
    if (!bot->ativo) return;

    /* DEBUG: log reduzido para o BOTO */
    if (bot->animal_data.nome && strcmp(bot->animal_data.nome, NOME_ANIMAL_BOTO) == 0) {
        static int _dbg_counter = 0;
        _dbg_counter++;
        if (_dbg_counter % 60 == 0) { /* a cada ~60 frames */
            float dx = bot->x - bot->area_restrita.centro_elipse_x;
            float dy = bot->y - bot->area_restrita.centro_elipse_y;
            float a = bot->area_restrita.raio_horizontal;
            float b = bot->area_restrita.raio_vertical;
            float termo = 0.0f;
            if (a > 0.0f && b > 0.0f) termo = (dx*dx)/(a*a) + (dy*dy)/(b*b);
            printf("[DBG BOTO] x=%.2f y=%.2f dir=(%.2f,%.2f) tempo_mud=%.2f termo_elipse=%.3f fugindo=%d vel=%.3f\n",
                bot->x, bot->y, bot->direcao_x, bot->direcao_y, bot->tempo_mudanca, termo, bot->fugindo ? 1 : 0, bot->velocidade);
        }
    }

    if (bot->cooldown_colisao > 0.0f) {
        bot->cooldown_colisao -= delta_time;
        if (bot->cooldown_colisao < 0.0f) bot->cooldown_colisao = 0.0f;
    }

    if (bot->fugindo) {
        bot->fuga_timer -= delta_time;
        if (bot->fuga_timer <= 0.0f) {
            bot->fugindo = false;
            garantir_direcao_valida(bot);
        }
    }

    // Se está parado, decrementar timer e manter inerte
    if (bot->parado) {
        bot->parado_timer -= delta_time;
        if (bot->parado_timer > 0.0f) {
            // permanece parado neste tick
            return;
        } else {
            // sair do estado parado: escolhe nova direção aleatória e reinicia tempo_mudanca
            bot->parado = false;
            float ang = ((float)rand() / RAND_MAX) * 6.283185f;
            bot->direcao_x = cosf(ang);
            bot->direcao_y = sinf(ang);
            bot->tempo_mudanca = TEMPO_MUDANCA_MIN + ((float)rand() / RAND_MAX) * TEMPO_MUDANCA_VARIACAO;
        }
    }

    float min_x = 0.0f, max_x = largura_mapa;
    float min_y = 0.0f, max_y = altura_mapa;
    
    switch (bot->cenario) {
    case CENARIO1:
        max_x = LIMITE_HORIZONTAL_MAPA;
        max_y = LIMITE_VERTICAL_MAPA;
        break;
    case CENARIO2:
        min_y = LIMITE_VERTICAL_MAPA;
        max_x = LIMITE_HORIZONTAL_MAPA;
        break;
    case CENARIO3:
        min_x = LIMITE_HORIZONTAL_MAPA;
        max_y = LIMITE_VERTICAL_MAPA;
        break;
    case CENARIO4:
        min_x = LIMITE_HORIZONTAL_MAPA;
        min_y = LIMITE_VERTICAL_MAPA;
        break;
    }

    bot->tempo_mudanca -= delta_time;
    if (bot->tempo_mudanca <= 0.0f) {
        float melhor_angulo = ((float)rand() / RAND_MAX) * 6.283185f;
        float melhor_score = -1.0f;

        for (int i = 0; i < TENTATIVAS_DIRECAO_ALEATORIA; i++) {
            float angulo_teste = ((float)rand() / RAND_MAX) * 6.283185f;
            float teste_x = cosf(angulo_teste);
            float teste_y = sinf(angulo_teste);

            float score = 0.0f;
            float pos_teste_x = bot->x + teste_x * DISTANCIA_TESTE_DIRECAO;
            float pos_teste_y = bot->y + teste_y * DISTANCIA_TESTE_DIRECAO;

            if (pos_teste_x > min_x + MARGEM_BORDA_ANIMAL && pos_teste_x < max_x - MARGEM_BORDA_ANIMAL) 
                score += SCORE_DIRECAO_VALIDA;
            if (pos_teste_y > min_y + MARGEM_BORDA_ANIMAL && pos_teste_y < max_y - MARGEM_BORDA_ANIMAL) 
                score += SCORE_DIRECAO_VALIDA;

            if (score > melhor_score) {
                melhor_score = score;
                melhor_angulo = angulo_teste;
            }
        }

        // chance de ficar parado em vez de escolher direção
        float chance = (float)(rand() % 100);
        if (chance < PARAR_CHANCE_PORCENT) {
            bot->parado = true;
            bot->parado_timer = PARAR_DURACAO_MIN + ((float)rand() / RAND_MAX) * PARAR_DURACAO_VAR;
            bot->direcao_x = 0.0f;
            bot->direcao_y = 0.0f;
            // garante que não vai tentar trocar direção por um tempo (uso parado_timer)
            // devolve tempo_mudanca para um valor razoável após o parado terminar
            bot->tempo_mudanca = bot->parado_timer;
            return;
        }

        bot->direcao_x = cosf(melhor_angulo);
        bot->direcao_y = sinf(melhor_angulo);
        bot->tempo_mudanca = TEMPO_MUDANCA_MIN + ((float)rand() / RAND_MAX) * TEMPO_MUDANCA_VARIACAO;
    }

    garantir_direcao_valida(bot);

    /* Se aqui, movimento normal */
    float velocidade = bot->velocidade * (bot->fugindo ? bot->fuga_velocidade_multiplicador : 1.0f);
    float passo = velocidade * delta_time * 60.0f;
    float novo_x = bot->x + bot->direcao_x * passo;
    float novo_y = bot->y + bot->direcao_y * passo;

    float margem = bot->raio + 5.0f;
    bool borda = false;
    
    if (novo_x < min_x + margem) { 
        novo_x = min_x + margem; 
        bot->direcao_x = fabsf(bot->direcao_x); 
        borda = true; 
    }
    if (novo_x > max_x - margem) { 
        novo_x = max_x - margem; 
        bot->direcao_x = -fabsf(bot->direcao_x); 
        borda = true; 
    }
    if (novo_y < min_y + margem) { 
        novo_y = min_y + margem; 
        bot->direcao_y = fabsf(bot->direcao_y); 
        borda = true; 
    }
    if (novo_y > max_y - margem) { 
        novo_y = max_y - margem; 
        bot->direcao_y = -fabsf(bot->direcao_y); 
        borda = true; 
    }

    if (borda) bot->tempo_mudanca = TEMPO_MUDANCA_BORDA;

    if (bot->area_restrita.tipo != AREA_NENHUMA) {
        if (posicao_valida(bot, novo_x, novo_y)) {
            bot->x = novo_x;
            bot->y = novo_y;
        } else {
            // tentativa de escapar: primeiro tenta mover perpendicularmente (duas direções)
            bool moved = false;
            float perp_x = -bot->direcao_y;
            float perp_y =  bot->direcao_x;
            for (int s = 0; s < 2 && !moved; s++) {
                float sinal = (s == 0) ? 1.0f : -1.0f;
                float tx = bot->x + perp_x * sinal * passo * 0.8f;
                float ty = bot->y + perp_y * sinal * passo * 0.8f;
                if (posicao_valida(bot, tx, ty)) {
                    bot->x = tx;
                    bot->y = ty;
                    bot->direcao_x = perp_x * sinal;
                    bot->direcao_y = perp_y * sinal;
                    moved = true;
                    break;
                }
            }

            // se não conseguiu, tenta reduzir o passo progressivamente
            if (!moved) {
                float factor = 0.5f;
                for (int k = 0; k < 4 && !moved; k++) {
                    float tx = bot->x + bot->direcao_x * passo * factor;
                    float ty = bot->y + bot->direcao_y * passo * factor;
                    if (posicao_valida(bot, tx, ty)) {
                        bot->x = tx;
                        bot->y = ty;
                        moved = true;
                        break;
                    }
                    factor *= 0.5f;
                }
            }

            // somente se todas as tentativas falharem, marca nova direção (evita reset contínuo)
            if (!moved) {
                bot->tempo_mudanca = TEMPO_MUDANCA_NOVA_DIRECAO;
            }
        }
    } else {
        bot->x = novo_x;
        bot->y = novo_y;
    }

    corrigir_posicao_area_restrita(bot);
    corrigir_posicao_borda(bot, largura_mapa, altura_mapa);
}

/* Ajustada assinatura para corresponder ao header: (largura_mapa, altura_mapa, delta_time) */
void atualizar_bot_com_cacador(Bot* bot, Cacador* cacador, float largura_mapa, float altura_mapa, float delta_time) {
    if (!bot->ativo) return;

    // guarda posição anterior para detectar se o animal "parou"
    float prev_x = bot->x;
    float prev_y = bot->y;

    /* DEBUG: encaminha também para atualização normal (mostra logs ali) */
    if (!cacador || !cacador->ativo || cacador->derrotado) {
        if (fabsf(bot->direcao_x) < 0.001f && fabsf(bot->direcao_y) < 0.001f) {
            float ang = ((float)rand() / RAND_MAX) * 6.283185f;
            bot->direcao_x = cosf(ang);
            bot->direcao_y = sinf(ang);
            bot->tempo_mudanca = TEMPO_MUDANCA_MIN;
        }
        atualizar_bot(bot, largura_mapa, altura_mapa, delta_time);
        return;
    }

    float dx = bot->x - cacador->x;
    float dy = bot->y - cacador->y;
    float distancia = sqrtf(dx * dx + dy * dy);

    if (distancia < RAIO_FUGA_ANIMAL && distancia > 0.1f) {
        bot->fugindo = true;
        bot->fuga_timer = TEMPO_FUGA_ANIMAL;

        float velocidade_fuga = bot->velocidade * bot->fuga_velocidade_multiplicador;

        float fuga_x = 0.0f, fuga_y = 0.0f;
        calcular_direcao_fuga_segura(bot, cacador->x, cacador->y, &fuga_x, &fuga_y, largura_mapa, altura_mapa);

        float novo_x = bot->x + fuga_x * velocidade_fuga * delta_time * 60.0f;
        float novo_y = bot->y + fuga_y * velocidade_fuga * delta_time * 60.0f;

        if (bot->area_restrita.tipo != AREA_NENHUMA) {
            if (posicao_valida(bot, novo_x, novo_y)) {
                bot->x = novo_x;
                bot->y = novo_y;
            }
            else {
                bool moved = false;

                // 1) Tentar usar reflexão da direção de fuga contra a normal da elipse (se elíptica)
                if (bot->area_restrita.tipo == AREA_ELIPTICA) {
                    float cx = bot->area_restrita.centro_elipse_x;
                    float cy = bot->area_restrita.centro_elipse_y;
                    float a = bot->area_restrita.raio_horizontal;
                    float b = bot->area_restrita.raio_vertical;

                    if (a > EPSILON_ELIPSE && b > EPSILON_ELIPSE) {
                        // normal da elipse no ponto (aprox) n = ( (x-cx)/a^2, (y-cy)/b^2 )
                        float nx = (bot->x - cx) / (a * a);
                        float ny = (bot->y - cy) / (b * b);
                        float nmag = sqrtf(nx * nx + ny * ny);
                        if (nmag > 1e-6f) {
                            nx /= nmag; ny /= nmag;
                            // refletir vetor fuga em relação à normal: r = v - 2*(v·n)*n
                            float dot = fuga_x * nx + fuga_y * ny;
                            float rx = fuga_x - 2.0f * dot * nx;
                            float ry = fuga_y - 2.0f * dot * ny;
                            float rmag = sqrtf(rx * rx + ry * ry);
                            if (rmag > 1e-6f) {
                                rx /= rmag; ry /= rmag;
                                float tx = bot->x + rx * 0.8f * velocidade_fuga * delta_time * 60.0f;
                                float ty = bot->y + ry * 0.8f * velocidade_fuga * delta_time * 60.0f;
                                if (posicao_valida(bot, tx, ty)) {
                                    bot->x = tx;
                                    bot->y = ty;
                                    bot->direcao_x = rx;
                                    bot->direcao_y = ry;
                                    moved = true;
                                }
                            }
                        }
                    }
                }

                // 2) Tentar mover perpendicularmente (duas direções), caso reflexão falhe
                if (!moved) {
                    float perpendicular_x = -fuga_y;
                    float perpendicular_y =  fuga_x;
                    for (int s = 0; s < 2 && !moved; s++) {
                        float sinal = (s == 0) ? 1.0f : -1.0f;
                        float teste_x = bot->x + perpendicular_x * sinal * 0.8f * velocidade_fuga * delta_time * 60.0f;
                        float teste_y = bot->y + perpendicular_y * sinal * 0.8f * velocidade_fuga * delta_time * 60.0f;
                        if (posicao_valida(bot, teste_x, teste_y)) {
                            bot->x = teste_x;
                            bot->y = teste_y;
                            bot->direcao_x = perpendicular_x * sinal;
                            bot->direcao_y = perpendicular_y * sinal;
                            moved = true;
                            break;
                        }
                    }
                }

                // 3) Se ainda não conseguiu, reduzir o passo progressivamente na direção original de fuga
                if (!moved) {
                    float factor = 0.5f;
                    for (int k = 0; k < 4 && !moved; k++) {
                        float tx = bot->x + fuga_x * velocidade_fuga * delta_time * 60.0f * factor;
                        float ty = bot->y + fuga_y * velocidade_fuga * delta_time * 60.0f * factor;
                        if (posicao_valida(bot, tx, ty)) {
                            bot->x = tx;
                            bot->y = ty;
                            moved = true;
                            break;
                        }
                        factor *= 0.5f;
                    }
                }

                // 4) fallback: marcar nova direção (evita reset contínuo do tempo de mudança)
                if (!moved) {
                    bot->tempo_mudanca = TEMPO_MUDANCA_NOVA_DIRECAO;
                }
            }
        } else {
            bot->x = novo_x;
            bot->y = novo_y;
        }

        // atualiza direção e evita trocar de direção imediatamente (mantém fuga)
        bot->direcao_x = fuga_x;
        bot->direcao_y = fuga_y;
        // não forçar tempo_mudanca grande aqui — deixamos fuga_timer controlar a duração da fuga
        // bot->tempo_mudanca = 1.0f;

        corrigir_posicao_area_restrita(bot);
    }
    else {
        // chamada atualizada para nova ordem de parâmetros
        atualizar_bot(bot, largura_mapa, altura_mapa, delta_time);
    }

    // Detecta se o animal praticamente não se moveu neste tick (ou seja "parou")
    {
        float dxm = bot->x - prev_x;
        float dym = bot->y - prev_y;
        float moved = sqrtf(dxm * dxm + dym * dym);

        if (!bot->fugindo && moved < STOPPED_THRESHOLD && cacador) {
            /* evita registrar repetidamente a mesma coordenada sem necessidade:
               registra se o caçador não tiver histórico ou se a última posição
               conhecida for suficientemente diferente */
            float last_dx = cacador->ultimo_animal_x - bot->x;
            float last_dy = cacador->ultimo_animal_y - bot->y;
            if (cacador->contador_posicao_animal == 0 ||
                fabsf(last_dx) > 0.5f || fabsf(last_dy) > 0.5f) {
                cacador_registrar_posicao(cacador, bot->x, bot->y);
            }
        }
    }

    corrigir_posicao_borda(bot, largura_mapa, altura_mapa);
}

void desenhar_bot(const Bot* bot, ALLEGRO_COLOR cor, float camera_x, float camera_y) {
    if (!bot->ativo) return;

    float bot_x_tela = (bot->x - camera_x) * ZOOM_FACTOR;
    float bot_y_tela = (bot->y - camera_y) * ZOOM_FACTOR;

    if (bot->usar_sprite && bot->sprite_animal) {
        desenhar_sprite_camera(bot->sprite_animal, bot->x, bot->y, camera_x, camera_y, bot->escala_sprite, ZOOM_FACTOR);
    } else {
        al_draw_filled_circle(bot_x_tela, bot_y_tela, bot->raio * ZOOM_FACTOR, cor);
    }

    int vida_atual = bot->vida_visual_atual < 0 ? 0 : bot->vida_visual_atual;
    if (vida_atual > bot->vida_visual_maxima) vida_atual = bot->vida_visual_maxima;

    float proporcao = (bot->vida_visual_maxima > 0) ? (float)vida_atual / (float)bot->vida_visual_maxima : 0.0f;

    float barra_largura = 40.0f * ZOOM_FACTOR;
    float barra_altura = 5.0f * ZOOM_FACTOR;
    float barra_x = bot_x_tela - barra_largura / 2.0f;
    float barra_y = bot_y_tela - (bot->raio + 20.0f) * ZOOM_FACTOR;

    al_draw_filled_rounded_rectangle(barra_x - 2.0f, barra_y - 2.0f,
                                     barra_x + barra_largura + 2.0f, barra_y + barra_altura + 2.0f,
                                     2.0f, 2.0f, al_map_rgba(0, 0, 0, 150));

    al_draw_filled_rounded_rectangle(barra_x, barra_y,
                                     barra_x + barra_largura, barra_y + barra_altura,
                                     2.0f, 2.0f, al_map_rgb(60, 60, 60));

    ALLEGRO_COLOR cor_vida =
           (proporcao > LIMIAR_VIDA_ALTA) ? al_map_rgb(COR_VIDA_ALTA_R, COR_VIDA_ALTA_G, COR_VIDA_ALTA_B) :
        (proporcao > LIMIAR_VIDA_MEDIA) ? al_map_rgb(COR_VIDA_MEDIA_R, COR_VIDA_MEDIA_G, COR_VIDA_MEDIA_B) :
                                          al_map_rgb(COR_VIDA_BAIXA_R, COR_VIDA_BAIXA_G, COR_VIDA_BAIXA_B);

    al_draw_filled_rounded_rectangle(barra_x, barra_y,
                                     barra_x + barra_largura * proporcao, barra_y + barra_altura,
                                     2.0f, 2.0f, cor_vida);

    al_draw_rounded_rectangle(barra_x, barra_y,
                              barra_x + barra_largura, barra_y + barra_altura,
                              2.0f, 2.0f, al_map_rgb(255, 255, 255), 1.0f);
}

bool verificar_colisao_bot(const entidade* jogador, const Bot* bot) {
    if (!bot->ativo) return false;
    if (bot->cooldown_colisao > 0.0f) return false;

    float dx = jogador->x - bot->x;
    float dy = jogador->y - bot->y;
    float distancia = sqrtf(dx * dx + dy * dy);

    return distancia <= (jogador->raio + bot->raio);
}

void destruir_bot(Bot* bot) {
    if (!bot) return;
    if (bot->sprite_animal) {
        destruir_sprite(bot->sprite_animal);
        bot->sprite_animal = NULL;
    }
}

// ========== SISTEMA DE VISÃO ==========

void inicializar_sistema_visao(SistemaVisao* visao, float raio, float angulo) {
    if (!visao) return;
    
    visao->raio_visao = raio;
    visao->angulo_visao = angulo;
    visao->direcao_olhar = 0.0f;  // Inicialmente olhando para direita
    
    visao->ameaca_detectada = false;
    visao->ameaca_x = 0.0f;
    visao->ameaca_y = 0.0f;
    visao->tempo_desde_deteccao = 999.0f;  // Muito tempo sem ver ameaça
    visao->distancia_ameaca = 999.0f;
    
    visao->mostrar_debug = false;  // Desligado por padrão
}

bool ponto_no_campo_visao(const Bot* bot, float px, float py) {
    if (!bot) return false;
    
    // Calcula distância até o ponto
    float dx = px - bot->x;
    float dy = py - bot->y;
    float distancia = sqrtf(dx * dx + dy * dy);
    
    // Se está além do raio de visão, não vê
    if (distancia > bot->visao.raio_visao) return false;
    
    // Se o ângulo de visão é >= 2π (360°), vê tudo ao redor
    if (bot->visao.angulo_visao >= 6.28f) return true;
    
    // Calcula ângulo até o ponto
    float angulo_ponto = atan2f(dy, dx);
    
    // Normaliza diferença de ângulo para [-π, π]
    float diff_angulo = angulo_ponto - bot->visao.direcao_olhar;
    while (diff_angulo > 3.14159f) diff_angulo -= 6.28318f;
    while (diff_angulo < -3.14159f) diff_angulo += 6.28318f;
    
    // Verifica se está dentro do cone de visão
    return fabsf(diff_angulo) <= (bot->visao.angulo_visao / 2.0f);
}

void atualizar_deteccao_ameacas(Bot* bot, float ameaca_x, float ameaca_y, float delta_time) {
    if (!bot) return;
    
    // Atualiza direção do olhar baseado na direção de movimento
    if (fabsf(bot->direcao_x) > 0.01f || fabsf(bot->direcao_y) > 0.01f) {
        bot->visao.direcao_olhar = atan2f(bot->direcao_y, bot->direcao_x);
    }
    
    // Verifica se a ameaça está no campo de visão
    bool pode_ver = ponto_no_campo_visao(bot, ameaca_x, ameaca_y);
    
    if (pode_ver) {
        // Ameaça detectada!
        bot->visao.ameaca_detectada = true;
        bot->visao.ameaca_x = ameaca_x;
        bot->visao.ameaca_y = ameaca_y;
        bot->visao.tempo_desde_deteccao = 0.0f;
        
        float dx = ameaca_x - bot->x;
        float dy = ameaca_y - bot->y;
        bot->visao.distancia_ameaca = sqrtf(dx * dx + dy * dy);
    } else {
        // Não consegue ver a ameaça
        bot->visao.tempo_desde_deteccao += delta_time;
        
        // Se passou muito tempo, "esquece" a ameaça
        if (bot->visao.tempo_desde_deteccao > TEMPO_MEMORIA_AMEACA) {
            bot->visao.ameaca_detectada = false;
        }
    }
}

void desenhar_campo_visao(const Bot* bot, float camera_x, float camera_y) {
    if (!bot || !bot->visao.mostrar_debug) return;
    
    float bot_x_tela = (bot->x - camera_x) * ZOOM_FACTOR;
    float bot_y_tela = (bot->y - camera_y) * ZOOM_FACTOR;
    float raio_tela = bot->visao.raio_visao * ZOOM_FACTOR;
    
    // Desenha círculo de raio de visão
    al_draw_circle(bot_x_tela, bot_y_tela, raio_tela, 
                   al_map_rgba(255, 255, 0, 100), 1.5f);
    
    // Se tem cone de visão (não é 360°), desenha o cone
    if (bot->visao.angulo_visao < 6.28f) {
        float angulo_inicio = bot->visao.direcao_olhar - bot->visao.angulo_visao / 2.0f;
        float angulo_fim = bot->visao.direcao_olhar + bot->visao.angulo_visao / 2.0f;
        
        // Desenha as linhas do cone
        float x1 = bot_x_tela + cosf(angulo_inicio) * raio_tela;
        float y1 = bot_y_tela + sinf(angulo_inicio) * raio_tela;
        float x2 = bot_x_tela + cosf(angulo_fim) * raio_tela;
        float y2 = bot_y_tela + sinf(angulo_fim) * raio_tela;
        
        al_draw_line(bot_x_tela, bot_y_tela, x1, y1, 
                     al_map_rgba(255, 255, 0, 150), 2.0f);
        al_draw_line(bot_x_tela, bot_y_tela, x2, y2, 
                     al_map_rgba(255, 255, 0, 150), 2.0f);
        
        // Desenha arco do cone
        al_draw_arc(bot_x_tela, bot_y_tela, raio_tela,
                    angulo_inicio, bot->visao.angulo_visao,
                    al_map_rgba(255, 255, 0, 150), 2.0f);
    }
    
    // Se detectou ameaça, desenha linha até ela
    if (bot->visao.ameaca_detectada) {
        float ameaca_x_tela = (bot->visao.ameaca_x - camera_x) * ZOOM_FACTOR;
        float ameaca_y_tela = (bot->visao.ameaca_y - camera_y) * ZOOM_FACTOR;
        
        al_draw_line(bot_x_tela, bot_y_tela, ameaca_x_tela, ameaca_y_tela,
                     al_map_rgba(255, 0, 0, 200), 2.0f);
    }
}

// ========== MÁQUINA DE ESTADOS DA IA ==========

void mudar_estado_ia(Bot* bot, EstadoIA novo_estado) {
    if (!bot) return;
    
    // Debug log
    printf("[IA] %s: %s -> %s\n", bot->animal_data.nome,
           nome_estado_ia(bot->estado_atual), nome_estado_ia(novo_estado));
    
    bot->estado_atual = novo_estado;
    bot->tempo_no_estado = 0.0f;
    
    // Ajusta velocidade baseado no novo estado
    switch (novo_estado) {
        case ESTADO_PATRULHA:
            bot->velocidade = bot->velocidade_base * VEL_MULTIPLICADOR_PATRULHA;
            break;
        case ESTADO_ALERTA:
            bot->velocidade = bot->velocidade_base * VEL_MULTIPLICADOR_ALERTA;
            break;
        case ESTADO_FUGA:
            bot->velocidade = bot->velocidade_base * VEL_MULTIPLICADOR_FUGA;
            bot->fugindo = true;  // Compatibilidade com sistema antigo
            bot->fuga_timer = TEMPO_FUGA_ANIMAL;
            break;
        case ESTADO_ESCONDIDO:
            bot->velocidade = bot->velocidade_base * VEL_MULTIPLICADOR_ESCONDIDO;
            bot->parado = true;
            bot->parado_timer = TEMPO_MIN_ESCONDIDO;
            break;
        case ESTADO_RETORNANDO:
            bot->velocidade = bot->velocidade_base * VEL_MULTIPLICADOR_RETORNANDO;
            break;
    }
}

const char* nome_estado_ia(EstadoIA estado) {
    switch (estado) {
        case ESTADO_PATRULHA:   return "PATRULHA";
        case ESTADO_ALERTA:     return "ALERTA";
        case ESTADO_FUGA:       return "FUGA";
        case ESTADO_ESCONDIDO:  return "ESCONDIDO";
        case ESTADO_RETORNANDO: return "RETORNANDO";
        default:                return "DESCONHECIDO";
    }
}

void atualizar_estado_ia(Bot* bot, float delta_time) {
    if (!bot) return;
    
    bot->tempo_no_estado += delta_time;
    
    // Máquina de estados
    switch (bot->estado_atual) {
        case ESTADO_PATRULHA:
            // Se detectou ameaça próxima, entra em ALERTA
            if (bot->visao.ameaca_detectada && bot->visao.distancia_ameaca < RAIO_FUGA_ANIMAL) {
                if (bot->visao.distancia_ameaca < DISTANCIA_PERIGO_IMINENTE) {
                    // Perigo muito próximo: foge direto
                    mudar_estado_ia(bot, ESTADO_FUGA);
                } else {
                    // Perigo detectado: fica alerta
                    mudar_estado_ia(bot, ESTADO_ALERTA);
                }
            }
            // Chance de parar para "descansar"
            else if (bot->tempo_no_estado > TEMPO_MIN_PATRULHA) {
                float chance = ((float)rand() / RAND_MAX);
                if (chance < CHANCE_PARAR_PATRULHA) {
                    mudar_estado_ia(bot, ESTADO_ESCONDIDO);
                }
            }
            break;
            
        case ESTADO_ALERTA:
            // Se ameaça ficou muito próxima, foge
            if (bot->visao.ameaca_detectada && bot->visao.distancia_ameaca < DISTANCIA_PERIGO_IMINENTE) {
                mudar_estado_ia(bot, ESTADO_FUGA);
            }
            // Se ameaça desapareceu ou está longe, volta a patrulhar
            else if (!bot->visao.ameaca_detectada || bot->visao.distancia_ameaca > DISTANCIA_SEGURA) {
                if (bot->tempo_no_estado > TEMPO_MIN_ALERTA) {
                    mudar_estado_ia(bot, ESTADO_PATRULHA);
                }
            }
            break;
            
        case ESTADO_FUGA:
            // Se já fugiu por tempo suficiente e está longe, tenta se esconder
            if (bot->tempo_no_estado > TEMPO_MIN_FUGA) {
                if (!bot->visao.ameaca_detectada || bot->visao.distancia_ameaca > DISTANCIA_SEGURA) {
                    mudar_estado_ia(bot, ESTADO_ESCONDIDO);
                }
            }
            break;
            
        case ESTADO_ESCONDIDO:
            // Se foi descoberto, volta a fugir
            if (bot->visao.ameaca_detectada && bot->visao.distancia_ameaca < RAIO_FUGA_ANIMAL) {
                mudar_estado_ia(bot, ESTADO_FUGA);
            }
            // Se ficou escondido tempo suficiente, volta a área segura
            else if (bot->tempo_no_estado > TEMPO_MIN_ESCONDIDO) {
                mudar_estado_ia(bot, ESTADO_RETORNANDO);
            }
            break;
            
        case ESTADO_RETORNANDO:
            // Verifica se chegou perto da posição segura
            float dx = bot->x - bot->ultima_pos_segura_x;
            float dy = bot->y - bot->ultima_pos_segura_y;
            float dist_segura = sqrtf(dx * dx + dy * dy);
            
            // Se chegou ou foi ameaçado novamente
            if (dist_segura < 30.0f || bot->tempo_no_estado > TEMPO_MIN_RETORNANDO) {
                mudar_estado_ia(bot, ESTADO_PATRULHA);
            }
            else if (bot->visao.ameaca_detectada && bot->visao.distancia_ameaca < RAIO_FUGA_ANIMAL) {
                mudar_estado_ia(bot, ESTADO_FUGA);
            }
            break;
    }
}

// FIM DO ARQUIVO