#pragma once
#ifndef CACADOR_H
#define CACADOR_H

#include <stdbool.h>
#include <allegro5/allegro.h>
#include "cenario.h"
#include "sprites.h"
#include "entidades.h"  // Inclui a definição completa
#include "inimigo.h"
#include "batalha.h"

// Forward declarations
typedef struct Bestiario Bestiario;
// NÃO declarar entidade aqui, já está em entidades.h

// Estados do caçador
typedef enum {
    CACADOR_PATRULHANDO,
    CACADOR_PERSEGUINDO,
    CACADOR_ESPERANDO,
    CACADOR_ATACANDO
} EstadoCacador;

// Dados do caçador para batalha
typedef struct {
    const char* nome;
    int vida_maxima;
    int vida_atual;
    int ataque_base;
    int defesa_base;
    bool derrotado;
    int vezes_derrotado;
    const char* caminho_sprite;
    const char* caminho_fundo_batalha;
} DadosCacador;

// Estrutura principal do caçador
typedef struct Cacador {
    float x, y;
    float raio;
    float velocidade;
    float velocidade_perseguicao;

    EstadoCacador estado;
    JogoCenas cenario;

    // Alvo e patrulha
    Bot* alvo;
    float alvo_x, alvo_y;
    float patrulha_x, patrulha_y;
    float tempo_espera;

    bool ativo;
    bool derrotado;

    float raio_deteccao;
    float raio_abandono;
    float distancia_deteccao;

    DadosCacador dados_batalha;

    // Atalhos para compatibilidade
    int vida;
    int vida_maxima;
    int dano_ataque;
    const char* nome;

    Sprite* sprite_cacador;
    bool usar_sprite;
    float escala_sprite;

    float cooldown_batalha;
    
    // Sistema de dano contínuo ao animal
    float cooldown_dano;
    float tempo_dano_interval;
    int dano_por_contato;
} Cacador;

// Caçador chefe (boss final)
typedef struct CacadorChefe {
    Cacador base;
    bool desbloqueado;
    bool liberado;
    bool derrotado;
    float x_spawn, y_spawn;
    JogoCenas cenario_spawn;
} CacadorChefe;

// ========== FUNÇÕES DE INICIALIZAÇÃO ==========

void iniciar_cacador(Cacador* cacador, float x, float y, const char* nome,
    JogoCenas cenario, const char* sprite_path,
    const char* fundo_batalha_path);

void iniciar_cacador_com_alvo(Cacador* cacador, float x, float y,
    JogoCenas cenario, Bot* alvo, const char* nome,
    const char* sprite_path);

void iniciar_cacador_chefe(CacadorChefe* chefe, float x, float y,
    JogoCenas cenario, const char* sprite_path,
    const char* fundo_batalha_path);

// ========== FUNÇÕES DE ATUALIZAÇÃO ==========

void atualizar_cacador(Cacador* cacador, float animal_x, float animal_y,
    bool animal_ativo, float delta_time,
    float largura_mapa, float altura_mapa);

void atualizar_cacador_chefe(CacadorChefe* chefe, entidade* jogador,
    float largura_mapa, float altura_mapa, float delta_time);

void cacador_perseguir_alvo(Cacador* cacador, float alvo_x, float alvo_y,
    float delta_time, float largura_mapa, float altura_mapa);

void cacador_patrulhar(Cacador* cacador, float delta_time,
    float largura_mapa, float altura_mapa);

// ========== FUNÇÕES DE DESENHO ==========

void desenhar_cacador(const Cacador* cacador, ALLEGRO_COLOR cor,
    float camera_x, float camera_y);

void desenhar_menu_batalha_cacador(ALLEGRO_FONT* fonte, int opcao, ALLEGRO_DISPLAY* display, 
                                    Cacador* cacador, entidade* jogador, RecursosBatalha* recursos,
                                    int vida_jogador, bool turno_jogador);


// ========== FUNÇÕES DE COLISÃO ==========

bool verificar_colisao_cacador_jogador(entidade* jogador, const Cacador* cacador);
bool verificar_colisao_cacador_animal(const Cacador* cacador, const Bot* bot);
bool verificar_deteccao_animal(const Cacador* cacador, float animal_x, float animal_y);

// ========== FUNÇÕES DE DANO ==========

void aplicar_dano_cacador_em_animal(Cacador* cacador, Bot* bot, float delta_time);

// ========== FUNÇÕES DE VERIFICAÇÃO ==========

bool verificar_desbloqueio_chefe(Cacador cacadores[], int total_cacadores);
bool verificar_liberacao_boss(int cacadores_derrotados);

// ========== LIMPEZA ==========

void destruir_cacador(Cacador* cacador);

#endif // CACADOR_H