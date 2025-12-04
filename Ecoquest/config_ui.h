#ifndef CONFIG_UI_H
#define CONFIG_UI_H

/* === CONFIGURAÇÕES DE UI / BATALHA === */

/* Caixa de texto - escala e offset padrão (pixels) */
#define BATALHA_CAIXA_ESCALA       0.30f
#define BATALHA_CAIXA_OFFSET_Y     10.0f
#define BATALHA_CAIXA_MIN_MARGIN_X 10.0f
#define BATALHA_CAIXA_MIN_MARGIN_Y 5.0f

/* === POSICIONAMENTO POR CENÁRIO DE BATALHA === */
/* Cada cenário define 3 setores fixos: JOGADOR, ANIMAL e CAÇADOR */

/* --- CENÁRIO: SELVA --- */
#define SELVA_JOGADOR_X            350.0f
#define SELVA_JOGADOR_Y	           425.0f
#define SELVA_JOGADOR_ESCALA       0.45f

#define SELVA_ANIMAL_X             885.0f
#define SELVA_ANIMAL_Y             445.0f
#define SELVA_ANIMAL_ESCALA        0.25f

#define SELVA_CACADOR_X            885.0f
#define SELVA_CACADOR_Y            435.0f
#define SELVA_CACADOR_ESCALA       0.30f

/* --- CENÁRIO: PANTANO --- */
#define PANTANO_JOGADOR_X          350.0f
#define PANTANO_JOGADOR_Y          425.0f
#define PANTANO_JOGADOR_ESCALA     0.45f

#define PANTANO_ANIMAL_X           890.0f
#define PANTANO_ANIMAL_Y           275.0f
#define PANTANO_ANIMAL_ESCALA      0.2f

#define PANTANO_CACADOR_X          900.0f
#define PANTANO_CACADOR_Y          250.0f
#define PANTANO_CACADOR_ESCALA     0.3f

/* --- CENÁRIO: LAGO --- */
#define LAGO_JOGADOR_X             375.0f
#define LAGO_JOGADOR_Y             425.0f
#define LAGO_JOGADOR_ESCALA        0.5f

#define LAGO_ANIMAL_X              935.0f
#define LAGO_ANIMAL_Y              450.0f
#define LAGO_ANIMAL_ESCALA         0.25f

#define LAGO_CACADOR_X             935.0f
#define LAGO_CACADOR_Y             400.0f
#define LAGO_CACADOR_ESCALA        0.30f

/* --- CENÁRIO: CERRADO --- */
#define CERRADO_JOGADOR_X          450.0f
#define CERRADO_JOGADOR_Y          450.0f
#define CERRADO_JOGADOR_ESCALA     0.45f

#define CERRADO_ANIMAL_X           940.0f
#define CERRADO_ANIMAL_Y           315.0f
#define CERRADO_ANIMAL_ESCALA      0.25f

#define CERRADO_CACADOR_X          925.0f
#define CERRADO_CACADOR_Y          315.0f
#define CERRADO_CACADOR_ESCALA     0.30f

/* === AJUSTES ESPECÍFICOS POR ANIMAL === */
/* Mico-leão-dourado: diminuir 50px na altura */
#define MICO_OFFSET_Y              50.0f

/* Harpia: aumentar escala */
#define HARPIA_ESCALA_EXTRA        0.10f  /* Adicional à escala base do cenário */

/* Jacaré: abaixar 35px na altura (aumentar Y) */
#define JACARE_OFFSET_Y            35.0f

/* --- CENÁRIO: BOSS (Caçador Chefe) --- */
#define BOSS_JOGADOR_X             200.0f
#define BOSS_JOGADOR_Y             170.0f
#define BOSS_JOGADOR_ESCALA        0.45f

#define BOSS_CACADOR_X             250.0f
#define BOSS_CACADOR_Y             325.0f
#define BOSS_CACADOR_ESCALA        0.35f

/* === MENSAGENS DO JOGO === */
/* Áreas Bloqueadas */
#define MSG_AREA_BLOQUEADA_1            "AREA BLOQUEADA - Estude a Onca na selva primeiro!"
#define MSG_AREA_BLOQUEADA_2            "AREA BLOQUEADA - Estude o Jacare no pantano primeiro!"
#define MSG_AREA_BLOQUEADA_3            "AREA BLOQUEADA - Estude o Boto no lago primeiro!"

/* Mensagens de Status */
#define MSG_ANIMAL_DESCANSO             "Animal em descanso - aguarde um momento!"
#define MSG_CACADOR_RECUPERACAO         "Cacador em recuperacao - aguarde para batalhar novamente!"
#define MSG_DERROTE_CACADOR             "! Derrote o cacador para estudar este animal !"
#define MSG_BOSS_NO_MAPA                "!!! CACADOR CHEFE NO MAPA !!!"

/* Mensagens de Progresso */
#define MSG_JOGO_COMPLETO               "JOGO COMPLETO! PARABENS!"
#define MSG_VOCE_VENCEU                 "VOCE VENCEU!"
#define MSG_VOCE_PERDEU                 "VOCE PERDEU!"
#define MSG_VOCE_DERROTADO              "VOCE FOI DERROTADO!"
#define MSG_ANIMAL_CAPTURADO            "Um animal foi capturado!"

/* NOVAS: Mensagens de Progressão por Cenário */
#define MSG_AREA_BLOQUEADA_1_NOVA       "AREA BLOQUEADA - Estude pelo menos 1 animal na Selva primeiro!"
#define MSG_AREA_BLOQUEADA_2_NOVA       "AREA BLOQUEADA - Estude pelo menos 1 animal no Pantano primeiro!"
#define MSG_AREA_BLOQUEADA_3_NOVA       "AREA BLOQUEADA - Estude pelo menos 1 animal no Lago primeiro!"
#define MSG_DERROTE_TODOS_CACADORES     "Derrote TODOS os cacadores do cenario para estudar!"
#define MSG_BOSS_REQUISITOS             "Boss: Estude TODOS os animais e derrote TODOS os cacadores!"

/* Mensagens de Interação */
#define MSG_PRESSIONE_ENTER             "Pressione ENTER"
#define MSG_PRESSIONE_ENTER_REINICIAR   "Pressione ENTER para reiniciar"
#define MSG_PRESSIONE_ENTER_CONTINUAR   "Pressione ENTER para continuar"

/* Formato de Contador */
#define FMT_CACADORES_DERROTADOS        "Cacadores Derrotados: %d/8"

/* === POSIÇÕES DE TEXTO NA TELA === */
/* Coordenadas Centrais */
#define POS_TEXTO_CENTRO_X              640
#define POS_TEXTO_CENTRO_Y              360

/* Mensagens de Topo */
#define POS_TEXTO_TOPO_Y                10
#define POS_TEXTO_MSG_ANIMAL_Y          40
#define POS_TEXTO_MSG_CACADOR_Y         70

/* Mensagens de Rodapé */
#define POS_TEXTO_BOSS_Y                700
#define POS_TEXTO_BLOQUEIO_Y            680

/* Mensagens de Vitória/Derrota */
#define POS_TEXTO_VITORIA_Y             350
#define POS_TEXTO_DERROTA_TITULO_Y      300
#define POS_TEXTO_DERROTA_DESC_Y        350
#define POS_TEXTO_DERROTA_ENTER_Y       420

/* Contador de Progresso */
#define POS_TEXTO_CONTADOR_X            10
#define POS_TEXTO_CONTADOR_Y            10

/* === CORES DE TEXTO (RGB) === */
/* Amarelo */
#define COR_TEXTO_AMARELO_R             255
#define COR_TEXTO_AMARELO_G             255
#define COR_TEXTO_AMARELO_B             0

/* Laranja */
#define COR_TEXTO_LARANJA_R             255
#define COR_TEXTO_LARANJA_G             200
#define COR_TEXTO_LARANJA_B             100

/* Rosa */
#define COR_TEXTO_ROSA_R                255
#define COR_TEXTO_ROSA_G                150
#define COR_TEXTO_ROSA_B                150

/* Vermelho */
#define COR_TEXTO_VERMELHO_R            255
#define COR_TEXTO_VERMELHO_G            100
#define COR_TEXTO_VERMELHO_B            100

/* Verde */
#define COR_TEXTO_VERDE_R               0
#define COR_TEXTO_VERDE_G               255
#define COR_TEXTO_VERDE_B               0

/* Magenta */
#define COR_TEXTO_MAGENTA_R             255
#define COR_TEXTO_MAGENTA_G             0
#define COR_TEXTO_MAGENTA_B             255

/* Branco */
#define COR_TEXTO_BRANCO_R              255
#define COR_TEXTO_BRANCO_G              255
#define COR_TEXTO_BRANCO_B              255

/* Preto */
#define COR_TEXTO_PRETO_R               0
#define COR_TEXTO_PRETO_G               0
#define COR_TEXTO_PRETO_B               0

/* Cinza */
#define COR_TEXTO_CINZA_R               120
#define COR_TEXTO_CINZA_G               120
#define COR_TEXTO_CINZA_B               120

#define COR_TEXTO_CINZA_CLARO_R         200
#define COR_TEXTO_CINZA_CLARO_G         200
#define COR_TEXTO_CINZA_CLARO_B         200

/* Ciano */
#define COR_TEXTO_CIANO_R               0
#define COR_TEXTO_CIANO_G               255
#define COR_TEXTO_CIANO_B               255

/* Ouro */
#define COR_TEXTO_OURO_R                255
#define COR_TEXTO_OURO_G                200
#define COR_TEXTO_OURO_B                0

/* Verde Claro */
#define COR_TEXTO_VERDE_CLARO_R         0
#define COR_TEXTO_VERDE_CLARO_G         200
#define COR_TEXTO_VERDE_CLARO_B         150

/* === CORES DE FUNDO (RGB) === */
/* Vitória */
#define COR_FUNDO_VITORIA_R             0
#define COR_FUNDO_VITORIA_G             50
#define COR_FUNDO_VITORIA_B             0

/* Derrota */
#define COR_FUNDO_DERROTA_R             50
#define COR_FUNDO_DERROTA_G             0
#define COR_FUNDO_DERROTA_B             0

#define COR_FUNDO_DERROTA_TELA_R        30
#define COR_FUNDO_DERROTA_TELA_G        0
#define COR_FUNDO_DERROTA_TELA_B        0

/* Preto */
#define COR_FUNDO_PRETO_R               0
#define COR_FUNDO_PRETO_G               0
#define COR_FUNDO_PRETO_B               0

#endif // CONFIG_UI_H