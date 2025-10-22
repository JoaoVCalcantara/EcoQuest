#include "entidades.h"
#include "cenario.h"
#include "allegro_init.h"
#include <allegro5/allegro_primitives.h>
#include <allegro5/keyboard.h>
#include <allegro5/allegro_color.h>
#include <math.h>

void iniciar_entidade(entidade* p, float width, float height) {
    p->x = 0.0f;
    p->y = 0.0f;
    p->raio = PLAYER_RADIUS;
    p->velocidade = PLAYER_SPEED;
}

void desenhar_jogador(const entidade* p, ALLEGRO_COLOR cor) {
    extern float camera_x, camera_y;
    
    float jogador_x_tela = (p->x - camera_x) * ZOOM_FACTOR;
    float jogador_y_tela = (p->y - camera_y) * ZOOM_FACTOR;
    
    al_draw_filled_circle(jogador_x_tela, jogador_y_tela, p->raio, cor);
}

void limitar_jogador(entidade* jogador, float largura_mapa, float altura_mapa) {
    if (jogador->x < jogador->raio) jogador->x = jogador->raio;
    if (jogador->y < jogador->raio) jogador->y = jogador->raio;
    if (jogador->x > largura_mapa - jogador->raio) jogador->x = largura_mapa - jogador->raio;
    if (jogador->y > altura_mapa - jogador->raio) jogador->y = altura_mapa - jogador->raio;
}

void processar_teclado(ALLEGRO_KEYBOARD_STATE* estado, entidade* jogador) {
    float dx = 0.0f, dy = 0.0f;
    
    if (al_key_down(estado, ALLEGRO_KEY_W)) dy -= 1.0f;
    if (al_key_down(estado, ALLEGRO_KEY_S)) dy += 1.0f;
    if (al_key_down(estado, ALLEGRO_KEY_A)) dx -= 1.0f;
    if (al_key_down(estado, ALLEGRO_KEY_D)) dx += 1.0f;

    if (dx != 0.0f || dy != 0.0f) {
        float magnitude = sqrt(dx * dx + dy * dy);
        jogador->x += (dx / magnitude) * jogador->velocidade;
        jogador->y += (dy / magnitude) * jogador->velocidade;
    }
}

bool colisao(float cx, float cy, float r, float rx, float ry, float rw, float rh) {
    float closestX = fmax(rx, fmin(cx, rx + rw));
    float closestY = fmax(ry, fmin(cy, ry + rh));
    float dx = cx - closestX;
    float dy = cy - closestY;
    return (dx * dx + dy * dy) <= (r * r);
}




