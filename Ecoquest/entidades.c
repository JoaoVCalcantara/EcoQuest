#include "entidades.h"
#include "cenario.h"
#include <allegro5/allegro_primitives.h>
#include <math.h>
#include <stdio.h>

void iniciar_entidade(entidade* p, float width, float height) {
    p->x = width / 2.0f;
    p->y = height / 2.0f;
    p->raio = PLAYER_RADIUS;
    p->velocidade = PLAYER_SPEED;
    p->direcao = 0;
    p->andando = false;
}

void desenhar_jogador(const entidade* p, const AllegroContext* ctx) {
    extern float camera_x, camera_y;

    float sx = floorf((p->x - camera_x) * ZOOM_FACTOR + 0.5f);
    float sy = floorf((p->y - camera_y) * ZOOM_FACTOR + 0.5f);

    ALLEGRO_BITMAP* sprite = NULL;

    // Escolhe sprite conforme direção e movimento
    switch (p->direcao) {
    case 0: sprite = p->andando ? ctx->heroi_andando_frente : ctx->heroi_frente; break;
    case 1: sprite = p->andando ? ctx->heroi_andando_costas : ctx->heroi_costas; break;
    case 2: sprite = p->andando ? ctx->heroi_andando_esquerda : ctx->heroi_esquerda; break;
    case 3: sprite = p->andando ? ctx->heroi_andando_direita : ctx->heroi_direita; break;
    }

    if (sprite) {
        int largura = al_get_bitmap_width(sprite);
        int altura = al_get_bitmap_height(sprite);
        float escala = 0.25f;

        al_draw_scaled_bitmap(sprite,
            0, 0, largura, altura,
            sx - (largura * escala / 2),
            sy - (altura * escala / 2),
            largura * escala,
            altura * escala,
            0);
    }
    else {
        fprintf(stderr, "Sprite não encontrado para direção %d!\n", p->direcao);
    }
}

void limitar_jogador(entidade* jogador, float largura_mapa, float altura_mapa) {
    if (jogador->x < jogador->raio) jogador->x = jogador->raio;
    if (jogador->y < jogador->raio) jogador->y = jogador->raio;
    if (jogador->x > largura_mapa - jogador->raio) jogador->x = largura_mapa - jogador->raio;
    if (jogador->y > altura_mapa - jogador->raio) jogador->y = altura_mapa - jogador->raio;
}

void processar_teclado(ALLEGRO_KEYBOARD_STATE* estado, entidade* jogador) {
    float dx = 0.0f, dy = 0.0f;
    jogador->andando = false;

    if (al_key_down(estado, ALLEGRO_KEY_W)) { dy -= 1.0f; jogador->direcao = 1; jogador->andando = true; }
    if (al_key_down(estado, ALLEGRO_KEY_S)) { dy += 1.0f; jogador->direcao = 0; jogador->andando = true; }
    if (al_key_down(estado, ALLEGRO_KEY_A)) { dx -= 1.0f; jogador->direcao = 2; jogador->andando = true; }
    if (al_key_down(estado, ALLEGRO_KEY_D)) { dx += 1.0f; jogador->direcao = 3; jogador->andando = true; }

    // movimento diagonal
    if (dx != 0 || dy != 0) {
        float len = sqrtf(dx * dx + dy * dy);
        dx /= len;
        dy /= len;

        // Atualiza a posição
        jogador->x += dx * jogador->velocidade;
        jogador->y += dy * jogador->velocidade;
    }
}


bool colisao(float cx, float cy, float r, float rx, float ry, float rw, float rh) {
    float closestX = fmaxf(rx, fminf(cx, rx + rw));
    float closestY = fmaxf(ry, fminf(cy, ry + rh));
    float dx = cx - closestX;
    float dy = cy - closestY;
    return (dx * dx + dy * dy) <= (r * r);
}