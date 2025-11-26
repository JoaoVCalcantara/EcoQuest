#ifndef CONFIG_UI_H
#define CONFIG_UI_H

/* === CONFIGURAÇÕES DE UI / BATALHA === */

/* === CONFIGURAÇÕES DE UI / BATALHA === */

/* Fallback relativo do jogador na tela de batalha (0.0 - 1.0) */
#define BATALHA_JOGADOR_X_RATIO    0.0f
#define BATALHA_JOGADOR_Y_RATIO    0.0f

/* Escala padrão do sprite do jogador na tela de batalha (fallback) */
#define BATALHA_JOGADOR_ESCALA     0.30f

/* Caixa de texto - escala e offset padrão (pixels) */
#define BATALHA_CAIXA_ESCALA       0.30f
#define BATALHA_CAIXA_OFFSET_Y     10.0f
#define BATALHA_CAIXA_MIN_MARGIN_X 10.0f
#define BATALHA_CAIXA_MIN_MARGIN_Y 5.0f

/* === PADRÃO DE POSICIONAMENTO E ESCALA (ANIMAIS / CAÇADORES / JOGADOR) === */
/* Valores em pixels para facilitar ajuste visual na tela de batalha. */

/* --- Animais (ConfigSpriteEntidade) --- */
/* Onça */
#define ENT_ONCA_X                  250.0f
#define ENT_ONCA_Y                  325.0f
#define ENT_ONCA_ESCALA             0.25f

/* Jacaré */
#define ENT_JACARE_X                385.0f
#define ENT_JACARE_Y                175.0f
#define ENT_JACARE_ESCALA           0.25f

/* Boto */
#define ENT_BOTO_X                  280.0f
#define ENT_BOTO_Y                  320.0f
#define ENT_BOTO_ESCALA             0.25f

/* Lobo (inclui "lobo guara") */
#define ENT_LOBO_X                  250.0f
#define ENT_LOBO_Y                  325.0f
#define ENT_LOBO_ESCALA             0.25f

/* --- Caçadores (ConfigSpriteEntidade) --- */
/* Caçador Selva */
#define ENT_CACADOR_SELVA_X         250.0f
#define ENT_CACADOR_SELVA_Y         300.0f
#define ENT_CACADOR_SELVA_ESCALA    0.25f

/* Caçador Pantano */
#define ENT_CACADOR_PANTANO_X       255.0f
#define ENT_CACADOR_PANTANO_Y       125.0f
#define ENT_CACADOR_PANTANO_ESCALA  0.25f

/* Caçador Lago */
#define ENT_CACADOR_LAGO_X          280.0f
#define ENT_CACADOR_LAGO_Y          320.0f
#define ENT_CACADOR_LAGO_ESCALA     0.25f

/* Caçador Cerrado */
#define ENT_CACADOR_CERRADO_X       250.0f
#define ENT_CACADOR_CERRADO_Y       200.0f
#define ENT_CACADOR_CERRADO_ESCALA  0.25f

/* Caçador Chefe */
#define ENT_CACADOR_CHEFE_X         250.0f
#define ENT_CACADOR_CHEFE_Y         325.0f
#define ENT_CACADOR_CHEFE_ESCALA    0.30f

/* === Posição/escala do JOGADOR por animal (ConfigSpriteJogador) === */
/* Se o valor de X/Y for <= 0 o código usará os ratios de BATALHA_JOGADOR_* */
/* Valores migrados de batalha.c (configs_jogador) */

#define JOGADOR_ONCA_X              380.0f
#define JOGADOR_ONCA_Y              425.0f
#define JOGADOR_ONCA_ESCALA         0.50f

#define JOGADOR_JACARE_X            400.0f
#define JOGADOR_JACARE_Y            400.0f
#define JOGADOR_JACARE_ESCALA       0.50f

#define JOGADOR_BOTO_X              175.0f
#define JOGADOR_BOTO_Y              175.0f
#define JOGADOR_BOTO_ESCALA         0.50f

#define JOGADOR_LOBO_X              150.0f
#define JOGADOR_LOBO_Y              200.0f
#define JOGADOR_LOBO_ESCALA         0.50f

#define JOGADOR_LOBO_GUARA_X        100.0f
#define JOGADOR_LOBO_GUARA_Y        150.0f
#define JOGADOR_LOBO_GUARA_ESCALA   0.50f

/* === Posição/escala do JOGADOR por CAÇADOR (NOVO) ===
   Permite ajustar X/Y/escala do jogador especificamente quando enfrenta
   um caçador — valores em pixels. Se algum for <= 0 usa os ratios. */
#define JOGADOR_CACADOR_SELVA_X         380.0f
#define JOGADOR_CACADOR_SELVA_Y         450.0f
#define JOGADOR_CACADOR_SELVA_ESCALA    0.40f

#define JOGADOR_CACADOR_PANTANO_X       400.0f
#define JOGADOR_CACADOR_PANTANO_Y       430.0f
#define JOGADOR_CACADOR_PANTANO_ESCALA  0.40f

#define JOGADOR_CACADOR_LAGO_X          170.0f
#define JOGADOR_CACADOR_LAGO_Y          180.0f
#define JOGADOR_CACADOR_LAGO_ESCALA     0.40f

#define JOGADOR_CACADOR_CERRADO_X       160.0f
#define JOGADOR_CACADOR_CERRADO_Y       150.0f
#define JOGADOR_CACADOR_CERRADO_ESCALA  0.40f

#define JOGADOR_CACADOR_CHEFE_X         200.0f
#define JOGADOR_CACADOR_CHEFE_Y         170.0f
#define JOGADOR_CACADOR_CHEFE_ESCALA    0.45f



/* === MENSAGENS, POSIÇÕES E CORES (mantidos) === */

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

/* Mensagens de Interação */
#define MSG_PRESSIONE_ENTER             "Pressione ENTER"
#define MSG_PRESSIONE_ENTER_REINICIAR   "Pressione ENTER para reiniciar"
#define MSG_PRESSIONE_ENTER_CONTINUAR   "Pressione ENTER para continuar"

/* Formato de Contador */
#define FMT_CACADORES_DERROTADOS        "Cacadores Derrotados: %d/4"

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