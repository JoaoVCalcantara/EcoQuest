#pragma once
#ifndef INIMIGO_H
#define INIMIGO_H

#include <stdbool.h>
#include <allegro5/allegro.h>
#include "batalha.h"
#include "entidades.h"
#include "sprites.h"

// Tipo de área de restrição
typedef enum {
    AREA_NENHUMA,      // Sem restrição específica
    AREA_CIRCULAR,     // Área circular
    AREA_ELIPTICA      // Área elíptica
} TipoAreaRestrita;

// Estrutura para definir área de movimento
typedef struct {
    TipoAreaRestrita tipo;
    
    // Para área circular
    float centro_x, centro_y;
    float raio_area;
    
    // Para área elíptica
    float centro_elipse_x, centro_elipse_y;
    float raio_horizontal; // Semi-eixo horizontal (a)
    float raio_vertical;   // Semi-eixo vertical (b)
} AreaRestrita;

typedef struct {
    float x;
    float y;
    float raio;
    float velocidade;
    float direcao_x;
    float direcao_y;
    float tempo_mudanca;
    Animal animal_data;
    bool ativo;
    int cenario; // Em qual cenário o bot está

    Sprite* sprite_animal;
    bool usar_sprite;
    float escala_sprite;
    
    float cooldown_colisao; // Tempo restante de cooldown após colisão
    
    AreaRestrita area_restrita; // Área de movimento limitada

    const char* caminho_sprite_batalha; // Caminho do sprite para usar na batalha
} Bot;


void iniciar_bot(Bot* bot, float x, float y, const char* nome, const char* tipo, int cenario);
void iniciar_bot_com_sprite(Bot* bot, float x, float y, const char* nome, const char* tipo, int cenario, const char* caminho_sprite);

// Nova: inicializa bot com área circular restrita
void iniciar_bot_com_area_circular(Bot* bot, float x, float y, const char* nome, const char* tipo, int cenario, const char* caminho_sprite, float centro_x, float centro_y, float raio_area);

// Nova: inicializa bot com área elíptica restrita
void iniciar_bot_com_area_eliptica(Bot* bot, float x, float y, const char* nome, const char* tipo, int cenario, const char* caminho_sprite, float centro_x, float centro_y, float raio_h, float raio_v);

void atualizar_bot(Bot* bot, float largura_mapa, float altura_mapa, float delta_time);
void desenhar_bot(const Bot* bot, ALLEGRO_COLOR cor, float camera_x, float camera_y);
bool verificar_colisao_bot(const entidade* jogador, const Bot* bot);
void destruir_bot(Bot* bot);


#endif
