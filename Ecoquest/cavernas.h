#ifndef cavernas_h
#define cavernas_h

#include "entidades.h"
#include "allegro_init.h"
#include "cenario.h"


//define quais são as variaveis que serão alocadas de forma externa (em outro arquivo .c), e definir a quantidade de entradas das cavernas, e altura/comprimento das portas.

#define NUM_ENTRADAS 4
extern float entradaX[NUM_ENTRADAS];
extern float entradaY[NUM_ENTRADAS];
extern float saidaX;
extern float saidaY;
extern float comprimentoPorta;
extern float alturaPorta;

void initPortas(int width, int height);
void checarEntrada(entidade *jogador, CenasDoJogo *atual);

#endif