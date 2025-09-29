#include "entidades.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include <stdbool.h>
#include <math.h>

void iniciarentidade(entidade *p,float width,float height) {
	p->x = width/2.0f;
	p->y = height/2.0f;
	p->raio = 50.0f;
	p->velocidade = 8.0f;
}
void desenharjogador(entidade* p) {
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

void limitar_jogador(entidade* p, float width, float height) {
	if (p->x < p->raio) p->x = p->raio;
	if (p->x > width - p->raio) p->x = width - p->raio;
	if (p->y < p->raio) p->y = p->raio;
	if (p->y > height - p->raio) p->y = height - p->raio;
}