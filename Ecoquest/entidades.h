#ifndef entidades_h
#define entidades_h

typedef struct {
	float x;
	float y;
	float raio;
	float velocidade;

}jogador;

void iniciarentidade(jogador *p,float x,float y);
void desenharjogador(jogador *p);
void limitarjogador(jogador* p, float width, float height);




#endif // entidades_h

