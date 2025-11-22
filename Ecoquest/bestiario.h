#pragma once
#ifndef BESTIARIO_H
#define BESTIARIO_H

#include <stdbool.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include "batalha.h"

// Níveis tróficos
typedef enum {
    NIVEL_PRODUTOR = 1,
    NIVEL_CONSUMIDOR_PRIMARIO = 2,
    NIVEL_CONSUMIDOR_SECUNDARIO = 3,
    NIVEL_CONSUMIDOR_TERCIARIO = 4,
    NIVEL_CONSUMIDOR_QUATERNARIO = 5,
    NIVEL_DECOMPOSITOR = 6
} NivelTrofico;

// Posição na cadeia alimentar
typedef enum {
    POSICAO_BASE,
    POSICAO_HERBIVORO,
    POSICAO_PREDADOR_PEQUENO,
    POSICAO_PREDADOR_MEDIO,
    POSICAO_PREDADOR_TOPO,
    POSICAO_ONIVORO,
    POSICAO_DECOMPOSITORES
} PosicaoCadeiaAlimentar;

// Informações ecológicas simplificadas
typedef struct {
    const char* nome_cientifico;
    NivelTrofico nivel_trofico;
    PosicaoCadeiaAlimentar posicao_cadeia;
    float fator_bioacumulacao;
    const char* dieta_principal;
    const char* papel_ecologico;
    const char* status_conservacao;
    bool especie_indicadora;
    const char* biomarcador_de;
} InformacoesEcologicas;

// Entrada simplificada do bestiário
typedef struct {
    Animal* animal_ref;
    InformacoesEcologicas info_ecologica;
    bool desbloqueado;
} EntradaBestiario;

// Bestiário simplificado
struct Bestiario {
    EntradaBestiario entradas[4];  // Apenas 4 animais fixos
    int total_especies;
    int especies_desbloqueadas;
};

// Funções principais simplificadas
Bestiario* criar_bestiario(void);
void adicionar_especie_bestiario(Bestiario* bestiario, Animal* animal);
void desbloquear_entrada_bestiario(Bestiario* bestiario, const char* nome_animal);
void destruir_bestiario(Bestiario* bestiario);

// Função de visualização - POPUP SIMPLES
void mostrar_bestiario_popup(Bestiario* bestiario, ALLEGRO_FONT* fonte, ALLEGRO_DISPLAY* display, ALLEGRO_EVENT_QUEUE* event_queue);

// Funções auxiliares
const char* obter_nome_nivel_trofico(NivelTrofico nivel);
const char* obter_nome_posicao_cadeia(PosicaoCadeiaAlimentar posicao);
InformacoesEcologicas criar_info_ecologica_lobo_guara(void);
InformacoesEcologicas criar_info_ecologica_jacare(void);
InformacoesEcologicas criar_info_ecologica_boto(void);
InformacoesEcologicas criar_info_ecologica_onca(void);

#endif
