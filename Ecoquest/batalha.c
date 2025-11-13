#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>  // ADICIONAR
#include <stdio.h>
#include <stdlib.h>  // ADICIONAR para malloc/free
#include <string.h>  // ADICIONAR para manipulação de strings
#include "batalha.h"

// Carrega todos os recursos visuais necessários para a batalha
RecursosBatalha* carregar_recursos_batalha(const char* caminho_fundo, const char* caminho_caixa_texto, const char* caminho_sprite_animal) {
    RecursosBatalha* recursos = (RecursosBatalha*)malloc(sizeof(RecursosBatalha));
    if (!recursos) {
        fprintf(stderr, "Erro ao alocar memória para RecursosBatalha\n");
        return NULL;
    }

    // Carrega imagem de fundo
    recursos->fundo_batalha = al_load_bitmap(caminho_fundo);
    if (!recursos->fundo_batalha) {
        fprintf(stderr, "Erro ao carregar fundo de batalha: %s\n", caminho_fundo);
    }

    // Carrega caixa de texto
    recursos->caixa_texto = al_load_bitmap(caminho_caixa_texto);
    if (!recursos->caixa_texto) {
        fprintf(stderr, "Erro ao carregar caixa de texto: %s\n", caminho_caixa_texto);
    }

    // Carrega sprite do animal
    recursos->sprite_animal = al_load_bitmap(caminho_sprite_animal);
    if (!recursos->sprite_animal) {
        fprintf(stderr, "Erro ao carregar sprite do animal: %s\n", caminho_sprite_animal);
    }

    return recursos;
}

// Libera memória dos recursos
void destruir_recursos_batalha(RecursosBatalha* recursos) {
    if (!recursos) return;

    if (recursos->fundo_batalha) {
        al_destroy_bitmap(recursos->fundo_batalha);
    }
    if (recursos->caixa_texto) {
        al_destroy_bitmap(recursos->caixa_texto);
    }
    if (recursos->sprite_animal) {
        al_destroy_bitmap(recursos->sprite_animal);
    }

    free(recursos);
}

void desenhar_menu_batalha(ALLEGRO_FONT* fonte, int opcao, ALLEGRO_DISPLAY* display, Animal* animal, RecursosBatalha* recursos) {
    int largura = al_get_display_width(display);
    int altura = al_get_display_height(display);

    // Limpa a tela
    al_clear_to_color(al_map_rgb(0, 0, 0));

    // ---- Desenha o fundo da batalha ----
    if (recursos && recursos->fundo_batalha) {
        al_draw_scaled_bitmap(recursos->fundo_batalha,
            0, 0,
            al_get_bitmap_width(recursos->fundo_batalha),
            al_get_bitmap_height(recursos->fundo_batalha),
            0, 0, largura, altura, 0);
    } else {
        // Fallback: fundo preto
        al_clear_to_color(al_map_rgb(0, 0, 0));
    }

    // ---- AJUSTES DE ESCALA E POSIÇÃO DO ANIMAL ----
    if (recursos && recursos->sprite_animal) {
        float sprite_largura = al_get_bitmap_width(recursos->sprite_animal);
        float sprite_altura = al_get_bitmap_height(recursos->sprite_animal);
        
        // CONFIGURAÇÕES AJUSTÁVEIS:
        float escala_animal = 0.3f;      // Tamanho do animal (teste: 2.0 a 5.0)
        float posicao_y = 280.0f;        // Altura do animal (teste: 100 a 300)
        float offset_x = 250.0f;           // Deslocamento horizontal (teste: -100 a 100)
        
        // Calcula posição centralizada com offsets
        float sprite_x = (largura - sprite_largura * escala_animal) / 2.0f + offset_x;
        
        al_draw_scaled_bitmap(recursos->sprite_animal,
            0, 0, sprite_largura, sprite_altura,
            sprite_x, posicao_y,
            sprite_largura * escala_animal, sprite_altura * escala_animal, 0);
    }

    // ---- AJUSTE DA CAIXA DE TEXTO ----
    float caixa_y = altura - 120;  // Posição da caixa (ajuste se necessário)
    
    if (recursos && recursos->caixa_texto) {
        float caixa_largura = al_get_bitmap_width(recursos->caixa_texto);
        float caixa_altura = al_get_bitmap_height(recursos->caixa_texto);
        
        // Escala da caixa (ajuste se a imagem ficar muito grande/pequena)
        float escala_caixa = 0.3f;  // Teste valores entre 0.8 e 1.5
        float offset_caixa_x = 300.0f;
        
        float caixa_x = ((largura - caixa_largura * escala_caixa) / 2.0f) + offset_caixa_x;
        
        if (escala_caixa != 1.0f) {
            al_draw_scaled_bitmap(recursos->caixa_texto,
                0, 0, caixa_largura, caixa_altura,
                caixa_x, caixa_y,
                caixa_largura * escala_caixa, caixa_altura * escala_caixa, 0);
        } else {
            al_draw_bitmap(recursos->caixa_texto, caixa_x, caixa_y, 0);
        }
    } else {
        // Fallback: losango azul
        float cx = largura / 2.0f;
        float cy = altura - 150;
        float tamanho = 200;

        ALLEGRO_COLOR azul_escuro = al_map_rgb(10, 30, 80);
        ALLEGRO_VERTEX vertices[4] = {
            {cx, cy - tamanho / 2, 0, 0, 0, azul_escuro},
            {cx + tamanho / 2, cy, 0, 0, 0, azul_escuro},
            {cx, cy + tamanho / 2, 0, 0, 0, azul_escuro},
            {cx - tamanho / 2, cy, 0, 0, 0, azul_escuro}
        };
        al_draw_filled_polygon((float*)&vertices, 4, azul_escuro);
    }

    // ---- Informações do animal no topo ----
    float cx = largura / 2.0f;
    char info_text[256];
    snprintf(info_text, sizeof(info_text), "%s - %s", animal->nome, animal->tipo);
    al_draw_text(fonte, al_map_rgb(255, 255, 255), cx, 20, ALLEGRO_ALIGN_CENTRE, info_text);

    // Barra de alimentação
    snprintf(info_text, sizeof(info_text), "Alimentacao: %d%%", animal->nivel_alimentacao);
    al_draw_text(fonte, al_map_rgb(255, 200, 0), cx, 45, ALLEGRO_ALIGN_CENTRE, info_text);
    
    float barra_x = cx - 100;
    float barra_y = 65;
    al_draw_filled_rectangle(barra_x, barra_y, barra_x + 200, barra_y + 15, al_map_rgb(50, 50, 50));
    al_draw_filled_rectangle(barra_x, barra_y, 
        barra_x + (200 * animal->nivel_alimentacao / 100.0f), 
        barra_y + 15, al_map_rgb(255, 200, 0));

    // Barra de estudo
    snprintf(info_text, sizeof(info_text), "Estudo: %d%%", animal->experiencia);
    al_draw_text(fonte, al_map_rgb(0, 255, 128), cx, 90, ALLEGRO_ALIGN_CENTRE, info_text);
    
    al_draw_filled_rectangle(barra_x, 110, barra_x + 200, 125, al_map_rgb(50, 50, 50));
    al_draw_filled_rectangle(barra_x, 110, 
        barra_x + (200 * animal->experiencia / 100.0f), 
        125, al_map_rgb(0, 255, 128));

    // Status
    if (animal->domado) {
        al_draw_text(fonte, al_map_rgb(0, 255, 0), cx, altura - 270, ALLEGRO_ALIGN_CENTRE, "DOMADO");
    } else {
        al_draw_text(fonte, al_map_rgb(255, 100, 100), cx, altura - 270, ALLEGRO_ALIGN_CENTRE, "SELVAGEM");
    }

    // ---- Opções dentro/sobre a caixa de texto ----
    float opcoes_y_base = altura - 160;  // Ajuste conforme a posição da caixa
    const char* opcoes[] = { "Alimentar", "Estudar", "Correr" };
    
    for (int i = 0; i < 3; i++) {
        ALLEGRO_COLOR cor = (i == opcao) ? al_map_rgb(255, 255, 0) : al_map_rgb(255, 255, 255);
        
        if (i == 1 && !animal->domado) {
            cor = al_map_rgb(100, 100, 100);
        }
        
        al_draw_text(fonte, cor, cx, opcoes_y_base + (i * 30), ALLEGRO_ALIGN_CENTRE, opcoes[i]);
    }

    // Mensagens de ajuda
    if (!animal->domado) {
        al_draw_text(fonte, al_map_rgb(255, 255, 100), cx, altura - 60, ALLEGRO_ALIGN_CENTRE, 
                    "Alimente o animal ate 100% para doma-lo!");
    } else if (!animal->estudado) {
        al_draw_text(fonte, al_map_rgb(255, 255, 100), cx, altura - 60, ALLEGRO_ALIGN_CENTRE, 
                    "Estude o animal ate 100% para completa-lo!");
    }

    al_flip_display();
}

void iniciar_batalha(ALLEGRO_FONT* fonte, Animal* animal, ALLEGRO_EVENT_QUEUE* event_queue, ALLEGRO_DISPLAY* display) {
    bool batalhando = true;
    int opcao = 0; // 0 = Alimentar, 1 = Estudar, 2 = Correr

    // Define caminhos dos recursos baseados no animal
    const char* caminho_caixa_texto = "assets/img/estruturas/caixa_de_texto.png";
    const char* caminho_fundo = animal->caminho_fundo_batalha ? animal->caminho_fundo_batalha : "assets/img/estruturas/selva.png";
    
    // Obtém o caminho do sprite do animal (converte nome para minúsculas)
    char caminho_sprite_animal[256];
    snprintf(caminho_sprite_animal, sizeof(caminho_sprite_animal), "assets/img/animais/%s.png", animal->nome);
    
    // Converte para minúsculas (caso o sistema de arquivos seja case-sensitive)
    for (int i = 0; caminho_sprite_animal[i]; i++) {
        if (caminho_sprite_animal[i] >= 'A' && caminho_sprite_animal[i] <= 'Z') {
            caminho_sprite_animal[i] = caminho_sprite_animal[i] + ('a' - 'A');
        }
    }

    // Carrega recursos visuais
    RecursosBatalha* recursos = carregar_recursos_batalha(caminho_fundo, caminho_caixa_texto, caminho_sprite_animal);

    while (batalhando) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch (ev.keyboard.keycode) {
            case ALLEGRO_KEY_W:
            case ALLEGRO_KEY_UP:
                do {
                    opcao = (opcao + 2) % 3;
                } while (opcao == 1 && !animal->domado);
                break;
                
            case ALLEGRO_KEY_S:
            case ALLEGRO_KEY_DOWN:
                do {
                    opcao = (opcao + 1) % 3;
                } while (opcao == 1 && !animal->domado);
                break;
                
            case ALLEGRO_KEY_ENTER:
            case ALLEGRO_KEY_SPACE:
                switch (opcao) {
                case 0: // Alimentar
                    printf("Opcao selecionada: Alimentar\n");
                    animal->nivel_alimentacao += 15;
                    
                    if (animal->nivel_alimentacao >= 100) {
                        animal->nivel_alimentacao = 100;
                        animal->alimentado = true;
                        animal->domado = true;
                        printf("%s foi domado!\n", animal->nome);
                        opcao = 1;
                    }
                    break;

                case 1: // Estudar
                    if (!animal->domado) {
                        printf("O animal precisa estar domado para ser estudado!\n");
                        break;
                    }
                    
                    printf("Opcao selecionada: Estudar\n");
                    animal->experiencia += 15;
                    
                    if (animal->experiencia >= 100) {
                        animal->experiencia = 100;
                        animal->estudado = true;
                        printf("%s foi completamente estudado!\n", animal->nome);
                        batalhando = false;
                    }
                    break;

                case 2: // Correr
                    printf("Voce correu da batalha!\n");
                    batalhando = false;
                    break;
                }
                break;

            case ALLEGRO_KEY_ESCAPE:
                batalhando = false;
                break;
            }
        }

        desenhar_menu_batalha(fonte, opcao, display, animal, recursos);
    }

    // Libera recursos ao sair da batalha
    destruir_recursos_batalha(recursos);
}