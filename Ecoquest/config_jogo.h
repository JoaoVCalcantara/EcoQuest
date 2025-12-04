#ifndef CONFIG_JOGO_H
#define CONFIG_JOGO_H

/* ========== CONFIGURACOES GLOBAIS DO JOGO ========== */

/* === DISPLAY E RESOLUCAO === */
#define LARGURA_TELA                    1280
#define ALTURA_TELA                     720
#define LARGURA_MAPA                    1280.0f
#define ALTURA_MAPA                     720.0f
#define FPS_JOGO                        144
#define ZOOM_FACTOR                     3.5f

/* === DIVISOES DO MAPA (4 CENARIOS) === */
#define LIMITE_HORIZONTAL_MAPA          640.0f
#define LIMITE_VERTICAL_MAPA            360.0f

/* Cenario 1 - Superior Esquerdo (Selva) */
#define CENARIO1_MIN_X                  0.0f
#define CENARIO1_MAX_X                  640.0f
#define CENARIO1_MIN_Y                  0.0f
#define CENARIO1_MAX_Y                  360.0f

/* Cenario 2 - Inferior Esquerdo (Pantano) */
#define CENARIO2_MIN_X                  0.0f
#define CENARIO2_MAX_X                  640.0f
#define CENARIO2_MIN_Y                  360.0f
#define CENARIO2_MAX_Y                  720.0f

/* Cenario 3 - Superior Direito (Lago) */
#define CENARIO3_MIN_X                  640.0f
#define CENARIO3_MAX_X                  1280.0f
#define CENARIO3_MIN_Y                  0.0f
#define CENARIO3_MAX_Y                  360.0f

/* Cenario 4 - Inferior Direito (Cerrado) */
#define CENARIO4_MIN_X                  640.0f
#define CENARIO4_MAX_X                  1280.0f
#define CENARIO4_MIN_Y                  360.0f
#define CENARIO4_MAX_Y                  720.0f

/* === JOGADOR === */
#define RAIO_JOGADOR                    12.0f
#define VELOCIDADE_JOGADOR              1.7f
#define VIDA_MAXIMA_JOGADOR             100

/* Animacao do Jogador */
#define TEMPO_FRAME_MOVIMENTO           0.2f
#define TEMPO_FRAME_IDLE                1.0f

/* === ANIMAIS (BOTS) === */
#define RAIO_ANIMAL                     10.0f
#define VELOCIDADE_ANIMAL               0.6f
#define VIDA_MAXIMA_ANIMAL              100
#define VELOCIDADE_FUGA_MULTIPLICADOR   1.7f

/* Tempo de mudanca de direcao */
#define TEMPO_MUDANCA_MIN               1.5f
#define TEMPO_MUDANCA_MAX               5.0f
#define TEMPO_MUDANCA_VARIACAO          3.5f    // MAX - MIN
#define TEMPO_FUGA_ANIMAL               2.5f
#define TEMPO_MUDANCA_BORDA             0.3f
#define TEMPO_MUDANCA_NOVA_DIRECAO      0.1f

/* Raio de fuga e detecao */
#define RAIO_FUGA_ANIMAL                120.0f
#define DISTANCIA_TESTE_DIRECAO         50.0f
#define MARGEM_BORDA_ANIMAL             30.0f

/* Cooldown de colisao */
#define COOLDOWN_COLISAO_ANIMAL         6.0f

/* Tentativas de direcao */
#define TENTATIVAS_DIRECAO_ALEATORIA    3

/* Peso de score de direcao */
#define SCORE_DIRECAO_VALIDA            1.0f

/* === SISTEMA DE VISAO DOS ANIMAIS === */
/* Raios de visao */
#define RAIO_VISAO_PADRAO               150.0f  // Distancia padrao que animais enxergam
#define RAIO_VISAO_PREDADOR             180.0f  // Predadores veem mais longe
#define RAIO_VISAO_PRESA                140.0f  // Presas tem visao um pouco menor

/* Angulos de visao (em radianos) */
#define ANGULO_VISAO_PADRAO             2.0f    // ~114 graus (cone amplo)
#define ANGULO_VISAO_FRONTAL            1.57f   // 90 graus (?/2)
#define ANGULO_VISAO_COMPLETO           6.28f   // 360 graus (2?)

/* Detecao de ameacas */
#define TEMPO_MEMORIA_AMEACA            3.0f    // Tempo que lembra da ameaca
#define DISTANCIA_SEGURA                200.0f  // Distancia considerada segura
#define DISTANCIA_PERIGO_IMINENTE       60.0f   // Perigo proximo (fuga imediata)

/* === ESTADOS DA IA === */
/* Duracoes minimas em cada estado */
#define TEMPO_MIN_PATRULHA              2.0f
#define TEMPO_MIN_ALERTA                1.0f
#define TEMPO_MIN_FUGA                  3.0f
#define TEMPO_MIN_ESCONDIDO             4.0f
#define TEMPO_MIN_RETORNANDO            2.0f

/* Chances de transicao */
#define CHANCE_PARAR_PATRULHA           0.15f   // 15% chance de parar ao patrulhar
#define TEMPO_PARADO_MIN                1.0f
#define TEMPO_PARADO_MAX                3.0f

/* Modificadores de velocidade por estado */
#define VEL_MULTIPLICADOR_PATRULHA      1.0f    // Normal
#define VEL_MULTIPLICADOR_ALERTA        0.7f    // Mais devagar
#define VEL_MULTIPLICADOR_FUGA          1.7f    // Rapido
#define VEL_MULTIPLICADOR_ESCONDIDO     0.0f    // Parado
#define VEL_MULTIPLICADOR_RETORNANDO    1.2f    // Um pouco rapido

/* === ELIPSE DO LAGO (BOTO) === */
#define ELIPSE_BOTO_CENTRO_X            950.0f
#define ELIPSE_BOTO_CENTRO_Y            205.0f
#define ELIPSE_BOTO_RAIO_H              165.0f
#define ELIPSE_BOTO_RAIO_V              105.0f

/* Margens de seguranca da elipse */
#define ELIPSE_MARGEM_VALIDACAO         0.90f
#define ELIPSE_LIMITE_CORRECAO          0.85f
#define ELIPSE_PERTO_BORDA              0.75f
#define EPSILON_ELIPSE                  1e-3f

/* Forca de correcao */
#define ELIPSE_FORCA_CORRECAO           3.0f

/* === COLISOES E INTERACOES === */
#define DISTANCIA_DETECCAO_MENSAGEM     50.0f
#define TEMPO_EXIBICAO_MENSAGEM         2.0f
#define EMPURRAO_JOGADOR_CACADOR        25.0f
#define EMPURRAO_JOGADOR_BOSS           30.0f

/* === PROGRESSAO DO JOGO === */
#define CACADORES_PARA_BOSS             8
#define TOTAL_ANIMAIS                   9
#define TOTAL_CACADORES                 8

/* === CORES DE VIDA (RGB) === */
/* Vida Alta (> 60%) */
#define COR_VIDA_ALTA_R                 50
#define COR_VIDA_ALTA_G                 200
#define COR_VIDA_ALTA_B                 50

/* Vida Media (30% - 60%) */
#define COR_VIDA_MEDIA_R                255
#define COR_VIDA_MEDIA_G                200
#define COR_VIDA_MEDIA_B                0

/* Vida Baixa (< 30%) */
#define COR_VIDA_BAIXA_R                255
#define COR_VIDA_BAIXA_G                50
#define COR_VIDA_BAIXA_B                50

/* Limiares de vida */
#define LIMIAR_VIDA_ALTA                0.6f
#define LIMIAR_VIDA_MEDIA               0.3f

/* === CAMINHOS DE ASSETS === */
/* Estruturas */
#define PATH_ESTRUTURA_SELVA            "assets/img/estruturas/selva.png"
#define PATH_ESTRUTURA_PANTANO          "assets/img/estruturas/pantano.png"
#define PATH_ESTRUTURA_LAGO             "assets/img/estruturas/lago.png"
#define PATH_ESTRUTURA_CERRADO          "assets/img/estruturas/cerrado.png"
#define PATH_CAIXA_TEXTO                "assets/img/estruturas/caixa_de_texto.png"

/* Animais */
#define PATH_ANIMAL_ONCA                "assets/img/animais/onca.png"
#define PATH_ANIMAL_JACARE              "assets/img/animais/jacare.png"
#define PATH_ANIMAL_BOTO                "assets/img/animais/boto.png"
#define PATH_ANIMAL_LOBO                "assets/img/animais/lobo_guara.png"
#define PATH_ANIMAL_HARPIA              "assets/img/animais/harpia.png"
#define PATH_ANIMAL_MICO                "assets/img/animais/mico_leao_dourado.png"
#define PATH_ANIMAL_CAPIVARA            "assets/img/animais/capivara.png"
#define PATH_ANIMAL_PIRARUCU            "assets/img/animais/pirarucu.png"
#define PATH_ANIMAL_TAMANDUA            "assets/img/animais/tamandua_bandeira.png"

/* Cacadores */
#define PATH_CACADOR_NORMAL             "assets/img/cacador/cacador.png"
#define PATH_CACADOR_BOSS               "assets/img/cacador/boss_cacador.png"

/* Jogador - Movimento */
#define PATH_JOGADOR_CIMA_0             "assets/img/Heroi/rear_0.png"
#define PATH_JOGADOR_CIMA_1             "assets/img/Heroi/rear_1.png"
#define PATH_JOGADOR_BAIXO_0            "assets/img/Heroi/front_0.png"
#define PATH_JOGADOR_BAIXO_1            "assets/img/Heroi/front_1.png"
#define PATH_JOGADOR_DIR_0              "assets/img/Heroi/player_right_0.png"
#define PATH_JOGADOR_DIR_1              "assets/img/Heroi/player_right_1.png"
#define PATH_JOGADOR_ESQ_0              "assets/img/Heroi/player_left_0.png"
#define PATH_JOGADOR_ESQ_1              "assets/img/Heroi/player_left_1.png"

/* Jogador - Idle */
#define PATH_JOGADOR_IDLE_BAIXO         "assets/img/Heroi/idle_down.png"
// #define PATH_JOGADOR_IDLE_CIMA          "assets/img/Heroi/idle_up.png"  // Arquivo nao existe - fallback usa rear_0.png
#define PATH_JOGADOR_IDLE_CIMA          "assets/img/Heroi/rear_0.png"  // Usando frame do movimento como idle
#define PATH_JOGADOR_IDLE_DIR           "assets/img/Heroi/idle_right.png"
#define PATH_JOGADOR_IDLE_ESQ           "assets/img/Heroi/idle_left.png"

/* === ESCALAS DE SPRITES === */
#define ESCALA_ANIMAL                   0.04f
#define ESCALA_CACADOR                  0.05f
#define ESCALA_SPRITE_JOGADOR          0.06f

/* === POSICOES INICIAIS === */
/* Animais */
#define POS_INICIAL_ONCA_X              300.0f
#define POS_INICIAL_ONCA_Y              200.0f

#define POS_INICIAL_JACARE_X            400.0f
#define POS_INICIAL_JACARE_Y            500.0f

#define POS_INICIAL_BOTO_X              1000.0f
#define POS_INICIAL_BOTO_Y              150.0f

#define POS_INICIAL_LOBO_X              1000.0f
#define POS_INICIAL_LOBO_Y              600.0f

/* Novos Animais */
/* SELVA - 2 novos */
#define POS_INICIAL_HARPIA_X            250.0f
#define POS_INICIAL_HARPIA_Y            100.0f

#define POS_INICIAL_MICO_X              450.0f
#define POS_INICIAL_MICO_Y              250.0f

/* PANTANO - 1 novo */
#define POS_INICIAL_CAPIVARA_X          250.0f
#define POS_INICIAL_CAPIVARA_Y          550.0f

/* LAGO - 1 novo */
#define POS_INICIAL_PIRARUCU_X          920.0f  // CORRIGIDO: posicao dentro da elipse (centro: 950, 205)
#define POS_INICIAL_PIRARUCU_Y          180.0f  // CORRIGIDO: posicao valida dentro da area permitida

/* CERRADO - 1 novo */
#define POS_INICIAL_TAMANDUA_X          900.0f
#define POS_INICIAL_TAMANDUA_Y          500.0f

/* Cacadores */
#define POS_INICIAL_CACADOR1_X          200.0f
#define POS_INICIAL_CACADOR1_Y          200.0f

#define POS_INICIAL_CACADOR2_X          300.0f
#define POS_INICIAL_CACADOR2_Y          600.0f

#define POS_INICIAL_CACADOR3_X          900.0f
#define POS_INICIAL_CACADOR3_Y          100.0f

#define POS_INICIAL_CACADOR4_X          1100.0f
#define POS_INICIAL_CACADOR4_Y          500.0f

/* Novos Cacadores */
#define POS_INICIAL_CACADOR5_X          150.0f
#define POS_INICIAL_CACADOR5_Y          300.0f

#define POS_INICIAL_CACADOR6_X          500.0f
#define POS_INICIAL_CACADOR6_Y          450.0f

#define POS_INICIAL_CACADOR7_X          750.0f
#define POS_INICIAL_CACADOR7_Y          250.0f

#define POS_INICIAL_CACADOR8_X          950.0f
#define POS_INICIAL_CACADOR8_Y          550.0f

/* Boss */
#define POS_INICIAL_BOSS_X              640.0f
#define POS_INICIAL_BOSS_Y              360.0f

/* === NOMES DAS ENTIDADES === */
#define NOME_ANIMAL_ONCA                "onca"
#define NOME_ANIMAL_JACARE              "Jacare"
#define NOME_ANIMAL_BOTO                "Boto"
#define NOME_ANIMAL_LOBO                "Lobo guara"
#define NOME_ANIMAL_HARPIA              "Harpia"
#define NOME_ANIMAL_MICO                "Mico-leao dourado"
#define NOME_ANIMAL_CAPIVARA            "Capivara"
#define NOME_ANIMAL_PIRARUCU            "Pirarucu"
#define NOME_ANIMAL_TAMANDUA            "Tamandua-bandeira"

#define NOME_CACADOR_SELVA              "Cacador da Selva"
#define NOME_CACADOR_PANTANO            "Cacador do Pantano"
#define NOME_CACADOR_LAGO               "Cacador do Lago"
#define NOME_CACADOR_CERRADO            "Cacador do Cerrado"
#define NOME_CACADOR_CHEFE              "Cacador Chefe"
#define NOME_CACADOR_SELVA2             "Cacador da Selva 2"
#define NOME_CACADOR_PANTANO2           "Cacador do Pantano 2"
#define NOME_CACADOR_LAGO2              "Cacador do Lago 2"
#define NOME_CACADOR_CERRADO2           "Cacador do Cerrado 2"

/* === TIPOS DE ANIMAIS === */
#define TIPO_CARNIVORO                  "Carnivoro"
#define TIPO_ONIVORO                    "Onivoro"
#define TIPO_HERBIVORO                  "Herbivoro"
#define TIPO_INSETIVORO                 "Insetivoro"

/* === BESTIARIO === */
#define MAX_ESPECIES_BESTIARIO          10  // 10 animais + 1 humano

/* === PROXIMIDADES E DISTANCIAS === */
#define DISTANCIA_VERIFICACAO_BLOQUEIO  20.0f   // Distancia para verificar bloqueio de areas
#define TOLERANCIA_ELIPSE_BLOQUEIO      20.0f   // Tolerancia adicional para elipse do Cenario 3

/* === CORES DE DESENHO (RGB) === */
/* Cores de entidades no mapa */
#define COR_ANIMAL_DESENHO_R            255
#define COR_ANIMAL_DESENHO_G            50
#define COR_ANIMAL_DESENHO_B            50

#define COR_CACADOR_DESENHO_R           150
#define COR_CACADOR_DESENHO_G           0
#define COR_CACADOR_DESENHO_B           0

#define COR_BOSS_DESENHO_R              200
#define COR_BOSS_DESENHO_G              0
#define COR_BOSS_DESENHO_B              200

/* === POSICOES DE TEXTO LABEL BOSS === */
#define OFFSET_LABEL_BOSS_Y             15.0f

#endif // CONFIG_JOGO_H