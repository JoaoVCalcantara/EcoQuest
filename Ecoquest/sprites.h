#ifndef sprites_h
#define sprites_h

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <stdbool.h>

typedef struct {
    ALLEGRO_BITMAP* imagem;
    float largura;
    float altura;
    bool carregado;
    const char* caminho_arquivo;
} Sprite;

typedef struct {
    ALLEGRO_BITMAP* spritesheet;
    int frames_total;
    int frame_atual;
    int frames_por_linha;
    float largura_frame;
    float altura_frame;
    float tempo_por_frame;
    float tempo_acumulado;
    bool carregado;
    bool loop;
} SpriteAnimado;

typedef struct {
    ALLEGRO_BITMAP** frames;
    int frames_total;
    int frame_atual;
    float tempo_por_frame;
    float tempo_acumulado;
    bool carregado;
    bool loop;
} SpriteAnimadoFrameAFrame;

// Funções de Sprite estático
Sprite* criar_sprite(const char* caminho_arquivo);
void desenhar_sprite(const Sprite* sprite, float x, float y, float escala);
void desenhar_sprite_camera(const Sprite* sprite, float x, float y, float camera_x, float camera_y, float escala, float zoom);
// Corrigido erro de digitação: 'spritwe' -> 'sprite'
void desenhar_sprite_rotacionado(const Sprite* sprite, float x, float y, float angulo, float escala);
void destruir_sprite(Sprite* sprite);

// Funções de Sprite Animado (Spritesheet)
SpriteAnimado* criar_sprite_animado(const char* caminho_arquivo, int frames_total, int frames_por_linha, float tempo_por_frame);
void atualizar_sprite_animado(SpriteAnimado* sprite, float delta_time);
void desenhar_sprite_animado(const SpriteAnimado* sprite, float x, float y, float escala);
void desenhar_sprite_animado_camera(const SpriteAnimado* sprite, float x, float y, float camera_x, float camera_y, float escala, float zoom);
void reiniciar_animacao(SpriteAnimado* sprite);
void set_loop_animado(SpriteAnimado* sprite, bool loop);
void destruir_sprite_animado(SpriteAnimado* sprite);

// Funções de Sprite Animado (Frame a Frame)
// Corrigido nome da função para bater com o .c: 'criar_sprite_animadofAf' -> 'criar_sprite_animado_frames'
SpriteAnimadoFrameAFrame* criar_sprite_animado_frames(const char* caminhos_arquivos, int frames_total, float tempo_por_frame);
SpriteAnimadoFrameAFrame* criar_sprite_animado_array(const char** caminhos_arquivos, int frames_total, float tempo_por_frame);

void atualizar_sprite_animado_frames(SpriteAnimadoFrameAFrame* sprite, float delta_time);
void desenhar_sprite_animado_frames(const SpriteAnimadoFrameAFrame* sprite, float x, float y, float escala);
void desenhar_sprite_animado_frames_camera(const SpriteAnimadoFrameAFrame* sprite, float x, float y, float camera_x, float camera_y, float escala, float zoom);
void reiniciar_animacao_frames(SpriteAnimadoFrameAFrame* sprite);
void set_loop_animacao_frames(SpriteAnimadoFrameAFrame* sprite, bool loop);
void destruir_sprite_animado_frames(SpriteAnimadoFrameAFrame* sprite);
bool sprite_animado_frames_valido(const SpriteAnimadoFrameAFrame* sprite);

#endif