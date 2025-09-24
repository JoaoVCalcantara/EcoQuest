#include "entidades.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <stdbool.h>
#include <math.h>

void iniciarentidade(jogador *p,float width,float height) {
	p->x = width/2.0f;
	p->y = height/2.0f;
	p->raio = 50.0f;
	p->velocidade = 6.0f;
}
void desenharjogador(jogador* p) {

	al_draw_filled_circle(p->x, p->y, p->raio, al_map_rgb(0, 0, 0));
}

bool colisao(float cx, float cy, float r,
	float rx, float ry, float rw, float rh) {
	//encontra o ponto mais próximo do centro do círculo dentro do retângulo
	float closestX = fmax(rx, fmin(cx, rx + rw));
	float closestY = fmax(ry, fmin(cy, ry + rh));
	float dx = cx - closestX;
	float dy = cy - closestY;
	return (dx * dx + dy * dy) <= (r * r);
}