#ifndef MENU_H
#define MENU_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <stdbool.h>
#include "allegro_init.h"

void desenhar_tela_inicio(AllegroContext* ctx, ALLEGRO_FONT* fonte, ALLEGRO_BITMAP* fundo, bool mostrar_texto);
bool executar_menu_inicial(AllegroContext* ctx);

#endif
