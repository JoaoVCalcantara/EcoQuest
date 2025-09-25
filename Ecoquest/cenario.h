#ifndef cenario_h
#define cenario_h
#include "entidades.h"
#include "allegro_init.h"

typedef enum {
	MENU,
	MUNDO,
	ANIMAL,
	FIM_DE_JOGO,

}CenasDoJogo;

void desenhar_quadrados(int width, int height);
void cenarios(CenasDoJogo *atual, entidade *jogador, AllegroContext *ctx);

#endif
