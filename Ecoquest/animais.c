#include "animais.h"
#include "cenario.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

#define ANIMAL_TAMANHO_PROP 0.04f  
#define VELOCIDADE_BASE 0.3f      

// Redimensiona bitmap para o tamanho desejado
static ALLEGRO_BITMAP* scale_bitmap(ALLEGRO_BITMAP* src, int w, int h) {
    if (!src) return NULL;
    ALLEGRO_BITMAP* old = al_get_target_bitmap();
    ALLEGRO_BITMAP* dst = al_create_bitmap(w, h);
    al_set_target_bitmap(dst);
    al_clear_to_color(al_map_rgba(0, 0, 0, 0));
    al_draw_scaled_bitmap(src, 0, 0,
        al_get_bitmap_width(src), al_get_bitmap_height(src),
        0, 0, w, h, 0);
    al_set_target_bitmap(old);
    return dst;
}

// Direção aleatória
static void dir_aleatoria(float* dx, float* dy) {
    float ang = ((float)rand() / (float)RAND_MAX) * 6.28318f;
    *dx = cosf(ang);
    *dy = sinf(ang);
    if (fabsf(*dx) < 0.001f && fabsf(*dy) < 0.001f) { *dx = 1; *dy = 0; }
}

static float clampf(float v, float a, float b) {
    return v < a ? a : (v > b ? b : v);
}

// Posiciona animais no bioma. O boto só aparece na água
static void posicionar_bioma(Animal* a, AllegroContext* ctx) {
    float margemX = a->largura * 0.5f;
    float margemY = a->altura * 0.5f;
    int tentativas = 0;

    while (tentativas < 200) {
        a->x = a->bx + margemX + (rand() % (int)fmaxf(1.0f, (a->bw - 2 * margemX)));
        a->y = a->by + margemY + (rand() % (int)fmaxf(1.0f, (a->bh - 2 * margemY)));

        // Se não for boto, qualquer ponto é válido
        if (strcmp(a->nome, "boto") != 0) break;

        // Verifica se é água
        if (ctx && ctx->mapa) {
            ALLEGRO_COLOR cor = al_get_pixel(ctx->mapa, (int)a->x, (int)a->y);
            unsigned char r, g, b;
            al_unmap_rgb(cor, &r, &g, &b);

            bool agua = (b > r + 20 && b > g + 20);
            if (agua) break; // posição aceita
        }
        tentativas++;
    }
}

// Inicializa os animais e define seus biomas
void init_animais(Animal animais[4], AllegroContext* ctx) {
    const int metadeW = ctx->metadelargura;
    const int metadeH = ctx->metadealtura;
    const int tamanho = (int)(ctx->largura * ANIMAL_TAMANHO_PROP);

    animais[0].nome = "onca";   animais[0].sprite_raw = ctx->onca;
    animais[1].nome = "jacare"; animais[1].sprite_raw = ctx->jacare;
    animais[2].nome = "boto";   animais[2].sprite_raw = ctx->boto;
    animais[3].nome = "raposa"; animais[3].sprite_raw = ctx->raposa;

    for (int i = 0; i < 4; i++) {
        animais[i].largura = tamanho;
        animais[i].altura = tamanho;
        animais[i].sprite_scaled = scale_bitmap(animais[i].sprite_raw, tamanho, tamanho);
        dir_aleatoria(&animais[i].vx, &animais[i].vy);
        animais[i].velocidade = VELOCIDADE_BASE;
        animais[i].cooldown_mudanca = 60 + rand() % 120;
    }

    // Biomas
    // Onça: canto superior esquerdo
    animais[0].bx = 0;         animais[0].by = 0;         animais[0].bw = metadeW; animais[0].bh = metadeH;
    // Jacaré: canto inferior esquerdo
    animais[1].bx = 0;         animais[1].by = metadeH;   animais[1].bw = metadeW; animais[1].bh = metadeH;
    // Boto: canto superior direito
    animais[2].bx = metadeW;   animais[2].by = 0;         animais[2].bw = metadeW; animais[2].bh = metadeH;
    // Raposa: canto inferior direito
    animais[3].bx = metadeW;   animais[3].by = metadeH;   animais[3].bw = metadeW; animais[3].bh = metadeH;

    for (int i = 0; i < 4; i++) {
        posicionar_bioma(&animais[i], ctx);
    }
}

// Atualiza o movimento dos animais
void update_animais(Animal animais[4], AllegroContext* ctx) {
    for (int i = 0; i < 4; i++) {
        Animal* a = &animais[i];
        if (!a->sprite_scaled) continue;

        if (--a->cooldown_mudanca <= 0) {
            dir_aleatoria(&a->vx, &a->vy);
            a->cooldown_mudanca = 60 + rand() % 120;
        }

        float nx = a->x + a->vx * a->velocidade;
        float ny = a->y + a->vy * a->velocidade;

        // O Boto não pode sair da água
        if (strcmp(a->nome, "boto") == 0 && ctx && ctx->mapa) {
            if (nx >= 0 && ny >= 0 && nx < ctx->largura && ny < ctx->altura) {
                ALLEGRO_COLOR cor = al_get_pixel(ctx->mapa, (int)nx, (int)ny);
                unsigned char r, g, b;
                al_unmap_rgb(cor, &r, &g, &b);

                bool agua = (b > r + 20 && b > g + 20);
                if (!agua) {
                    // Rebater direção e tentar novo ponto
                    a->vx = -a->vx;
                    a->vy = -a->vy;
                    a->cooldown_mudanca = 30 + rand() % 60;
                    continue;
                }
            }
        }

        float halfw = a->largura * 0.5f;
        float halfh = a->altura * 0.5f;
        float minx = a->bx + halfw;
        float maxx = a->bx + a->bw - halfw;
        float miny = a->by + halfh;
        float maxy = a->by + a->bh - halfh;

        if (nx < minx || nx > maxx) { a->vx = -a->vx; nx = clampf(nx, minx, maxx); }
        if (ny < miny || ny > maxy) { a->vy = -a->vy; ny = clampf(ny, miny, maxy); }

        a->x = nx;
        a->y = ny;
    }
}

// Desenha todos os animais
void draw_animais(const Animal animais[4]) {
    extern float camera_x, camera_y;

    for (int i = 0; i < 4; i++) {
        const Animal* a = &animais[i];
        if (!a->sprite_scaled) continue;

        float sx = (a->x - camera_x) * ZOOM_FACTOR;
        float sy = (a->y - camera_y) * ZOOM_FACTOR;

        al_draw_scaled_bitmap(
            a->sprite_scaled,
            0, 0, a->largura, a->altura,
            sx - (a->largura * ZOOM_FACTOR / 2),
            sy - (a->altura * ZOOM_FACTOR / 2),
            a->largura * ZOOM_FACTOR,
            a->altura * ZOOM_FACTOR,
            0
        );
    }
}