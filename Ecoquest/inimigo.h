#pragma once
#ifndef INIMIGO_H
#define INIMIGO_H

#include <allegro5/allegro.h>
#include <stdbool.h>
#include "batalha.h"
#include "sprites.h"
#include "cenario.h"

typedef enum {
    AREA_NENHUMA,
    AREA_CIRCULAR,
    AREA_ELIPTICA
} TipoArea;

typedef struct {
    TipoArea tipo;
    float centro_x, centro_y;
    float raio_area;
    float centro_elipse_x, centro_elipse_y;
    float raio_horizontal, raio_vertical;
} AreaRestrita;

typedef struct {
    float x, y;
    float raio;
    float velocidade;
    float direcao_x, direcao_y;
    float tempo_mudanca;
    bool ativo;
    int cenario;
    float cooldown_colisao;

    // Comportamento de fuga
    bool fugindo;
    float fuga_timer;
    float fuga_velocidade_multiplicador;

    Animal animal_data;

    // NOVO: Barra de vida visual no cenário (separada da alimentação)
    int vida_visual_atual;
    int vida_visual_maxima;

    Sprite* sprite_animal;
    bool usar_sprite;
    float escala_sprite;
    const char* caminho_sprite_batalha;

    AreaRestrita area_restrita;
} Bot;

// Forward declarations (definições completas estão em cacador.h)
typedef struct Cacador Cacador;
typedef struct CacadorChefe CacadorChefe;

// ========== FUNÇÕES BOT ==========

void iniciar_bot(Bot* bot, float x, float y, const char* nome, const char* tipo, int cenario);
void iniciar_bot_com_sprite(Bot* bot, float x, float y, const char* nome, const char* tipo, int cenario, const char* caminho_sprite);
void iniciar_bot_com_area_circular(Bot* bot, float x, float y, const char* nome, const char* tipo, int cenario, const char* caminho_sprite, float centro_x, float centro_y, float raio_area);
void iniciar_bot_com_area_eliptica(Bot* bot, float x, float y, const char* nome, const char* tipo, JogoCenas cenario, const char* caminho_sprite, const char* fundo_batalha_path, float centro_x, float centro_y, float raio_h, float raio_v);
void iniciar_bot_com_sprite_e_fundo(Bot* bot, float x, float y, const char* nome,
    const char* tipo, int cenario,
    const char* caminho_sprite,
    const char* caminho_fundo_batalha);

void atualizar_bot(Bot* bot, float largura_mapa, float altura_mapa, float delta_time);
void atualizar_bot_com_cacador(Bot* bot, Cacador* cacador, float largura_mapa, float altura_mapa, float delta_time);
void desenhar_bot(const Bot* bot, ALLEGRO_COLOR cor, float camera_x, float camera_y);
bool verificar_colisao_bot(const entidade* jogador, const Bot* bot);
void destruir_bot(Bot* bot);

#endif