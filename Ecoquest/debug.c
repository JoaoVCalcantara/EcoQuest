#include "debug.h"
#include "cenario.h"
#include <stdio.h>

void inicializar_debug(DebugContext* debug, ALLEGRO_FONT* fonte) {
    if (!debug) return;

    debug->mostrar_hitboxes = false;
    debug->mostrar_areas_restritas = false;
    debug->mostrar_fps = true;
    debug->mostrar_info_bots = false;
    debug->mostrar_grid = false;
    debug->mostrar_coordenadas = false;

    debug->fps_atual = 0;
    debug->frame_count = 0;
    debug->tempo_acumulado = 0.0;

    debug->fonte_debug = fonte;

#if DEBUG_MODE
    printf("[DEBUG] Sistema de debug inicializado\n");
#endif
}

void destruir_debug(DebugContext* debug) {
    if (!debug) return;
#if DEBUG_MODE
    printf("[DEBUG] Sistema de debug finalizado\n");
#endif
}

void debug_desenhar_hitbox_circulo(float x, float y, float raio, ALLEGRO_COLOR cor, float camera_x, float camera_y, float zoom) {
    float x_tela = (x - camera_x) * zoom;
    float y_tela = (y - camera_y) * zoom;
    float raio_tela = raio * zoom;

    al_draw_circle(x_tela, y_tela, raio_tela, cor, 2.0f);

    al_draw_line(x_tela - 5, y_tela, x_tela + 5, y_tela, cor, 1.0f);
    al_draw_line(x_tela, y_tela - 5, x_tela, y_tela + 5, cor, 1.0f);
}

void debug_desenhar_hitbox_retangulo(float x, float y, float largura, float altura, ALLEGRO_COLOR cor, float camera_x, float camera_y, float zoom) {
    float x_tela = (x - camera_x) * zoom;
    float y_tela = (y - camera_y) * zoom;
    float largura_tela = largura * zoom;
    float altura_tela = altura * zoom;

    al_draw_rectangle(x_tela, y_tela, x_tela + largura_tela, y_tela + altura_tela, cor, 2.0f);
}

void debug_desenhar_area_circular(float centro_x, float centro_y, float raio, ALLEGRO_COLOR cor, float camera_x, float camera_y, float zoom) {
    float x_tela = (centro_x - camera_x) * zoom;
    float y_tela = (centro_y - camera_y) * zoom;
    float raio_tela = raio * zoom;

    ALLEGRO_COLOR cor_transparente = al_map_rgba_f(cor.r, cor.g, cor.b, 0.2f);
    al_draw_filled_circle(x_tela, y_tela, raio_tela, cor_transparente);
    al_draw_circle(x_tela, y_tela, raio_tela, cor, 2.0f);

    al_draw_filled_circle(x_tela, y_tela, 3.0f, cor);
}

void debug_desenhar_area_retangular(float min_x, float max_x, float min_y, float max_y, ALLEGRO_COLOR cor, float camera_x, float camera_y, float zoom) {
    float x1_tela = (min_x - camera_x) * zoom;
    float y1_tela = (min_y - camera_y) * zoom;
    float x2_tela = (max_x - camera_x) * zoom;
    float y2_tela = (max_y - camera_y) * zoom;

    ALLEGRO_COLOR cor_transparente = al_map_rgba_f(cor.r, cor.g, cor.b, 0.2f);
    al_draw_filled_rectangle(x1_tela, y1_tela, x2_tela, y2_tela, cor_transparente);
    al_draw_rectangle(x1_tela, y1_tela, x2_tela, y2_tela, cor, 2.0f);
}

void debug_desenhar_area_eliptica(float centro_x, float centro_y,
    float raio_h, float raio_v,
    ALLEGRO_COLOR cor,
    float camera_x, float camera_y,
    float zoom) {
    float cx_tela = (centro_x - camera_x) * zoom;
    float cy_tela = (centro_y - camera_y) * zoom;
    float rx = raio_h * zoom;
    float ry = raio_v * zoom;

    al_draw_ellipse(cx_tela, cy_tela, rx, ry, cor, 2.0f);

    float tamanho_cruz = 5.0f;
    al_draw_line(cx_tela - tamanho_cruz, cy_tela,
        cx_tela + tamanho_cruz, cy_tela, cor, 1.0f);
    al_draw_line(cx_tela, cy_tela - tamanho_cruz,
        cx_tela, cy_tela + tamanho_cruz, cor, 1.0f);
}

void debug_desenhar_grid(int largura_mapa, int altura_mapa, int tamanho_celula, ALLEGRO_COLOR cor, float camera_x, float camera_y, float zoom) {
    for (int x = 0; x <= largura_mapa; x += tamanho_celula) {
        float x1 = ((float)x - camera_x) * zoom;
        float y1 = (0.0f - camera_y) * zoom;
        float y2 = ((float)altura_mapa - camera_y) * zoom;
        al_draw_line(x1, y1, x1, y2, cor, 1.0f);
    }

    for (int y = 0; y <= altura_mapa; y += tamanho_celula) {
        float x1 = (0.0f - camera_x) * zoom;
        float y1 = ((float)y - camera_y) * zoom;
        float x2 = ((float)largura_mapa - camera_x) * zoom;
        al_draw_line(x1, y1, x2, y1, cor, 1.0f);
    }
}

void debug_desenhar_linha(float x1, float y1, float x2, float y2, ALLEGRO_COLOR cor, float espessura, float camera_x, float camera_y, float zoom) {
    float x1_tela = (x1 - camera_x) * zoom;
    float y1_tela = (y1 - camera_y) * zoom;
    float x2_tela = (x2 - camera_x) * zoom;
    float y2_tela = (y2 - camera_y) * zoom;

    al_draw_line(x1_tela, y1_tela, x2_tela, y2_tela, cor, espessura);
}

void debug_desenhar_ponto(float x, float y, float tamanho, ALLEGRO_COLOR cor, float camera_x, float camera_y, float zoom) {
    float x_tela = (x - camera_x) * zoom;
    float y_tela = (y - camera_y) * zoom;

    al_draw_filled_circle(x_tela, y_tela, tamanho, cor);
}

void debug_desenhar_texto(DebugContext* debug, const char* texto, float x, float y, ALLEGRO_COLOR cor) {
    if (!debug || !debug->fonte_debug || !texto) return;
    al_draw_text(debug->fonte_debug, cor, x, y, 0, texto);
}

void debug_desenhar_info_entidade(DebugContext* debug, const char* nome, float x, float y, float vel_x, float vel_y, float camera_x, float camera_y, float zoom) {
    if (!debug || !debug->fonte_debug || !nome) return;

    float x_tela = (x - camera_x) * zoom;
    float y_tela = (y - camera_y) * zoom - 30.0f;

    char buffer[128];
    snprintf(buffer, sizeof(buffer), "%s", nome);
    al_draw_text(debug->fonte_debug, al_map_rgb(255, 255, 0), x_tela, y_tela, ALLEGRO_ALIGN_CENTER, buffer);

    snprintf(buffer, sizeof(buffer), "(%.0f, %.0f)", x, y);
    al_draw_text(debug->fonte_debug, al_map_rgb(200, 200, 200), x_tela, y_tela + 12, ALLEGRO_ALIGN_CENTER, buffer);
}

void debug_atualizar_fps(DebugContext* debug, float delta_time) {
    if (!debug) return;

    debug->frame_count++;
    debug->tempo_acumulado += delta_time;

    if (debug->tempo_acumulado >= 1.0) {
        debug->fps_atual = debug->frame_count;
        debug->frame_count = 0;
        debug->tempo_acumulado = 0.0;
    }
}

void debug_desenhar_fps(DebugContext* debug) {
    if (!debug || !debug->mostrar_fps || !debug->fonte_debug) return;

    char buffer[32];
    snprintf(buffer, sizeof(buffer), "FPS: %d", debug->fps_atual);

    ALLEGRO_COLOR cor = debug->fps_atual >= 60 ? al_map_rgb(0, 255, 0) :
        debug->fps_atual >= 30 ? al_map_rgb(255, 255, 0) :
        al_map_rgb(255, 0, 0);

    al_draw_text(debug->fonte_debug, cor, 10, 30, 0, buffer);
}

void debug_processar_teclas(DebugContext* debug, ALLEGRO_EVENT* evento) {
    if (!debug || !evento) return;

    if (evento->type != ALLEGRO_EVENT_KEY_DOWN) return;

    switch (evento->keyboard.keycode) {
    case ALLEGRO_KEY_F1:
        debug->mostrar_hitboxes = !debug->mostrar_hitboxes;
#if DEBUG_MODE
        printf("[DEBUG] Hitboxes: %s\n", debug->mostrar_hitboxes ? "ON" : "OFF");
#endif
        break;
    case ALLEGRO_KEY_F2:
        debug->mostrar_areas_restritas = !debug->mostrar_areas_restritas;
#if DEBUG_MODE
        printf("[DEBUG] Areas Restritas: %s\n", debug->mostrar_areas_restritas ? "ON" : "OFF");
#endif
        break;
    case ALLEGRO_KEY_F3:
        debug->mostrar_fps = !debug->mostrar_fps;
#if DEBUG_MODE
        printf("[DEBUG] FPS: %s\n", debug->mostrar_fps ? "ON" : "OFF");
#endif
        break;
    case ALLEGRO_KEY_F4:
        debug->mostrar_info_bots = !debug->mostrar_info_bots;
#if DEBUG_MODE
        printf("[DEBUG] Info Bots: %s\n", debug->mostrar_info_bots ? "ON" : "OFF");
#endif
        break;
    case ALLEGRO_KEY_F5:
        debug->mostrar_grid = !debug->mostrar_grid;
#if DEBUG_MODE
        printf("[DEBUG] Grid: %s\n", debug->mostrar_grid ? "ON" : "OFF");
#endif
        break;
    case ALLEGRO_KEY_F6:
        debug->mostrar_coordenadas = !debug->mostrar_coordenadas;
#if DEBUG_MODE
        printf("[DEBUG] Coordenadas: %s\n", debug->mostrar_coordenadas ? "ON" : "OFF");
#endif
        break;
    }
}

void debug_desenhar_painel_info(DebugContext* debug, float jogador_x, float jogador_y, int cenario_atual, int bots_ativos) {
    if (!debug || !debug->fonte_debug) return;

    const char* nomes_cenas[] = { "CENARIO1", "CENARIO2", "CENARIO3", "CENARIO4" };

    char buffer[256];
    int y_offset = 50;

    snprintf(buffer, sizeof(buffer), "Jogador: (%.0f, %.0f)", jogador_x, jogador_y);
    al_draw_text(debug->fonte_debug, al_map_rgb(255, 255, 255), 10, (float)y_offset, 0, buffer);
    y_offset += 15;

    snprintf(buffer, sizeof(buffer), "Cenario: %s", nomes_cenas[cenario_atual]);
    al_draw_text(debug->fonte_debug, al_map_rgb(255, 255, 255), 10, (float)y_offset, 0, buffer);
    y_offset += 15;

    snprintf(buffer, sizeof(buffer), "Bots Ativos: %d", bots_ativos);
    al_draw_text(debug->fonte_debug, al_map_rgb(255, 255, 255), 10, (float)y_offset, 0, buffer);
    y_offset += 20;

    al_draw_text(debug->fonte_debug, al_map_rgb(150, 150, 150), 10, (float)y_offset, 0, "[F1] Hitboxes");
    y_offset += 12;
    al_draw_text(debug->fonte_debug, al_map_rgb(150, 150, 150), 10, (float)y_offset, 0, "[F2] Areas");
    y_offset += 12;
    al_draw_text(debug->fonte_debug, al_map_rgb(150, 150, 150), 10, (float)y_offset, 0, "[F3] FPS");
    y_offset += 12;
    al_draw_text(debug->fonte_debug, al_map_rgb(150, 150, 150), 10, (float)y_offset, 0, "[F4] Info Bots");
    y_offset += 12;
    al_draw_text(debug->fonte_debug, al_map_rgb(150, 150, 150), 10, (float)y_offset, 0, "[F5] Grid");
}

const char* debug_obter_nome_tecla(int keycode) {
    return al_keycode_to_name(keycode);
}