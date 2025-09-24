#include "entidades.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

void iniciarentidade(jogador *p,float x, float y) {
	p->x = x;
	p->y = y;
	p->raio = 50.0f;
	p->velocidade = 6.0f;
}
void desenharjogador(jogador* p) {
	al_draw_filled_circle(p->x, p->y, p->raio, al_map_rgb(255, 255, 255));
}

void limitarjogador(jogador* p, float width, float height) {

	if (p->x < width) p->x = p->raio;
	if (p->x > width - p->raio) p->x = width - p->raio;
	if (p->y < p->raio) p->y = p->raio;
	if (p->y > height - p->raio) p->y = height - p->raio;

}