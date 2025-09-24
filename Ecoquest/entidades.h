#ifndef entidades_h
#define entidades_h
#include <stdbool.h>

typedef struct {
	float x;
	float y;
	float raio;
	float velocidade;

}jogador;

void iniciarentidade(jogador *p, float width, float height);
void desenharjogador(jogador *p);
bool colisao(float cx, float cy, float r, float rx, float ry, float rw, float rh);




#endif // entidades_h

