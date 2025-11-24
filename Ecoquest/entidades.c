#include "entidades.h"
#include "cenario.h"
#include "config_jogo.h"  // ADICIONAR
#include <allegro5/allegro_primitives.h>
#include <allegro5/keyboard.h>
#include <math.h>

void iniciar_entidade(entidade* p, float width, float height) {
    p->x = 0.0f;
    p->y = 0.0f;
    p->raio = RAIO_JOGADOR;           // SUBSTITUIR PLAYER_RADIUS
    p->velocidade = VELOCIDADE_JOGADOR; // SUBSTITUIR PLAYER_SPEED

    p->sprite_cima = NULL;
    p->sprite_baixo = NULL;
    p->sprite_esquerda = NULL;
    p->sprite_direita = NULL;

    p->sprite_idle_cima = NULL;
    p->sprite_idle_baixo = NULL;
    p->sprite_idle_esquerda = NULL;
    p->sprite_idle_direita = NULL;

    p->Direcao_atual = DIRECAO_BAIXO;
    p->movendo = false;
    p->usar_sprite = false;
    
    // Inicializar progresso do jogo
    p->progresso.cenario1_completo = false;
    p->progresso.cenario2_completo = false;
    p->progresso.cenario3_completo = false;
    p->progresso.cenario4_completo = false;
}

void desenhar_jogador(const entidade* p, ALLEGRO_COLOR cor) {
    extern float camera_x, camera_y;

    float jogador_x_tela = (p->x - camera_x) * ZOOM_FACTOR;
    float jogador_y_tela = (p->y - camera_y) * ZOOM_FACTOR;

    if (p->usar_sprite) {
        SpriteAnimadoFrameAFrame* sprite_atual = NULL;

        if (p->movendo) {
            switch (p->Direcao_atual) {
            case DIRECAO_CIMA:
                sprite_atual = p->sprite_cima;
                break;
            case DIRECAO_BAIXO:
                sprite_atual = p->sprite_baixo;
                break;
            case DIRECAO_ESQ:
                sprite_atual = p->sprite_esquerda;
                break;
            case DIRECAO_DIR:
                sprite_atual = p->sprite_direita;
                break;
            }
        }
        else {
            // Estado IDLE - com fallback para frame 0 do movimento
            switch (p->Direcao_atual) {
            case DIRECAO_CIMA:
                sprite_atual = p->sprite_idle_cima;
                // FALLBACK: Se idle_cima não existe, usa frame 0 do movimento
                if (!sprite_animado_frames_valido(sprite_atual)) {
                    sprite_atual = p->sprite_cima;
                }
                break;
            case DIRECAO_BAIXO:
                sprite_atual = p->sprite_idle_baixo;
                // FALLBACK: Se idle_baixo não existe, usa frame 0 do movimento
                if (!sprite_animado_frames_valido(sprite_atual)) {
                    sprite_atual = p->sprite_baixo;
                }
                break;
            case DIRECAO_ESQ:
                sprite_atual = p->sprite_idle_esquerda;
                // FALLBACK: Se idle_esquerda não existe, usa frame 0 do movimento
                if (!sprite_animado_frames_valido(sprite_atual)) {
                    sprite_atual = p->sprite_esquerda;
                }
                break;
            case DIRECAO_DIR:
                sprite_atual = p->sprite_idle_direita;
                // FALLBACK: Se idle_direita não existe, usa frame 0 do movimento
                if (!sprite_animado_frames_valido(sprite_atual)) {
                    sprite_atual = p->sprite_direita;
                }
                break;
            }
        }

        if (sprite_animado_frames_valido(sprite_atual)) {
            desenhar_sprite_animado_frames_camera(
                sprite_atual,
                p->x,
                p->y,
                camera_x,
                camera_y,
                0.06f,
                ZOOM_FACTOR
            );
            return;
        }
    }

    al_draw_filled_circle(jogador_x_tela, jogador_y_tela, p->raio, cor);
}

void limitar_jogador(entidade* jogador, float largura_mapa, float altura_mapa) {
    if (jogador->x < jogador->raio) jogador->x = jogador->raio;
    if (jogador->y < jogador->raio) jogador->y = jogador->raio;
    if (jogador->x > largura_mapa - jogador->raio) jogador->x = largura_mapa - jogador->raio;
    if (jogador->y > altura_mapa - jogador->raio) jogador->y = altura_mapa - jogador->raio;
}

void limitar_jogador_com_progresso(entidade* jogador, float largura_mapa, float altura_mapa, ProgressoJogo* progresso) {
    // Limites básicos do mapa
    if (jogador->x < jogador->raio) jogador->x = jogador->raio;
    if (jogador->y < jogador->raio) jogador->y = jogador->raio;
    if (jogador->x > largura_mapa - jogador->raio) jogador->x = largura_mapa - jogador->raio;
    if (jogador->y > altura_mapa - jogador->raio) jogador->y = altura_mapa - jogador->raio;
    
    // SUBSTITUIR valores hardcoded por constantes
    if (!progresso->cenario1_completo) {
        if (jogador->y > LIMITE_VERTICAL_MAPA - jogador->raio && jogador->x < LIMITE_HORIZONTAL_MAPA) {
            jogador->y = LIMITE_VERTICAL_MAPA - jogador->raio;
        }
        
        if (jogador->x > LIMITE_HORIZONTAL_MAPA - jogador->raio && jogador->y < LIMITE_VERTICAL_MAPA) {
            jogador->x = LIMITE_HORIZONTAL_MAPA - jogador->raio;
        }
        
        if (jogador->x > LIMITE_HORIZONTAL_MAPA - jogador->raio && jogador->y > LIMITE_VERTICAL_MAPA) {
            jogador->x = LIMITE_HORIZONTAL_MAPA - jogador->raio;
        }
    }
    
    else if (!progresso->cenario2_completo) {
        if (jogador->x > LIMITE_HORIZONTAL_MAPA - jogador->raio && jogador->y < LIMITE_VERTICAL_MAPA) {
            jogador->x = LIMITE_HORIZONTAL_MAPA - jogador->raio;
        }
        
        if (jogador->x > LIMITE_HORIZONTAL_MAPA - jogador->raio && jogador->y > LIMITE_VERTICAL_MAPA) {
            jogador->x = LIMITE_HORIZONTAL_MAPA - jogador->raio;
        }
    }
    
    else if (!progresso->cenario3_completo) {
        if (jogador->x > LIMITE_HORIZONTAL_MAPA && jogador->y > LIMITE_VERTICAL_MAPA - jogador->raio && jogador->y < LIMITE_VERTICAL_MAPA + 50.0f) {
            jogador->y = LIMITE_VERTICAL_MAPA - jogador->raio;
        }
        
        if (jogador->y > LIMITE_VERTICAL_MAPA && jogador->x > LIMITE_HORIZONTAL_MAPA - jogador->raio && jogador->x < LIMITE_HORIZONTAL_MAPA + 50.0f) {
            jogador->x = LIMITE_HORIZONTAL_MAPA - jogador->raio;
        }
    }
}

void processar_teclado(ALLEGRO_KEYBOARD_STATE* estado, entidade* jogador) {
    float dx = 0.0f, dy = 0.0f;

    bool tecla_w = al_key_down(estado, ALLEGRO_KEY_W);
    bool tecla_s = al_key_down(estado, ALLEGRO_KEY_S);
    bool tecla_a = al_key_down(estado, ALLEGRO_KEY_A);
    bool tecla_d = al_key_down(estado, ALLEGRO_KEY_D);

    if (tecla_w) dy -= 1.0f;
    if (tecla_s) dy += 1.0f;
    if (tecla_a) dx -= 1.0f;
    if (tecla_d) dx += 1.0f;

    if (dx != 0.0f || dy != 0.0f) {
        jogador->movendo = true;

        if (fabsf(dy) > fabsf(dx)) {
            jogador->Direcao_atual = (dy < 0) ? DIRECAO_CIMA : DIRECAO_BAIXO;
        }
        else {
            jogador->Direcao_atual = (dx < 0) ? DIRECAO_ESQ : DIRECAO_DIR;
        }

        float magnitude = sqrtf(dx * dx + dy * dy);
        if (magnitude > 0) {
            jogador->x += (dx / magnitude) * jogador->velocidade;
            jogador->y += (dy / magnitude) * jogador->velocidade;
        }
    }
    else {
        jogador->movendo = false;
    }
}

bool colisao(float cx, float cy, float r, float rx, float ry, float rw, float rh) {
    float closestX = fmaxf(rx, fminf(cx, rx + rw));
    float closestY = fmaxf(ry, fminf(cy, ry + rh));

    float dx = cx - closestX;
    float dy = cy - closestY;

    return (dx * dx + dy * dy) <= (r * r);
}

void destruir_entidade(entidade* p) {
    if (p->sprite_cima) destruir_sprite_animado_frames(p->sprite_cima);
    if (p->sprite_baixo) destruir_sprite_animado_frames(p->sprite_baixo);
    if (p->sprite_direita) destruir_sprite_animado_frames(p->sprite_direita);
    if (p->sprite_esquerda) destruir_sprite_animado_frames(p->sprite_esquerda);

    if (p->sprite_idle_cima) destruir_sprite_animado_frames(p->sprite_idle_cima);
    if (p->sprite_idle_baixo) destruir_sprite_animado_frames(p->sprite_idle_baixo);
    if (p->sprite_idle_direita) destruir_sprite_animado_frames(p->sprite_idle_direita);
    if (p->sprite_idle_esquerda) destruir_sprite_animado_frames(p->sprite_idle_esquerda);
}