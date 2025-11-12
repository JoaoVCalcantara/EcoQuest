#include "batalha.h"
#include <allegro5/allegro_primitives.h>

void desenhar_menu_batalha(ALLEGRO_FONT* fonte, int opcao, ALLEGRO_DISPLAY* display) {
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_text(fonte, al_map_rgb(255, 255, 255), 640, 300, ALLEGRO_ALIGN_CENTRE, "BATALHA INICIADA!");
    al_flip_display();
}

void iniciar_batalha(ALLEGRO_FONT* fonte, Animal* animal, ALLEGRO_EVENT_QUEUE* event_queue, ALLEGRO_DISPLAY* display) {
    desenhar_menu_batalha(fonte, 0, display);
    al_rest(1.5); // Pausa 1,5 segundos simulando a batalha
}
