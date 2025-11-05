#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include "batalha.h"

void desenhar_menu_batalha(ALLEGRO_FONT* fonte, int opcao, ALLEGRO_DISPLAY* display) {
    int largura = al_get_display_width(display);
    int altura = al_get_display_height(display);

    // Fundo preto
    al_clear_to_color(al_map_rgb(0, 0, 0));

    // ---- Desenhar losango azul escuro na parte inferior ----
    float cx = largura / 2.0f;
    float cy = altura - 150;  // parte inferior
    float tamanho = 200;

    ALLEGRO_COLOR azul_escuro = al_map_rgb(10, 30, 80);
    ALLEGRO_VERTEX vertices[4] = {
        {cx, cy - tamanho / 2, 0, 0, 0, azul_escuro},
        {cx + tamanho / 2, cy, 0, 0, 0, azul_escuro},
        {cx, cy + tamanho / 2, 0, 0, 0, azul_escuro},
        {cx - tamanho / 2, cy, 0, 0, 0, azul_escuro}
    };
    al_draw_filled_polygon((float*)&vertices, 4, azul_escuro);

    // ---- Opcoes dentro do losango ----
    const char* opcoes[] = { "Alimentar", "Estudar", "Correr" };
    for (int i = 0; i < 3; i++) {
        ALLEGRO_COLOR cor = (i == opcao) ? al_map_rgb(0, 255, 255) : al_map_rgb(255, 255, 255);
        al_draw_text(fonte, cor, cx, cy - 25 + (i * 25), ALLEGRO_ALIGN_CENTRE, opcoes[i]);
    }

    al_flip_display();
}

void iniciar_batalha(ALLEGRO_FONT* fonte, Animal* animal, ALLEGRO_EVENT_QUEUE* event_queue, ALLEGRO_DISPLAY* display) {
    bool batalhando = true;
    int opcao = 0;

    while (batalhando) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch (ev.keyboard.keycode) {
            case ALLEGRO_KEY_W:
                opcao = (opcao + 2) % 3; // sobe
                break;
            case ALLEGRO_KEY_S:
                opcao = (opcao + 1) % 3; // desce
                break;
            case ALLEGRO_KEY_ENTER:
                switch (opcao) {
                case 0: // Alimentar
                    printf("Opcao selecionada: Alimentar\n");
                    animal->alimentado = true;
                    animal->experiencia += 10;
                    if (animal->experiencia >= 100) {
                        printf("O animal atingiu 100 de experiencia!\n");
                        batalhando = false;
                    }
                    break;

                case 1: // Estudar
                    printf("Opcao selecionada: Estudar\n");
                    animal->experiencia += 20;
                    if (animal->experiencia >= 100) {
                        printf("O animal atingiu 100 de experiencia!\n");
                        batalhando = false;
                    }
                    break;

                case 2: // Correr -> sai da batalha
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

        desenhar_menu_batalha(fonte, opcao, display);
    }
}