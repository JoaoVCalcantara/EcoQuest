#ifndef DEBUG_H
#define DEBUG_H

#include <stdbool.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>

// Define se o modo debug está ativo (mude para 0 para desabilitar)
#ifndef DEBUG_MODE
#define DEBUG_MODE 1
#endif

// Macros condicionais de debug
#if DEBUG_MODE
    #define DEBUG_PRINT(fmt, ...) printf("[DEBUG] " fmt "\n", ##__VA_ARGS__)
    #define DEBUG_LOG(msg) printf("[DEBUG] %s\n", msg)
#else
    #define DEBUG_PRINT(fmt, ...) ((void)0)
    #define DEBUG_LOG(msg) ((void)0)
#endif

// Estrutura para armazenar informações de debug
typedef struct {
    bool mostrar_hitboxes;        // Mostra círculos de colisão
    bool mostrar_areas_restritas; // Mostra áreas de movimento limitado
    bool mostrar_fps;             // Mostra FPS na tela
    bool mostrar_info_bots;       // Mostra info dos bots
    bool mostrar_grid;            // Mostra grid do mapa
    bool mostrar_coordenadas;     // Mostra coordenadas do mouse
    
    int fps_atual;
    int frame_count;
    double tempo_acumulado;
    
    ALLEGRO_FONT* fonte_debug;
} DebugContext;

// Funções de inicialização e finalização
void inicializar_debug(DebugContext* debug, ALLEGRO_FONT* fonte);
void destruir_debug(DebugContext* debug);

// Funções de desenho
void debug_desenhar_hitbox_circulo(float x, float y, float raio, ALLEGRO_COLOR cor, float camera_x, float camera_y, float zoom);
void debug_desenhar_hitbox_retangulo(float x, float y, float largura, float altura, ALLEGRO_COLOR cor, float camera_x, float camera_y, float zoom);
void debug_desenhar_area_circular(float centro_x, float centro_y, float raio, ALLEGRO_COLOR cor, float camera_x, float camera_y, float zoom);
void debug_desenhar_area_retangular(float min_x, float max_x, float min_y, float max_y, ALLEGRO_COLOR cor, float camera_x, float camera_y, float zoom);
void debug_desenhar_area_eliptica(float centro_x, float centro_y, float raio_h, float raio_v, ALLEGRO_COLOR cor, float camera_x, float camera_y, float zoom);
void debug_desenhar_grid(int largura_mapa, int altura_mapa, int tamanho_celula, ALLEGRO_COLOR cor, float camera_x, float camera_y, float zoom);
void debug_desenhar_linha(float x1, float y1, float x2, float y2, ALLEGRO_COLOR cor, float espessura, float camera_x, float camera_y, float zoom);
void debug_desenhar_ponto(float x, float y, float tamanho, ALLEGRO_COLOR cor, float camera_x, float camera_y, float zoom);

// Funções de texto
void debug_desenhar_texto(DebugContext* debug, const char* texto, float x, float y, ALLEGRO_COLOR cor);
void debug_desenhar_info_entidade(DebugContext* debug, const char* nome, float x, float y, float vel_x, float vel_y, float camera_x, float camera_y, float zoom);

// Funções de FPS
void debug_atualizar_fps(DebugContext* debug, float delta_time);
void debug_desenhar_fps(DebugContext* debug);

// Funções de controle (teclas para alternar opções)
void debug_processar_teclas(DebugContext* debug, ALLEGRO_EVENT* evento);

// Funções auxiliares
void debug_desenhar_painel_info(DebugContext* debug, float jogador_x, float jogador_y, int cenario_atual, int bots_ativos);
const char* debug_obter_nome_tecla(int keycode);

#endif // DEBUG_H
