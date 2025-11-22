#include "inimigo.h"
#include "cenario.h"
#include "sprites.h"
#include "entidades.h"
#include "cacador.h"
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
        float termo_x = (dx * dx) / (a * a);
        float termo_y = (dy * dy) / (b * b);
        float margem_seguranca = 0.95f;
        return (termo_x + termo_y) <= (margem_seguranca * margem_seguranca);
    }
    return true;
}

static void corrigir_posicao_borda(Bot* bot, float largura_mapa, float altura_mapa) {
    float min_x = 0.0f, max_x = largura_mapa;
    float min_y = 0.0f, max_y = altura_mapa;

    switch (bot->cenario) {
    case CENARIO1:
        max_x = 640.0f; max_y = 360.0f; break;
    case CENARIO2:
        min_y = 360.0f; max_x = 640.0f; max_y = 720.0f; break;
    case CENARIO3:
        min_x = 640.0f; max_x = 1280.0f; max_y = 360.0f; break;
    case CENARIO4:
        min_x = 640.0f; max_x = 1280.0f; min_y = 360.0f; max_y = 720.0f; break;
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
    
    float min_x = 0.0f, max_x = largura_mapa;
    float min_y = 0.0f, max_y = altura_mapa;
    float margem_seguranca = 60.0f;

    // Definir limites do cenário
    switch (bot->cenario) {
    case CENARIO1:
        max_x = 640.0f; max_y = 360.0f; break;
    case CENARIO2:
        min_y = 360.0f; max_x = 640.0f; break;
    case CENARIO3:
        min_x = 640.0f; max_y = 360.0f; break;
    case CENARIO4:
        min_x = 640.0f; min_y = 360.0f; break;
    }

    // Direção original: fugir do caçador
    float dx = bot->x - cacador_x;
    float dy = bot->y - cacador_y;
    float distancia = sqrtf(dx * dx + dy * dy);

    if (distancia > 0.1f) {
        *fuga_x = dx / distancia;
        *fuga_y = dy / distancia;
    }

    // Calcular centro do cenário
    float centro_x = (min_x + max_x) / 2.0f;
    float centro_y = (min_y + max_y) / 2.0f;

    // Verificar proximidade com bordas
    bool perto_borda_esquerda = bot->x < (min_x + margem_seguranca);
    bool perto_borda_direita = bot->x > (max_x - margem_seguranca);
    bool perto_borda_cima = bot->y < (min_y + margem_seguranca);
    bool perto_borda_baixo = bot->y > (max_y - margem_seguranca);

    // Se está perto de alguma borda, ajustar direção
    if (perto_borda_esquerda || perto_borda_direita || perto_borda_cima || perto_borda_baixo) {
        float dx_centro = centro_x - bot->x;
        float dy_centro = centro_y - bot->y;
        float dist_centro = sqrtf(dx_centro * dx_centro + dy_centro * dy_centro);
        
        if (dist_centro > 0.1f) {
            float peso_centro = 0.7f;  // 70% em direção ao centro
            
            *fuga_x = (*fuga_x * (1.0f - peso_centro)) + ((dx_centro / dist_centro) * peso_centro);
            *fuga_y = (*fuga_y * (1.0f - peso_centro)) + ((dy_centro / dist_centro) * peso_centro);
            
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
    bot->raio = 8.0f;
    bot->velocidade = 0.6f;  // Aumentado de 0.5f para 0.6f
    bot->direcao_x = 1.0f;
    bot->direcao_y = 0.0f;
    bot->tempo_mudanca = 0.0f;
    bot->ativo = true;
    bot->cenario = cenario;
    bot->cooldown_colisao = 0.0f;

    bot->fugindo = false;
    bot->fuga_timer = 0.0f;
    bot->fuga_velocidade_multiplicador = 2.0f;  // Reduzido de 2.5f para 2.0f

    bot->animal_data.experiencia = 0;
    bot->animal_data.nivel = 1;
    bot->animal_data.nivel_alimentacao = 100;
    bot->animal_data.domado = false;
    bot->animal_data.estudado = false;
    bot->animal_data.alimentado = false;
    bot->animal_data.nome = nome;
    bot->animal_data.tipo = tipo;
    bot->animal_data.caminho_fundo_batalha = NULL;

    bot->sprite_animal = NULL;
    bot->usar_sprite = false;
    bot->escala_sprite = 0.03f;
    bot->caminho_sprite_batalha = NULL;

    bot->area_restrita.tipo = AREA_NENHUMA;
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

void iniciar_bot_com_sprite_e_fundo(Bot* bot, float x, float y, const char* nome, const char* tipo,
    JogoCenas cenario, const char* sprite_path, const char* fundo_batalha_path) {
    iniciar_bot_com_sprite(bot, x, y, nome, tipo, cenario, sprite_path);
    bot->animal_data.caminho_fundo_batalha = fundo_batalha_path;
}

void atualizar_bot(Bot* bot, float delta_time, float largura_mapa, float altura_mapa) {
    if (!bot->ativo) return;

    // Atualizar cooldown de colisão
    if (bot->cooldown_colisao > 0.0f) {
        bot->cooldown_colisao -= delta_time;
        if (bot->cooldown_colisao < 0.0f) {
            bot->cooldown_colisao = 0.0f;
        }
    }

    // Sistema de fuga
    if (bot->fugindo) {
        bot->fuga_timer -= delta_time;
        if (bot->fuga_timer <= 0.0f) {
            bot->fugindo = false;
        }
    }

    // Determinar área de movimento baseada no cenário
    float min_x = 0.0f, max_x = largura_mapa;
    float min_y = 0.0f, max_y = altura_mapa;
    
    switch (bot->cenario) {
    case CENARIO1:
        max_x = 640.0f; 
        max_y = 360.0f; 
        break;
    case CENARIO2:
        min_y = 360.0f; 
        max_x = 640.0f;
        break;
    case CENARIO3:
        min_x = 640.0f; 
        max_y = 360.0f;
        break;
    case CENARIO4:
        min_x = 640.0f; 
        min_y = 360.0f;
        break;
    }

    // Movimento aleatório natural
    bot->tempo_mudanca -= delta_time;
    
    if (bot->tempo_mudanca <= 0.0f) {
        // Escolher nova direção aleatória em toda a área
        float angulo = ((float)rand() / RAND_MAX) * 2.0f * 3.14159f;
        bot->direcao_x = cosf(angulo);
        bot->direcao_y = sinf(angulo);
        bot->tempo_mudanca = 2.0f + ((float)rand() / RAND_MAX) * 3.0f;  // Tempo maior entre mudanças
    }

    // Movimento básico (sempre ativo, mesmo se não estiver fugindo)
    float velocidade_atual = bot->velocidade;
    if (bot->fugindo) {
        velocidade_atual *= bot->fuga_velocidade_multiplicador;
    }
    
    float novo_x = bot->x + bot->direcao_x * velocidade_atual * delta_time * 60.0f;
    float novo_y = bot->y + bot->direcao_y * velocidade_atual * delta_time * 60.0f;

    // Verificar limites com margem menor para permitir movimento no centro
    float margem = bot->raio + 5.0f;  // Reduzido de 10.0f para 5.0f
    
    bool colidiu_borda = false;
    
    if (novo_x < min_x + margem) {
        novo_x = min_x + margem;
        bot->direcao_x = fabsf(bot->direcao_x);
        colidiu_borda = true;
    }
    if (novo_x > max_x - margem) {
        novo_x = max_x - margem;
        bot->direcao_x = -fabsf(bot->direcao_x);
        colidiu_borda = true;
    }
    if (novo_y < min_y + margem) {
        novo_y = min_y + margem;
        bot->direcao_y = fabsf(bot->direcao_y);
        colidiu_borda = true;
    }
    if (novo_y > max_y - margem) {
        novo_y = max_y - margem;
        bot->direcao_y = -fabsf(bot->direcao_y);
        colidiu_borda = true;
    }

    // Se colidiu com borda, escolher nova direção imediatamente
    if (colidiu_borda) {
        bot->tempo_mudanca = 0.2f;  // Mudar direção rapidamente após borda
    }

    bot->x = novo_x;
    bot->y = novo_y;

    // Tratamento especial para área elíptica (Boto)
    if (bot->area_restrita.tipo == AREA_ELIPTICA) {
        float dx_centro = bot->x - bot->area_restrita.centro_elipse_x;
        float dy_centro = bot->y - bot->area_restrita.centro_elipse_y;
        
        float termo_x = (dx_centro * dx_centro) / (bot->area_restrita.raio_horizontal * bot->area_restrita.raio_horizontal);
        float termo_y = (dy_centro * dy_centro) / (bot->area_restrita.raio_vertical * bot->area_restrita.raio_vertical);
        
        if (termo_x + termo_y > 1.0f) {
            float angulo = atan2f(dy_centro, dx_centro);
            bot->x = bot->area_restrita.centro_elipse_x + cosf(angulo) * (bot->area_restrita.raio_horizontal - bot->raio - 5.0f);
            bot->y = bot->area_restrita.centro_elipse_y + sinf(angulo) * (bot->area_restrita.raio_vertical - bot->raio - 5.0f);
            
            // Inverter direção
            bot->direcao_x = -bot->direcao_x;
            bot->direcao_y = -bot->direcao_y;
            bot->tempo_mudanca = 0.5f;
        }
    }
}

void atualizar_bot_com_cacador(Bot* bot, Cacador* cacador, float delta_time, float largura_mapa, float altura_mapa) {
    if (!bot->ativo) return;

    // Se caçador não está ativo, apenas atualizar bot normalmente
    if (!cacador || !cacador->ativo || cacador->derrotado) {
        atualizar_bot(bot, delta_time, largura_mapa, altura_mapa);
        return;
    }

    float dx = bot->x - cacador->x;
    float dy = bot->y - cacador->y;
    float distancia = sqrtf(dx * dx + dy * dy);

    // Animal foge quando caçador está próximo
    float raio_fuga = 200.0f;
    
    if (distancia < raio_fuga && distancia > 0.1f) {
        bot->fugindo = true;
        bot->fuga_timer = 2.0f;

        float velocidade_fuga = bot->velocidade * bot->fuga_velocidade_multiplicador;
        
        // Calcular direção de fuga segura (evitando bordas)
        float fuga_x = 0.0f, fuga_y = 0.0f;
        calcular_direcao_fuga_segura(bot, cacador->x, cacador->y, &fuga_x, &fuga_y, largura_mapa, altura_mapa);
        
        float novo_x = bot->x + fuga_x * velocidade_fuga * delta_time * 60.0f;
        float novo_y = bot->y + fuga_y * velocidade_fuga * delta_time * 60.0f;

        // Verificar áreas restritas
        if (bot->area_restrita.tipo != AREA_NENHUMA) {
            if (posicao_valida(bot, novo_x, novo_y)) {
                bot->x = novo_x;
                bot->y = novo_y;
            }
            else {
                // Tentar fugir perpendicularmente
                bot->x += -fuga_y * 0.7f * velocidade_fuga * delta_time * 60.0f;
                bot->y += fuga_x * 0.7f * velocidade_fuga * delta_time * 60.0f;
            }
        }
        else {
            bot->x = novo_x;
            bot->y = novo_y;
        }
        
        // Atualizar direção para o próximo frame
        bot->direcao_x = fuga_x;
        bot->direcao_y = fuga_y;
    }
    else {
        // Sempre atualizar bot normalmente (para movimento de patrulha quando não está fugindo)
        atualizar_bot(bot, delta_time, largura_mapa, altura_mapa);
    }
}

void desenhar_bot(const Bot* bot, ALLEGRO_COLOR cor, float camera_x, float camera_y) {
    if (!bot->ativo) return;

    float bot_x_tela = (bot->x - camera_x) * ZOOM_FACTOR;
    float bot_y_tela = (bot->y - camera_y) * ZOOM_FACTOR;

    /* Desenhar sprite se disponível */
    if (bot->usar_sprite && bot->sprite_animal) {
        desenhar_sprite_camera(
            bot->sprite_animal,
            bot->x,
            bot->y,
            camera_x,
            camera_y,
            bot->escala_sprite,
            ZOOM_FACTOR
        );
    }
    else {
        /* Fallback para círculo sem borda */
        al_draw_filled_circle(bot_x_tela, bot_y_tela, bot->raio * ZOOM_FACTOR, cor);
    }

    /* BARRA DE VIDA ESTILIZADA */
    float barra_largura = 40.0f * ZOOM_FACTOR;
    float barra_altura = 5.0f * ZOOM_FACTOR;
    float barra_x = bot_x_tela - barra_largura / 2.0f;
    float barra_y = bot_y_tela - (bot->raio + 20.0f) * ZOOM_FACTOR;

    /* Fundo da barra (sombra) */
    al_draw_filled_rounded_rectangle(
        barra_x - 2, barra_y - 2, 
        barra_x + barra_largura + 2, barra_y + barra_altura + 2,
        2, 2, al_map_rgba(0, 0, 0, 150)
    );

    /* Barra de fundo cinza */
    al_draw_filled_rounded_rectangle(
        barra_x, barra_y, 
        barra_x + barra_largura, barra_y + barra_altura,
        2, 2, al_map_rgb(60, 60, 60)
    );

    /* Barra de vida colorida com gradiente */
    float vida_proporcao = (float)bot->animal_data.nivel_alimentacao / 100.0f;
    ALLEGRO_COLOR cor_vida;
    
    if (vida_proporcao > 0.6f) {
        cor_vida = al_map_rgb(50, 200, 50);  /* Verde */
    } else if (vida_proporcao > 0.3f) {
        cor_vida = al_map_rgb(255, 200, 0);  /* Amarelo */
    } else {
        cor_vida = al_map_rgb(255, 50, 50);  /* Vermelho */
    }

    al_draw_filled_rounded_rectangle(
        barra_x, barra_y,
        barra_x + barra_largura * vida_proporcao, barra_y + barra_altura,
        2, 2, cor_vida
    );

    /* Borda branca */
    al_draw_rounded_rectangle(
        barra_x, barra_y, 
        barra_x + barra_largura, barra_y + barra_altura,
        2, 2, al_map_rgb(255, 255, 255), 1.0f
    );
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