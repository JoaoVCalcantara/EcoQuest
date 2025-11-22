#pragma once
#ifndef BATALHA_H
#define BATALHA_H

#include <stdbool.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

#include "entidades.h"  // ← ADICIONAR ESTA LINHA

// Forward declarations - apenas para tipos que não foram incluídos
typedef struct Bestiario Bestiario;
typedef struct Cacador Cacador;

// Estruturas
typedef struct {
    int experiencia;
    int nivel_alimentacao;
    int nivel;
    bool alimentado;
    bool domado;
    bool estudado;
    const char* nome;
    const char* tipo;
    const char* caminho_fundo_batalha;
} Animal;

typedef struct {
    ALLEGRO_BITMAP* fundo_batalha;
    ALLEGRO_BITMAP* caixa_texto;
    ALLEGRO_BITMAP* sprite_animal;
    ALLEGRO_BITMAP* sprite_jogador;
    ALLEGRO_BITMAP* sprite_cacador;
} RecursosBatalha;

typedef struct {
    const char* nome_animal;
    float jogador_x;
    float jogador_y;
    float jogador_escala;
} ConfigSpriteJogador;

typedef struct {
    const char* nome_cacador;
    float cacador_x;
    float cacador_y;
    float cacador_escala;
} ConfigSpriteCacador;

// Funções principais - Batalha com Animais
void desenhar_menu_batalha(ALLEGRO_FONT* fonte, int opcao, ALLEGRO_DISPLAY* display, Animal* animal, RecursosBatalha* recursos);
void iniciar_batalha(ALLEGRO_FONT* fonte, Animal* animal, ALLEGRO_EVENT_QUEUE* event_queue, ALLEGRO_DISPLAY* display);
void iniciar_batalha_com_bestiario(ALLEGRO_FONT* fonte, Animal* animal, ALLEGRO_EVENT_QUEUE* event_queue, ALLEGRO_DISPLAY* display, Bestiario* bestiario);

// Funções principais - Batalha com Caçadores
void desenhar_menu_batalha_cacador(ALLEGRO_FONT* fonte, int opcao, ALLEGRO_DISPLAY* display, 
                                    Cacador* cacador, entidade* jogador, RecursosBatalha* recursos,
                                    int vida_jogador, bool turno_jogador);

void iniciar_batalha_cacador_visual(ALLEGRO_FONT* fonte, Cacador* cacador, 
                                     ALLEGRO_EVENT_QUEUE* event_queue, 
                                     ALLEGRO_DISPLAY* display, entidade* jogador);

// Funções auxiliares
RecursosBatalha* carregar_recursos_batalha(const char* caminho_fundo, const char* caminho_caixa_texto, 
                                           const char* caminho_sprite_animal, const char* caminho_sprite_jogador);

RecursosBatalha* carregar_recursos_batalha_cacador(const char* caminho_fundo, const char* caminho_caixa_texto, 
                                                     const char* caminho_sprite_cacador, const char* caminho_sprite_jogador);

void destruir_recursos_batalha(RecursosBatalha* recursos);

#endif