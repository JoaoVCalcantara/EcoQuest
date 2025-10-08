#ifndef cenario_h
#define cenario_h
#include "entidades.h"
#include "allegro_init.h"

//Enumera e nomeia valores inteiros, como o nome de cada cenario.

typedef enum {
	
	mundo,
	caverna1,
	caverna2,
	caverna3,
	caverna4,
	
}CenasDoJogo;

void quadrado(float entradaX1[], float entradaY1[], float entradaX2[], float entradaY2[],ALLEGRO_COLOR cor);

void cenarios(CenasDoJogo atual, AllegroContext *ctx);


#endif
