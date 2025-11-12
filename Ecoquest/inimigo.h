#pragma once
#ifndef INIMIGO_H
#define INIMIGO_H

#include <stdbool.h>
#include <allegro5/allegro.h>
#include "batalha.h"
#include "entidades.h"

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
} Bot;


void iniciar_bot(Bot* bot, float x, float y, const char* nome, const char* tipo, int cenario);
void iniciar_bot_com_sprite(Bot* bot, float x, float y, const char* nome, const char* tipo, int cenario, const char* caminho_sprite);
void atualizar_bot(Bot* bot, float largura_mapa, float altura_mapa, float delta_time);
void desenhar_bot(const Bot* bot, ALLEGRO_COLOR cor, float camera_x, float camera_y);
bool verificar_colisao_bot(const entidade* jogador, const Bot* bot);
void destruir_bot(Bot* bot);


#endif
