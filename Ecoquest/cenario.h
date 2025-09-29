#ifndef cenario_h
#define cenario_h
#include "entidades.h"
#include "allegro_init.h"

typedef enum {
	
	MUNDO,
	ANIMAL,
	

}CenasDoJogo;

void quadrado(AllegroContext* ctx);

void cenarios(CenasDoJogo atual, AllegroContext *ctx);


#endif
