#pragma once
#ifndef BATALHA_H
#define BATALHA_H

#include <stdbool.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro.h>

typedef struct {
    int experiencia;        // XP de estudo (0-100)
    int nivel_alimentacao;  // Nível de alimentação (0-100)
    int nivel;
    bool alimentado;        //True quando nivel_alimentacao >= 100
    bool domado;            // True quando alimentado pela primeira vez
    bool estudado;          // True quando experiencia >= 100
    const char* nome;
    const char* tipo;
} Animal;

void desenhar_menu_batalha(ALLEGRO_FONT* fonte, int opcao, ALLEGRO_DISPLAY* display);
void iniciar_batalha(ALLEGRO_FONT* fonte, Animal* animal, ALLEGRO_EVENT_QUEUE* event_queue, ALLEGRO_DISPLAY* display);

#endif 