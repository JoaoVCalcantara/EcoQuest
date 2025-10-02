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

void quadrado(AllegroContext* ctx);

void cenarios(CenasDoJogo atual, AllegroContext *ctx);


#endif
