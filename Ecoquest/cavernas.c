#include "cavernas.h"
#include "entidades.h"

//aloca as variaveis definidas no header

float entradaX[NUM_ENTRADAS];
float entradaY[NUM_ENTRADAS];
float saidaX;
float saidaY;
float comprimentoPorta;
float alturaPorta;

//inicia alguma das variaveis

void initPortas(int width, int height) {
    float tamanho = width * 0.125f;
    comprimentoPorta = tamanho;
    alturaPorta = tamanho;

    entradaX[0] = 0.0f; entradaY[0] = 0.0f;
    entradaX[1] = width - tamanho; entradaY[1] = 0.0f;
    entradaX[2] = 0.0f; entradaY[2] = height - tamanho;
    entradaX[3] = width - tamanho; entradaY[3] = height - tamanho;

}

//Faz a Checagem de qual entrada o jogador interagiu, e onde será gerada a saida da respectiva caverna

void checarEntrada(entidade* jogador, CenasDoJogo* atual) {

    if (*atual == mundo) {
        for (int i = 0; i < NUM_ENTRADAS; i++) {
            if (colisao(jogador->x, jogador->y, jogador->raio,
                entradaX[i], entradaY[i], comprimentoPorta, alturaPorta)) {
                switch (i) {
                case 0:
                    *atual = caverna1;
                    saidaX = 0.0f;
                    saidaY = 0.0f;
                    break;
                case 1:
                    *atual = caverna2;
                    saidaX = 1280;
                    saidaY = 0;
                    break;
                case 2:
                    *atual = caverna3;
                    saidaX = 0;
                    saidaY = 720;
                    break;
                case 3:
                    *atual = caverna4;
                    saidaX = 1280;
                    saidaY = 720;
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
