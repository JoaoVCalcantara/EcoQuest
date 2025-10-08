#include "cavernas.h"
#include "entidades.h"
#include "allegro_init.h"

//aloca as variaveis definidas no header
float entradaX1[NUM_ENTRADAS];
float entradaY1[NUM_ENTRADAS];
float entradaX2[NUM_ENTRADAS];
float entradaY2[NUM_ENTRADAS];
float saidaX;
float saidaY;
float comprimentoPorta;
float alturaPorta;
float tamanho;

//inicia alguma das variaveis

void initPortas(int width, int height) {

        tamanho = width * 0.125;
        comprimentoPorta = tamanho;
        alturaPorta = tamanho;

    entradaX1[0] = 0.0f;                entradaY1[0] = 0.0f;
    entradaX1[1] = width - tamanho;   entradaY1[1] = 0.0f;
    entradaX1[2] = 0.0f;             entradaY1[2] = height - tamanho;
    entradaX1[3] = width - tamanho; entradaY1[3] = height - tamanho;

    entradaX2[0] = tamanho;         entradaY2[0] = tamanho;
    entradaX2[1] = width;           entradaY2[1] = alturaPorta;
    entradaX2[2] = comprimentoPorta;  entradaY2[2] = height;
    entradaX2[3] = width;               entradaY2[3] = height;

}

//Faz a Checagem de qual entrada o jogador interagiu, e onde será gerada a saida da respectiva caverna

void checarEntrada(entidade* jogador, CenasDoJogo* atual) {

    if (*atual == mundo) {
        for (int i = 0; i < NUM_ENTRADAS; i++) {
            if (colisao(jogador->x, jogador->y, jogador->raio,
                entradaX1[i], entradaY1[i], entradaX2[i], entradaY2[i])) {
                switch (i) {
                case 0:
                    *atual = caverna1;
                    saidaX = entradaX1[0];
                    saidaY = entradaY1[0];
                    comprimentoPorta = entradaX2[0];
                    alturaPorta = entradaY2[0];
                    break;
                case 1:
                    *atual = caverna2;
                    saidaX = entradaX1[1];
                    saidaY = entradaY1[1];
                    comprimentoPorta = entradaX2[1];
                    alturaPorta = entradaY2[1];
                    break;
                case 2:
                    *atual = caverna3;
                    saidaX = entradaX1[2];
                    saidaY = entradaY1[2];
                    comprimentoPorta = entradaX2[2];
                    alturaPorta = entradaY2[2];
                    break;
                case 3:
                    *atual = caverna4;
                    saidaX = entradaX1[3];
                    saidaY = entradaY1[3];
                    comprimentoPorta = entradaX2[3];
                    alturaPorta = entradaY2[3];
                    break;

                }
            }
        }
    }
    else if (*atual != mundo) {
        if (colisao(jogador->x, jogador->y, jogador->raio,
            saidaX, saidaY, comprimentoPorta, alturaPorta)) {
            *atual = mundo;

        }
    }
}
