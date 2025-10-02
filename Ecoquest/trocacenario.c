#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

#include "entidades.h"
#include "allegro_init.h"
#include "cenario.h"
#include "trocacenario.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#define NUM_ENTRADAS 4
#define NUM_SAIDAS 1

// Variaveisg
float entradaX[NUM_ENTRADAS];
float entradaY[NUM_ENTRADAS];
float saidaX;
float saidaY;
float comprimentoPorta;
float alturaPorta;


void initPortas(int width, int height) {
    float tamanho = width * 0.125f;  // same as in quadrado()
     comprimentoPorta = tamanho;
     alturaPorta = tamanho;
    // Example entry positions in MUNDO scenario
    entradaX[0] = 0.0f;
    entradaY[0] = 0.0f;
    entradaX[1] = width - tamanho;
    entradaY[1] = 0.0f;
    entradaX[2] = 0.0f;
    entradaY[2] = height - tamanho;
    entradaX[3] = width - tamanho;
    entradaY[3] = height - tamanho;
    // Example exit position in ANIMAL scenario
    saidaX = width / 2.0f - tamanho / 2.0f;
    saidaY = height / 2.0f - tamanho / 2.0f;
}



void trocar_cenario(AllegroContext* ctx, entidade jogador,CenasDoJogo atual){
    
    

        if (event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == ALLEGRO_KEY_E) {
            if (atual == MUNDO) {
                for (int i = 0; i < NUM_ENTRADAS; i++) {
                    if (colisao(jogador.x, jogador.y, jogador.raio,
                        entradaX[i], entradaY[i], comprimentoPorta, alturaPorta)) {
                        atual = ANIMAL1;
                       
                        break;
                    }
                }
            }
            else if (atual == ANIMAL) {
                if (colisao(jogador.x, jogador.y, jogador.raio,
                    saidaX, saidaY, comprimentoPorta, alturaPorta)) {
                    atual = MUNDO;
                    
                }
            }
        }
}
