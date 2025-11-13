#pragma once
#ifndef BATALHA_H
#define BATALHA_H

#include <stdbool.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

typedef struct {
    int experiencia;        // XP de estudo (0-100)
    int nivel_alimentacao;  // Nível de alimentação (0-100)
    int nivel;
    bool alimentado;        //True quando nivel_alimentacao >= 100
    bool domado;            // True quando alimentado pela primeira vez
    bool estudado;          // True quando experiencia >= 100
    const char* nome;
    const char* tipo;
    const char* caminho_fundo_batalha;  // Caminho da imagem de fundo para batalha
} Animal;

// Estrutura para gerenciar recursos visuais da batalha
typedef struct {
    ALLEGRO_BITMAP* fundo_batalha;
    ALLEGRO_BITMAP* caixa_texto;
    ALLEGRO_BITMAP* sprite_animal;
    ALLEGRO_BITMAP* sprite_jogador;  // NOVO: Sprite do jogador
} RecursosBatalha;

// Configuração de posição do sprite do jogador por animal
typedef struct {
    const char* nome_animal;
    float jogador_x;
    float jogador_y;
    float jogador_escala;
} ConfigSpriteJogador;

// Funções principais
void desenhar_menu_batalha(ALLEGRO_FONT* fonte, int opcao, ALLEGRO_DISPLAY* display, Animal* animal, RecursosBatalha* recursos);
void iniciar_batalha(ALLEGRO_FONT* fonte, Animal* animal, ALLEGRO_EVENT_QUEUE* event_queue, ALLEGRO_DISPLAY* display);

// Funções auxiliares para gerenciar recursos
RecursosBatalha* carregar_recursos_batalha(const char* caminho_fundo, const char* caminho_caixa_texto, const char* caminho_sprite_animal, const char* caminho_sprite_jogador);
void destruir_recursos_batalha(RecursosBatalha* recursos);

#endif