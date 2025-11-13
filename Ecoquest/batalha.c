#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "batalha.h"

// Estrutura para configurar posição individual de cada animal
typedef struct {
    const char* nome;
    float escala;
    float pos_x;
    float pos_y;
} ConfigAnimal;

// Configurações individuais para cada animal
static const ConfigAnimal configs_animais[] = {
    {"raposa", 0.25f, 250.0f, 325.0f},
    {"jacare", 0.25f, 275.0f, 125.0f},
    {"boto", 0.25f, 280.0f, 280.0f},
    {"onca", 0.25f, 250.0f, 175.0f}
};

// NOVO: Configurações de posição do sprite do jogador para cada animal
static const ConfigSpriteJogador configs_jogador[] = {
    {"raposa", 100.0f, 150.0f, 0.5f},   // x, y, escala
    {"jacare", 80.0f, 400.0f, 0.5f},
    {"boto", 90.0f, 380.0f, 0.5f},
    {"onca", 100.0f, 370.0f, 0.5f}
};

// Obtém a configuração específica do animal pelo nome
ConfigAnimal obter_config_animal(const char* nome_animal) {
    // Configuração padrão caso o animal não esteja na lista
    ConfigAnimal padrao = {"desconhecido", 0.3f, 250.0f, 280.0f};
    
    if (!nome_animal) return padrao;
    
    // Converte o nome para minúsculas para comparação
    char nome_lower[64];
    snprintf(nome_lower, sizeof(nome_lower), "%s", nome_animal);
    for (int i = 0; nome_lower[i]; i++) {
        if (nome_lower[i] >= 'A' && nome_lower[i] <= 'Z') {
            nome_lower[i] = nome_lower[i] + ('a' - 'A');
        }
    }
    
    // Busca a configuração específica
    for (int i = 0; i < sizeof(configs_animais) / sizeof(ConfigAnimal); i++) {
        if (strcmp(configs_animais[i].nome, nome_lower) == 0) {
            return configs_animais[i];
        }
    }
    
    return padrao;
}

// NOVO: Obtém configuração do sprite do jogador
ConfigSpriteJogador obter_config_sprite_jogador(const char* nome_animal) {
    ConfigSpriteJogador padrao = {"desconhecido", 100.0f, 350.0f, 0.15f};
    
    if (!nome_animal) return padrao;
    
    char nome_lower[64];
    snprintf(nome_lower, sizeof(nome_lower), "%s", nome_animal);
    for (int i = 0; nome_lower[i]; i++) {
        if (nome_lower[i] >= 'A' && nome_lower[i] <= 'Z') {
            nome_lower[i] = nome_lower[i] + ('a' - 'A');
        }
    }
    
    for (int i = 0; i < sizeof(configs_jogador) / sizeof(ConfigSpriteJogador); i++) {
        if (strcmp(configs_jogador[i].nome_animal, nome_lower) == 0) {
            return configs_jogador[i];
        }
    }
    
    return padrao;
}

// Carrega todos os recursos visuais necessários para a batalha
RecursosBatalha* carregar_recursos_batalha(const char* caminho_fundo, const char* caminho_caixa_texto, const char* caminho_sprite_animal, const char* caminho_sprite_jogador) {
    RecursosBatalha* recursos = (RecursosBatalha*)malloc(sizeof(RecursosBatalha));
    if (!recursos) {
        fprintf(stderr, "Erro ao alocar memória para Recursos Batalha\n");
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

    // NOVO: Carrega sprite do jogador
    recursos->sprite_jogador = al_load_bitmap(caminho_sprite_jogador);
    if (!recursos->sprite_jogador) {
        fprintf(stderr, "Erro ao carregar sprite do jogador: %s\n", caminho_sprite_jogador);
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
    if (recursos->sprite_jogador) {
        al_destroy_bitmap(recursos->sprite_jogador);
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
        al_clear_to_color(al_map_rgb(0, 0, 0));
    }

    // ---- NOVO: DESENHA O SPRITE DO JOGADOR ----
    if (recursos && recursos->sprite_jogador) {
        ConfigSpriteJogador config_jogador = obter_config_sprite_jogador(animal->nome);
        
        float jogador_largura = al_get_bitmap_width(recursos->sprite_jogador);
        float jogador_altura = al_get_bitmap_height(recursos->sprite_jogador);
        
        al_draw_scaled_bitmap(recursos->sprite_jogador,
            0, 0, jogador_largura, jogador_altura,
            config_jogador.jogador_x, config_jogador.jogador_y,
            jogador_largura * config_jogador.jogador_escala, 
            jogador_altura * config_jogador.jogador_escala, 0);
    }

    // ---- DESENHA O ANIMAL COM CONFIGURAÇÃO INDIVIDUAL ----
    if (recursos && recursos->sprite_animal) {
        float sprite_largura = al_get_bitmap_width(recursos->sprite_animal);
        float sprite_altura = al_get_bitmap_height(recursos->sprite_animal);
        
        // Obtém configuração específica do animal
        ConfigAnimal config = obter_config_animal(animal->nome);
        
        // Calcula posição centralizada com offsets personalizados
        float sprite_x = (largura - sprite_largura * config.escala) / 2.0f + config.pos_x;
        
        al_draw_scaled_bitmap(recursos->sprite_animal,
            0, 0, sprite_largura, sprite_altura,
            sprite_x, config.pos_y,
            sprite_largura * config.escala, sprite_altura * config.escala, 0);
    }

    // ---- DESENHA A CAIXA DE TEXTO ----
    float caixa_y = altura - 125;  // Posição da caixa
    float caixa_x_real;  // Posição X real da caixa
    float caixa_largura_escalada;  // Largura da caixa após escala
    
    if (recursos && recursos->caixa_texto) {
        float caixa_largura = al_get_bitmap_width(recursos->caixa_texto);
        float caixa_altura = al_get_bitmap_height(recursos->caixa_texto);
        
        float escala_caixa = 0.3f;
        float offset_caixa_x = 250.0f;
        
        caixa_x_real = ((largura - caixa_largura * escala_caixa) / 2.0f) + offset_caixa_x;
        caixa_largura_escalada = caixa_largura * escala_caixa;
        
        al_draw_scaled_bitmap(recursos->caixa_texto,
            0, 0, caixa_largura, caixa_altura,
            caixa_x_real, caixa_y,
            caixa_largura_escalada, caixa_altura * escala_caixa, 0);
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
        
        // Define valores para fallback
        caixa_x_real = cx - tamanho / 2;
        caixa_largura_escalada = tamanho;
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

    // ---- TEXTO DAS OPÇÕES DENTRO DA CAIXA ----
    // Calcula o centro da caixa baseado na posição real
    float caixa_x_centro = (caixa_x_real + (caixa_largura_escalada / 2.0f)) - 100;
    
    // Ajuste estas coordenadas para alinhar com sua imagem da caixa
    float opcoes_y_base = caixa_y + 25;  // Offset do topo da caixa
    float espacamento = 20;  // Espaço entre as opções
    
    const char* opcoes[] = { "Alimentar", "Estudar", "Correr" };
    
    for (int i = 0; i < 3; i++) {
        ALLEGRO_COLOR cor = (i == opcao) ? al_map_rgb(0, 255, 255) : al_map_rgb(0, 0, 0);
        
        if (i == 1 && !animal->domado) {
            cor = al_map_rgb(255, 0, 0);
        }
        
        al_draw_text(fonte, cor, caixa_x_centro, opcoes_y_base + (i * espacamento), ALLEGRO_ALIGN_CENTRE, opcoes[i]);
    }

    // Mensagens de ajuda (abaixo da caixa)
    if (!animal->domado) {
        al_draw_text(fonte, al_map_rgb(255, 255, 100), caixa_x_centro + 100, altura - 35, ALLEGRO_ALIGN_CENTRE, 
                    "Alimente o animal ate 100% para doma-lo!");
    } else if (!animal->estudado) {
        al_draw_text(fonte, al_map_rgb(255, 255, 100), caixa_x_centro + 100, altura - 35, ALLEGRO_ALIGN_CENTRE, 
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
    const char* caminho_sprite_jogador = "assets/img/Heroi/idle_right.png";  // NOVO: Sprite padrão do jogador
    
    // Obtém o caminho do sprite do animal (converte nome para minúsculas)
    char caminho_sprite_animal[256];
    snprintf(caminho_sprite_animal, sizeof(caminho_sprite_animal), "assets/img/animais/%s.png", animal->nome);
    
    // Converte para minúsculas (caso o sistema de arquivos seja case-sensitive)
    for (int i = 0; caminho_sprite_animal[i]; i++) {
        if (caminho_sprite_animal[i] >= 'A' && caminho_sprite_animal[i] <= 'Z') {
            caminho_sprite_animal[i] = caminho_sprite_animal[i] + ('a' - 'A');
        }
    }

    // Carrega recursos visuais (AGORA COM SPRITE DO JOGADOR)
    RecursosBatalha* recursos = carregar_recursos_batalha(caminho_fundo, caminho_caixa_texto, caminho_sprite_animal, caminho_sprite_jogador);

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