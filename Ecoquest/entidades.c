#include "entidades.h"
#include "cenario.h"
#include "allegro_init.h" // CORREÇÃO: Necessário para LARGURA_TELA e ALTURA_TELA
#include <allegro5/allegro_primitives.h>
#include <allegro5/keyboard.h>
#include <allegro5/allegro_color.h>
#include <math.h>




void iniciar_entidade(entidade* p, float width, float height) {
    p->x = width / 2.0f;
    p->y = height / 2.0f;
    p->raio = PLAYER_RADIUS;
    p->velocidade = PLAYER_SPEED;

    p->sprite_cima = NULL;
    p->sprite_baixo = NULL;
    p->sprite_esquerda = NULL;
    p->sprite_direita = NULL;

    p->sprite_idle_cima = NULL;        // ← ADICIONAR
    p->sprite_idle_baixo = NULL;
    p->sprite_idle_esquerda = NULL;
    p->sprite_idle_direita = NULL;

	p->Direcao_atual = DIRECAO_BAIXO;
    p->movendo = false;
	p->usar_sprite = false;
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
            switch (p->Direcao_atual) {
            case DIRECAO_CIMA:          // ← ADICIONAR
                sprite_atual = p->sprite_idle_cima;
                break;
            case DIRECAO_BAIXO:
                sprite_atual = p->sprite_idle_baixo;
                break;
            case DIRECAO_ESQ:
                sprite_atual = p->sprite_idle_esquerda;
                break;
            case DIRECAO_DIR:
                sprite_atual = p->sprite_idle_direita;
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

void limitar_jogador(entidade* p, float width, float height) {
    if (p->x < p->raio) p->x = p->raio;
    if (p->x > width - p->raio) p->x = width - p->raio;
    if (p->y < p->raio) p->y = p->raio;
    if (p->y > height - p->raio) p->y = height - p->raio;
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

    // MELHORIA: Normalizar o movimento para evitar velocidade maior na diagonal.
    if (dx != 0.0f || dy != 0.0f) {
        jogador->movendo = true;

        // ========== ADICIONE ESTA PARTE ==========
        // Determina a direcao principal
        if (fabs(dy) > fabs(dx)) {
            // Movimento vertical predomina
            if (dy < 0) {
                jogador->Direcao_atual = DIRECAO_CIMA;
            }
            else {
                jogador->Direcao_atual = DIRECAO_BAIXO;
            }
        }
        else {
            // Movimento horizontal predomina
            if (dx < 0) {
                jogador->Direcao_atual = DIRECAO_ESQ;
            }
            else {
                jogador->Direcao_atual = DIRECAO_DIR;
            }
        }

        // Move o jogador (normaliza o vetor de movimento)
        float magnitude = sqrt(dx * dx + dy * dy);
        jogador->x += (dx / magnitude) * jogador->velocidade;
        jogador->y += (dy / magnitude) * jogador->velocidade;
        // ==========================================

    }
    else {
        // Nenhuma tecla pressionada
        jogador->movendo = false;
    }
}

//Reposiciona o jogador fora da porta ao sair de uma caverna.
void reposicionar_jogador_saida(entidade* jogador, const CavernaState* portas) {
    // Determina a direção da porta de saída e posiciona o jogador do lado oposto.
    if (portas->saidaX < jogador->raio) { // Porta no lado esquerdo (x pequeno)
        jogador->x = portas->comprimentoPorta + jogador->raio + 5;
        jogador->y = portas->saidaY + (portas->alturaPorta / 2);
    }
    else if (portas->saidaX > LARGURA_TELA / 2) { // Porta no lado direito (x grande)
        jogador->x = portas->saidaX - jogador->raio - 5;
        jogador->y = portas->saidaY + (portas->alturaPorta / 2);
    }
    else if (portas->saidaY < jogador->raio) { // Porta no topo (y pequeno)
        jogador->y = portas->alturaPorta + jogador->raio + 5;
        jogador->x = portas->saidaX + (portas->comprimentoPorta / 2);
    }
    else { // Porta na parte inferior (y grande)
        jogador->y = portas->saidaY - jogador->raio - 5;
        jogador->x = portas->saidaX + (portas->comprimentoPorta / 2);
    }
}

bool colisao(float cx, float cy, float r, float rx, float ry, float rw, float rh) {
    float closestX = fmax(rx, fmin(cx, rx + rw));
    float closestY = fmax(ry, fmin(cy, ry + rh));
    float dx = cx - closestX;
    float dy = cy - closestY;
    return (dx * dx + dy * dy) <= (r * r);
}

void destruir_entidade(entidade* p) {
    // Destruir sprites se necessário
    if (p->sprite_cima) destruir_sprite_animado_frames(p->sprite_cima);
    if (p->sprite_baixo) destruir_sprite_animado_frames(p->sprite_baixo);
    if (p->sprite_direita) destruir_sprite_animado_frames(p->sprite_direita);
    if (p->sprite_esquerda) destruir_sprite_animado_frames(p->sprite_esquerda);
    if (p->sprite_idle_cima) destruir_sprite_animado_frames(p->sprite_idle_cima);
    if (p->sprite_idle_baixo) destruir_sprite_animado_frames(p->sprite_idle_baixo);
    if (p->sprite_idle_direita) destruir_sprite_animado_frames(p->sprite_idle_direita);
    if (p->sprite_idle_esquerda) destruir_sprite_animado_frames(p->sprite_idle_esquerda);
}





