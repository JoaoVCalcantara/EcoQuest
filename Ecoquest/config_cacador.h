#ifndef CONFIG_CACADOR_H
#define CONFIG_CACADOR_H

/* ========== CONFIGURAÇÕES CENTRALIZADAS DO CAÇADOR ========== */

/* Cooldowns e Timers */
#define COOLDOWN_DANO_CACADOR           2.3f    // Intervalo entre danos ao animal
#define COOLDOWN_BATALHA_CACADOR        5.0f    // Cooldown após batalha com jogador
#define TEMPO_ESPERA_PATRULHA           2.0f    // Tempo de espera no ponto de patrulha

/* Dano e Combate */
#define DANO_POR_CONTATO_PADRAO         15      // Dano causado ao animal por contato
#define DANO_POR_CONTATO_BOSS           20      // Dano do boss
#define RAIO_COLISAO_CACADOR_ANIMAL     13.0f   // Raio extra para detecção de colisão

/* Detecção e Perseguição */
#define RAIO_DETECCAO_PADRAO            25.0f  // Raio de detecção do animal
#define RAIO_ABANDONO_PADRAO            50.0f  // Raio para abandonar perseguição (2x detecção)
#define RAIO_DETECCAO_BOSS              200.0f  // Raio de detecção do boss

/* Velocidades */
#define VELOCIDADE_PATRULHA             0.7f    // Velocidade durante patrulha
#define VELOCIDADE_PERSEGUICAO          1.3f    // Velocidade ao perseguir animal
#define VELOCIDADE_PERSEGUICAO_BOSS     1.4f    // Velocidade do boss

/* Margem de Segurança para Fuga do Animal */
#define MARGEM_SEGURANCA_BORDA          100.0f   // Distância da borda para animal evitar
#define PESO_FUGA_CENTRO                0.30f   // Peso da direção ao centro (0.0-1.0)

/* Distâncias Mínimas */
#define DISTANCIA_MINIMA_PERSEGUICAO    15.0f   // Distância mínima para parar de perseguir
#define DISTANCIA_OBJETIVO_PATRULHA     10.0f    // Distância para considerar chegada ao ponto

/* Margem de Patrulha */
#define MARGEM_PATRULHA                 50.0f   // Margem das bordas para pontos de patrulha

/* Força de Repulsão */
#define FORCA_REPULSAO_CACADOR          2.0f    // Força ao empurrar caçador para trás

/* Sistema de Vida dos Animais */
#define VIDA_VISUAL_MAXIMA_ANIMAL       100     // Vida visual máxima de todos os animais

/* Atributos de Batalha */
#define VIDA_MAXIMA_CACADOR_NORMAL      100     // Vida dos caçadores normais
#define VIDA_MAXIMA_CACADOR_BOSS        200     // Vida do boss
#define ATAQUE_BASE_CACADOR             20      // Ataque base dos caçadores
#define ATAQUE_BASE_BOSS                25      // Ataque do boss
#define DEFESA_BASE_CACADOR             10      // Defesa base dos caçadores
#define DEFESA_BASE_BOSS                15      // Defesa do boss

/* Raios Físicos */
#define RAIO_CACADOR                    13.0f   // Raio físico do caçador

/* Multiplicadores de Comportamento */
#define MULTIPLICADOR_FUGA_ANIMAL       1.8f    // Multiplicador de velocidade do animal em fuga

#endif // CONFIG_CACADOR_H