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
} Bot;

// Inicializa um bot em uma posição específica
void iniciar_bot(Bot* bot, float x, float y, const char* nome, const char* tipo, int cenario);

// Atualiza o movimento do bot (IA simples)
void atualizar_bot(Bot* bot, float largura_mapa, float altura_mapa, float delta_time);

// Desenha o bot na tela
void desenhar_bot(const Bot* bot, ALLEGRO_COLOR cor, float camera_x, float camera_y);

// Verifica colisão entre jogador e bot
bool verificar_colisao_bot(const entidade* jogador, const Bot* bot);

#endif
