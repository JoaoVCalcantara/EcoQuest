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
}

void desenhar_jogador(const entidade* p, ALLEGRO_COLOR cor) {
    al_draw_filled_circle(p->x, p->y, p->raio,cor);
}

void limitar_jogador(entidade* p, float width, float height) {
    if (p->x < p->raio) p->x = p->raio;
    if (p->x > width - p->raio) p->x = width - p->raio;
    if (p->y < p->raio) p->y = p->raio;
    if (p->y > height - p->raio) p->y = height - p->raio;
}

void processar_teclado(ALLEGRO_KEYBOARD_STATE* estado, entidade* jogador) {
    float dx = 0.0f, dy = 0.0f;

    if (al_key_down(estado, ALLEGRO_KEY_W)) dy -= 1.0f;
    if (al_key_down(estado, ALLEGRO_KEY_S)) dy += 1.0f;
    if (al_key_down(estado, ALLEGRO_KEY_A)) dx -= 1.0f;
    if (al_key_down(estado, ALLEGRO_KEY_D)) dx += 1.0f;

    // MELHORIA: Normalizar o movimento para evitar velocidade maior na diagonal.
    if (dx != 0.0f || dy != 0.0f) {
        float magnitude = sqrt(dx * dx + dy * dy);
        jogador->x += (dx / magnitude) * jogador->velocidade;
        jogador->y += (dy / magnitude) * jogador->velocidade;
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