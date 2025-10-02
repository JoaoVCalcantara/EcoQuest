#ifndef cenario_h
#define cenario_h
#include "entidades.h"
#include "allegro_init.h"

//Enumera e nomeia valores inteiros, como o nome de cada cenario.

typedef enum {
	
<<<<<<< HEAD
	mundo,
	caverna1,
	caverna2,
	caverna3,
	caverna4,
	
=======
	MUNDO,
	CAVERNA_1,
	CAVERNA_2,
	CAVERNA_3,
	CAVERNA_4,

>>>>>>> d9fdfa19086c7d0ddddcd8badce85e5e32dc0a15
}CenasDoJogo;

void quadrado(AllegroContext* ctx);

void cenarios(CenasDoJogo atual, AllegroContext *ctx, entidade jogador);


#endif
