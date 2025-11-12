#include "sprites.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ========== SPRITES ESTATICOS ==========

Sprite* criar_sprite(const char* caminho_arquivo) {
    Sprite* sprite = (Sprite*)malloc(sizeof(Sprite));
    if (!sprite) {
        fprintf(stderr, "Erro ao alocar memoria para sprite\n");
        return NULL;
    }

    sprite->imagem = al_load_bitmap(caminho_arquivo);
    if (!sprite->imagem) {
        fprintf(stderr, "Erro ao carregar sprite: %s\n", caminho_arquivo);
        sprite->carregado = false;
        sprite->largura = 0;
        sprite->altura = 0;
    } else {
        sprite->carregado = true;
        sprite->largura = (float)al_get_bitmap_width(sprite->imagem);
        sprite->altura = (float)al_get_bitmap_height(sprite->imagem);
    }
    
    sprite->caminho_arquivo = caminho_arquivo;
    return sprite;
}

void desenhar_sprite(const Sprite* sprite, float x, float y, float escala) {
    if (!sprite || !sprite->carregado) return;
    
    al_draw_scaled_bitmap(
        sprite->imagem,
        0, 0, sprite->largura, sprite->altura,
        x - (sprite->largura * escala / 2),
        y - (sprite->altura * escala / 2),
        sprite->largura * escala,
        sprite->altura * escala,
        0
    );
}

void desenhar_sprite_camera(const Sprite* sprite, float x, float y, 
                           float camera_x, float camera_y, 
                           float escala, float zoom) {
    if (!sprite || !sprite->carregado) return;
    
    float x_tela = (x - camera_x) * zoom;
    float y_tela = (y - camera_y) * zoom;
    float largura_escalada = sprite->largura * escala * zoom;
    float altura_escalada = sprite->altura * escala * zoom;
    
    al_draw_scaled_bitmap(
        sprite->imagem,
        0, 0, sprite->largura, sprite->altura,
        x_tela - largura_escalada / 2,
        y_tela - altura_escalada / 2,
        largura_escalada,
        altura_escalada,
        0
    );
}

void desenhar_sprite_rotacionado(const Sprite* sprite, float x, float y, 
                                float angulo, float escala) {
    if (!sprite || !sprite->carregado) return;
    
    al_draw_scaled_rotated_bitmap(
        sprite->imagem,
        sprite->largura / 2, sprite->altura / 2,
        x, y,
        escala, escala,
        angulo,
        0
    );
}

void destruir_sprite(Sprite* sprite) {
    if (!sprite) return;
    
    if (sprite->imagem) {
        al_destroy_bitmap(sprite->imagem);
    }
    free(sprite);
}

// ========== SPRITES ANIMADOS (SPRITESHEET) ==========

SpriteAnimado* criar_sprite_animado(const char* caminho_arquivo, 
                                   int frames_total, 
                                   int frames_por_linha,
                                   float tempo_por_frame) {
    SpriteAnimado* sprite = (SpriteAnimado*)malloc(sizeof(SpriteAnimado));
    if (!sprite) {
        fprintf(stderr, "Erro ao alocar memoria para sprite animado\n");
        return NULL;
    }

    sprite->spritesheet = al_load_bitmap(caminho_arquivo);
    if (!sprite->spritesheet) {
        fprintf(stderr, "Erro ao carregar spritesheet: %s\n", caminho_arquivo);
        sprite->carregado = false;
        free(sprite);
        return NULL;
    }

    sprite->carregado = true;
    sprite->frames_total = frames_total;
    sprite->frames_por_linha = frames_por_linha;
    sprite->frame_atual = 0;
    sprite->tempo_por_frame = tempo_por_frame;
    sprite->tempo_acumulado = 0.0f;
    sprite->loop = true;
    
    float largura_sheet = (float)al_get_bitmap_width(sprite->spritesheet);
    float altura_sheet = (float)al_get_bitmap_height(sprite->spritesheet);
    
    sprite->largura_frame = largura_sheet / frames_por_linha;
    int linhas = (frames_total + frames_por_linha - 1) / frames_por_linha;
    sprite->altura_frame = altura_sheet / linhas;
    
    return sprite;
}

void atualizar_sprite_animado(SpriteAnimado* sprite, float delta_time) {
    if (!sprite || !sprite->carregado) return;
    
    sprite->tempo_acumulado += delta_time;
    
    if (sprite->tempo_acumulado >= sprite->tempo_por_frame) {
        sprite->tempo_acumulado -= sprite->tempo_por_frame;
        sprite->frame_atual++;
        
        if (sprite->frame_atual >= sprite->frames_total) {
            if (sprite->loop) {
                sprite->frame_atual = 0;
            } else {
                sprite->frame_atual = sprite->frames_total - 1;
            }
        }
    }
}

void desenhar_sprite_animado(const SpriteAnimado* sprite, float x, float y, float escala) {
    if (!sprite || !sprite->carregado) return;
    
    int frame_x = (sprite->frame_atual % sprite->frames_por_linha);
    int frame_y = (sprite->frame_atual / sprite->frames_por_linha);
    
    float sx = frame_x * sprite->largura_frame;
    float sy = frame_y * sprite->altura_frame;
    
    al_draw_scaled_bitmap(
        sprite->spritesheet,
        sx, sy, sprite->largura_frame, sprite->altura_frame,
        x - (sprite->largura_frame * escala / 2),
        y - (sprite->altura_frame * escala / 2),
        sprite->largura_frame * escala,
        sprite->altura_frame * escala,
        0
    );
}

void desenhar_sprite_animado_camera(const SpriteAnimado* sprite, 
                                   float x, float y,
                                   float camera_x, float camera_y,
                                   float escala, float zoom) {
    if (!sprite || !sprite->carregado) return;
    
    int frame_x = (sprite->frame_atual % sprite->frames_por_linha);
    int frame_y = (sprite->frame_atual / sprite->frames_por_linha);
    
    float sx = frame_x * sprite->largura_frame;
    float sy = frame_y * sprite->altura_frame;
    
    float x_tela = (x - camera_x) * zoom;
    float y_tela = (y - camera_y) * zoom;
    float largura_escalada = sprite->largura_frame * escala * zoom;
    float altura_escalada = sprite->altura_frame * escala * zoom;
    
    al_draw_scaled_bitmap(
        sprite->spritesheet,
        sx, sy, sprite->largura_frame, sprite->altura_frame,
        x_tela - largura_escalada / 2,
        y_tela - altura_escalada / 2,
        largura_escalada,
        altura_escalada,
        0
    );
}

void reiniciar_animacao(SpriteAnimado* sprite) {
    if (!sprite) return;
    sprite->frame_atual = 0;
    sprite->tempo_acumulado = 0.0f;
}

void set_loop_animado(SpriteAnimado* sprite, bool loop) {
    if (!sprite) return;
    sprite->loop = loop;
}

void destruir_sprite_animado(SpriteAnimado* sprite) {
    if (!sprite) return;
    
    if (sprite->spritesheet) {
        al_destroy_bitmap(sprite->spritesheet);
    }
    free(sprite);
}

// ========== SPRITES ANIMADOS COM FRAMES SEPARADOS ==========

SpriteAnimadoFrameAFrame* criar_sprite_animado_frames(const char* padrao_caminho, int frames_total, float tempo_por_frame) {
    SpriteAnimadoFrameAFrame* sprite = (SpriteAnimadoFrameAFrame*)malloc(sizeof(SpriteAnimadoFrameAFrame));
    if (!sprite) {
        fprintf(stderr, "Erro ao alocar memoria para sprite animado frames\n");
        return NULL;
    }

    sprite->frames = (ALLEGRO_BITMAP**)malloc(sizeof(ALLEGRO_BITMAP*) * frames_total);
    if (!sprite->frames) {
        fprintf(stderr, "Erro ao alocar array de frames\n");
        free(sprite);
        return NULL;
    }

    sprite->frames_total = frames_total;
    sprite->frame_atual = 0;
    sprite->tempo_por_frame = tempo_por_frame;
    sprite->tempo_acumulado = 0.0f;
    sprite->loop = true;
    sprite->carregado = true;

    // Carrega cada frame usando o padrao (ex: "player_%d.png")
    char caminho_completo[256];
    for (int i = 0; i < frames_total; i++) {
        snprintf(caminho_completo, sizeof(caminho_completo), padrao_caminho, i);
        sprite->frames[i] = al_load_bitmap(caminho_completo);
        
        if (!sprite->frames[i]) {
            fprintf(stderr, "Erro ao carregar frame %d: %s\n", i, caminho_completo);
            sprite->carregado = false;
            // Libera frames ja carregados
            for (int j = 0; j < i; j++) {
                if (sprite->frames[j]) al_destroy_bitmap(sprite->frames[j]);
            }
            free(sprite->frames);
            free(sprite);
            return NULL;
        }
    }

    return sprite;
}

SpriteAnimadoFrameAFrame* criar_sprite_animado_array(const char** caminhos_arquivos, int frames_total, float tempo_por_frame) {
    SpriteAnimadoFrameAFrame* sprite = (SpriteAnimadoFrameAFrame*)malloc(sizeof(SpriteAnimadoFrameAFrame));
    if (!sprite) {
        fprintf(stderr, "Erro ao alocar memoria para sprite animado frames\n");
        return NULL;
    }

    sprite->frames = (ALLEGRO_BITMAP**)malloc(sizeof(ALLEGRO_BITMAP*) * frames_total);
    if (!sprite->frames) {
        fprintf(stderr, "Erro ao alocar array de frames\n");
        free(sprite);
        return NULL;
    }

    sprite->frames_total = frames_total;
    sprite->frame_atual = 0;
    sprite->tempo_por_frame = tempo_por_frame;
    sprite->tempo_acumulado = 0.0f;
    sprite->loop = true;
    sprite->carregado = true;

    // Carrega cada frame a partir do array de caminhos
    for (int i = 0; i < frames_total; i++) {
        sprite->frames[i] = al_load_bitmap(caminhos_arquivos[i]);
        
        if (!sprite->frames[i]) {
            fprintf(stderr, "Erro ao carregar frame %d: %s\n", i, caminhos_arquivos[i]);
            sprite->carregado = false;
            for (int j = 0; j < i; j++) {
                if (sprite->frames[j]) al_destroy_bitmap(sprite->frames[j]);
            }
            free(sprite->frames);
            free(sprite);
            return NULL;
        }
    }

    return sprite;
}

void atualizar_sprite_animado_frames(SpriteAnimadoFrameAFrame* sprite, float delta_time) {
    if (!sprite || !sprite->carregado) return;
    
    sprite->tempo_acumulado += delta_time;
    
    if (sprite->tempo_acumulado >= sprite->tempo_por_frame) {
        sprite->tempo_acumulado -= sprite->tempo_por_frame;
        sprite->frame_atual++;
        
        if (sprite->frame_atual >= sprite->frames_total) {
            if (sprite->loop) {
                sprite->frame_atual = 0;
            } else {
                sprite->frame_atual = sprite->frames_total - 1;
            }
        }
    }
}

void desenhar_sprite_animado_frames(const SpriteAnimadoFrameAFrame* sprite, float x, float y, float escala) {
    if (!sprite || !sprite->carregado) return;
    
    ALLEGRO_BITMAP* frame_atual = sprite->frames[sprite->frame_atual];
    if (!frame_atual) return;
    
    float largura = (float)al_get_bitmap_width(frame_atual);
    float altura = (float)al_get_bitmap_height(frame_atual);
    
    al_draw_scaled_bitmap(
        frame_atual,
        0, 0, largura, altura,
        x - (largura * escala / 2),
        y - (altura * escala / 2),
        largura * escala,
        altura * escala,
        0
    );
}

void desenhar_sprite_animado_frames_camera(const SpriteAnimadoFrameAFrame* sprite, 
                                          float x, float y,
                                          float camera_x, float camera_y,
                                          float escala, float zoom) {
    if (!sprite || !sprite->carregado) return;
    
    ALLEGRO_BITMAP* frame_atual = sprite->frames[sprite->frame_atual];
    if (!frame_atual) return;
    
    float largura = (float)al_get_bitmap_width(frame_atual);
    float altura = (float)al_get_bitmap_height(frame_atual);
    
    float x_tela = (x - camera_x) * zoom;
    float y_tela = (y - camera_y) * zoom;
    float largura_escalada = largura * escala * zoom;
    float altura_escalada = altura * escala * zoom;
    
    al_draw_scaled_bitmap(
        frame_atual,
        0, 0, largura, altura,
        x_tela - largura_escalada / 2,
        y_tela - altura_escalada / 2,
        largura_escalada,
        altura_escalada,
        0
    );
}

void reiniciar_animacao_frames(SpriteAnimadoFrameAFrame* sprite) {
    if (!sprite) return;
    sprite->frame_atual = 0;
    sprite->tempo_acumulado = 0.0f;
}

void set_loop_animacao_frames(SpriteAnimadoFrameAFrame* sprite, bool loop) {
    if (!sprite) return;
    sprite->loop = loop;
}

void destruir_sprite_animado_frames(SpriteAnimadoFrameAFrame* sprite) {
    if (!sprite) return;
    
    if (sprite->frames) {
        for (int i = 0; i < sprite->frames_total; i++) {
            if (sprite->frames[i]) {
                al_destroy_bitmap(sprite->frames[i]);
            }
        }
        free(sprite->frames);
    }
    free(sprite);
}

bool sprite_animado_frames_valido(const SpriteAnimadoFrameAFrame* sprite) {
    return sprite && sprite->carregado && sprite->frames;
}