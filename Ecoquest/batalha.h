#pragma once
#ifndef BATALHA_H
#define BATALHA_H

#include <stdbool.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include "entidades.h"

// Forward declarations
typedef struct Bestiario Bestiario;
typedef struct Cacador Cacador;

// Agora vida separada de alimentação
typedef struct {
    int experiencia;            // Progresso de estudo
    int nivel_alimentacao;      // Nível de alimentação (0–100) usado para domar
    int nivel;                  // (se já usado por você)
    int vida_atual;             // Vida atual independente da alimentação
    int vida_maxima;            // Vida máxima
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
    const char* nome_entidade;
    float entidade_x;
    float entidade_y;
    float entidade_escala;
} ConfigSpriteEntidade;

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

// NOVA: Enum para tipo de batalha
typedef enum {
    BATALHA_ANIMAL,
    BATALHA_CACADOR
} TipoBatalha;

// NOVA: Estrutura unificada de oponente
typedef struct {
    TipoBatalha tipo;
    
    // Dados comuns
    const char* nome;
    int vida_atual;
    int vida_maxima;
    
    // Ponteiros para dados específicos
    Animal* animal;
    Cacador* cacador;
    
    // Configuração visual
    ALLEGRO_BITMAP* sprite;
    const char* caminho_fundo;
    ConfigSpriteEntidade config_sprite;
} OponenteBatalha;

// Funções principais - Animais (mantidas para compatibilidade)
void desenhar_menu_batalha(ALLEGRO_FONT* fonte, int opcao, ALLEGRO_DISPLAY* display, Animal* animal, RecursosBatalha* recursos);
void iniciar_batalha(ALLEGRO_FONT* fonte, Animal* animal, ALLEGRO_EVENT_QUEUE* event_queue, ALLEGRO_DISPLAY* display);
void iniciar_batalha_com_bestiario(ALLEGRO_FONT* fonte, Animal* animal, ALLEGRO_EVENT_QUEUE* event_queue, ALLEGRO_DISPLAY* display, Bestiario* bestiario);

// Funções principais - Caçadores (mantidas para compatibilidade)
void desenhar_menu_batalha_cacador(ALLEGRO_FONT* fonte, int opcao, ALLEGRO_DISPLAY* display,
                                   Cacador* cacador, entidade* jogador, RecursosBatalha* recursos,
                                   int vida_jogador, bool turno_jogador);
void iniciar_batalha_cacador_visual(ALLEGRO_FONT* fonte, Cacador* cacador,
                                    ALLEGRO_EVENT_QUEUE* event_queue,
                                    ALLEGRO_DISPLAY* display, entidade* jogador);

// NOVAS: Funções unificadas
OponenteBatalha* criar_oponente_animal(Animal* animal);
OponenteBatalha* criar_oponente_cacador(Cacador* cacador);
void destruir_oponente_batalha(OponenteBatalha* oponente);

void desenhar_batalha_unificada(ALLEGRO_FONT* fonte, int opcao, ALLEGRO_DISPLAY* display,
                               OponenteBatalha* oponente, entidade* jogador,
                               RecursosBatalha* recursos, int vida_jogador, bool turno_jogador);
                               
void iniciar_batalha_unificada(ALLEGRO_FONT* fonte, OponenteBatalha* oponente,
                              ALLEGRO_EVENT_QUEUE* event_queue, ALLEGRO_DISPLAY* display,
                              entidade* jogador, Bestiario* bestiario);

// Recursos
RecursosBatalha* carregar_recursos_batalha(const char* caminho_fundo, const char* caminho_caixa_texto,
                                           const char* caminho_sprite_animal, const char* caminho_sprite_jogador);
RecursosBatalha* carregar_recursos_batalha_cacador(const char* caminho_fundo, const char* caminho_caixa_texto,
                                                   const char* caminho_sprite_cacador, const char* caminho_sprite_jogador);
RecursosBatalha* carregar_recursos_batalha_unificada(const char* caminho_fundo, const char* caminho_caixa_texto,
                                                     const char* caminho_sprite_oponente, const char* caminho_sprite_jogador);
void destruir_recursos_batalha(RecursosBatalha* recursos);

#endif