#ifndef cenario_h
#define cenario_h
#include "entidades.h"
#include "allegro_init.h"

typedef enum {
	
	MUNDO,
	CAVERNA_1,
	CAVERNA_2,
	CAVERNA_3,
	CAVERNA_4,

}CenasDoJogo;

void quadrado(AllegroContext* ctx);

void cenarios(CenasDoJogo atual, AllegroContext *ctx, entidade jogador);


#endif
