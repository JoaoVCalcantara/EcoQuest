#pragma once
#ifndef INIMIGO_H
#define INIMIGO_H

#include <allegro5/allegro.h>
#include <stdbool.h>
#include "batalha.h"
#include "sprites.h"
#include "cenario.h"

// ========== ENUMS DE ESTADOS DA IA ==========

typedef enum {
    ESTADO_PATRULHA,      // Movimento aleatório normal
    ESTADO_ALERTA,        // Detectou algo, mas não está fugindo ainda
    ESTADO_FUGA,          // Fugindo ativamente de ameaça
    ESTADO_ESCONDIDO,     // Parado, tentando não ser visto
    ESTADO_RETORNANDO     // Voltando para área segura após fuga
} EstadoIA;

typedef enum {
    AREA_NENHUMA,
    AREA_CIRCULAR,
    AREA_ELIPTICA
} TipoArea;

// ========== SISTEMA DE VISÃO ==========

typedef struct {
    float raio_visao;              // Distância que o animal pode "ver"
    float angulo_visao;            // Ângulo do cone de visão (em radianos)
    float direcao_olhar;           // Direção atual que está olhando (em radianos)
    
    // Detecção de ameaças
    bool ameaca_detectada;         // Se detectou alguma ameaça
    float ameaca_x, ameaca_y;      // Posição da última ameaça detectada
    float tempo_desde_deteccao;    // Quanto tempo desde que viu a ameaça
    float distancia_ameaca;        // Distância até a ameaça
    
    // Debug visual
    bool mostrar_debug;            // Desenhar cone de visão
} SistemaVisao;

// ========== ESTRUTURAS DE ÁREA ==========

typedef struct {
    TipoArea tipo;
    float centro_x, centro_y;
    float raio_area;
    float centro_elipse_x, centro_elipse_y;
    float raio_horizontal, raio_vertical;
} AreaRestrita;

// ========== ESTRUTURA PRINCIPAL DO BOT ==========

typedef struct {
    // Posição e física
    float x, y;
    float raio;
    float velocidade;
    float velocidade_base;         // Velocidade original (para resetar)
    float direcao_x, direcao_y;
    
    // Estado e timers
    EstadoIA estado_atual;
    float tempo_no_estado;         // Quanto tempo está neste estado
    float tempo_mudanca;           // Timer para mudança de direção
    bool ativo;
    int cenario;
    float cooldown_colisao;
    
    // Sistema de visão
    SistemaVisao visao;
    
    // Comportamento de fuga (legacy - integrado ao estado FUGA)
    bool fugindo;
    float fuga_timer;
    float fuga_velocidade_multiplicador;
    
    // Dados do animal
    Animal animal_data;
    
    // Vida visual no cenário (separada da alimentação)
    int vida_visual_atual;
    int vida_visual_maxima;
    
    // Sprites
    Sprite* sprite_animal;
    bool usar_sprite;
    float escala_sprite;
    const char* caminho_sprite_batalha;
    
    // Área restrita
    AreaRestrita area_restrita;
    
    // Paradas ocasionais (realismo)
    bool parado;
    float parado_timer;
    
    // Memória/histórico (para IA mais inteligente)
    float ultima_pos_segura_x, ultima_pos_segura_y;  // Último local seguro conhecido
    int tentativas_fuga;                              // Quantas vezes tentou fugir
} Bot;

// Forward declarations
typedef struct Cacador Cacador;
typedef struct CacadorChefe CacadorChefe;

// ========== FUNÇÕES DE INICIALIZAÇÃO ==========

void iniciar_bot(Bot* bot, float x, float y, const char* nome, const char* tipo, int cenario);
void iniciar_bot_com_sprite(Bot* bot, float x, float y, const char* nome, const char* tipo, int cenario, const char* caminho_sprite);
void iniciar_bot_com_area_circular(Bot* bot, float x, float y, const char* nome, const char* tipo, int cenario, const char* caminho_sprite, float centro_x, float centro_y, float raio_area);
void iniciar_bot_com_area_eliptica(Bot* bot, float x, float y, const char* nome, const char* tipo, JogoCenas cenario, const char* caminho_sprite, const char* fundo_batalha_path, float centro_x, float centro_y, float raio_h, float raio_v);
void iniciar_bot_com_sprite_e_fundo(Bot* bot, float x, float y, const char* nome,
    const char* tipo, int cenario,
    const char* caminho_sprite,
    const char* caminho_fundo_batalha);

// ========== FUNÇÕES DE VISÃO ==========

// Inicializa sistema de visão com valores padrão
void inicializar_sistema_visao(SistemaVisao* visao, float raio, float angulo);

// Verifica se um ponto está no campo de visão do bot
bool ponto_no_campo_visao(const Bot* bot, float px, float py);

// Atualiza detecção de ameaças (jogador, caçadores, etc)
void atualizar_deteccao_ameacas(Bot* bot, float ameaca_x, float ameaca_y, float delta_time);

// Desenha cone de visão (debug)
void desenhar_campo_visao(const Bot* bot, float camera_x, float camera_y);

// ========== FUNÇÕES DE ESTADO DA IA ==========

// Muda o estado da IA
void mudar_estado_ia(Bot* bot, EstadoIA novo_estado);

// Retorna nome do estado (para debug)
const char* nome_estado_ia(EstadoIA estado);

// Atualiza lógica baseada no estado atual
void atualizar_estado_ia(Bot* bot, float delta_time);

// ========== FUNÇÕES DE ATUALIZAÇÃO E DESENHO ==========

void atualizar_bot(Bot* bot, float largura_mapa, float altura_mapa, float delta_time);
void atualizar_bot_com_cacador(Bot* bot, Cacador* cacador, float largura_mapa, float altura_mapa, float delta_time);
void desenhar_bot(const Bot* bot, ALLEGRO_COLOR cor, float camera_x, float camera_y);

// ========== FUNÇÕES AUXILIARES ==========

bool verificar_colisao_bot(const entidade* jogador, const Bot* bot);
void destruir_bot(Bot* bot);

#endif