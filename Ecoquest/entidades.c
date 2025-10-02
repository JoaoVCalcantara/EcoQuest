#include "entidades.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/keyboard.h>

#include <stdbool.h>
#include <math.h>

//Inicia a entidade que eu quiser iniciar, passando o ponteiro dela da struct e alguamas informações da tela.

void iniciarentidade(entidade *p,float width,float height) {
	p->x = width/2.0f;
	p->y = height/2.0f;
	p->raio = 50.0f;
	p->velocidade = 8.0f;
}
// Desenha o jogador na tela

void desenharjogador(entidade* p) {
	al_draw_filled_circle(p->x, p->y, p->raio, al_map_rgb(0, 255, 0));

}


// Função genérica de colisão entre circulo e quadrado.
// cx, cy: coordenadas do centro do círculo
// r: raio do círculo
// rx, ry: coordenadas do canto superior esquerdo do retângulo
// rw, rh: largura e altura do retângulo
// Retorna true se houver colisão, false caso contrário
//
// A função calcula o ponto mais próximo do centro do círculo dentro do retângulo
// e verifica se a distância entre esse ponto e o centro do círculo é menor ou igual
// ao raio do círculo. Se for, significa que o círculo e o retângulo estão colidindo.

bool colisao(float cx, float cy, float r,
	float rx, float ry, float rw, float rh) {
	float closestX = fmax(rx, fmin(cx, rx + rw));
	float closestY = fmax(ry, fmin(cy, ry + rh));
	float dx = cx - closestX;
	float dy = cy - closestY;
	return (dx * dx + dy * dy) <= (r * r);
}

//Quando chamada, ira limitar o jogador ao espaço de tela disponivel

void limitar_jogador(entidade* p, float width, float height) {
	if (p->x < p->raio) p->x = p->raio;
	if (p->x > width - p->raio) p->x = width - p->raio;
	if (p->y < p->raio) p->y = p->raio;
	if (p->y > height - p->raio) p->y = height - p->raio;
}

//Processa as teclas de movimentação, ao mesmo tempo que limita o jogador na area da tela, opcional ou situacional esse ultimo

void processar_teclado(ALLEGRO_KEYBOARD_STATE* estado, entidade* jogador, float width, float height) {

	if (al_key_down(estado, ALLEGRO_KEY_W)) jogador->y -= jogador->velocidade;
	if (al_key_down(estado, ALLEGRO_KEY_S)) jogador->y += jogador->velocidade;
	if (al_key_down(estado, ALLEGRO_KEY_A)) jogador->x -= jogador->velocidade;
	if (al_key_down(estado, ALLEGRO_KEY_D)) jogador->x += jogador->velocidade;

	limitar_jogador(jogador, width, height);
}
