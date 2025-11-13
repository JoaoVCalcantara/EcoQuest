#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include "batalha.h"

void desenhar_menu_batalha(ALLEGRO_FONT* fonte, int opcao, ALLEGRO_DISPLAY* display, Animal* animal) {
    int largura = al_get_display_width(display);
    int altura = al_get_display_height(display);

    // Fundo preto
    al_clear_to_color(al_map_rgb(0, 0, 0));

    // ---- Desenhar losango azul escuro na parte inferior ----
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

    // ---- Informações do animal no topo ----
    char info_text[256];
    snprintf(info_text, sizeof(info_text), "%s - %s", animal->nome, animal->tipo);
    al_draw_text(fonte, al_map_rgb(255, 255, 255), cx, 50, ALLEGRO_ALIGN_CENTRE, info_text);

    // Barra de alimentação
    snprintf(info_text, sizeof(info_text), "Alimentacao: %d%%", animal->nivel_alimentacao);
    al_draw_text(fonte, al_map_rgb(255, 200, 0), cx, 80, ALLEGRO_ALIGN_CENTRE, info_text);
    
    // Desenha barra de progresso de alimentação
    float barra_x = cx - 100;
    float barra_y = 100;
    al_draw_filled_rectangle(barra_x, barra_y, barra_x + 200, barra_y + 15, al_map_rgb(50, 50, 50));
    al_draw_filled_rectangle(barra_x, barra_y, 
        barra_x + (200 * animal->nivel_alimentacao / 100.0f), 
        barra_y + 15, al_map_rgb(255, 200, 0));

    // Barra de estudo
    snprintf(info_text, sizeof(info_text), "Estudo: %d%%", animal->experiencia);
    al_draw_text(fonte, al_map_rgb(0, 255, 128), cx, 130, ALLEGRO_ALIGN_CENTRE, info_text);
    
    // Desenha barra de progresso de estudo
    al_draw_filled_rectangle(barra_x, 150, barra_x + 200, 165, al_map_rgb(50, 50, 50));
    al_draw_filled_rectangle(barra_x, 150, 
        barra_x + (200 * animal->experiencia / 100.0f), 
        165, al_map_rgb(0, 255, 128));

    // Status do animal
    if (animal->domado) {
        al_draw_text(fonte, al_map_rgb(0, 255, 0), cx, 180, ALLEGRO_ALIGN_CENTRE, "DOMADO");
    } else {
        al_draw_text(fonte, al_map_rgb(255, 100, 100), cx, 180, ALLEGRO_ALIGN_CENTRE, "SELVAGEM");
    }

    // ---- Opcoes dentro do losango ----
    const char* opcoes[] = { "Alimentar", "Estudar", "Correr" };
    for (int i = 0; i < 3; i++) {
        ALLEGRO_COLOR cor = (i == opcao) ? al_map_rgb(0, 255, 255) : al_map_rgb(255, 255, 255);
        
        // Desabilita "Estudar" se não estiver domado
        if (i == 1 && !animal->domado) {
            cor = al_map_rgb(100, 100, 100);
        }
        
        al_draw_text(fonte, cor, cx, cy - 25 + (i * 25), ALLEGRO_ALIGN_CENTRE, opcoes[i]);
    }

    // Mensagem de ajuda
    if (!animal->domado) {
        al_draw_text(fonte, al_map_rgb(255, 255, 100), cx, cy + 80, ALLEGRO_ALIGN_CENTRE, 
                    "Alimente o animal ate 100% para doma-lo!");
    } else if (!animal->estudado) {
        al_draw_text(fonte, al_map_rgb(255, 255, 100), cx, cy + 80, ALLEGRO_ALIGN_CENTRE, 
                    "Estude o animal ate 100% para completa-lo!");
    }

    al_flip_display();
}

void iniciar_batalha(ALLEGRO_FONT* fonte, Animal* animal, ALLEGRO_EVENT_QUEUE* event_queue, ALLEGRO_DISPLAY* display) {
    bool batalhando = true;
    int opcao = 0; // 0 = Alimentar, 1 = Estudar, 2 = Correr

    while (batalhando) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch (ev.keyboard.keycode) {
            case ALLEGRO_KEY_W:
            case ALLEGRO_KEY_UP:
                // Move para cima, pulando opções desabilitadas
                do {
                    opcao = (opcao + 2) % 3; // Move para cima
                } while (opcao == 1 && !animal->domado); // Pula "Estudar" se não domado
                break;
                
            case ALLEGRO_KEY_S:
            case ALLEGRO_KEY_DOWN:
                // Move para baixo, pulando opções desabilitadas
                do {
                    opcao = (opcao + 1) % 3; // Move para baixo
                } while (opcao == 1 && !animal->domado); // Pula "Estudar" se não domado
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
                        // Após domar, move automaticamente para "Estudar"
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

        desenhar_menu_batalha(fonte, opcao, display, animal);
    }
}