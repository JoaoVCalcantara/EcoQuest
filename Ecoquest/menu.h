#pragma once
#ifndef MENU_H
#define MENU_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <stdbool.h>
#include "allegro_init.h"

void desenhar_tela_inicio(AllegroContext* ctx, ALLEGRO_FONT* fonte, ALLEGRO_BITMAP* fundo, bool mostrar_texto);
bool executar_menu_inicial(AllegroContext* ctx);

/* Nova: mostrar tutorial antes de iniciar o jogo */
void mostrar_tutorial(ALLEGRO_FONT* fonte, ALLEGRO_DISPLAY* display, ALLEGRO_EVENT_QUEUE* event_queue);

#endif
