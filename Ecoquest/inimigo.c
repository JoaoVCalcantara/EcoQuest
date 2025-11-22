#include "inimigo.h"
#include "cenario.h"
#include "sprites.h"
#include <allegro5/allegro_primitives.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
// time.h removido se não usado aqui

// ... (As funções iniciar_bot permanecem iguais) ...
// COPIE AS FUNÇÕES DE INICIALIZAÇÃO DO SEU CÓDIGO ORIGINAL AQUI
// APENAS A FUNÇÃO atualizar_bot PRECISA SER SUBSTITUÍDA ABAIXO

void iniciar_bot(Bot* bot, float x, float y, const char* nome, const char* tipo, int cenario) {
    bot->x = x;
    bot->y = y;
    bot->raio = 8.0f;
    bot->velocidade = 0.5f;
    bot->direcao_x = 1.0f;
    bot->direcao_y = 0.0f;
    bot->tempo_mudanca = 0.0f;
    bot->ativo = true;
    bot->cenario = cenario;
    bot->cooldown_colisao = 0.0f;

    bot->animal_data.experiencia = 0;
    bot->animal_data.nivel = 1;
    bot->animal_data.nivel_alimentacao = 0;
    bot->animal_data.domado = false;
    bot->animal_data.estudado = false;
    bot->animal_data.alimentado = false;
    bot->animal_data.nome = nome;
    bot->animal_data.tipo = tipo;
    bot->animal_data.caminho_fundo_batalha = NULL; // Inicialização segura

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
    // Reutiliza lógica para evitar duplicação
    iniciar_bot_com_sprite(bot, x, y, nome, tipo, cenario, sprite_path);
    bot->animal_data.caminho_fundo_batalha = fundo_batalha_path;
}

// Verifica se a posição está dentro da área permitida
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

void atualizar_bot(Bot* bot, float largura_mapa, float altura_mapa, float delta_time) {
    if (!bot->ativo) return;

    if (bot->cooldown_colisao > 0.0f) {
        bot->cooldown_colisao -= delta_time;
        if (bot->cooldown_colisao < 0.0f) bot->cooldown_colisao = 0.0f;
    }

    bot->tempo_mudanca += delta_time;

    if (bot->tempo_mudanca >= 3.0f) {
        float angulo = ((float)rand() / RAND_MAX) * 2.0f * 3.14159f;
        bot->direcao_x = cosf(angulo);
        bot->direcao_y = sinf(angulo);
        bot->tempo_mudanca = 0.0f;
    }

    float novo_x = bot->x + bot->direcao_x * bot->velocidade;
    float novo_y = bot->y + bot->direcao_y * bot->velocidade;

    if (bot->area_restrita.tipo != AREA_NENHUMA) {
        if (posicao_valida(bot, novo_x, novo_y)) {
            bot->x = novo_x;
            bot->y = novo_y;
        }
        else {
            bot->direcao_x = -bot->direcao_x;
            bot->direcao_y = -bot->direcao_y;
            bot->tempo_mudanca = 2.5f;
        }
        return;
    }

    // CORREÇÃO AQUI: Inicializar valores padrão para evitar aviso
    float min_x = 0.0f, max_x = largura_mapa;
    float min_y = 0.0f, max_y = altura_mapa;

    switch (bot->cenario) {
    case 0: // CENARIO1
        max_x = 640.0f; max_y = 360.0f; break;
    case 1: // CENARIO2
        min_y = 360.0f; max_x = 640.0f; break;
    case 2: // CENARIO3
        min_x = 640.0f; max_y = 360.0f; break;
    case 3: // CENARIO4
        min_x = 640.0f; min_y = 360.0f; break;
    default: // Fallback seguro
        break;
    }

    if (novo_x >= min_x + bot->raio && novo_x <= max_x - bot->raio) {
        bot->x = novo_x;
    }
    else {
        bot->direcao_x = -bot->direcao_x;
    }

    if (novo_y >= min_y + bot->raio && novo_y <= max_y - bot->raio) {
        bot->y = novo_y;
    }
    else {
        bot->direcao_y = -bot->direcao_y;
    }
}

void desenhar_bot(const Bot* bot, ALLEGRO_COLOR cor, float camera_x, float camera_y) {
    if (!bot->ativo) return;

    float bot_x_tela = (bot->x - camera_x) * ZOOM_FACTOR;
    float bot_y_tela = (bot->y - camera_y) * ZOOM_FACTOR;

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
        return;
    }

    al_draw_filled_circle(bot_x_tela, bot_y_tela, bot->raio * ZOOM_FACTOR, cor);
    al_draw_circle(bot_x_tela, bot_y_tela, bot->raio * ZOOM_FACTOR, al_map_rgb(255, 255, 255), 2.0f);
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