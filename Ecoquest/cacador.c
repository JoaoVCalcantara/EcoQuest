#define _CRT_SECURE_NO_WARNINGS

#include "cacador.h"
#include "inimigo.h"
#include "config_cacador.h"
#include "config_jogo.h"      // NOVO: Configurações do jogo
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <allegro5/allegro_primitives.h>

extern void marcar_reinicio_jogo();

// ========== FUNÇÕES AUXILIARES ==========

static void sincronizar_dados_cacador(Cacador* cacador) {
    cacador->vida = cacador->dados_batalha.vida_atual;
    cacador->vida_maxima = cacador->dados_batalha.vida_maxima;
    cacador->dano_ataque = cacador->dados_batalha.ataque_base;
    cacador->nome = cacador->dados_batalha.nome;
    cacador->derrotado = cacador->dados_batalha.derrotado;
    cacador->distancia_deteccao = cacador->raio_deteccao;
}

static void sincronizar_dados_para_batalha(Cacador* cacador) {
    cacador->dados_batalha.vida_atual = cacador->vida;
    cacador->dados_batalha.derrotado = cacador->derrotado;
}

static void limitar_posicao_cacador(float* x, float* y, float raio,
    float largura, float altura, JogoCenas cenario) {

    if (*x < raio) *x = raio;
    if (*y < raio) *y = raio;
    if (*x > largura - raio) *x = largura - raio;
    if (*y > altura - raio) *y = altura - raio;

    float min_x = 0.0f, max_x = largura;
    float min_y = 0.0f, max_y = altura;

    switch (cenario) {
    case CENARIO1:
        max_x = LIMITE_HORIZONTAL_MAPA;
        max_y = LIMITE_VERTICAL_MAPA;
        break;
    case CENARIO2:
        min_y = LIMITE_VERTICAL_MAPA;
        max_x = LIMITE_HORIZONTAL_MAPA;
        max_y = altura;
        break;
    case CENARIO3:
        min_x = LIMITE_HORIZONTAL_MAPA;
        max_x = largura;
        max_y = LIMITE_VERTICAL_MAPA;
        break;
    case CENARIO4:
        min_x = LIMITE_HORIZONTAL_MAPA;
        min_y = LIMITE_VERTICAL_MAPA;
        break;
    }

    if (*x < min_x + raio) *x = min_x + raio;
    if (*y < min_y + raio) *y = min_y + raio;
    if (*x > max_x - raio) *x = max_x - raio;
    if (*y > max_y - raio) *y = max_y - raio;
}

// Função para evitar que o animal fuja para as bordas
static void evitar_bordas_animal(float animal_x, float animal_y, float* fuga_x, float* fuga_y,
    float largura_mapa, float altura_mapa, JogoCenas cenario) {

    float min_x = 0.0f, max_x = largura_mapa;
    float min_y = 0.0f, max_y = altura_mapa;

    switch (cenario) {
    case CENARIO1:
        max_x = LIMITE_HORIZONTAL_MAPA;
        max_y = LIMITE_VERTICAL_MAPA;
        break;
    case CENARIO2:
        min_y = LIMITE_VERTICAL_MAPA;
        max_x = LIMITE_HORIZONTAL_MAPA;
        max_y = altura_mapa;
        break;
    case CENARIO3:
        min_x = LIMITE_HORIZONTAL_MAPA;
        max_x = largura_mapa;
        max_y = LIMITE_VERTICAL_MAPA;
        break;
    case CENARIO4:
        min_x = LIMITE_HORIZONTAL_MAPA;
        min_y = LIMITE_VERTICAL_MAPA;
        break;
    }

    bool perto_borda_esquerda = animal_x < (min_x + MARGEM_SEGURANCA_BORDA);
    bool perto_borda_direita = animal_x > (max_x - MARGEM_SEGURANCA_BORDA);
    bool perto_borda_cima = animal_y < (min_y + MARGEM_SEGURANCA_BORDA);
    bool perto_borda_baixo = animal_y > (max_y - MARGEM_SEGURANCA_BORDA);

    if (perto_borda_esquerda || perto_borda_direita || perto_borda_cima || perto_borda_baixo) {
        float centro_x = (min_x + max_x) / 2.0f;
        float centro_y = (min_y + max_y) / 2.0f;

        float dx_centro = centro_x - animal_x;
        float dy_centro = centro_y - animal_y;
        float dist_centro = sqrtf(dx_centro * dx_centro + dy_centro * dy_centro);

        if (dist_centro > 0.1f) {
            *fuga_x = (*fuga_x * (1.0f - PESO_FUGA_CENTRO)) + (dx_centro / dist_centro * PESO_FUGA_CENTRO);
            *fuga_y = (*fuga_y * (1.0f - PESO_FUGA_CENTRO)) + (dy_centro / dist_centro * PESO_FUGA_CENTRO);

            float mag = sqrtf((*fuga_x) * (*fuga_x) + (*fuga_y) * (*fuga_y));
            if (mag > 0.1f) {
                *fuga_x /= mag;
                *fuga_y /= mag;
            }
        }
    }
}

// ========== INICIALIZAÇÃO ==========

void iniciar_cacador(Cacador* cacador, float x, float y, const char* nome,
    JogoCenas cenario, const char* sprite_path,
    const char* fundo_batalha_path) {

    cacador->x = x;
    cacador->y = y;
    cacador->raio = RAIO_CACADOR;
    cacador->velocidade = VELOCIDADE_PATRULHA;
    cacador->velocidade_perseguicao = VELOCIDADE_PERSEGUICAO;

    cacador->estado = CACADOR_PATRULHANDO;
    cacador->cenario = cenario;

    cacador->alvo = NULL;
    cacador->patrulha_x = x;
    cacador->patrulha_y = y;
    cacador->tempo_espera = 0.0f;

    cacador->ativo = true;
    cacador->derrotado = false;
    cacador->raio_deteccao = RAIO_DETECCAO_PADRAO;
    cacador->raio_abandono = RAIO_ABANDONO_PADRAO;
    cacador->distancia_deteccao = cacador->raio_deteccao;

    cacador->cooldown_batalha = 0.0f;
    cacador->cooldown_dano = 0.0f;
    cacador->tempo_dano_interval = COOLDOWN_DANO_CACADOR;
    cacador->dano_por_contato = DANO_POR_CONTATO_PADRAO;

    cacador->dados_batalha.nome = nome;
    cacador->dados_batalha.vida_maxima = VIDA_MAXIMA_CACADOR_NORMAL;
    cacador->dados_batalha.vida_atual = VIDA_MAXIMA_CACADOR_NORMAL;
    cacador->dados_batalha.ataque_base = ATAQUE_BASE_CACADOR;
    cacador->dados_batalha.defesa_base = DEFESA_BASE_CACADOR;
    cacador->dados_batalha.derrotado = false;
    cacador->dados_batalha.vezes_derrotado = 0;
    cacador->dados_batalha.caminho_sprite = sprite_path;
    cacador->dados_batalha.caminho_fundo_batalha = fundo_batalha_path;

    sincronizar_dados_cacador(cacador);

    cacador->sprite_cacador = NULL;
    cacador->usar_sprite = false;
    cacador->escala_sprite = ESCALA_CACADOR;

    if (sprite_path) {
        cacador->sprite_cacador = criar_sprite(sprite_path);
        cacador->usar_sprite = (cacador->sprite_cacador && cacador->sprite_cacador->carregado);
    }
}

void iniciar_cacador_com_alvo(Cacador* cacador, float x, float y,
    JogoCenas cenario, Bot* alvo, const char* nome,
    const char* sprite_path) {

    const char* fundo = PATH_ESTRUTURA_SELVA;

    switch (cenario) {
    case CENARIO1: fundo = PATH_ESTRUTURA_SELVA; break;
    case CENARIO2: fundo = PATH_ESTRUTURA_PANTANO; break;
    case CENARIO3: fundo = PATH_ESTRUTURA_LAGO; break;
    case CENARIO4: fundo = PATH_ESTRUTURA_CERRADO; break;
    }

    iniciar_cacador(cacador, x, y, nome, cenario, sprite_path, fundo);
    cacador->alvo = alvo;
}

void iniciar_cacador_chefe(CacadorChefe* chefe, float x, float y,
    JogoCenas cenario, const char* sprite_path,
    const char* fundo_batalha_path) {

    iniciar_cacador(&chefe->base, x, y, NOME_CACADOR_CHEFE, cenario,
        sprite_path, fundo_batalha_path);

    chefe->base.dados_batalha.vida_maxima = VIDA_MAXIMA_CACADOR_BOSS;
    chefe->base.dados_batalha.vida_atual = VIDA_MAXIMA_CACADOR_BOSS;
    chefe->base.dados_batalha.ataque_base = ATAQUE_BASE_BOSS;
    chefe->base.dados_batalha.defesa_base = DEFESA_BASE_BOSS;
    chefe->base.velocidade_perseguicao = VELOCIDADE_PERSEGUICAO_BOSS;
    chefe->base.raio_deteccao = RAIO_DETECCAO_BOSS;
    chefe->base.dano_por_contato = DANO_POR_CONTATO_BOSS;

    sincronizar_dados_cacador(&chefe->base);

    chefe->desbloqueado = false;
    chefe->liberado = false;
    chefe->derrotado = false;
    chefe->base.ativo = false;
    chefe->x_spawn = x;
    chefe->y_spawn = y;
    chefe->cenario_spawn = cenario;
}

// ========== DANO AO ANIMAL ==========

void aplicar_dano_cacador_em_animal(Cacador* cacador, Bot* bot, float delta_time) {
    if (!cacador || !bot || !cacador->ativo || cacador->derrotado || !bot->ativo) return;

    // Bloqueio por cenário
    if (cacador->cenario != (JogoCenas)bot->cenario) {
        return;  // CORREÇÃO: Não reseta cooldown aqui
    }

    // CORREÇÃO: Sempre decrementa o cooldown, independente de colisão
    if (cacador->cooldown_dano > 0.0f) {
        cacador->cooldown_dano -= delta_time;
        if (cacador->cooldown_dano < 0.0f) {
            cacador->cooldown_dano = 0.0f;
        }
    }

    // Verifica se está em colisão
    if (!verificar_colisao_cacador_animal(cacador, bot)) {
        return;  // CORREÇÃO: Não reseta cooldown, apenas retorna
    }

    // Só aplica dano se o cooldown permitir
    if (cacador->cooldown_dano <= 0.0f) {
        if (bot->vida_visual_atual > 0) {
            bot->vida_visual_atual -= cacador->dano_por_contato;
            if (bot->vida_visual_atual < 0) bot->vida_visual_atual = 0;

            printf("[DANO] %s causou %d de dano a %s! Vida: %d/%d\n",
                   cacador->nome, cacador->dano_por_contato,
                   bot->animal_data.nome, bot->vida_visual_atual, bot->vida_visual_maxima);
        }

        if (bot->vida_visual_atual <= 0) {
            bot->vida_visual_atual = 0;
            bot->ativo = false;
            printf("[CAPTURA] %s foi capturado por %s!\n",
                   bot->animal_data.nome, cacador->nome);
            marcar_reinicio_jogo();
        }

        // CORREÇÃO: Reseta o cooldown apenas APÓS aplicar o dano
        cacador->cooldown_dano = COOLDOWN_DANO_CACADOR;
    }

    // Empurrar apenas se bot ATIVO
    if (bot->ativo) {
        float dx = bot->x - cacador->x;
        float dy = bot->y - cacador->y;
        float distancia = sqrtf(dx * dx + dy * dy);

        if (distancia > 0.1f && distancia < (cacador->raio + bot->raio + RAIO_COLISAO_CACADOR_ANIMAL)) {
            cacador->x -= (dx / distancia) * FORCA_REPULSAO_CACADOR;
            cacador->y -= (dy / distancia) * FORCA_REPULSAO_CACADOR;
        }
    }
}

bool verificar_colisao_cacador_animal(const Cacador* cacador, const Bot* bot) {
    if (!cacador->ativo || cacador->derrotado || !bot || !bot->ativo) return false;

    if (cacador->cenario != (JogoCenas)bot->cenario) return false;

    float dx = bot->x - cacador->x;
    float dy = bot->y - cacador->y;
    float distancia = sqrtf(dx * dx + dy * dy);

    return distancia <= (bot->raio + cacador->raio + RAIO_COLISAO_CACADOR_ANIMAL);
}

// ========== MOVIMENTO ==========

void cacador_perseguir_alvo(Cacador* cacador, float alvo_x, float alvo_y,
    float delta_time, float largura_mapa, float altura_mapa) {

    float dx = alvo_x - cacador->x;
    float dy = alvo_y - cacador->y;
    float distancia = sqrtf(dx * dx + dy * dy);

    if (distancia > DISTANCIA_MINIMA_PERSEGUICAO) {
        float vel = cacador->velocidade_perseguicao * delta_time * 60.0f;
        cacador->x += (dx / distancia) * vel;
        cacador->y += (dy / distancia) * vel;

        limitar_posicao_cacador(&cacador->x, &cacador->y, cacador->raio,
            largura_mapa, altura_mapa, cacador->cenario);
    }
    else {
        cacador->estado = CACADOR_ATACANDO;
    }
}

void cacador_patrulhar(Cacador* cacador, float delta_time,
    float largura_mapa, float altura_mapa) {

    float dx = cacador->patrulha_x - cacador->x;
    float dy = cacador->patrulha_y - cacador->y;
    float distancia = sqrtf(dx * dx + dy * dy);

    if (distancia < DISTANCIA_OBJETIVO_PATRULHA) {
        cacador->tempo_espera += delta_time;

        // Tempo de espera aleatório
        float tempo_espera_aleatorio = TEMPO_ESPERA_PATRULHA * 0.75f + 
            ((float)rand() / RAND_MAX) * TEMPO_ESPERA_PATRULHA;

        if (cacador->tempo_espera >= tempo_espera_aleatorio) {
            float min_x = 0.0f, max_x = largura_mapa;
            float min_y = 0.0f, max_y = altura_mapa;

            switch (cacador->cenario) {
            case CENARIO1:
                max_x = LIMITE_HORIZONTAL_MAPA;
                max_y = LIMITE_VERTICAL_MAPA;
                break;
            case CENARIO2:
                min_y = LIMITE_VERTICAL_MAPA;
                max_x = LIMITE_HORIZONTAL_MAPA;
                break;
            case CENARIO3:
                min_x = LIMITE_HORIZONTAL_MAPA;
                max_y = LIMITE_VERTICAL_MAPA;
                break;
            case CENARIO4:
                min_x = LIMITE_HORIZONTAL_MAPA;
                min_y = LIMITE_VERTICAL_MAPA;
                break;
            }

            // Múltiplas tentativas para ponto mais distante
            for (int tentativa = 0; tentativa < 3; tentativa++) {
                float novo_x = (min_x + MARGEM_PATRULHA) + 
                    ((float)rand() / RAND_MAX) * (max_x - min_x - 2 * MARGEM_PATRULHA);
                float novo_y = (min_y + MARGEM_PATRULHA) + 
                    ((float)rand() / RAND_MAX) * (max_y - min_y - 2 * MARGEM_PATRULHA);
                
                float dist_nova = sqrtf((novo_x - cacador->x) * (novo_x - cacador->x) + 
                                       (novo_y - cacador->y) * (novo_y - cacador->y));
                
                if (dist_nova > 80.0f) {
                    cacador->patrulha_x = novo_x;
                    cacador->patrulha_y = novo_y;
                    break;
                }
            }
            
            cacador->tempo_espera = 0.0f;
        }
    }
    else {
        float vel = cacador->velocidade * delta_time * 60.0f;
        cacador->x += (dx / distancia) * vel;
        cacador->y += (dy / distancia) * vel;

        limitar_posicao_cacador(&cacador->x, &cacador->y, cacador->raio,
            largura_mapa, altura_mapa, cacador->cenario);
    }
}

void atualizar_cacador(Cacador* cacador, float animal_x, float animal_y,
    bool animal_ativo, float delta_time,
    float largura_mapa, float altura_mapa) {

    if (!cacador->ativo || cacador->derrotado) return;

    // Cooldown de batalha
    if (cacador->cooldown_batalha > 0.0f) {
        cacador->cooldown_batalha -= delta_time;
        if (cacador->cooldown_batalha < 0.0f) {
            cacador->cooldown_batalha = 0.0f;
        }
        cacador_patrulhar(cacador, delta_time, largura_mapa, altura_mapa);
        return;
    }

    Bot* alvo = cacador->alvo;
    bool alvo_valido = (alvo && alvo->ativo && (cacador->cenario == (JogoCenas)alvo->cenario));

    if (!alvo_valido) {
        cacador_patrulhar(cacador, delta_time, largura_mapa, altura_mapa);
        return;
    }

    animal_x = alvo->x;
    animal_y = alvo->y;
    animal_ativo = alvo->ativo;

    bool animal_detectado = animal_ativo && verificar_deteccao_animal(cacador, animal_x, animal_y);

    switch (cacador->estado) {
    case CACADOR_PATRULHANDO:
        if (animal_detectado) {
            cacador->estado = CACADOR_PERSEGUINDO;
            cacador->alvo_x = animal_x;
            cacador->alvo_y = animal_y;
        }
        else {
            cacador_patrulhar(cacador, delta_time, largura_mapa, altura_mapa);
        }
        break;

    case CACADOR_PERSEGUINDO:
    case CACADOR_ATACANDO: {
        float dx = animal_x - cacador->x;
        float dy = animal_y - cacador->y;
        float distancia = sqrtf(dx * dx + dy * dy);

        if (!animal_ativo || distancia > cacador->raio_abandono) {
            cacador->estado = CACADOR_PATRULHANDO;
            cacador->cooldown_dano = 0.0f;
        }
        else {
            cacador->alvo_x = animal_x;
            cacador->alvo_y = animal_y;
            cacador_perseguir_alvo(cacador, animal_x, animal_y, delta_time,
                largura_mapa, altura_mapa);
        }
        break;
    }

    case CACADOR_ESPERANDO:
        cacador->tempo_espera -= delta_time;
        if (cacador->tempo_espera <= 0.0f) {
            cacador->estado = CACADOR_PATRULHANDO;
        }
        break;
    }
}

void atualizar_cacador_chefe(CacadorChefe* chefe, entidade* jogador,
    float largura_mapa, float altura_mapa, float delta_time) {

    if (!chefe->liberado || chefe->derrotado) return;

    if (!chefe->base.ativo) {
        chefe->base.ativo = true;
        chefe->base.x = chefe->x_spawn;
        chefe->base.y = chefe->y_spawn;
    }

    float dx = jogador->x - chefe->base.x;
    float dy = jogador->y - chefe->base.y;
    float distancia = sqrtf(dx * dx + dy * dy);

    float distancia_minima = chefe->base.raio + jogador->raio + DISTANCIA_MINIMA_PERSEGUICAO;

    if (distancia > distancia_minima) {
        float vel = chefe->base.velocidade_perseguicao * delta_time * 60.0f;
        chefe->base.x += (dx / distancia) * vel;
        chefe->base.y += (dy / distancia) * vel;
    }

    limitar_posicao_cacador(&chefe->base.x, &chefe->base.y, chefe->base.raio,
        largura_mapa, altura_mapa, chefe->base.cenario);
}

// ========== DESENHO ==========

void desenhar_cacador(const Cacador* cacador, ALLEGRO_COLOR cor,
    float camera_x, float camera_y) {

    if (!cacador->ativo || cacador->derrotado) return;

    float cacador_x_tela = (cacador->x - camera_x) * ZOOM_FACTOR;
    float cacador_y_tela = (cacador->y - camera_y) * ZOOM_FACTOR;

    if (cacador->usar_sprite && cacador->sprite_cacador) {
        desenhar_sprite_camera(
            cacador->sprite_cacador,
            cacador->x,
            cacador->y,
            camera_x,
            camera_y,
            cacador->escala_sprite,
            ZOOM_FACTOR
        );
    }
    else {
        al_draw_filled_circle(cacador_x_tela, cacador_y_tela,
            cacador->raio * ZOOM_FACTOR, cor);
    }

    float barra_largura = 35.0f * ZOOM_FACTOR;
    float barra_altura = 4.0f * ZOOM_FACTOR;
    float barra_x = cacador_x_tela - barra_largura / 2.0f;
    float barra_y = cacador_y_tela - (cacador->raio + 18.0f) * ZOOM_FACTOR;

    al_draw_filled_rounded_rectangle(
        barra_x - 1.0f, barra_y - 1.0f,
        barra_x + barra_largura + 1.0f, barra_y + barra_altura + 1.0f,
        2.0f, 2.0f, al_map_rgba(0, 0, 0, 180)
    );

    al_draw_filled_rounded_rectangle(
        barra_x, barra_y,
        barra_x + barra_largura, barra_y + barra_altura,
        2.0f, 2.0f, al_map_rgb(50, 50, 50)
    );

    float vida_proporcao = (float)cacador->vida / (float)cacador->vida_maxima;
    al_draw_filled_rounded_rectangle(
        barra_x, barra_y,
        barra_x + barra_largura * vida_proporcao, barra_y + barra_altura,
        2.0f, 2.0f, al_map_rgb(220, 20, 20)
    );

    al_draw_rounded_rectangle(
        barra_x, barra_y,
        barra_x + barra_largura, barra_y + barra_altura,
        2.0f, 2.0f, al_map_rgb(255, 255, 255), 1.0f
    );
}

// ========== COLISÃO ==========

bool verificar_colisao_cacador_jogador(entidade* jogador, const Cacador* cacador) {
    if (!cacador->ativo || cacador->derrotado) return false;

    float dx = jogador->x - cacador->x;
    float dy = jogador->y - cacador->y;
    float distancia = sqrtf(dx * dx + dy * dy);

    return distancia <= (jogador->raio + cacador->raio);
}

bool verificar_deteccao_animal(const Cacador* cacador, float animal_x, float animal_y) {
    float dx = animal_x - cacador->x;
    float dy = animal_y - cacador->y;
    float distancia = sqrtf(dx * dx + dy * dy);

    return distancia <= cacador->raio_deteccao;
}

// ========== VERIFICAÇÕES ==========

bool verificar_desbloqueio_chefe(Cacador cacadores[], int total_cacadores) {
    int derrotados = 0;
    for (int i = 0; i < total_cacadores; i++) {
        if (cacadores[i].derrotado) derrotados++;
    }
    return derrotados >= CACADORES_PARA_BOSS;
}

bool verificar_liberacao_boss(int cacadores_derrotados) {
    return cacadores_derrotados >= CACADORES_PARA_BOSS;
}

// ========== LIMPEZA ==========

void destruir_cacador(Cacador* cacador) {
    if (!cacador) return;
    if (cacador->sprite_cacador) {
        destruir_sprite(cacador->sprite_cacador);
        cacador->sprite_cacador = NULL;
    }
}