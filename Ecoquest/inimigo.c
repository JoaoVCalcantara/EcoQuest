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

// ========== FUNÇÕES AUXILIARES ==========

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

    float dx = bot->x - cacador_x;
    float dy = bot->y - cacador_y;
    float distancia = sqrtf(dx * dx + dy * dy);

    if (distancia > 0.1f) {
        *fuga_x = dx / distancia;
        *fuga_y = dy / distancia;
    }

    // Calcular centro do cenário (ou centro da área restrita se aplicável)
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

    // Verificar proximidade com bordas
    bool perto_borda_esquerda = bot->x < (min_x + MARGEM_SEGURANCA_BORDA);
    bool perto_borda_direita = bot->x > (max_x - MARGEM_SEGURANCA_BORDA);
    bool perto_borda_cima = bot->y < (min_y + MARGEM_SEGURANCA_BORDA);
    bool perto_borda_baixo = bot->y > (max_y - MARGEM_SEGURANCA_BORDA);

    // CORREÇÃO PARA ELIPSE: verificar se está perto da borda da elipse
    if (bot->area_restrita.tipo == AREA_ELIPTICA) {
        float dx_elipse = bot->x - bot->area_restrita.centro_elipse_x;
        float dy_elipse = bot->y - bot->area_restrita.centro_elipse_y;
        float a = bot->area_restrita.raio_horizontal;
        float b = bot->area_restrita.raio_vertical;
        
        float termo_x = (dx_elipse * dx_elipse) / (a * a);
        float termo_y = (dy_elipse * dy_elipse) / (b * b);
        
        // Se está perto da borda da elipse
        if (termo_x + termo_y > ELIPSE_PERTO_BORDA) {
            perto_borda_esquerda = true;
        }
    }

    // Se está perto de alguma borda, ajustar direção para o centro
    if (perto_borda_esquerda || perto_borda_direita || perto_borda_cima || perto_borda_baixo) {
        float dx_centro = centro_x - bot->x;
        float dy_centro = centro_y - bot->y;
        float dist_centro = sqrtf(dx_centro * dx_centro + dy_centro * dy_centro);
        
        if (dist_centro > 0.1f) {
            *fuga_x = (*fuga_x * (1.0f - PESO_FUGA_CENTRO)) + ((dx_centro / dist_centro) * PESO_FUGA_CENTRO);
            *fuga_y = (*fuga_y * (1.0f - PESO_FUGA_CENTRO)) + ((dy_centro / dist_centro) * PESO_FUGA_CENTRO);
            
            // Normalizar
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
    
    // Direção inicial aleatória
    float angulo_inicial = ((float)rand() / RAND_MAX) * 6.283185f;
    bot->direcao_x = cosf(angulo_inicial);
    bot->direcao_y = sinf(angulo_inicial);
    
    // Tempo de mudança inicial aleatório
    bot->tempo_mudanca = TEMPO_MUDANCA_MIN + ((float)rand() / RAND_MAX) * TEMPO_MUDANCA_VARIACAO;
    
    bot->ativo = true;
    bot->cenario = cenario;
    bot->cooldown_colisao = 0.0f;

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
    bot->vida_visual_atual = VIDA_VISUAL_MAXIMA_ANIMAL;
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
    float mag = fabsf(bot->direcao_x) + fabsf(bot->direcao_y);
    if (mag < 0.001f) {
        float ang = ((float)rand() / RAND_MAX) * 6.283185f;
        bot->direcao_x = cosf(ang);
        bot->direcao_y = sinf(ang);
        bot->tempo_mudanca = TEMPO_MUDANCA_MIN;
    }
}

void atualizar_bot(Bot* bot, float delta_time, float largura_mapa, float altura_mapa) {
    if (!bot->ativo) return;

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

        bot->direcao_x = cosf(melhor_angulo);
        bot->direcao_y = sinf(melhor_angulo);
        bot->tempo_mudanca = TEMPO_MUDANCA_MIN + ((float)rand() / RAND_MAX) * TEMPO_MUDANCA_VARIACAO;
    }

    garantir_direcao_valida(bot);

    float velocidade = bot->velocidade * (bot->fugindo ? bot->fuga_velocidade_multiplicador : 1.0f);
    float novo_x = bot->x + bot->direcao_x * velocidade * delta_time * 60.0f;
    float novo_y = bot->y + bot->direcao_y * velocidade * delta_time * 60.0f;

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
            bot->tempo_mudanca = TEMPO_MUDANCA_NOVA_DIRECAO;
        }
    } else {
        bot->x = novo_x;
        bot->y = novo_y;
    }

    corrigir_posicao_area_restrita(bot);
    corrigir_posicao_borda(bot, largura_mapa, altura_mapa);
}

void atualizar_bot_com_cacador(Bot* bot, Cacador* cacador, float delta_time, float largura_mapa, float altura_mapa) {
    if (!bot->ativo) return;

    if (!cacador || !cacador->ativo || cacador->derrotado) {
        if (fabsf(bot->direcao_x) < 0.001f && fabsf(bot->direcao_y) < 0.001f) {
            float ang = ((float)rand() / RAND_MAX) * 6.283185f;
            bot->direcao_x = cosf(ang);
            bot->direcao_y = sinf(ang);
            bot->tempo_mudanca = TEMPO_MUDANCA_MIN;
        }
        atualizar_bot(bot, delta_time, largura_mapa, altura_mapa);
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
                float perpendicular_x = -fuga_y;
                float perpendicular_y =  fuga_x;

                float teste_x = bot->x + perpendicular_x * 0.8f * velocidade_fuga * delta_time * 60.0f;
                float teste_y = bot->y + perpendicular_y * 0.8f * velocidade_fuga * delta_time * 60.0f;

                if (posicao_valida(bot, teste_x, teste_y)) {
                    bot->x = teste_x;
                    bot->y = teste_y;
                } else {
                    teste_x = bot->x - perpendicular_x * 0.8f * velocidade_fuga * delta_time * 60.0f;
                    teste_y = bot->y - perpendicular_y * 0.8f * velocidade_fuga * delta_time * 60.0f;

                    if (posicao_valida(bot, teste_x, teste_y)) {
                        bot->x = teste_x;
                        bot->y = teste_y;
                    }
                }
            }
        } else {
            bot->x = novo_x;
            bot->y = novo_y;
        }

        bot->direcao_x = fuga_x;
        bot->direcao_y = fuga_y;
        bot->tempo_mudanca = 1.0f;

        corrigir_posicao_area_restrita(bot);
    }
    else {
        atualizar_bot(bot, delta_time, largura_mapa, altura_mapa);
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

// FIM DO ARQUIVO - NÃO ADICIONAR MAIS NADA AQUI