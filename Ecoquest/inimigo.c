#include "inimigo.h"
#include "cenario.h"
#include <allegro5/allegro_primitives.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

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
    
    bot->animal_data.experiencia = 0;
    bot->animal_data.nivel = 1;
    bot->animal_data.alimentado = false;
    bot->animal_data.nome = nome;
    bot->animal_data.tipo = tipo;
}

void atualizar_bot(Bot* bot, float largura_mapa, float altura_mapa, float delta_time) {
    if (!bot->ativo) return;
    
    bot->tempo_mudanca += delta_time;
    
    // Muda direção a cada 3 segundos
    if (bot->tempo_mudanca >= 3.0f) {
        float angulo = ((float)rand() / RAND_MAX) * 2.0f * 3.14159f;
        bot->direcao_x = cosf(angulo);
        bot->direcao_y = sinf(angulo);
        bot->tempo_mudanca = 0.0f;
    }
    
    // Move o bot
    float novo_x = bot->x + bot->direcao_x * bot->velocidade;
    float novo_y = bot->y + bot->direcao_y * bot->velocidade;
    
    // Limita dentro do mapa
    if (novo_x >= bot->raio && novo_x <= largura_mapa - bot->raio) {
        bot->x = novo_x;
    } else {
        bot->direcao_x = -bot->direcao_x;
    }
    
    if (novo_y >= bot->raio && novo_y <= altura_mapa - bot->raio) {
        bot->y = novo_y;
    } else {
        bot->direcao_y = -bot->direcao_y;
    }
}

void desenhar_bot(const Bot* bot, ALLEGRO_COLOR cor, float camera_x, float camera_y) {
    if (!bot->ativo) return;
    
    float bot_x_tela = (bot->x - camera_x) * ZOOM_FACTOR;
    float bot_y_tela = (bot->y - camera_y) * ZOOM_FACTOR;
    
    al_draw_filled_circle(bot_x_tela, bot_y_tela, bot->raio * ZOOM_FACTOR, cor);
    al_draw_circle(bot_x_tela, bot_y_tela, bot->raio * ZOOM_FACTOR, al_map_rgb(255, 255, 255), 2.0f);
}

bool verificar_colisao_bot(const entidade* jogador, const Bot* bot) {
    if (!bot->ativo) return false;
    
    float dx = jogador->x - bot->x;
    float dy = jogador->y - bot->y;
    float distancia = sqrtf(dx * dx + dy * dy);
    
    return distancia <= (jogador->raio + bot->raio);
}