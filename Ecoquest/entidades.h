#ifndef entidades_h
#define entidades_h
#include <stdbool.h>

typedef struct {
	float x;
	float y;
	float raio;
	float velocidade;

}entidade;

void iniciarentidade(entidade *p, float width, float height);
void desenharjogador(entidade *p);
void limitar_jogador(entidade *p, float width, float height);
bool colisao(float cx, float cy, float r, float rx, float ry, float rw, float rh);

#endif // entidades_h

